#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <stdexcept>
#include <iostream>

enum struct RedBlackColor
{
    Red, Black
};

template<typename T>
struct RedBlackStatNode
{
    RedBlackStatNode(T value, RedBlackStatNode<T>* parent = nullptr);
    ~RedBlackStatNode();

    T value;
    int size = 1;
    RedBlackColor color = RedBlackColor::Red;
    RedBlackStatNode<T>* parent = nullptr;
    RedBlackStatNode<T>* left = nullptr;
    RedBlackStatNode<T>* right = nullptr;

    RedBlackStatNode<T>* getMin();
    RedBlackStatNode<T>* getMax();
};

template<typename T>
RedBlackStatNode<T>::RedBlackStatNode(T value, RedBlackStatNode<T>* parent)
: value(value), parent(parent)
{}

template<typename T>
RedBlackStatNode<T>::~RedBlackStatNode()
{
    if(left != nullptr)
        delete left;
    if(right != nullptr)
        delete right;
}

template<typename T>
RedBlackStatNode<T>* RedBlackStatNode<T>::getMin()
{
    RedBlackStatNode<T>* currentNode = this;
    while(currentNode->left != nullptr)
        currentNode = currentNode->left;
    return currentNode;
}

template<typename T>
RedBlackStatNode<T>* RedBlackStatNode<T>::getMax()
{
    RedBlackStatNode<T>* currentNode = this;
    while(currentNode->right != nullptr)
        currentNode = currentNode->right;
    return currentNode;
}




template<typename T>
class RedBlackStatTree
{
public:
    RedBlackStatTree() = default;
    ~RedBlackStatTree();

    void insert(T value);
    bool remove(const T& value);
    bool contains(const T& value) const;
    T findByRank(int rank) const;
    int getRank(const T& value) const;

    void printInOrder();

protected:
private:
    RedBlackStatNode<T>* root = nullptr;

    void rotateLeft(RedBlackStatNode<T>* pivot);
    void rotateRight(RedBlackStatNode<T>* pivot);
    RedBlackStatNode<T>* getNext(RedBlackStatNode<T>* node) const;
    RedBlackStatNode<T>* getPrev(RedBlackStatNode<T>* node) const;

    RedBlackColor getColor(RedBlackStatNode<T>* node) const;
    int getSize(RedBlackStatNode<T>* node) const;

    void insertFix(RedBlackStatNode<T>* newNode);
    RedBlackStatNode<T>* remove(RedBlackStatNode<T>* node);
    void removeFix(RedBlackStatNode<T>* deleteNode);
    RedBlackStatNode<T>* findNode(const T& value) const;
    void printInOrder(RedBlackStatNode<T>* node, bool& firstPrint);
    T findByRank(int rank, RedBlackStatNode<T>* currentNode) const;
    int getRank(RedBlackStatNode<T>* node) const;
};

template<typename T>
RedBlackStatTree<T>::~RedBlackStatTree()
{
    if(root != nullptr)
        delete root;
}

/**
 * Стандартні функції для обходу дерева у порядку зростання.
 */

template<typename T>
void RedBlackStatTree<T>::printInOrder()
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
void RedBlackStatTree<T>::printInOrder(RedBlackStatNode<T>* node, bool& firstPrint)
{
    if(node->left != nullptr)
        printInOrder(node->left, firstPrint);

    if(!firstPrint)
        std::cout << ", ";
    else
        firstPrint = false;
    std::cout << getRank(node) << ": " << node->value;

    if(node->right != nullptr)
        printInOrder(node->right, firstPrint);
}

/**
 * У NIL-вузлів колір ввжається чорним.
 * Щоб уникати зайвих перевірок на nullptr, вводимо допоміжну функцію:
 */

template<typename T>
RedBlackColor RedBlackStatTree<T>::getColor(RedBlackStatNode<T>* node) const
{
    if(node == nullptr)
        return RedBlackColor::Black;
    else
        return node->color;
}


/**
 * У NIL-вузлів size ввжається 0.
 * Щоб уникати зайвих перевірок на nullptr, вводимо допоміжну функцію:
 */

template<typename T>
int RedBlackStatTree<T>::getSize(RedBlackStatNode<T>* node) const
{
    if(node == nullptr)
        return 0;
    else
        return node->size;
}

/**
 * Функції для знаходження наступного і попереднього елементів, виконуються за O(h)
 */
template<typename T>
RedBlackStatNode<T>* RedBlackStatTree<T>::getNext(RedBlackStatNode<T>* node) const
{
    if(node->right != nullptr)
        return node->right->getMin();

    RedBlackStatNode<T>* parent = node->parent;
    while(parent != nullptr && node == parent->right)
    {
        node = parent;
        parent = node->parent;
    }
    return parent;
}

template<typename T>
RedBlackStatNode<T>* RedBlackStatTree<T>::getPrev(RedBlackStatNode<T>* node) const
{
    if(node->left != nullptr)
        return node->left->getMax();

    RedBlackStatNode<T>* parent = node->parent;
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
void RedBlackStatTree<T>::insert(T value)
{
    //Спочатку вставка, як у звичайному бінарному дереві пошуку
    RedBlackStatNode<T>* parentNode = nullptr;
    RedBlackStatNode<T>* currentNode = root;

    while(currentNode != nullptr)
    {
        //Підтримка розміру піддерев
        currentNode->size++;
        parentNode = currentNode;
        if(value < currentNode->value)
            currentNode = currentNode->left;
        else
            currentNode = currentNode->right;
    }

    currentNode = new RedBlackStatNode<T>(value, parentNode);
    //(currentNode створюється червоним)
    if(parentNode == nullptr)
    {
        root = currentNode;
    }
    else
    {
        if(value < parentNode->value)
            parentNode->left = currentNode;
        else
            parentNode->right = currentNode;
    }
    //Тепер відновлюємо властивості червоно-чорного дерева.
    insertFix(currentNode);
}


template<typename T>
void RedBlackStatTree<T>::insertFix(RedBlackStatNode<T>* newNode)
{
    while(getColor(newNode->parent) == RedBlackColor::Red)
    {
        //(батько червоний, отєе існує дідусь)
        RedBlackStatNode<T>* grandparent = newNode->parent->parent;
        RedBlackStatNode<T>* uncle;

        if(newNode->parent == grandparent->left)
        {
            uncle = grandparent->right;
            if(getColor(uncle) == RedBlackColor::Red)
            {
                //Випадок 1
                /*  «Дідусь» чорний, «дядько» червоний.

                    «Батько» і «дядько» стають чорними, «дідусь»
                    червоним. Перевірка продовжується, бо предок дідуся може
                    виявитися червоним. */
                newNode->parent->color = RedBlackColor::Black;
                uncle->color = RedBlackColor::Black;
                grandparent->color = RedBlackColor::Red;
                newNode = grandparent;
            }
            else
            {
                if(newNode == newNode->parent->right)
                {
                    //Випадок 2
                    /*  «Дядько» чорний, х є правим потомком.

                        Лівий поворот відносно батька.
                        Зведення до випадку 3. */
                    newNode = newNode->parent;
                    rotateLeft(newNode);
                }

                //Випадок 3
                /*  «Дядько» чорний, х є лівим потомком.

                    Правий поворот відносно дідуся та зміна кольорів.
                    Вихід з алгоритму. */
                newNode->parent->color = RedBlackColor::Black;
                grandparent->color = RedBlackColor::Red;
                rotateRight(grandparent);
            }
        }
        else //Аналогічно, тільки міняємо місцями left і right
        {
            uncle = grandparent->left;
            if(getColor(uncle) == RedBlackColor::Red)
            {
                //Випадок 1
                newNode->parent->color = RedBlackColor::Black;
                uncle->color = RedBlackColor::Black;
                grandparent->color = RedBlackColor::Red;
                newNode = grandparent;
            }
            else
            {
                if(newNode == newNode->parent->left)
                {
                    //Випадок 2
                    newNode = newNode->parent;
                    rotateRight(newNode);
                }

                //Випадок 3
                newNode->parent->color = RedBlackColor::Black;
                grandparent->color = RedBlackColor::Red;
                rotateLeft(grandparent);
            }
        }
    }

    root->color = RedBlackColor::Black; //корінь завжди чорний
}


/**
 * Стандартні функції повороту для бінарного дерева.
 */
template<typename T>
void RedBlackStatTree<T>::rotateLeft(RedBlackStatNode<T>* pivot)
{
    RedBlackStatNode<T>* replaceNode = pivot->right;

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

    //Підтримка розміру піддерев
    replaceNode->size = pivot->size;
    pivot->size = getSize(pivot->left) + getSize(pivot->right) + 1;
}

template<typename T>
void RedBlackStatTree<T>::rotateRight(RedBlackStatNode<T>* pivot)
{
    RedBlackStatNode<T>* replaceNode = pivot->left;

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

    //Підтримка розміру піддерев
    replaceNode->size = pivot->size;
    pivot->size = getSize(pivot->left) + getSize(pivot->right) + 1;
}

/**
 * Функції для пошуку елемента у дереві
 */
template<typename T>
RedBlackStatNode<T>* RedBlackStatTree<T>::findNode(const T& value) const
{
    //Стандартний пошук у дереві бінарного пошуку
    RedBlackStatNode<T>* currentNode = root;
    while(currentNode != nullptr)
    {
        if(value == currentNode->value)
            return currentNode;
        else if(value < currentNode->value)
            currentNode = currentNode->left;
        else
            currentNode = currentNode->right;
    }
    return nullptr; //Не знайшли потрібний елемент
}

template<typename T>
bool RedBlackStatTree<T>::contains(const T& value) const
{
    return findNode(value) != nullptr;
}

/**
 * Функції для видалення елемента з дерева
 */
//Повертає true, якщо елемент знайдено і видалено. Якщо не знайдено, то falsr
template<typename T>
bool RedBlackStatTree<T>::remove(const T& value)
{
    RedBlackStatNode<T>* node = findNode(value);
    if(node == nullptr)
        return false;

    remove(node);
    return true;
}

template<typename T>
RedBlackStatNode<T>* RedBlackStatTree<T>::remove(RedBlackStatNode<T>* node)
{
    //Спочатку видалення як у звичайному бінарному дереві пошуку

    RedBlackStatNode<T>* deleteNode;
    if(node->left == nullptr || node->right == nullptr)
        deleteNode = node;
    else
        deleteNode = getNext(node);

    RedBlackStatNode<T>* replaceNode;
    if(deleteNode->left != nullptr)
        replaceNode = deleteNode->left;
    else
        replaceNode = deleteNode->right;

    if(replaceNode != nullptr)
        replaceNode->parent = deleteNode->parent;

    if(deleteNode->parent == nullptr)
    {
        root = replaceNode;
    }
    else
    {
        if(deleteNode == deleteNode->parent->left)
            deleteNode->parent->left = replaceNode;
        else
            deleteNode->parent->right = replaceNode;
    }

    if(deleteNode != node)
        node->value = deleteNode->value;

    //Підтримка розміру піддерев
    RedBlackStatNode<T>* sizeDecreaseNode = deleteNode->parent;
    while(sizeDecreaseNode != nullptr)
    {
        sizeDecreaseNode->size--;
        sizeDecreaseNode = sizeDecreaseNode->parent;
    }

    //При видаленні червоних вузлів властивості не порушуються
    if(getColor(deleteNode) == RedBlackColor::Black && replaceNode != nullptr)
        removeFix(replaceNode);

    return deleteNode;
}

template<typename T>
void RedBlackStatTree<T>::removeFix(RedBlackStatNode<T>* deleteNode)
{
    while(deleteNode != root && getColor(deleteNode) == RedBlackColor::Black)
    {
        RedBlackStatNode<T>* brother;

        if(deleteNode == deleteNode->parent->left)
        {
            //(брат чорного вузла не може бути NIL)
            brother = deleteNode->parent->right;
            if(getColor(brother) == RedBlackColor::Red)
            {
                //Випадок 1
                /*  «Брат» вершини x червоний.

                    Ліве обертання відносно батька та перефарбування.
                    Зведення до випадків 2, 3, 4. */
                brother->color = RedBlackColor::Black;
                deleteNode->parent->color = RedBlackColor::Red;
                rotateLeft(deleteNode->parent);
                brother = deleteNode->parent->right;
            }

            if(getColor(brother->left) == RedBlackColor::Black &&
               getColor(brother->right) == RedBlackColor::Black)
            {
                //Випадок 2
                /*  «Брат» вершини x чорний.
                    Обидва потомки «брата» чорні.

                    Перефарбовуємо «брата».
                    Далі розглядаємо вузла-батька. */
                brother->color = RedBlackColor::Red;
                deleteNode = deleteNode->parent;
            }
            else
            {
                if(getColor(brother->right) == RedBlackColor::Black)
                {
                    //Випадок 3.
                    /*  «Брат» вершини x чорний.
                        Лівий потомок «брата» червоний, правий чорний.

                        Перефарбовуємо «брата» і його лівого сина.
                        Правий поворот відносно брата.
                        Зведення до випадку 4. */
                    brother->left->color = RedBlackColor::Black;
                    brother->color = RedBlackColor::Red;
                    rotateRight(brother);
                    brother = deleteNode->parent->right;
                }

                //Випадок 4.
                /*  «Брат» вершини x чорний.
                    Правий потомок «брата» червоний.

                    Перефарбовуємо і робимо лівий поворот відносно батька.
                    Вихід з алгоритму. */
                brother->color = deleteNode->parent->color;
                deleteNode->parent->color = RedBlackColor::Black;
                brother->right->color = RedBlackColor::Black;
                rotateLeft(deleteNode->parent);
                break;
            }
        }
        else  //Аналогічно, тільки міняємо місцями left і right
        {
            brother = deleteNode->parent->left;
            if(getColor(brother) == RedBlackColor::Red)
            {
                //Випадок 1
                brother->color = RedBlackColor::Black;
                deleteNode->parent->color = RedBlackColor::Red;
                rotateRight(deleteNode->parent);
                brother = deleteNode->parent->left;
            }

            if(getColor(brother->right) == RedBlackColor::Black &&
               getColor(brother->left) == RedBlackColor::Black)
            {
                //Випадок 2
                brother->color = RedBlackColor::Red;
                deleteNode = deleteNode->parent;
            }
            else
            {
                if(getColor(brother->left) == RedBlackColor::Black)
                {
                    //Випадок 3.
                    brother->right->color = RedBlackColor::Black;
                    brother->color = RedBlackColor::Red;
                    rotateLeft(brother);
                    brother = deleteNode->parent->left;
                }


                //Випадок 4.
                brother->color = deleteNode->parent->color;
                deleteNode->parent->color = RedBlackColor::Black;
                brother->left->color = RedBlackColor::Black;
                rotateRight(deleteNode->parent);
                break;
            }
        }
    }
    deleteNode->color = RedBlackColor::Black;
}

/**
 * Дерево порядкової статистики
 */
//Пошук елемента з рангом rank
template<typename T>
T RedBlackStatTree<T>::findByRank(int rank) const
{
    if(root == nullptr)
        throw std::invalid_argument("Tree does not contain that many elements");
    if(rank <= 0)
        throw std::invalid_argument("rank must be more than 0");
    return findByRank(rank, root);
}

template<typename T>
T RedBlackStatTree<T>::findByRank(int rank, RedBlackStatNode<T>* currentNode) const
{
    //(size[left[x]]+1) – ранг вузла x в піддереві з коренем x.
    int currentRank = getSize(currentNode->left) + 1;
    if(rank == currentRank)
        return currentNode->value;
    else if(rank < currentRank)
    {
        //Шукаємо в лівому піддереві, серед менших елементів
        //(перевірку на null не робимо, адже вважаємо, що rank > 0)
        return findByRank(rank, currentNode->left);
    }
    else
    {
        //Шукаємо в правому піддереві, серед більших елементів
        if(!currentNode->right)
            throw std::invalid_argument("Tree does not contain that many elements");
        return findByRank(rank - currentRank, currentNode->right);
    }
}

template<typename T>
int RedBlackStatTree<T>::getRank(const T& value) const
{
    RedBlackStatNode<T>* node = findNode(value);
    if(!node)
        throw std::invalid_argument("Tree does not contain value");
    return getRank(node);
}

template<typename T>
int RedBlackStatTree<T>::getRank(RedBlackStatNode<T>* node) const
{
    /*Ранг вузла x – кількість вершин, обійдених при
    симетричному обході до x, плюс 1 для самого вузла x.*/

    //В циклі рухаємось від x до кореня;
    int currentRank = getSize(node->left) + 1;
    while(node != root)
    {
        /*якщо по дорозі проходимо елемент, що є чиїмось
        правим сином, додаємо кількість елементів, які треба
        обійти перед ним, плюс 1 для нього.*/
        if(node == node->parent->right)
            currentRank += getSize(node->parent->left) + 1;
        node = node->parent;
    }
    return currentRank;
}

#endif // REDBLACKTREE_H
