#ifndef BPLUSTREE_HPP
#define BPLUSTREE_HPP

#include <vector>
#include <iostream>

template<typename T>
struct BPlusNode
{
    bool isLeaf = true;
    BPlusNode<T>* next = nullptr; //���� isLeaf, ����� �� ��������� leaf

    std::vector<T> keys; //n ������, t-1 <= n <=2t-1
    std::vector<BPlusNode<T>*> children;
    //������� ���� = n + 1;
    //�� ����� ����� N ������ �������� �� keys[N-1] �� keys[N]

    //����� ��������
    bool contains(const T& element) const;

    //�������� �����
    void splitChild(size_t index, size_t t);

    //������� �������� (����������: keys.size() < 2*t - 1)
    void insertNonFull(const T& element, size_t t);

    //��������� �������� (����������: keys.size() > t - 1, ���� �� �� �����)
    void remove(const T& element, size_t t);

    //��� ����������� ������
    void deleteChildren();

    //���� �� �����
    void printStructure(std::ostream& os, int depth) const;
};

//˳����� ����� ����� ������, ���� �������, ����������
template<typename T>
bool BPlusNode<T>::contains(const T& element) const
{
    size_t i = 0;
    //����� ������� �� binary search ��� ����� ����������� ��� ������� keys.size()
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

    //��������� ����� � ����������� ���� (t�1) ������
    //������ �� �������� t ������ ������� �����.

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

    //��������� ������� ������ � child
    if(child->isLeaf)
    {
        child->keys.resize(t);
    }
    if(!child->isLeaf)
    {
        child->keys.resize(t - 1);
        child->children.resize(t);
    }

    //������� newNode �� ���������� ����� this � ��������� ������
    //� child � this ��� ��������� child �� newNode

    children.insert(children.begin() + index + 1, newNode); //�������� O(n)
    keys.insert(keys.begin() + index, child->keys[t - 1]); //�������� O(n)

    //��������� next
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
        //������ ������� � ����

        if(keys.empty() || element > keys.back())
        {
            keys.push_back(element);
        }
        else
        {
            //������ �������� ������ � �������� ��������
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
        //�� ����, ������ �������� ��� ������
        size_t i = keys.size();

        while(i > 0 && element < keys[i - 1])
            i--;

        //DISK_READ(children[i])
        if(children[i]->keys.size() == 2*t - 1)
        {
            //����� ����� ������ - �����
            splitChild(i, t);

            //���� ���������� ���� �����
            if(element > keys[i])
                i++;
        }
        children[i]->insertNonFull(element, t);
    }
}

template<typename T>
void BPlusNode<T>::remove(const T& element, size_t t)
{
    //����������: keys.size() > t - 1, ���� �� �� �����
    if(isLeaf)
    {
        //���� ���� k ����������� � ���� x �� ������� �
        //������ � ��������� k � x.

        for(size_t i = 0; i < keys.size(); i++)
        {
            if(keys[i] == element)
            {
                keys.erase(keys.begin() + i); //�������� O(n)
                return; //������� � ��������
            }
        }
        return; //�� ������� �������
    }

    //���� �� �������� �����:
    //��������� ��������, ��� ���� ������ element
    size_t i = 0;
    while(i < keys.size() && element > keys[i])
        i++;

    BPlusNode<T>* node = children[i];
    if(node->keys.size() > t - 1)
    {
        //����� ����������
        node->remove(element, t);
    }
    else
    {
        //���� � ����� ���� t-1 ��������:
        //���������� �����.

        //���� � ����� ���� ��������� ��������, �������� � �����
        if(i > 0 && children[i - 1]->keys.size() > t - 1)
        {
            BPlusNode<T>* leftBro = children[i - 1];
            if(leftBro->isLeaf)
            {
                //��� �����:

                //���������� ��������� �������
                node->keys.insert(node->keys.begin(), leftBro->keys.back());
                leftBro->keys.pop_back();

                //������� ����-���������
                keys[i - 1] = leftBro->keys.back();
            }
            else
            {
                //��� ����������� �����:

                //��������� ���������� � ��������
                node->keys.insert(node->keys.begin(), keys[i - 1]);
                //�� ���� ���� ������� ������ ��������� �����
                keys[i - 1] = leftBro->keys.back();
                leftBro->keys.pop_back();

                //� ���������� ��������� ��������
                node->children.insert(node->children.begin(), leftBro->children.back());
                leftBro->children.pop_back();
            }

            //����� ����� ����������
            node->remove(element, t);
            return;
        }
        //��������� ������
        if(i < children.size() - 1 && children[i + 1]->keys.size() > t - 1)
        {
            BPlusNode<T>* rightBro = children[i + 1];
            if(rightBro->isLeaf)
            {
                //��� �����:

                //���������� ��������� �������
                node->keys.push_back(rightBro->keys.front());
                rightBro->keys.erase(rightBro->keys.begin());

                //������� ����-���������
                keys[i] = rightBro->keys.front();
            }
            else
            {
                //��� ����������� �����:

                //��������� ���������� � ��������
                node->keys.push_back(keys[i]);
                //�� ���� ���� ������� ������ ��������� �����
                keys[i] = rightBro->keys.front();
                rightBro->keys.erase(rightBro->keys.begin());

                //� ���������� ��������� ��������
                node->children.push_back(rightBro->children.front());
                rightBro->children.erase(rightBro->children.begin());
            }

            //����� ����� ����������
            node->remove(element, t);
            return;
        }
        //� ���� ����� �� (t-1) �����, ��`������
        if(i < children.size() - 1)
        {
            //��`������� ������

            BPlusNode<T>* rightBro = children[i + 1];

            if(node->isLeaf)
            {
                //��������� next
                node->next = rightBro->next;
                //����-�������� ���������
                keys.erase(keys.begin() + i);
            }
            else
            {
                //����-��������� ��� �������
                node->keys.push_back(keys[i]);
                keys.erase(keys.begin() + i);

            }

            //��'������ ����� (O(n))
            node->keys.reserve(node->keys.size() + rightBro->keys.size());
            node->keys.insert(node->keys.end(), rightBro->keys.begin(), rightBro->keys.end());

            if(!node->isLeaf)
            {
                //��'������ ��������� (O(n))
                node->children.reserve(node->children.size() + rightBro->children.size());
                node->children.insert(node->children.end(), rightBro->children.begin(), rightBro->children.end());
            }
            //��������� �����
            delete rightBro;
            children.erase(children.begin() + i + 1);

            //����� ����� ����������
            node->remove(element, t);
            return;
        }
        if(i > 0)
        {
            //��`������� ����

            BPlusNode<T>* leftBro = children[i - 1];

            if(node->isLeaf)
            {
                //��������� next
                leftBro->next = node->next;
                //����-�������� ���������
                keys.erase(keys.begin() + i - 1);
            }
            else
            {
                //����-��������� ��� �������
                leftBro->keys.push_back(keys[i - 1]);
                keys.erase(keys.begin() + i - 1);
            }

            //��'������ ����� (O(n))
            leftBro->keys.reserve(leftBro->keys.size() + node->keys.size());
            leftBro->keys.insert(leftBro->keys.end(), node->keys.begin(), node->keys.end());

            if(!node->isLeaf)
            {
                //��'������ ��������� (O(n))
                leftBro->children.reserve(leftBro->children.size() + node->children.size());
                leftBro->children.insert(leftBro->children.end(), node->children.begin(), node->children.end());
            }
            //��������� �����
            delete node;
            children.erase(children.begin() + i);

            //����� ����� ����������
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
        //������� ����������� ������

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
    //��������� ������ ������
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
    //���� �� ����� �� ��������� next

    //��������� ��������� ����
    BPlusNode<T>* node = root;
    while(!node->children.empty())
        node = node->children[0];

    //���� �� �����
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
