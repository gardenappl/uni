#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <iostream>
#include "Tree.h"
#include <vector>
#include "misc.h"

namespace trees
{

    template<typename T>
    struct BinaryTreeLinear
    {
        static bool verbose_print;
        struct Node
        {
            T value;
            size_t right; //right == current_index => no right element
            bool is_next_left;

            Node(const T& value, size_t current_index, bool is_next_left)
            : value(value), right(current_index), is_next_left(is_next_left)
            {}

            friend std::ostream& operator<<(std::ostream& os, const Node& node)
            {
                if(verbose_print)
                    return os << "( " << node.value << ", is_next_left: "
                            << std::boolalpha << node.is_next_left << ", right: " << node.right << " )";
                else
                    return os << node.value;
            }
        };
        std::vector<Node> nodes;

        void print(std::ostream& os) const;
    };

    template<typename T>
    bool BinaryTreeLinear<T>::verbose_print = false;

    template<typename T>
    struct BinaryTree
    {

        ~BinaryTree();
        static BinaryTree* from_normal_tree(const Tree<T>* value);
        BinaryTreeLinear<T>* to_linear() const;

        void print(std::ostream& os) const;

        struct Node
        {
            friend BinaryTree;
            T value;
            Node* left = nullptr;
            Node* right = nullptr;

            Node(T value)
            : value(value)
            {}

            ~Node();

        private:
            void print(std::ostream& os, int current_level) const;
            void to_linear(BinaryTreeLinear<T>* tree_linear, size_t& current_index) const;
        };
        Node* root = nullptr;

    private:
        static Node* from_normal_tree(typename Tree<T>::Node* const* siblings, size_t sibling_count);
    };

    template<typename T>
    BinaryTree<T>::~BinaryTree()
    {
        if(root)
            delete root;
    }

    template<typename T>
    BinaryTree<T>::Node::~Node()
    {
        if(left)
            delete left;
        if(right)
            delete right;
    }

    template<typename T>
    void BinaryTree<T>::print(std::ostream& os) const
    {
        if(!root)
            os << "[Empty]\n";
        else
            root->print(os, 0);
    }

    template<typename T>
    void BinaryTree<T>::Node::print(std::ostream& os, int current_level) const
    {
        for(int i = 0; i < current_level; i++)
            os << "- ";
        os << value << '\n';
        if(left)
        {
            for(int i = 0; i < current_level; i++)
                os << "- ";
            os << "Left:\n";
            left->print(os, current_level + 1);
        }
        if(right)
        {
            for(int i = 0; i < current_level; i++)
                os << "- ";
            os << "Right:\n";
            right->print(os, current_level + 1);
        }
    }


    template<typename T>
    BinaryTree<T>* BinaryTree<T>::from_normal_tree(const Tree<T>* tree)
    {
        BinaryTree<T>* bin_tree = new BinaryTree();
        if(tree->root)
        {
            bin_tree->root = from_normal_tree(&tree->root, 1);
        }
        return bin_tree;
    }

    template<typename T>
    typename BinaryTree<T>::Node* BinaryTree<T>::from_normal_tree(typename Tree<T>::Node* const* siblings, size_t sibling_count)
    {
        if(sibling_count == 0)
            return nullptr;

        Node* new_node = new Node(siblings[0]->value);
        //left = child
        new_node->left = from_normal_tree(siblings[0]->children.data(), siblings[0]->children.size());
        //right = sibling
        if(sibling_count > 1)
            new_node->right = from_normal_tree(siblings + 1, sibling_count - 1);
        else
            new_node->right = nullptr;
        return new_node;
    }

    template<typename T>
    BinaryTreeLinear<T>* BinaryTree<T>::to_linear() const
    {
        BinaryTreeLinear<T>* tree_linear = new BinaryTreeLinear<T>();
        if(!root)
            return tree_linear;

        size_t index = 0;
        root->to_linear(tree_linear, index);
        return tree_linear;
    }

    template<typename T>
    void BinaryTree<T>::Node::to_linear(BinaryTreeLinear<T>* tree_linear, size_t& current_index) const
    {
        tree_linear->nodes.emplace_back(value, current_index, left ? true : false);
        int node_index = tree_linear->nodes.size() - 1;

        current_index++;
        if(left)
            left->to_linear(tree_linear, current_index);
        if(right)
        {
            tree_linear->nodes[node_index].right = current_index;
            right->to_linear(tree_linear, current_index);
        }
    }

    template<typename T>
    void BinaryTreeLinear<T>::print(std::ostream& os) const
    {
        print_collection(os, nodes.begin(), nodes.end());
    }
}

#endif // BINARYTREE_H
