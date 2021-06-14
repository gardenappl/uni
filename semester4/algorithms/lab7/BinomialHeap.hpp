#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include <stdexcept>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

template<typename T>
struct BinomialNode
{
    BinomialNode(const T& element);
    BinomialNode();
    ~BinomialNode();

    T* key = nullptr; //вказівник на ключ, якщо nullptr - ключ дорівнює "мінус нескінченність"
    BinomialNode* parent = nullptr;
    std::vector<BinomialNode*> children; //піддерева, відсортовані в порядку зростання degree
    //degree == children.size()
    BinomialNode* rightBro = nullptr; //вказівник на правого брата (якщо це корінь - вказує на наступний корінь)

    //метод для деструктора дерева
    void deleteChildren();

    //зв'язує інще біномальне дерево такого ж порядку
    void link(BinomialNode* node);

    //друкує структуру вузла на заданій глибині
    void printStructure(std::ostream& os, int depth) const;

    //покук вузла з певним значенням key
    BinomialNode<T>* findNode(const T* searchKey) const;

    //порівняння ключів з урахуванням "мінус нескінченність"
    bool operator<(const BinomialNode& node) const;
    bool operator>(const BinomialNode& node) const;
    bool operator<=(const BinomialNode& node) const;
    bool operator>=(const BinomialNode& node) const;
};

template<typename T>
BinomialNode<T>::BinomialNode(const T& element)
: key(new T(element))
{}

template<typename T>
BinomialNode<T>::BinomialNode()
: key(nullptr)
{}

template<typename T>
BinomialNode<T>::~BinomialNode()
{
    if(key)
        delete key;
}

template<typename T>
void BinomialNode<T>::deleteChildren()
{
    for(BinomialNode<T>* child : children)
    {
        child->deleteChildren();
        delete child;
    }
}

template<typename T>
void BinomialNode<T>::link(BinomialNode<T>* node)
{
    node->parent = this;
    if(children.empty())
        node->rightBro = nullptr;
    else
        node->rightBro = children.back();
    children.push_back(node);
}

template<typename T>
void BinomialNode<T>::printStructure(std::ostream& os, int depth) const
{
    for(int i = 0; i < depth; i++)
        os << "   ";
    os << *key;
//    if(rightBro)
//        os << " (sibling: " << *(rightBro->key) << ')';
    os << std::endl;

    if(!children.empty())
    {
        BinomialNode<T>* child = children.back();
        while(child)
        {
            child->printStructure(os, depth + 1);
            child = child->rightBro;
        }
    }
}

//Порівнюємо значення key, якщо key == nullptr, значення вважається "мінус нескінченність"

template<typename T>
bool BinomialNode<T>::operator<(const BinomialNode<T>& node) const
{
    if(!node.key)
        return false;
    return !key || *key < *node.key;
}

template<typename T>
bool BinomialNode<T>::operator>(const BinomialNode<T>& node) const
{
    if(!key)
        return false;
    return !node.key || *key > *node.key;
}

template<typename T>
bool BinomialNode<T>::operator<=(const BinomialNode<T>& node) const
{
    if(!node.key)
        return !key;
    return key && *key <= *node.key;
}

template<typename T>
bool BinomialNode<T>::operator>=(const BinomialNode<T>& node) const
{
    if(!key)
        return !node.key;
    return node.key && *key <= *node.key;
}

template<typename T>
BinomialNode<T>* BinomialNode<T>::findNode(const T* searchKey) const
{
    if((!searchKey && !key) ||
            (searchKey && key && *searchKey == *key))
        return const_cast<BinomialNode<T>*>(this);

    const BinomialNode<T>* result = nullptr;

    //шукаємо серед братів
    if(rightBro)
    {
        result = rightBro->findNode(searchKey);
        if(result)
            return const_cast<BinomialNode<T>*>(result);
    }
    //шукаємо серед дітей
    if(!children.empty())
        result = children.back()->findNode(searchKey);
    return const_cast<BinomialNode<T>*>(result);
}





template<typename T>
class BinomialHeap
{
public:
    BinomialHeap() = default;
    ~BinomialHeap();

    //пошук мін. елемента
    T peek() const;

    //об'єднання двох куп
    void merge(BinomialHeap& heap2);

    //вставка нового елемента (використовує merge)
    void insert(const T& element);

    //вилучення мін. елемента
    T pop();

    //друкування структури піраміди
    void printStructure(std::ostream& os) const;

    //зменшення ключа
    void decreaseKey(const T& key, const T& newKey);

    //перевірка, чи міститься такий key в піраміді
    bool contains(const T& key) const;

    //видалення ключа
    void deleteKey(const T& key);

private:
     //список коренів дерев, у порядку зростання degree
    BinomialNode<T>* root1 = nullptr;

    //Переміщує всі елементи roots у trees, зберігаючи порядок
    void mergeRoots(BinomialNode<T>* roots);

    //приватна функція для вилучення мін. елемента, може повертати "мінус нескінченність"
    BinomialNode<T>* extractMinNode();

    //приватна функція для зменшення ключа, може працювати з "мінус нескінченністю"
    void decreaseKey(BinomialNode<T>* key, T* newKey);
};

template<typename T>
BinomialHeap<T>::~BinomialHeap()
{
    //видалення списку коренів
    BinomialNode<T>* currentRoot = root1;
    while(currentRoot)
    {
        BinomialNode<T>* nextRoot = currentRoot->rightBro;
        currentRoot->deleteChildren();
        delete currentRoot;
        currentRoot = nextRoot;
    }
}

template<typename T>
T BinomialHeap<T>::peek() const
{
    if(!root1)
        throw std::out_of_range("heap is empty");

    //Шукаємо мін. елеемент серед коренів дерев
    //(вважаємо, що на момент виклику в піраміді не зберігається "мінус нескінченність")
    T minElement = *(root1->key);

    BinomialNode<T>* currentNode = root1->rightBro;
    while(currentNode)
    {
        if(*(currentNode->key) < minElement)
            minElement = *(currentNode->key);
        currentNode = currentNode->rightBro;
    }

    return minElement;
}

template<typename T>
void BinomialHeap<T>::mergeRoots(BinomialNode<T>* roots)
{
    //Аналогічно до merge sort
    BinomialNode<T>* node1 = root1;
    BinomialNode<T>* node2 = roots;
    BinomialNode<T>** currentNodePtr = &root1;

    while(node1 && node2)
    {
        if(node1->children.size() <= node2->children.size())
        {
            currentNodePtr = &(node1->rightBro);
            node1 = node1->rightBro;
        }
        else
        {
            BinomialNode<T>* nextNode2 = node2->rightBro;
            node2->rightBro = node1;
            *currentNodePtr = node2;
            currentNodePtr = &(node2->rightBro);
            node2 = nextNode2;
        }
    }
    if(node1)
        *currentNodePtr = node1;
    else if(node2)
        *currentNodePtr = node2;
}

template<typename T>
void BinomialHeap<T>::merge(BinomialHeap<T>& heap)
{
//    std::cout << "--MERGE INFO BEGIN--" << std::endl;
//    std::cout << "--this--\n";
//    printStructure(std::cout);
//    std::cout << "--heap--\n";
//    heap.printStructure(std::cout);
//    std::cout << "--MERGE INFO END--" << std::endl;

    mergeRoots(heap.root1);
    heap.root1 = nullptr;

    if(!root1)
        return;

    //Відновлюємо властивості
    BinomialNode<T>* prevNode = nullptr;
    BinomialNode<T>* currentNode = root1;
    BinomialNode<T>* nextNode = root1->rightBro;

    while(nextNode)
    {
        if(currentNode->children.size() != nextNode->children.size() ||
           (nextNode->rightBro && nextNode->children.size() == nextNode->rightBro->children.size()))
        {
            //випадки 1 і 2 - йдемо далі
            prevNode = currentNode;
            currentNode = nextNode;
        }
        else if(*currentNode < *nextNode)
        {
            //випадок 3 - nextNode причіпляємо до currentNode
            currentNode->rightBro = nextNode->rightBro;
            currentNode->link(nextNode);
        }
        else
        {
            //випадок 4 - currentNode причіпляємо до nextNode
            if(!prevNode)
                root1 = nextNode;
            else
                prevNode->rightBro = nextNode;
            nextNode->link(currentNode);
            currentNode = nextNode;
        }
        nextNode = currentNode->rightBro;
    }
}

template<typename T>
void BinomialHeap<T>::insert(const T& element)
{
    //Створюємо біномінальну пірамідку і зливаємо її
    BinomialHeap<T> tempHeap;
    tempHeap.root1 = new BinomialNode<T>(element);
    merge(tempHeap);
}

template<typename T>
BinomialNode<T>* BinomialHeap<T>::extractMinNode()
{
    if(!root1)
        throw std::out_of_range("heap is empty");

    //Пошук кореня х з мінімальним значенням ключа,
    BinomialNode<T>* prevNode = root1;
    BinomialNode<T>* currentNode = root1->rightBro;
    BinomialNode<T>** minNodePtr = &root1;

    while(currentNode)
    {
        if(*currentNode < **minNodePtr)
        {
            minNodePtr = &(prevNode->rightBro);
        }
        prevNode = currentNode;
        currentNode = currentNode->rightBro;
    }

    //і видалення його зі списку
    currentNode = *minNodePtr;
    *minNodePtr = (*minNodePtr)->rightBro;

//    std::cout << "\n\nCURRENT NODE CONTAINS " << *((*minNodePtr)->key) << "\n\n";

    //Утворюємо з його синів нову біноміальну піраміду
    //(перестановкою у зворотному порядку).
    BinomialHeap<T> tempHeap;

    if(currentNode->children.size() == 1)
    {
        tempHeap.root1 = currentNode->children[0];
    }
    else if(currentNode->children.size() > 1)
    {
        BinomialNode<T>* swapNode = currentNode->children.back();
        prevNode = swapNode->rightBro;
        swapNode->rightBro = nullptr;

        while(prevNode)
        {
            BinomialNode<T>* prevNodeBro = prevNode->rightBro;
            prevNode->rightBro = swapNode;

            swapNode = prevNode;
            prevNode = prevNodeBro;
        }
        tempHeap.root1 = swapNode;
    }

    //Зливаємо новоутворену піраміду з вихідною.
    merge(tempHeap);

    return currentNode;
}

template<typename T>
T BinomialHeap<T>::pop()
{
    BinomialNode<T>* node = extractMinNode();
    //це публічна функція, тому вважаємо,
    //що на момент виклику в піраміді не зберігається "мінус нескінченність"

    T minElement = *(node->key);
    delete node;
    return minElement;
}

template<typename T>
void BinomialHeap<T>::printStructure(std::ostream& os) const
{
    if(!root1)
    {
        os << "<empty>" << std::endl;
        return;
    }
    BinomialNode<T>* node = root1;
    while(node)
    {
        node->printStructure(os, 0);
        node = node->rightBro;
    }
}

template<typename T>
bool BinomialHeap<T>::contains(const T& key) const
{
    if(!root1)
        return false;

    BinomialNode<T>* node = root1->findNode(&key);
    //повертаємо true, якщо вузол знайдено
    return node != nullptr;
}

template<typename T>
void BinomialHeap<T>::decreaseKey(const T& key, const T& newKey)
{
    if(!root1)
        throw std::out_of_range("Heap is empty");
    BinomialNode<T>* node = root1->findNode(&key);
    if(!node)
        throw std::out_of_range("Heap does not contain key");

    decreaseKey(node, new T(newKey));
}

template<typename T>
void BinomialHeap<T>::decreaseKey(BinomialNode<T>* node, T* newKey)
{
    if(newKey && *(node->key) < *newKey)
        throw std::invalid_argument("newKey must be smaller");

    //Значення ключа замінюється на менше.
    delete node->key;
    node->key = newKey;

    //Рухаємось у напрямку кореня, обмінюючи значення, якщо порушується умова неспадаючої піраміди

    BinomialNode<T>* parentNode = node->parent;
    BinomialNode<T>* currentNode = node;

    while(parentNode && *currentNode < *parentNode)
    {
        std::swap(currentNode->key, parentNode->key);
        currentNode = parentNode;
        parentNode = currentNode->parent;
    }
}

template<typename T>
void BinomialHeap<T>::deleteKey(const T& key)
{
    if(!root1)
        throw std::out_of_range("Heap is empty");
    BinomialNode<T>* node = root1->findNode(&key);
    if(!node)
        throw std::out_of_range("Heap does not contain key");

    //Ключ у вузлі для видалення робиться "мінус нескінченність. При цьому
    //розглянута вершина стає одним з коренів, мінімальним,
    //і може бути вилучена за допомогою процедури
    //BINOMIAL_HEAP_EXTRACT_MIN.

    decreaseKey(node, nullptr);
    delete extractMinNode();
}



#endif // BINOMIALHEAP_H
