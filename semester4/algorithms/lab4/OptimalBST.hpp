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
 * ��������� ���������� �������� ������ ������.
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
 * ������� ��� ������ �����, �� ����������� � �����.
 * ������ ��'���� �� �������.
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
 * ������� ��� ������ �������� � �����
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
    return nullptr; //�� ������� �������� �������
}

/**
 * ���������� ������� ��� ��������� (��)������
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
 * ���� ��� �������� ������������ ��. ������ ������ �� ������ � ���.
 */
template<typename T>
class OptimalBST
{
public:
    /**
     * \param values ����� �������� A0, A1 .. An-1, ��� ���� ����� "����". ����� ���� ����������.
     * \param freqs ���� �������� A0, A1 .. An-1
     * \param auxFreqs ���� ��������� �������� B0, B1, Bn, �� Bi ����������� �� Ai-1 �� Ai
     */
    OptimalBST(const std::vector<T>& values, const std::vector<float>& freqs, const std::vector<float>& auxFreqs);
    virtual ~OptimalBST();

    void printStructure();
    bool contains(const T& value);

private:
    /**
     * ���� ��. ������ ������ � ������� roots
     * �� roots [i, j] - ������ ������ ������������ ��������, �� ������ �������� Ai..Aj
     * (��� ������ ����� ������ ������, �������� ������� � i = 0, j = size - 1
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

    //�������� �� ����������� ������� �����

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

    //costOBST - ������� ������� n+1 �� n+1,
    //� ��� costOBST[i, j] - ���� ������ ������������ ������, �� ������ �������� Ai-1, .. Aj-1
    //�� ������� �������� Bi-1, .. Bj-1. Bj
    //������� �������: ���� j = i - 1, �� cost[i, i-1] = ���� Bi-1
    float* costOBST = new float[(size + 1) * (size + 1)]();

    //sumCost[i, j] - ���� ����������� �������� Ai-1, .. Aj-1
    //�� ��������� ��������� �������� Bi-1, .. Bj-1, Bj
    //������� �������: ���� j = i - 1, �� sumCost[i, i-1] = ���� Bi-1
    float* sumCost = new float[(size + 1) * (size + 1)]();

    //����� �������:
    for(size_t i = 0; i < size; i++)
    {
        //costOBST[i+1, i] = sumCost[i+1, i] = ��������� �������� Bi
        get2D(costOBST, size + 1, i + 1, i) = auxFreqs[i];
        get2D(sumCost, size + 1, i + 1, i) = auxFreqs[i];
    }

    //"Bottom-up":
    //��� ������ �������� constOBST � sumCost ��� ������� ������� 1, 2, 3 � �.�. �� size
    //���� costOBST[0, size - 1] ����� ����������� ��� ������ ������

    for(size_t n = 1; n <= size; n++)
    {
        for(size_t i = 1; i <= size - n + 1; i++)
        {
            size_t j = i + n - 1;

            //�� ����� ���� ��� ��� i..j-1, ����� ��������� ��� i..j
            get2D(sumCost, size + 1, i, j) = get2D(sumCost, size + 1, i, j - 1) + freqs[j - 1] + auxFreqs[j];

            //��������� �������� ���� �������� ��� i..j
            //�������� ����������� �������� ������� ��������, ���� ��������� ������ ������� �� �����
            get2D(costOBST, size + 1, i, j) = std::numeric_limits<float>::infinity();

            for(size_t r = i; r <= j; r++)
            {
                //���������� r - ������� ������ ��� �������� i..j

                float cost = get2D(sumCost, size + 1, i, j)
                        + get2D(costOBST, size + 1, i, r - 1);
                if(r < j)
                    cost += get2D(costOBST, size + 1, r + 1, j);

                //���� �� ��������� �� ������ cost, �������� ����.
                if(cost < get2D(costOBST, size + 1, i, j))
                {
                    get2D(costOBST, size + 1, i, j) = cost;
                    get2D(roots, size, i - 1, j - 1) = r - 1; //����� �������� "�����������" �����
                }
            }
        }
    }

    //costOBST[1, size] - ���� ������������ ��. ������ ��� ��� ��������.
    std::cout << "Optimal cost: " << get2D(costOBST, size + 1, 1, size) << std::endl;

    //����� ������ ��. ������ � "�����������" �����
    root = buildBST(0, size - 1, roots, values);


    delete[] roots;
    delete[] costOBST;
    delete[] sumCost;
}

template<typename T>
BSTNode<T>* OptimalBST<T>::buildBST(size_t i, size_t j, size_t* roots, const std::vector<T>& values)
{
    size_t size = values.size();

    //����� ������������ ������ � ���������� Ai..Aj
    size_t rootIndex = get2D(roots, size, i, j);
    BSTNode<T>* node = new BSTNode<T>(values[rootIndex]);

    //���� � ��������, ������ �� ����
    if(rootIndex > i)
        node->left = buildBST(i, rootIndex - 1, roots, values);
    if(rootIndex < j)
        node->right = buildBST(rootIndex + 1, j, roots, values);

    return node;
}

//������� ��� ������ � ������ ������� �������.
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
