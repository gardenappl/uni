#ifndef BPLUSTREE_HPP
#define BPLUSTREE_HPP

#include <vector>
#include <iostream>

template<typename T>
struct BPlusNode
{
    bool isLeaf = true;
    BPlusNode<T>* next = nullptr; //якщо isLeaf, вказує на наступний leaf

    std::vector<T> keys; //n ключів, t-1 <= n <=2t-1
    std::vector<BPlusNode<T>*> children;
    //кількість дітей = n + 1;
    //де вузол номер N містить значення між keys[N-1] та keys[N]

    //пошук елемента
    bool contains(const T& element) const;

    //розбиття вузла
    void splitChild(size_t index, size_t t);

    //вставка елемента (передумова: keys.size() < 2*t - 1)
    void insertNonFull(const T& element, size_t t);

    //видалення елемента (передумова: keys.size() > t - 1, якщо це не корінь)
    void remove(const T& element, size_t t);

    //для деструктора дерева
    void deleteChildren();

    //вивід на екран
    void printStructure(std::ostream& os, int depth) const;
};

//Лінійний пошук серед ключів, якщо потрібно, спускаємось
template<typename T>
bool BPlusNode<T>::contains(const T& element) const
{
    size_t i = 0;
    //Можна замінити на binary search для більшої ефективності при великих keys.size()
    for(; i < keys.size(); i++)
    {
        if(keys[i] >= element)
            break;
    }
    if(isLeaf && i < keys.size() && keys[i] == element)
        return true;

    if(!isLeaf)
    {
        //DISK_READ
        return children[i]->contains(element);
    }
    return false;
}

template<typename T>
void BPlusNode<T>::splitChild(size_t index, size_t t)
{
    BPlusNode<T>* child = children[index];

    //Створення вузла і перенесення туди (t–1) більших
    //ключів та відповідно t правих дочірніх вузлів.

    BPlusNode<T>* newNode = new BPlusNode<T>();
    newNode->isLeaf = child->isLeaf;

    newNode->keys.reserve(t - 1);
    for(size_t i = 0; i < t - 1; i++)
        newNode->keys.push_back(child->keys[t + i]);

    if(!child->isLeaf)
    {
        newNode->children.reserve(t);
        for(size_t i = 0; i < t; i++)
            newNode->children.push_back(child->children[t + i]);
    }

    //Оновлення кільності ключів в child
    if(child->isLeaf)
    {
        child->keys.resize(t);
    }
    if(!child->isLeaf)
    {
        child->keys.resize(t - 1);
        child->children.resize(t);
    }

    //Вставка newNode як дочірнього вузла this з переносом медіани
    //з child в this для розділення child та newNode

    children.insert(children.begin() + index + 1, newNode); //операція O(n)
    keys.insert(keys.begin() + index, child->keys[t - 1]); //операція O(n)

    //Оновлюємо next
    newNode->next = child->next;
    child->next = newNode;

    //DISK_WRITE(this)
    //DISK_WRITE(newNode)
    //DISK_WRITE(child)
}

template<typename T>
void BPlusNode<T>::insertNonFull(const T& element, size_t t)
{
    if(isLeaf)
    {
        //проста вставка в лист

        if(keys.empty() || element > keys.back())
        {
            keys.push_back(element);
        }
        else
        {
            //шукаємо потрібний індекс і зміщуюємо елементи
            size_t i = keys.size() - 1;

            keys.push_back(keys.back());
            while(i > 0 && element < keys[i - 1])
            {
                keys[i] = keys[i - 1];
                i--;
            }
            keys[i] = element;
            //DISK_WRITE(this)
        }
    }
    else
    {
        //Не лист, шукаємо піддерево для спуску
        size_t i = keys.size();

        while(i > 0 && element < keys[i - 1])
            i--;

        //DISK_READ(children[i])
        if(children[i]->keys.size() == 2*t - 1)
        {
            //вузол внизу повний - ділимо
            splitChild(i, t);

            //куди спускатися після поділу
            if(element > keys[i])
                i++;
        }
        children[i]->insertNonFull(element, t);
    }
}

template<typename T>
void BPlusNode<T>::remove(const T& element, size_t t)
{
    //передумова: keys.size() > t - 1, якщо це не корінь
    if(isLeaf)
    {
        //Якщо ключ k знаходиться у вузлі x та останній є
        //листом – видаляємо k з x.

        for(size_t i = 0; i < keys.size(); i++)
        {
            if(keys[i] == element)
            {
                keys.erase(keys.begin() + i); //операція O(n)
                return; //знайшли і видалили
            }
        }
        return; //не знайшли елемент
    }

    //якщо це внутрішній вузол:
    //знаходимо піддерево, яке може містити element
    size_t i = 0;
    while(i < keys.size() && element > keys[i])
        i++;

    BPlusNode<T>* node = children[i];
    if(node->keys.size() > t - 1)
    {
        //умова виконується
        node->remove(element, t);
    }
    else
    {
        //якщо в ньому лише t-1 елементів:
        //розглядаємо сусідів.

        //якщо у сусіда зліва достатньо елементів, заберемо у нього
        if(i > 0 && children[i - 1]->keys.size() > t - 1)
        {
            BPlusNode<T>* leftBro = children[i - 1];
            if(leftBro->isLeaf)
            {
                //Для листа:

                //переносимо найбільший елемент
                node->keys.insert(node->keys.begin(), leftBro->keys.back());
                leftBro->keys.pop_back();

                //Замінимо ключ-роздільник
                keys[i - 1] = leftBro->keys.back();
            }
            else
            {
                //Для внутрішнього вузла:

                //Роздільник переносимо в піддерево
                node->keys.insert(node->keys.begin(), keys[i - 1]);
                //На його місце ставимо крайній роздільник сусіда
                keys[i - 1] = leftBro->keys.back();
                leftBro->keys.pop_back();

                //І переносимо відповідний вказівник
                node->children.insert(node->children.begin(), leftBro->children.back());
                leftBro->children.pop_back();
            }

            //тепер умова виконується
            node->remove(element, t);
            return;
        }
        //аналогічно справа
        if(i < children.size() - 1 && children[i + 1]->keys.size() > t - 1)
        {
            BPlusNode<T>* rightBro = children[i + 1];
            if(rightBro->isLeaf)
            {
                //Для листа:

                //переносимо найменший елемент
                node->keys.push_back(rightBro->keys.front());
                rightBro->keys.erase(rightBro->keys.begin());

                //Замінимо ключ-роздільник
                keys[i] = rightBro->keys.front();
            }
            else
            {
                //Для внутрішнього вузла:

                //Роздільник переносимо в піддерево
                node->keys.push_back(keys[i]);
                //На його місце ставимо крайній роздільник сусіда
                keys[i] = rightBro->keys.front();
                rightBro->keys.erase(rightBro->keys.begin());

                //І переносимо відповідний вказівник
                node->children.push_back(rightBro->children.front());
                rightBro->children.erase(rightBro->children.begin());
            }

            //тепер умова виконується
            node->remove(element, t);
            return;
        }
        //У обох сусідів по (t-1) ключу, об`єднаємо
        if(i < children.size() - 1)
        {
            //Об`єднання справа

            BPlusNode<T>* rightBro = children[i + 1];

            if(node->isLeaf)
            {
                //Оновлюємо next
                node->next = rightBro->next;
                //Ключ-вказівник видаляємо
                keys.erase(keys.begin() + i);
            }
            else
            {
                //Ключ-роздільник стає медіаною
                node->keys.push_back(keys[i]);
                keys.erase(keys.begin() + i);

            }

            //Об'єднуємо ключі (O(n))
            node->keys.reserve(node->keys.size() + rightBro->keys.size());
            node->keys.insert(node->keys.end(), rightBro->keys.begin(), rightBro->keys.end());

            if(!node->isLeaf)
            {
                //Об'єднуємо вказівники (O(n))
                node->children.reserve(node->children.size() + rightBro->children.size());
                node->children.insert(node->children.end(), rightBro->children.begin(), rightBro->children.end());
            }
            //видаляємо брата
            delete rightBro;
            children.erase(children.begin() + i + 1);

            //тепер умова виконується
            node->remove(element, t);
            return;
        }
        if(i > 0)
        {
            //Об`єднання зліва

            BPlusNode<T>* leftBro = children[i - 1];

            if(node->isLeaf)
            {
                //Оновлюємо next
                leftBro->next = node->next;
                //Ключ-вказівник видаляємо
                keys.erase(keys.begin() + i - 1);
            }
            else
            {
                //Ключ-роздільник стає медіаною
                leftBro->keys.push_back(keys[i - 1]);
                keys.erase(keys.begin() + i - 1);
            }

            //Об'єднуємо ключі (O(n))
            leftBro->keys.reserve(leftBro->keys.size() + node->keys.size());
            leftBro->keys.insert(leftBro->keys.end(), node->keys.begin(), node->keys.end());

            if(!node->isLeaf)
            {
                //Об'єднуємо вказівники (O(n))
                leftBro->children.reserve(leftBro->children.size() + node->children.size());
                leftBro->children.insert(leftBro->children.end(), node->children.begin(), node->children.end());
            }
            //видаляємо брата
            delete node;
            children.erase(children.begin() + i);

            //тепер умова виконується
            leftBro->remove(element, t);
            return;
        }
    }
}

template<typename T>
void BPlusNode<T>::deleteChildren()
{
    for(size_t i = 0; i < children.size(); i++)
    {
        if(children[i])
        {
            children[i]->deleteChildren();
            delete children[i];
        }
    }
}

template<typename T>
void BPlusNode<T>::printStructure(std::ostream& os, int depth) const
{
//    for(int i = 0; i < depth; i++)
//        os << "   ";
//    os << "leaf? " << isLeaf << " children: " << children.size() << " keys: " << keys.size() << '\n';
    for(size_t i = 0; i < keys.size(); i++)
    {
        if(!isLeaf)
            children[i]->printStructure(os, depth + 1);
        for(int i = 0; i < depth; i++)
            os << "   ";
        os << keys[i] << std::endl;
    }
    if(!children.empty())
        children.back()->printStructure(os, depth + 1);
}





template<typename T>
class BPlusTree
{
public:
    BPlusTree(size_t t);
    virtual ~BPlusTree();

    void insert(const T& element);
    void printStructure(std::ostream& os) const;
    void printInOrder(std::ostream& os) const;
    bool contains(const T& element) const;
    void remove(const T& element);
private:
    size_t t;
    BPlusNode<T>* root;

    friend BPlusNode<T>;
};

template<typename T>
BPlusTree<T>::BPlusTree(size_t t)
: t(t), root(new BPlusNode<T>())
{}

template<typename T>
BPlusTree<T>::~BPlusTree()
{
    root->deleteChildren();
    delete root;
}

template<typename T>
void BPlusTree<T>::insert(const T& element)
{
    BPlusNode<T>* currentRoot = root;

    if(currentRoot->keys.size() == 2*t - 1)
    {
        //випадок заповненого кореня

        root = new BPlusNode<T>();
        root->isLeaf = false;
        root->children.push_back(currentRoot);
        root->splitChild(0, t);
        root->insertNonFull(element, t);
    }
    else
    {
        currentRoot->insertNonFull(element, t);
    }
}

template<typename T>
void BPlusTree<T>::remove(const T& element)
{
    root->remove(element, t);
    //Зменшення висоти дерева
    if(root->children.size() == 1)
    {
        BPlusNode<T>* child = root->children[0];
        delete root;
        root = child;
    }
}


template<typename T>
void BPlusTree<T>::printStructure(std::ostream& os) const
{
    root->printStructure(os, 0);
}

template<typename T>
bool BPlusTree<T>::contains(const T& element) const
{
    return root->contains(element);
}

template<typename T>
void BPlusTree<T>::printInOrder(std::ostream& os) const
{
    //Вивід на екран за допомогою next

    //Знаходимо мінімальний лист
    BPlusNode<T>* node = root;
    while(!node->children.empty())
        node = node->children[0];

    //Вивід на екран
    bool first = true;
    os << "[ ";
    while(node)
    {
        for(size_t i = 0; i < node->keys.size(); i++)
        {
            if(!first)
                os << ", ";
            os << node->keys[i];
            first = false;
        }
        node = node->next;
    }
    os << " ]";
}


#endif // BPLUSTREE_HPP
