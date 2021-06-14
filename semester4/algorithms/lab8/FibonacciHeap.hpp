#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>

using std::endl;
using std::vector;

template<typename T>
struct FibonacciNode {
    T key;
    //Кількість дітей
    unsigned degree;
    //Маркер видаленої дитини
    bool mark;
    FibonacciNode* left, *right, *parent, *child;


    FibonacciNode(T const &_key) : key(_key), degree(0), mark(false), left(this), right(this), parent(nullptr), child(nullptr) {}

    //Додає брата зліва
    void addBrother(FibonacciNode* brother) {
        if (brother) {
            brother->right = this;
            brother->left = this->left;
            this->left->right = brother;
            this->left = brother;
        }
    }

    //Вставляє список братів справа
    void addSiblings(FibonacciNode* start, FibonacciNode* end) {
        if (start && end) {
            start->left = this;
            end->right = this->right;
            this->right->left = end;
            this->right = start;
        }
    }

    void link(FibonacciNode* new_child) {
        //Підєднуємо нову дитину
        if (!new_child) return;
        if (child) {
            child->addBrother(new_child);
        }
        else {
            child = new_child;
            new_child->left = new_child->right = new_child;
        }
        degree++;
        //Скидаємо маркер
        new_child->mark = false;
        new_child->parent = this;
    }

    //Вилучає даний вузол з піраміди, скидає маркер
    void pop() {
        left->right = right;
        right->left = left;
        if (parent) {
            if (parent->child == this) {
                parent->child = parent->degree == 1 ? nullptr : right;
            }
            parent->degree--;
        }
        parent = nullptr;
        mark = false;
    }
};


template<typename T>
class FibonacciHeap {
private:
	FibonacciNode<T>* root;
	unsigned int n, max_degree;

	//Зливає біноміальні дерева однакового степеня
	void consolidate() {
		if (!root) return;
		//Масив, що містить вже переглянуті дерева, їх індекси дорівнюють їх степеням
		vector<FibonacciNode<T>*> degree_cache(max_degree + 1, nullptr);
		FibonacciNode<T>* current = root, *x, *y, *next;
		unsigned degree;

		do {
			x = current;
			next = current->right;
			degree = x->degree;

			//Допоки існують дерева того ж степеня, що й х, - зливаємо їх з х
			while (degree_cache[degree]) {
				y = degree_cache[degree];
				if (x == y) break;

				//Обираємо, яке дерево підчіпити, щоб не порушити впорядкованість ключів
				if (x->key > y->key) {
					std::swap(x, y);
				}
				//Якщо дерево, яке вилучається зі списку коренів, є коренем піраміди, то визначаємо новим коренем лівий вузол
				if (y == root) {
					root = root->right;
				}
				//Якщо дерево, яке вилучається зі списку коренів, є наступним для розгляду вузлом, то переміщуємо вказівник наступного вузла вправо
				if (y == next) {
					next = next->right;
				}
				y->pop();
				x->link(y);

				degree_cache[degree] = nullptr;
				//Після злиття степінь х зростає
				degree++;
				if (degree > max_degree) {
					degree_cache.push_back(nullptr);
					max_degree++;
				}
			}

			degree_cache[degree] = x;
			current = next;
		} while (current != root);

		//Шукаємо новий корінь піраміди (мінімум)
		for (FibonacciNode<T>* cur : degree_cache) {
			if (cur) {
				if (root) {
					if (cur->key < root->key) {
						root = cur;
					}
				}
				else {
					root = cur;
				}
			}
		}
	}

	//Каскадно вилучає помічені вершини та додає їх до списку коренів
	void cascadingCut(FibonacciNode<T>* current) {
		if (current && current->parent) {
			FibonacciNode<T>* parent = current->parent;
			//Якщо вершина вже помічена (тобто в неї був видалений один із синів), то вилучаємо її, додаємо до списку коренів, каскадно вирізаємо батька
			if (current->mark) {
				current->pop();
				root->addBrother(current);
				cascadingCut(parent);
			}
			//Інакше помічаємо її
			else {
				current->mark = true;
			}
		}
	}

	FibonacciNode<T>* searchRecursive(T const &key, FibonacciNode<T>* current) const {
		if (current->key == key) return current;

		FibonacciNode<T>* found = nullptr;
		if (current->key <= key) {
			if (current->child) {
				FibonacciNode<T>* iter = current->child;
				for (unsigned i = 0; i < current->degree && !found; i++) {
					found = searchRecursive(key, iter);
					iter = iter->right;
				}
			}
		}

		return found;
	}

	//Шукає вершину з даним ключем
	FibonacciNode<T>* search(T const &key) const {
		if (root) {
			FibonacciNode<T>* current = root, *found;
			do {
				found = searchRecursive(key, current);
				if (found) return found;
				current = current->right;
			} while (current != root);
		}

		return nullptr;
	}

	//Зменшує ключ
	void decreaseKey(FibonacciNode<T>* aim, T const &new_key) {
		if (!aim || aim->key < new_key) return;

		aim->key = new_key;
		FibonacciNode<T>* parent = aim->parent;
		//Якщо батько містить більший ключ, вилучаємо дану вершину, вставляємо в список коренів, каскадно вирізаємо батька
		if (parent && new_key < parent->key) {
			aim->pop();
			root->addBrother(aim);
			cascadingCut(parent);
		}

		//Якщо новий ключ менший за ключ кореня, то новим коренем стає даний вузол
		if (aim->key < root->key) {
			root = aim;
		}
	}

	void printStructure(FibonacciNode<T>* current, std::ostream& os, unsigned level = 0) const {
		if (current) {
			for (unsigned i = 0; i < level; i++)
                os << "   ";
			os << current->key;
			//"*" означає, що дана вершина помічена
			if (current->mark)
                os << "(*)";
            os << std::endl;

			if (current->child) {
				FibonacciNode<T>* iter = current->child;
				for (unsigned i = 0; i < current->degree; i++) {
					printStructure(iter, os, level + 1);
					iter = iter->right;
				}
			}
		}
	}

	void removeRecursive(FibonacciNode<T>* current) {
		if (current) {
			if (current->child) {
				FibonacciNode<T>* iter = current->child;
				for (unsigned i = 0; i < current->degree; i++) {
					removeRecursive(iter);
					iter = iter->right;
				}
			}

			delete current;
		}
	}

public:
	FibonacciHeap() : root(nullptr), n(0), max_degree(0) {}

	bool empty() const {
		return root == nullptr;
	}

	unsigned int size() const {
		return n;
	}

	//Перевіряє, чи є даний ключ в піраміді
	bool contains(const T& key) const {
		return search(key) != nullptr;
	}

	void insert(T const &key) {
		FibonacciNode<T>* new_Node = new FibonacciNode<T>(key);

		if (root) {
			root->addBrother(new_Node);
			if (key < root->key) {
				root = new_Node;
			}
		}
		else {
			root = new_Node;
		}

		n++;
	}

	T peek() const {
		if (!root) {
			throw std::invalid_argument("The heap is empty.");
		}
		return root->key;
	}

	//Зливає вказану піраміду з даною, очищуючи її
	void merge(FibonacciHeap<T> &to_unite) {
		if (to_unite.size()) {
			if (to_unite.size() == 1) {
				root->addBrother(to_unite.root);
			}
			else {
				root->addSiblings(to_unite.root, to_unite.root->left);
			}

			if (to_unite.root->key < root->key) {
				root = to_unite.root;
			}
			if (to_unite.max_degree > max_degree) {
				max_degree = to_unite.max_degree;
			}

			n += to_unite.n;
			to_unite.root = nullptr;
			to_unite.n = 0;
		}
	}

	T pop() {
		if (!root) {
			throw std::invalid_argument("The heap is empty.");
		}

		if (root->child) {
			root->addSiblings(root->child, root->child->left);
			FibonacciNode<T>* child = root->child, *current = child;
			do {
				current->parent = nullptr;
				current = current->right;
			} while (current != child);
		}

		FibonacciNode<T>* min = root;
		min->pop();
		root = n == 1 ? nullptr : min->right;
		consolidate();
		n--;

		T data = min->key;
		delete min;
		return data;
	}

	void decreaseKey(const T& key, const T& newKey) {
		if (key < newKey) {
			throw std::invalid_argument("The new key is greater than old one.");
		}

		FibonacciNode<T>* found = search(key);
		if (!found) {
			throw std::invalid_argument("The key is absent in the heap.");
		}
		decreaseKey(found, newKey);
	}

	void deleteKey(const T& key) {
		FibonacciNode<T>* found = search(key);
		if (!found) {
			throw std::invalid_argument("The key is absent in the heap.");
		}
		decreaseKey(found, peek());
		pop();
	}

	//Друкує піраміду
	void printStructure(std::ostream& os) const {
		if (root) {
			FibonacciNode<T>* current = root;
			do {
				printStructure(current, os);
				current = current->right;
			} while (current != root);
		}
		else {
			os << "The heap is empty." << endl;
		}
	}

	~FibonacciHeap() {
		if (root) {
			FibonacciNode<T>* current = root, *next;
			root->left->right = nullptr;
			do {
				next = current->right;
				removeRecursive(current);
				current = next;
			} while (current);
		}
	}
};
