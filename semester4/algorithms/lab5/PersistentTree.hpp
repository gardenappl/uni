#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <stack>
#include <stdexcept>
#include <iostream>
#include <vector>

enum struct RedBlackColor
{
    Red, Black
};

template<typename T>
struct RedBlackNode
{
    RedBlackNode(T value);

    //Звільнення пам'яті виконується класом RedBlackTree
    //~RedBlackNode();

    //Зауважте, що не зберігається вказівник на parent.

    T value;
    RedBlackColor color = RedBlackColor::Red;
    RedBlackNode<T>* left = nullptr;
    RedBlackNode<T>* right = nullptr;

    //Пошук мінімільного вузла серед піддерев
    //Якщо стак ancestors != nullptr, то до нього додамо всіх предків на шляху до результату
    RedBlackNode<T>* getMin(std::deque<RedBlackNode<T>*>* ancestors = nullptr);
};

template<typename T>
RedBlackNode<T>::RedBlackNode(T value)
: value(value)
{}

//template<typename T>
//RedBlackNode<T>::~RedBlackNode()
//{
//    if(left != nullptr)
//        delete left;
//    if(right != nullptr)
//        delete right;
//}

template<typename T>
RedBlackNode<T>* RedBlackNode<T>::getMin(std::deque<RedBlackNode<T>*>* ancestors)
{
    RedBlackNode<T>* currentNode = this;
    while(currentNode->left != nullptr)
    {
        if(ancestors)
            ancestors->push_back(currentNode);
        currentNode = currentNode->left;
    }
    return currentNode;
}






template<typename T>
class PersistentTree
{
public:
    PersistentTree();
    ~PersistentTree();

    void insert(T value);
    size_t getLatestVersion() const;
    void revert(size_t versionNum);
    bool remove(const T& value);
    bool contains(const T& value, size_t versionNum = 0) const;

    void printInOrder(size_t versionNum = 0) const;
    void printStructure(size_t versionNum = 0) const;

protected:
private:
    //Список коренів для різних версій дерева, від найстаршої до найновішої
    std::vector<RedBlackNode<T>*> roots;

    //Список нововведенних вузлів, що відповідають і-тій версії кореня.
    std::vector<std::vector<RedBlackNode<T>*>> versionNodes;

    //Створює нову версію даного вузла
    RedBlackNode<T>* clone(RedBlackNode<T>* node, RedBlackNode<T>* clonedParent);

    void rotateLeft(RedBlackNode<T>* pivot, RedBlackNode<T>* parent);
    void rotateRight(RedBlackNode<T>* pivot, RedBlackNode<T>* parent);

    RedBlackNode<T>* getNext(RedBlackNode<T>* node, std::deque<RedBlackNode<T>*>& ancestors) const;

    RedBlackColor getColor(RedBlackNode<T>* node) const;

    //Відновлення властивостей червоно-чорного дерева
    void insertFix(RedBlackNode<T>* newNode, std::deque<RedBlackNode<T>*>& ancestors);
    RedBlackNode<T>* remove(RedBlackNode<T>* node, std::deque<RedBlackNode<T>*>& ancestors);
    void removeFix(RedBlackNode<T>* deleteNode, std::deque<RedBlackNode<T>*>& ancestors);

    //Знаходження елемента, що містить value
    //(використовується для публічного методу remove(const T& value);
    RedBlackNode<T>* findNode(RedBlackNode<T>* root, const T& value) const;
    RedBlackNode<T>* findNode(RedBlackNode<T>* root, const T& value, std::deque<RedBlackNode<T>*>& ancestors);

    //Допоміжні функції для виводу дерева, рекурсивні
    void printInOrder(const RedBlackNode<T>* node, bool& firstPrint) const;
    void printStructure(const RedBlackNode<T>* node, int depth) const;
};

template<typename T>
PersistentTree<T>::~PersistentTree()
{
    for (std::vector<RedBlackNode<T>*>& version : versionNodes)
    {
        for (RedBlackNode<T>* node : version)
            delete node;
    }
}

template<typename T>
PersistentTree<T>::PersistentTree()
{
    //Найперша версія - пусте дерево
    roots.push_back(nullptr);
    versionNodes.emplace_back();
}

template<typename T>
size_t PersistentTree<T>::getLatestVersion() const
{
    return roots.size() - 1;
}

template<typename T>
void PersistentTree<T>::revert(size_t versionNum)
{
    for (size_t i = versionNodes.size() - 1; i > versionNum; i--)
    {
        for (RedBlackNode<T>* node : versionNodes[i])
            delete node;
        versionNodes.pop_back();
        roots.pop_back();
    }
}

/**
 * Стандартні функції для обходу дерева у порядку зростання.
 * \param versionNum номер версії дерева. Якщо versionNum == 0, то друкує останню версію
 */

template<typename T>
void PersistentTree<T>::printInOrder(size_t versionNum) const
{
    if(versionNum == 0)
        versionNum = roots.size() - 1;
    else if(versionNum >= roots.size())
        throw std::invalid_argument("Version not available");

    if(!roots[versionNum])
    {
        std::cout << "<empty>";
        return;
    }
    std::cout << "{ ";
    bool firstPrint = true;
    printInOrder(roots[versionNum], firstPrint);
    std::cout << " }";
}

template<typename T>
void PersistentTree<T>::printInOrder(const RedBlackNode<T>* node, bool& firstPrint) const
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

template<typename T>
void PersistentTree<T>::printStructure(size_t versionNum) const
{
    if(versionNum == 0)
        versionNum = roots.size() - 1;
    else if(versionNum >= roots.size())
        throw std::invalid_argument("Version not available");

    if(!roots[versionNum])
    {
        std::cout << "<empty>\n";
        return;
    }
    printStructure(roots[versionNum], 0);
}

template<typename T>
void PersistentTree<T>::printStructure(const RedBlackNode<T>* node, int depth) const
{
    if(node->left)
    {
        for(int i = 0; i < depth; i++)
            std::cout << "    ";
        std::cout << "Left:" << std::endl;
        printStructure(node->left, depth + 1);
    }

    for(int i = 0; i < depth; i++)
        std::cout << "   ";
    if(node->color == RedBlackColor::Red)
        std::cout << "RED ";
    else
        std::cout << "BLK ";
    std::cout << node->value << std::endl;

    if(node->right)
    {
        for(int i = 0; i < depth; i++)
            std::cout << "   ";
        std::cout << "Right:" << std::endl;
        printStructure(node->right, depth + 1);
    }
}



/**
 * У NIL-вузлів колір ввжається чорним.
 * Щоб уникати зайвих перевірок на nullptr, вводимо допоміжну функцію:
 */

template<typename T>
RedBlackColor PersistentTree<T>::getColor(RedBlackNode<T>* node) const
{
    if(node == nullptr)
        return RedBlackColor::Black;
    else
        return node->color;
}

/**
 * Створює оновлену версію node і приєднує її до clonedParent.
 * Вважаємо, що clonedParent вже є останнією версією.
 */
template<typename T>
RedBlackNode<T>* PersistentTree<T>::clone(RedBlackNode<T>* node, RedBlackNode<T>* clonedParent)
{
    if (!node)
        return nullptr;

    //Створюємо клон
    RedBlackNode<T>* clone = new RedBlackNode<T>(node->value);
    clone->color = node->color;
    clone->left = node->left;
    clone->right = node->right;
    if(clonedParent)
    {
        //Приєднуємо нового сина до нового батька
        if(node == clonedParent->left)
            clonedParent->left = clone;
        else
            clonedParent->right = clone;
    }
    else
    {
        //Батька немає: це новий корінь
        roots.push_back(clone);
    }

    versionNodes.back().push_back(clone);

    return clone;
}



/**
 * Функція для знаходження наступного елемента, виконується за O(h)
 * Стак parents буде вказувати на батьків знайденого вузла
 */
template<typename T>
RedBlackNode<T>* PersistentTree<T>::getNext(RedBlackNode<T>* node, std::deque<RedBlackNode<T>*>& ancestors) const
{
    if(node->right != nullptr)
    {
        ancestors.push_back(node);
        return node->right->getMin(&ancestors);
    }

    RedBlackNode<T>* parent = ancestors.back();
    ancestors.pop_back();
    while(parent != nullptr && node == parent->right)
    {
        node = parent;
        parent = ancestors.back();
        ancestors.pop_back();
    }
    return parent;
}



/**
 * Функції для вставки нового елемента у дерево
 * (створює нову версію дерева)
 */

template<typename T>
void PersistentTree<T>::insert(T value)
{
    versionNodes.emplace_back();

    //Спочатку вставка, як у звичайному бінарному дереві пошуку
    //При цьому всі вузли на нашому шляху копіюємо

    RedBlackNode<T>* parentNode = nullptr;
    RedBlackNode<T>* currentNode = roots.back();

    //(також зберігаємо всіх предків на нашому шляху у стак ancestors,
    // який використається у функції insertFix)
    std::deque<RedBlackNode<T>*> ancestors;

    while(currentNode != nullptr)
    {
        currentNode = clone(currentNode, parentNode);

        ancestors.push_back(currentNode);

        parentNode = currentNode;
        if(value < currentNode->value)
            currentNode = currentNode->left;
        else
            currentNode = currentNode->right;
    }

    currentNode = new RedBlackNode<T>(value);
    //(currentNode створюється червоним)

    if(!parentNode)
    {
        roots.push_back(currentNode);

        //корінь завжди чорний
        roots.back()->color = RedBlackColor::Black;
    }
    else
    {
        if(value < parentNode->value)
            parentNode->left = currentNode;
        else
            parentNode->right = currentNode;

        //Тепер відновлюємо властивості червоно-чорного дерева.
        insertFix(currentNode, ancestors);
    }
}

template<typename T>
void PersistentTree<T>::insertFix(RedBlackNode<T>* newNode, std::deque<RedBlackNode<T>*>& ancestors)
{
    while(true)
    {
        //Використовуємо ancestors, оскільки персистентна множина не може зберігати parent для кожного вузла
        RedBlackNode<T>* parent = nullptr;
        if(!ancestors.empty())
        {
            parent = ancestors.back();
            ancestors.pop_back();
        }

        //Якщо батько чорний, то властивості не будуть порушуватись
        if(getColor(parent) == RedBlackColor::Black)
            break;


        //(батько червоний, отже існує дідусь)
        RedBlackNode<T>* grandparent = ancestors.back();
        RedBlackNode<T>* uncle;

        if(parent == grandparent->left)
        {
            uncle = clone(grandparent->right, grandparent);
            if(getColor(uncle) == RedBlackColor::Red)
            {
                //Випадок 1
                /*  «Дідусь» чорний, «дядько» червоний.

                    «Батько» і «дядько» стають чорними, «дідусь»
                    червоним. Перевірка продовжується, бо предок дідуся може
                    виявитися червоним. */
                parent->color = RedBlackColor::Black;
                uncle->color = RedBlackColor::Black;
                grandparent->color = RedBlackColor::Red;
                newNode = grandparent;
                ancestors.pop_back();
                continue;
            }
            else
            {
                if(newNode == parent->right)
                {
                    //Випадок 2
                    /*  «Дядько» чорний, х є правим потомком.

                        Лівий поворот відносно батька.
                        Зведення до випадку 3. */
                    clone(parent->right, parent);
                    rotateLeft(parent, grandparent);
                    std::swap(newNode, parent);
                }

                //Випадок 3
                /*  «Дядько» чорний, х є лівим потомком.

                    Правий поворот відносно дідуся та зміна кольорів.
                    Вихід з алгоритму. */
                parent->color = RedBlackColor::Black;
                grandparent->color = RedBlackColor::Red;

                ancestors.pop_back();
                RedBlackNode<T>* grandgrandparent = nullptr;
                if(!ancestors.empty())
                    grandgrandparent = ancestors.back();

                rotateRight(grandparent, grandgrandparent);
                break;
            }
        }
        else //Аналогічно, тільки міняємо місцями left і right
        {
            uncle = clone(grandparent->left, grandparent);
            if(getColor(uncle) == RedBlackColor::Red)
            {
                //Випадок 1
                parent->color = RedBlackColor::Black;
                uncle->color = RedBlackColor::Black;
                grandparent->color = RedBlackColor::Red;
                newNode = grandparent;
                ancestors.pop_back();
                continue;
            }
            else
            {
                if(newNode == parent->left)
                {
                    //Випадок 2
                    clone(parent->left, parent);
                    rotateRight(parent, grandparent);
                    std::swap(newNode, parent);
                }

                //Випадок 3
                parent->color = RedBlackColor::Black;
                grandparent->color = RedBlackColor::Red;

                ancestors.pop_back();
                RedBlackNode<T>* grandgrandparent = nullptr;
                if(!ancestors.empty())
                    grandgrandparent = ancestors.back();

                rotateLeft(grandparent, grandgrandparent);
                break;
            }
        }
    }

    roots.back()->color = RedBlackColor::Black; //корінь завжди чорний
}



/**
 * Стандартні функції повороту для бінарного дерева.
 * parent, pivot та pivot->right вважаються останньої версії
 */
template<typename T>
void PersistentTree<T>::rotateLeft(RedBlackNode<T>* pivot, RedBlackNode<T>* parent)
{
    RedBlackNode<T>* replaceNode = pivot->right;

    pivot->right = replaceNode->left;
    if(replaceNode->left != nullptr)
        replaceNode->left = pivot;

    if(pivot == roots.back())
    {
        roots.back() = replaceNode;
    }
    else
    {
        if(pivot == parent->left)
            parent->left = replaceNode;
        else
            parent->right = replaceNode;
    }
    replaceNode->left = pivot;
}


/**
 * Стандартні функції повороту для бінарного дерева.
 * parent, pivot та pivot->left вважаються останньої версії
 */
template<typename T>
void PersistentTree<T>::rotateRight(RedBlackNode<T>* pivot, RedBlackNode<T>* parent)
{
    RedBlackNode<T>* replaceNode = pivot->left;

    pivot->left = replaceNode->right;
    if(replaceNode->right != nullptr)
        replaceNode->right = pivot;

    if(pivot == roots.back())
    {
        roots.back() = replaceNode;
    }
    else
    {
        if(pivot == parent->left)
            parent->left = replaceNode;
        else
            parent->right = replaceNode;
    }
    replaceNode->right = pivot;
}



/**
 * Функції для пошуку елемента у дереві
 */
template<typename T>
RedBlackNode<T>* PersistentTree<T>::findNode(RedBlackNode<T>* root, const T& value) const
{
    //Стандартний пошук у дереві бінарного пошуку
    RedBlackNode<T>* currentNode = root;
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

/**
 * \param ancestors стак з усіма "предками" знайденого вузла, спускаючись від кореня вниз
 */
template<typename T>
RedBlackNode<T>* PersistentTree<T>::findNode(RedBlackNode<T>* root, const T& value, std::deque<RedBlackNode<T>*>& ancestors)
{
    //Стандартний пошук у дереві бінарного пошуку
    RedBlackNode<T>* currentNode = root;
    while(currentNode != nullptr)
    {
        if(value == currentNode->value)
            return currentNode;

        ancestors.push_back(currentNode);
        if(value < currentNode->value)
            currentNode = currentNode->left;
        else
            currentNode = currentNode->right;
    }
    return nullptr; //Не знайшли потрібний елемент
}

template<typename T>
bool PersistentTree<T>::contains(const T& value, size_t versionNum) const
{
    if(versionNum == 0)
        versionNum = roots.size() - 1;
    else if(versionNum >= roots.size())
        throw std::invalid_argument("Version not available");

    return findNode(roots[versionNum], value) != nullptr;
}

/**
 * Функції для видалення елемента з дерева
 */
//Повертає true, якщо елемент знайдено і видалено. Якщо не знайдено, то false
template<typename T>
bool PersistentTree<T>::remove(const T& value)
{
    std::deque<RedBlackNode<T>*> ancestors;
    RedBlackNode<T>* node = findNode(roots.back(), value, ancestors);

    if(node == nullptr)
        return false;

    if(node == roots.back())
        roots.push_back(nullptr);

    remove(node, ancestors);
    return true;
}

template<typename T>
RedBlackNode<T>* PersistentTree<T>::remove(RedBlackNode<T>* node, std::deque<RedBlackNode<T>*>& ancestors)
{
    //Спочатку видалення, як у звичайному бінарному дереві пошуку

    //(також зберігаємо всіх предків на нашому шляху у стак ancestors,
    // який використається у функції removeFix)

    RedBlackNode<T>* deleteNode;
    if(node->left == nullptr || node->right == nullptr)
        deleteNode = node;
    else
        //оскільки node->right існує, то наступний вузол getNext() матиме спільних предків
        deleteNode = getNext(node, ancestors);

    //Створюємо клони
    versionNodes.emplace_back();
    std::deque<RedBlackNode<T>*> clonedAncestors;
    if (ancestors.empty())
    {
        clone(node, nullptr);
    }
    else
    {
        RedBlackNode<T>* parentNode = nullptr;
        for (RedBlackNode<T>* currentNode : ancestors)
        {
            RedBlackNode<T>* nodeClone = clone(currentNode, parentNode);
            if (currentNode == node)
                node = nodeClone;
            currentNode = nodeClone;
            clonedAncestors.push_back(currentNode);

            parentNode = currentNode;
        }
    }
    if (deleteNode == node)
    {
        deleteNode = clone(deleteNode, clonedAncestors.back());
        node = deleteNode;
    }
    else
    {
        deleteNode = clone(deleteNode, clonedAncestors.back());
    }


    RedBlackNode<T>* replaceNode;
    if(deleteNode->left != nullptr)
        replaceNode = clone(deleteNode->left, deleteNode);
    else
        replaceNode = clone(deleteNode->right, deleteNode);


    if(deleteNode == roots.back())
    {
        roots.back() = replaceNode;
    }
    else
    {
        if(deleteNode == clonedAncestors.back()->left)
            clonedAncestors.back()->left = replaceNode;
        else
            clonedAncestors.back()->right = replaceNode;
    }

    if(deleteNode != node)
        node->value = deleteNode->value;

    //При видаленні червоних вузлів властивості не порушуються
    if(getColor(deleteNode) == RedBlackColor::Black)
        removeFix(replaceNode, clonedAncestors);

    return deleteNode;
}

template<typename T>
void PersistentTree<T>::removeFix(RedBlackNode<T>* deleteNode, std::deque<RedBlackNode<T>*>& ancestors)
{
    while(deleteNode != roots.back() && getColor(deleteNode) == RedBlackColor::Black)
    {
        //Використовуємо ancestors, оскільки персистентна множина не може зберігати parent для кожного вузла
        RedBlackNode<T>* parent = nullptr;
        if(!ancestors.empty())
        {
            parent = ancestors.back();
            ancestors.pop_back();
        }
        RedBlackNode<T>* grandparent = nullptr;
        if(!ancestors.empty())
            grandparent = ancestors.back();

        RedBlackNode<T>* brother;


        if(deleteNode == parent->left)
        {
            //(брат чорного вузла не може бути NIL)
            brother = clone(parent->right, parent);
            if(getColor(brother) == RedBlackColor::Red)
            {
                //Випадок 1
                /*  «Брат» вершини x червоний.

                    Ліве обертання відносно батька та перефарбування.
                    Зведення до випадків 2, 3, 4. */
                brother->color = RedBlackColor::Black;
                parent->color = RedBlackColor::Red;
                rotateLeft(parent, grandparent);
                brother = parent->right;
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
                deleteNode = parent;
                continue;
            }
            else
            {
                bool brotherRightCloned = false;
                if(getColor(brother->right) == RedBlackColor::Black)
                {
                    //Випадок 3.
                    /*  «Брат» вершини x чорний.
                        Лівий потомок «брата» червоний, правий чорний.

                        Перефарбовуємо «брата» і його лівого сина.
                        Правий поворот відносно брата.
                        Зведення до випадку 4. */
                    RedBlackNode<T>* brotherLeft = clone(brother->left, brother);
                    brotherLeft->color = RedBlackColor::Black;
                    brother->color = RedBlackColor::Red;
                    rotateRight(brother, parent);
                    brotherRightCloned = true;
                    brother = parent->right;
                }

                //Випадок 4.
                /*  «Брат» вершини x чорний.
                    Правий потомок «брата» червоний.

                    Перефарбовуємо і робимо лівий поворот відносно батька.
                    Вихід з алгоритму. */
                RedBlackNode<T>* brotherRight = brother->right;
                if (!brotherRightCloned)
                    brotherRight = clone(brotherRight, brother);

                brother->color = parent->color;
                parent->color = RedBlackColor::Black;
                brotherRight->color = RedBlackColor::Black;
                rotateLeft(parent, grandparent);
                break;
            }
        }
        else  //Аналогічно, тільки міняємо місцями left і right
        {
            brother = clone(parent->left, parent);
            if(getColor(brother) == RedBlackColor::Red)
            {
                //Випадок 1
                brother->color = RedBlackColor::Black;
                parent->color = RedBlackColor::Red;
                rotateRight(parent, grandparent);
                brother = parent->left;
            }

            if(getColor(brother->right) == RedBlackColor::Black &&
               getColor(brother->left) == RedBlackColor::Black)
            {
                //Випадок 2
                brother->color = RedBlackColor::Red;
                deleteNode = parent;
                continue;
            }
            else
            {
                bool brotherLeftCloned = false;
                if(getColor(brother->left) == RedBlackColor::Black)
                {
                    //Випадок 3.
                    RedBlackNode<T>* brotherRight = clone(brother->right, brother);
                    brotherRight->color = RedBlackColor::Black;
                    brother->color = RedBlackColor::Red;
                    rotateLeft(brother, parent);
                    brotherLeftCloned = true;
                    brother = parent->left;
                }


                //Випадок 4.
                RedBlackNode<T>* brotherLeft = brother->left;
                if (!brotherLeftCloned)
                    brotherLeft = clone(brotherLeft, brother);

                brother->color = parent->color;
                parent->color = RedBlackColor::Black;
                brotherLeft->color = RedBlackColor::Black;
                rotateRight(parent, grandparent);
                break;
            }
        }
    }
    if(deleteNode)
        deleteNode->color = RedBlackColor::Black;
}

#endif // REDBLACKTREE_H
