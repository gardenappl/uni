#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <stdexcept>
#include <iostream>
#include <stack>

template<typename T>
struct SplayNode
{
    SplayNode(T value, SplayNode<T>* parent = nullptr);

    T value;
    SplayNode<T>* parent = nullptr;
    SplayNode<T>* left = nullptr;
    SplayNode<T>* right = nullptr;

    SplayNode<T>* getMin();
    SplayNode<T>* getMax();
};

template<typename T>
SplayNode<T>::SplayNode(T value, SplayNode<T>* parent)
: value(value), parent(parent)
{}

template<typename T>
SplayNode<T>* SplayNode<T>::getMin()
{
    SplayNode<T>* currentNode = this;
    while(currentNode->left != nullptr)
        currentNode = currentNode->left;
    return currentNode;
}

template<typename T>
SplayNode<T>* SplayNode<T>::getMax()
{
    SplayNode<T>* currentNode = this;
    while(currentNode->right != nullptr)
        currentNode = currentNode->right;
    return currentNode;
}




template<typename T>
class SplayTree
{
public:
    SplayTree() = default;
    ~SplayTree();

    void insert(T value);
    bool remove(const T& value);
    bool contains(const T& value);
    SplayTree splitLesser(const T& value);
    void mergeGreater(SplayTree& tree);

    void printInOrder();
    void printStructure();

protected:
private:
    SplayNode<T>* root = nullptr;

    void rotateLeft(SplayNode<T>* pivot);
    void rotateRight(SplayNode<T>* pivot);
    SplayNode<T>* getNext(SplayNode<T>* node) const;
    SplayNode<T>* getPrev(SplayNode<T>* node) const;
    void deleteSubtree(SplayNode<T>* node);

    SplayNode<T>* remove(SplayNode<T>* node);
    SplayNode<T>* findNode(const T& value) const;
    SplayNode<T>* findMinNodeNoLessThan(const T& value) const;
    void mergeGreaterNode(SplayNode<T>* greater);
    SplayNode<T>* splitLesserNode(const T& value);
    void splay(SplayNode<T>* node);

    void printInOrder(SplayNode<T>* node, bool& firstPrint);
    void printStructure(SplayNode<T>* node, int depth);
};

/**
 * Видалення всіх вузлів дерева
 */

template<typename T>
SplayTree<T>::~SplayTree()
{
    deleteSubtree(root);
}

template<typename T>
void SplayTree<T>::deleteSubtree(SplayNode<T>* node)
{
    if(!node)
        return;
    deleteSubtree(node->left);
    deleteSubtree(node->right);
    delete node;
}

/**
 * Стандартні функції для обходу дерева у порядку зростання.
 */

template<typename T>
void SplayTree<T>::printInOrder()
{
    if(root == nullptr)
    {
        std::cout << "<empty>";
        return;
    }
    std::cout << "{ ";
    bool firstPrint = true;
    printInOrder(root, firstPrint);
    std::cout << " }";
}

template<typename T>
void SplayTree<T>::printInOrder(SplayNode<T>* node, bool& firstPrint)
{
    if(node->left != nullptr)
        printInOrder(node->left, firstPrint);

    if(!firstPrint)
        std::cout << ", ";
    else
        firstPrint = false;
    std::cout << node->value;

    if(node->right != nullptr)
        printInOrder(node->right, firstPrint);
}

/**
 * Функції, що наглядно показують користувачу внутрішню структуру дерева
 */
template<typename T>
void SplayTree<T>::printStructure()
{
    if(!root)
        std::cout << "<empty>" << std::endl;
    else
        printStructure(root, 0);
}

template<typename T>
void SplayTree<T>::printStructure(SplayNode<T>* node, int depth)
{
    for(int i = 0; i < depth; i++)
        std::cout << "   ";
    std::cout << node->value << std::endl;

    if(node->left)
    {
        for(int i = 0; i < depth; i++)
            std::cout << "   ";
        std::cout << "Left:" << std::endl;
        printStructure(node->left, depth + 1);
    }

    if(node->right)
    {
        for(int i = 0; i < depth; i++)
            std::cout << "   ";
        std::cout << "Right:" << std::endl;
        printStructure(node->right, depth + 1);
    }
}

/**
 * Функції для знаходження наступного і попереднього елементів, виконуються за O(h)
 */
template<typename T>
SplayNode<T>* SplayTree<T>::getNext(SplayNode<T>* node) const
{
    if(node->right != nullptr)
        return node->right->getMin();

    SplayNode<T>* parent = node->parent;
    while(parent != nullptr && node == parent->right)
    {
        node = parent;
        parent = node->parent;
    }
    return parent;
}

template<typename T>
SplayNode<T>* SplayTree<T>::getPrev(SplayNode<T>* node) const
{
    if(node->left != nullptr)
        return node->left->getMax();

    SplayNode<T>* parent = node->parent;
    while(parent != nullptr && node == parent->left)
    {
        node = parent;
        parent = node->parent;
    }
    return parent;
}

/**
 * Функції для вставки нового елемента у дерево
 */

template<typename T>
void SplayTree<T>::insert(T value)
{
    //Запускаємо Split від елементу, що додається...
    SplayNode<T>* lesser = splitLesserNode(value);
    //(this містить greater)

    //...і підвішуємо дерева, що вийшли, до нього.
    SplayNode<T>* newNode = new SplayNode<T>(value);
    if(lesser)
    {
        newNode->left = lesser;
        lesser->parent = newNode;
    }
    if(root)
    {
        newNode->right = root;
        root->parent = newNode;
    }
    root = newNode;
}


/**
 * Стандартні функції повороту для бінарного дерева.
 */
template<typename T>
void SplayTree<T>::rotateLeft(SplayNode<T>* pivot)
{
    SplayNode<T>* replaceNode = pivot->right;

    pivot->right = replaceNode->left;
    if(replaceNode->left != nullptr)
        replaceNode->left = pivot;

    replaceNode->parent = pivot->parent;
    if(pivot == root)
    {
        root = replaceNode;
    }
    else
    {
        if(pivot == pivot->parent->left)
            pivot->parent->left = replaceNode;
        else
            pivot->parent->right = replaceNode;
    }
    replaceNode->left = pivot;
    pivot->parent = replaceNode;
}

template<typename T>
void SplayTree<T>::rotateRight(SplayNode<T>* pivot)
{
    SplayNode<T>* replaceNode = pivot->left;

    pivot->left = replaceNode->right;
    if(replaceNode->right != nullptr)
        replaceNode->right = pivot;

    replaceNode->parent = pivot->parent;
    if(pivot == root)
    {
        root = replaceNode;
    }
    else
    {
        if(pivot == pivot->parent->left)
            pivot->parent->left = replaceNode;
        else
            pivot->parent->right = replaceNode;
    }
    replaceNode->right = pivot;
    pivot->parent = replaceNode;
}

/**
 * Функції для пошуку елемента у дереві
 */
template<typename T>
SplayNode<T>* SplayTree<T>::findNode(const T& value) const
{
    //Стандартний пошук у дереві бінарного пошуку
    SplayNode<T>* currentNode = root;
    while(currentNode != nullptr)
    {
        if(value == currentNode->value)
            return currentNode;
        else if(value > currentNode->value)
            currentNode = currentNode->right;
        else
            currentNode = currentNode->left;
    }
    return nullptr; //Не знайшли потрібний елемент
}

//Повертає найменший елемент, більший або рівний value.
template<typename T>
SplayNode<T>* SplayTree<T>::findMinNodeNoLessThan(const T& value) const
{
    //Стандартний пошук у дереві бінарного пошуку
    if(!root)
        return nullptr;

    SplayNode<T>* currentNode = root;
    SplayNode<T>* parentNode = nullptr;
    while(currentNode != nullptr)
    {
        if(value == currentNode->value)
            return currentNode;

        if(value > currentNode->value)
        {
            parentNode = currentNode;
            currentNode = currentNode->right;
        }
        else
        {
            parentNode = currentNode;
            currentNode = currentNode->left;
        }
    }
    //Не знайшли потрібний елемент, знайшли найближчий
    if(parentNode->value > value)
        return parentNode;
    else
        return getNext(parentNode);
}

template<typename T>
bool SplayTree<T>::contains(const T& value)
{
    SplayNode<T>* node = findNode(value);
    if(!node)
        return false;

    //Знайдений елемент переміщуємо наверх
    splay(node);
    return true;
}

/**
 * Функція для видалення елемента з дерева.
 * Повертає true, якщо елемент знайдено і видалено. Якщо не знайдено, то false.
 */
//
template<typename T>
bool SplayTree<T>::remove(const T& value)
{
    //Знаходимо елемент в дереві
    SplayNode<T>* node = findNode(value);
    if(node == nullptr)
        return false;

    //робимо Splay для нього
    splay(node);

    //робимо поточним деревом Merge його дітей.
    root = node->left;
    SplayNode<T>* greater = node->right;
    mergeGreaterNode(greater);
    delete node;

    return true;
}

/**
 * Splay - "операція розширення"
 * Переміщує вершину х в корінь за допомогою операцій
 * Zig, Zig-Zig та Zig-Zag.
 */

template<typename T>
void SplayTree<T>::splay(SplayNode<T>* node)
{
    while(node != root)
    {
        if(!(node->parent)->parent)
        {
            //Немає "дідуся" - Zig
            if(node == node->parent->left)
                rotateRight(node->parent);
            else
                rotateLeft(node->parent);
        }
        else if(node->parent == node->parent->parent->left)
        {
            if(node == node->parent->left)
            {
                //"Батько" і "дідусь" по один бік - Zig-Zig
                rotateRight(node->parent->parent);
                rotateRight(node->parent);
            }
            else
            {
                //"Батько" і "дідусь" по різні боки - Zig-Zag
                rotateLeft(node->parent);
                rotateRight(node->parent);
            }
        }
        else
        {
            //Аналогічно з іншого боку
            if(node == node->parent->right)
            {
                //"Батько" і "дідусь" по один бік - Zig-Zig
                rotateLeft(node->parent->parent);
                rotateLeft(node->parent);
            }
            else
            {
                //"Батько" і "дідусь" по різні боки - Zig-Zag
                rotateRight(node->parent);
                rotateLeft(node->parent);
            }
        }
    }
}

/**
 * Функції для злиття дерев
 */

/**
 * Злиття дерева з більшими (або рівними) елементами разом із this.
 */
template<typename T>
void SplayTree<T>::mergeGreaterNode(SplayNode<T>* greater)
{
    if(!root)
    {
        root = greater;
    }
    else
    {
        //Splay для найбільшого елемету у lesser.
        SplayNode<T>* lesserMax = root->getMax();
        //Одночасно робимо перевірку:
        //всі елементи lesser повинні бути менші за елементи greater
        SplayNode<T>* greaterMin = greater->getMin();
        if(lesserMax->value > greaterMin->value)
            throw std::invalid_argument("lesser tree must have lesser elements than every element in greater");

        splay(root->getMax());

        //У ньому тепер немає правого піддерева, робимо greater правим піддеревом.
        root->right = greater;
        if(greater)
            greater->parent = root;
    }
}

template<typename T>
void SplayTree<T>::mergeGreater(SplayTree& tree)
{
    if(tree.root != nullptr)
        mergeGreaterNode(this, tree.root);
    tree.root = nullptr;
}

/**
 * Розбиття дерева відносно елементу.
 */

/**
 * Повертає піддерево з елементами, більшими або рівними value.
 * this буде містити лише елементи, менші за value.
 */
template<typename T>
SplayNode<T>* SplayTree<T>::splitLesserNode(const T& value)
{
    //Знаходимо найменший елемент, менший або рівний value
    SplayNode<T>* newRoot = findMinNodeNoLessThan(value);
    if(newRoot == nullptr)
    {
        return nullptr;
    }

    splay(newRoot);

    SplayNode<T>* lesserNode = newRoot->left;
    newRoot->left = nullptr;
    return lesserNode;
}

/**
 * Те саме, тільки створює новий SplayTree<T>
 */
template<typename T>
SplayTree<T> SplayTree<T>::splitLesser(const T& value)
{
    SplayTree<T> lesserTree();
    lesserTree.root = splitLesserNode(value);
    return lesserTree;
}

#endif // REDBLACKTREE_H
