#ifndef OPTIMALBST_H
#define OPTIMALBST_H

#include <cstddef>
#include <utility>
#include <vector>
#include <limits>
#include <stdexcept>

#include "utils.hpp"
using utils::get2D;


/**
 * Структура звичайного бінарного дерева пошуку.
 */
template<typename T>
struct BSTNode
{
    T value;
    BSTNode* left = nullptr;
    BSTNode* right = nullptr;

    BSTNode(const T& value);

    void printStructure(int depth);
    BSTNode<T>* findNode(const T& value);
    void deleteChildren();
};

template<typename T>
BSTNode<T>::BSTNode(const T& value)
: value(value)
{}

/**
 * Функція для виводу даних, що зберігаються у дереві.
 * Показує зв'язки між вузлами.
 */
template<typename T>
void BSTNode<T>::printStructure(int depth)
{
    for(int i = 0; i < depth; i++)
        std::cout << "   ";
    std::cout << value << std::endl;

    if(left)
    {
        for(int i = 0; i < depth; i++)
            std::cout << "   ";
        std::cout << "Left:" << std::endl;
        left->printStructure(depth + 1);
    }

    if(right)
    {
        for(int i = 0; i < depth; i++)
            std::cout << "   ";
        std::cout << "Right:" << std::endl;
        right->printStructure(depth + 1);
    }
}

/**
 * Функції для пошуку елемента у дереві
 */
template<typename T>
BSTNode<T>* BSTNode<T>::findNode(const T& value)
{
    if(value == this->value)
        return this;
    else if(value < this->value && left)
        return left->findNode(value);
    else if(value > this->value && right)
        return right->findNode(value);
    return nullptr; //Не знайшли потрібний елемент
}

/**
 * Стандартна функція для видалення (під)дерева
 */
template<typename T>
void BSTNode<T>::deleteChildren()
{
    if(left)
    {
        left->deleteChildren();
        delete left;
    }
    if(right)
    {
        right->deleteChildren();
        delete right;
    }
}



/**
 * Клас для побудови оптимального бін. дерева пошуку та роботи з ним.
 */
template<typename T>
class OptimalBST
{
public:
    /**
     * \param values масив елементів A0, A1 .. An-1, для яких відома "ціна". Мають бути відсортовані.
     * \param freqs ціна елементів A0, A1 .. An-1
     * \param auxFreqs ціна фіктивних елементів B0, B1, Bn, де Bi знаходиться між Ai-1 та Ai
     */
    OptimalBST(const std::vector<T>& values, const std::vector<float>& freqs, const std::vector<float>& auxFreqs);
    virtual ~OptimalBST();

    void printStructure();
    bool contains(const T& value);

private:
    /**
     * Будує бін. дерево пошуку з таблиці roots
     * де roots [i, j] - індекс кореня оптимального піддерева, що містить елементи Ai..Aj
     * (щоб знайти корінь всього дерева, виконуємо функцію з i = 0, j = size - 1
     */
    BSTNode<T>* buildBST(size_t i, size_t j, size_t* roots, const std::vector<T>& values);

    BSTNode<T>* root;
};

template<typename T>
OptimalBST<T>::OptimalBST(const std::vector<T>& values, const std::vector<float>& freqs, const std::vector<float>& auxFreqs)
{
    if(values.empty())
    {
        root = nullptr;
        return;
    }

    //Перевірка на правильність вхідних даних

    size_t size = values.size();
    if(freqs.size() != size)
        throw std::invalid_argument("freqs vector must have n values");
    if(auxFreqs.size() != size + 1)
        throw std::invalid_argument("auxFreqs vector must have n+1 values");


    for(size_t i = 0; i < size - 1; i++)
    {
        if(values[i] > values[i + 1])
            throw std::invalid_argument("values must be sorted");
    }

    size_t* roots = new size_t[size * size];

    //costOBST - таблиця розміром n+1 на n+1,
    //в якій costOBST[i, j] - ціна пошуку оптимального дерева, що містить елементи Ai-1, .. Aj-1
    //та фіктивні елементи Bi-1, .. Bj-1. Bj
    //Базовий випадок: якщо j = i - 1, то cost[i, i-1] = ціна Bi-1
    float* costOBST = new float[(size + 1) * (size + 1)]();

    //sumCost[i, j] - сума вірогідностей елементів Ai-1, .. Aj-1
    //та відповідних фіктивних елементів Bi-1, .. Bj-1, Bj
    //Базовий випадок: якщо j = i - 1, то sumCost[i, i-1] = ціна Bi-1
    float* sumCost = new float[(size + 1) * (size + 1)]();

    //Базові випадки:
    for(size_t i = 0; i < size; i++)
    {
        //costOBST[i+1, i] = sumCost[i+1, i] = вірогідність елемента Bi
        get2D(costOBST, size + 1, i + 1, i) = auxFreqs[i];
        get2D(sumCost, size + 1, i + 1, i) = auxFreqs[i];
    }

    //"Bottom-up":
    //Далі рахуємо значення constOBST і sumCost для піддерев розміром 1, 2, 3 і т.д. до size
    //Потім costOBST[0, size - 1] стане результатом для всього дерева

    for(size_t n = 1; n <= size; n++)
    {
        for(size_t i = 1; i <= size - n + 1; i++)
        {
            size_t j = i + n - 1;

            //Ми знаємо суму цін для i..j-1, легко знаходимо для i..j
            get2D(sumCost, size + 1, i, j) = get2D(sumCost, size + 1, i, j - 1) + freqs[j - 1] + auxFreqs[j];

            //Знаходимо мінімальну ціну піддерева для i..j
            //Спочатку виставляємо найбільше можливе значення, потім поступово будемо заміняти на менші
            get2D(costOBST, size + 1, i, j) = std::numeric_limits<float>::infinity();

            for(size_t r = i; r <= j; r++)
            {
                //Перебираємо r - індекси кореня для піддерева i..j

                float cost = get2D(sumCost, size + 1, i, j)
                        + get2D(costOBST, size + 1, i, r - 1);
                if(r < j)
                    cost += get2D(costOBST, size + 1, r + 1, j);

                //Якщо це найкращий із відомих cost, зберігаємо його.
                if(cost < get2D(costOBST, size + 1, i, j))
                {
                    get2D(costOBST, size + 1, i, j) = cost;
                    get2D(roots, size, i - 1, j - 1) = r - 1; //Також зберігаємо "оптимальний" корінь
                }
            }
        }
    }

    //costOBST[1, size] - ціня оптимального бін. дерева для всіх елементів.
    std::cout << "Optimal cost: " << get2D(costOBST, size + 1, 1, size) << std::endl;

    //Тепер будуємо бін. дерево з "оптимальних" корнів
    root = buildBST(0, size - 1, roots, values);


    delete[] roots;
    delete[] costOBST;
    delete[] sumCost;
}

template<typename T>
BSTNode<T>* OptimalBST<T>::buildBST(size_t i, size_t j, size_t* roots, const std::vector<T>& values)
{
    size_t size = values.size();

    //Корінь оптимального дерева з елементами Ai..Aj
    size_t rootIndex = get2D(roots, size, i, j);
    BSTNode<T>* node = new BSTNode<T>(values[rootIndex]);

    //Якщо є піддерева, шукаємо їх корні
    if(rootIndex > i)
        node->left = buildBST(i, rootIndex - 1, roots, values);
    if(rootIndex < j)
        node->right = buildBST(rootIndex + 1, j, roots, values);

    return node;
}

//Функції для роботи з власне бінарним деревом.
template<typename T>
void OptimalBST<T>::printStructure()
{
    if(!root)
        std::cout << "<empty>" << std::endl;
    else
        root->printStructure(0);
}

template<typename T>
bool OptimalBST<T>::contains(const T& value)
{
    if(!root)
        return false;
    BSTNode<T>* result = root->findNode(value);
    return result != nullptr;
}

template<typename T>
OptimalBST<T>::~OptimalBST()
{
    if(root)
    {
        root->deleteChildren();
        delete root;
    }
}

#endif // OPTIMALBST_H
