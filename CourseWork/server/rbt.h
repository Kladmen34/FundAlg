#pragma once

#include <cassert>
#include <vector>

template<class K, class V>
class AbstractTree {
public:
    AbstractTree() = default;
    virtual ~AbstractTree() {}

    virtual size_t Size() const = 0;
    virtual void Insert(K k, V v) = 0;
    virtual void Update(const K & k, V v) = 0;
    virtual const V & Get(const K & k) const = 0;
    virtual V & Get(const K & k) = 0;
    virtual std::vector<V> ValuesFromRange(const K & minbound, const K & maxbound) = 0;
    virtual void Remove(const K & k) = 0;
    virtual void Clear() = 0;
    virtual bool Contains(const K & k) const = 0;
    virtual bool IsEmpty() const = 0;
};

template <class K, class V>
class RBTree : public AbstractTree<K,V> {
    

    enum Order { Less, Great };

    template <typename KEY, typename VALUE>
    struct Node {
        KEY key;                           
        VALUE value;                       
        Node<KEY,VALUE>* left = nullptr;   
        Node<KEY,VALUE>* right = nullptr;  
        Node<KEY,VALUE>* parent = nullptr; 
        bool color = false;                // цвет узла (0 - ч, 1 - к)
        Node () = default;
        Node (KEY k, VALUE v) {
            key    = std::move(k);
            value  = std::move(v);
            parent = left = right = nullptr;
            color  = false;
        }
        bool IsRed() const   { return color;  }
        bool IsBlack() const { return !color; }
        void SetRed()   { color = true;  }
        void SetBlack() { color = false; }
    }; 

    size_t _size = 0;                    
    Node<K,V>* _nil  = new Node<K,V>();  
    Node<K,V>* _root = nullptr;          
    Order _order;                        

    Node<K,V>* Search(const K & k) const {
        if ( _root == _nil ) return _nil;
        Node<K,V>* x = _root;
        for (; (x != _nil) && ((x->key < k || k < x->key));
            x = (k < x->key) ? x->left : x->right);
        return x;
    }

    template<class Func>
    void InorderWalk(Node<K,V>*x, Func func) {
        if ( x == _nil ) return;
        InorderWalk(x->left, func);
        func(x->value);
        InorderWalk(x->right, func);
    }

    template<class Compare, class Func>
    void InorderWalk(Node<K,V>*x, Compare compare, Func func) {
        if ( x == _nil ) return;
        InorderWalk(x->left, compare, func);
        if (compare(x->key)) func(x->value);
        InorderWalk(x->right, compare, func);
    }

    void LeftRotate(Node<K,V>* x) {
        auto y = x->right;
        x->right = y->left;
        if (y->left != _nil) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == _nil) {
            _root = y;
        } else {
            if (x == x->parent->left) {
                x->parent->left = y;
            } else {
                x->parent->right = y;
            }
        }
        y->left = x;
        x->parent = y;
    }

    void RightRotate(Node<K,V>* x) {
        auto y  = x->left;
        x->left = y->right;
        if (y->right != _nil) {
            y->right->parent = x;
        }
        y->parent = x->parent;

        if (x->parent == _nil) {
            _root = y;
        } else {
            if (x == x->parent->right) {
                x->parent->right = y;
            } else {
                x->parent->left = y;
            }
        }
        y->right = x;
        x->parent = y;
    }

    void FixupAfterInsert(Node<K,V>* z) {
        Node<K,V>* y = nullptr;
        assert (z->parent);
        while (z->parent->IsRed()) {
            if (z->parent == z->parent->parent->left) {
                y = z->parent->parent->right;
                if (y->IsRed()) {
                    z->parent->SetBlack();
                    y->SetBlack();
                    z = z->parent->parent;
                    z->SetRed();
                } else {
                    if (z == z->parent->parent) {
                        z = z->parent;
                        LeftRotate(z);
                    }
                    z->parent->SetBlack();
                    z->parent->parent->SetRed();
                    RightRotate(z->parent->parent);
                }
            } else {
                y = z->parent->parent->left;
                if (y->IsRed()) {
                    z->parent->SetBlack();
                    y->SetBlack();
                    z = z->parent->parent;
                    z->SetRed();
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        RightRotate(z);
                    }
                    z->parent->SetBlack();
                    z->parent->parent->SetRed();
                    LeftRotate(z->parent->parent);
                }
            }
        }
        _root->SetBlack();
    }

    void FixupAfterRemove(Node<K,V>* x) {
        Node<K,V>* w = nullptr;
        while (x != _root && x->IsBlack()) {
            if (x == x->parent->left) {
                w = x->parent->right;
                if (w->IsRed()) {
                    w->SetBlack();
                    x->parent->SetRed();
                    LeftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->IsBlack() && w->right->IsBlack()) {
                    w->SetRed();
                    x = x->parent;
                } else {
                    if (w->right->IsBlack()) {
                        w->left->SetBlack();
                        w->SetRed();
                        RightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->SetBlack();
                    w->right->SetBlack();
                    LeftRotate(x->parent);
                    x = _root;
                }
            } else {
                w = x->parent->left;
                if (w->IsRed()) {
                    w->SetBlack();
                    x->parent->SetRed();
                    RightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->IsBlack() && w->left->IsBlack()) {
                    w->SetRed();
                    x = x->parent;
                } else {
                    if (w->left->IsBlack()) {
                        w->right->SetBlack();
                        w->SetRed();
                        LeftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->SetBlack();
                    w->left->SetBlack();
                    RightRotate(x->parent);
                    x = _root;
                }
            }
        }
        x->SetBlack();
    }

    Node<K,V>* LeftMost(Node<K,V>* x) {
        if (x != _nil) {
            for (; x->left != _nil; x = x->left);
        }
        return x;
    }

    Node<K,V>* RightMost(Node<K,V>* x) {
        if (x != _nil) {
            for (; x->right != _nil; x = x->right);
        }
        return x;
    }

    Node<K,V>* Successor(Node<K,V>* x) {
        auto y = x->parent;
        if (_order == Order::Less) {
            if (x->right != _nil) return LeftMost(x->right);
            for (; y != _nil && x == y->right; x = y, y = y->parent);
        } else {
            if (x->left != _nil) return RightMost(x->left);
            for (; y != _nil && x == y->left; x = y, y = y->parent);
        }
        return y;
    }

    void Remove(Node<K,V>* z) {
        Node<K,V>* y = ((z->left == _nil)||(z->right == _nil))
                ? z : Successor(z);
        Node<K,V>* x = (y->left != _nil)
                ? y->left : y->right;
        x->parent = y->parent;
        if (y->parent == _nil) {
            _root = x;
        } else {
            if (y == y->parent->left) {
                y->parent->left = x;
            } else {
                y->parent->right = x;
            }
        }
        if (y != z) {
            z->key = y->key;
            z->value = y->value;
        }
        if (y->IsBlack()) {
            FixupAfterRemove(x);
        }
        _size -= 1;
        delete y;
    }

    void DeleteNode(Node<K,V>* node) {
        if (node == _nil) return;
        DeleteNode(node->left);
        DeleteNode(node->right);
        delete node;
    }

public:

    RBTree()
        : _size(0)
        , _nil(new Node<K,V>())
        , _root(_nil)
        , _order(Order::Less)
    { }

    ~RBTree() {
        Clear();
        delete _nil;
    }

    size_t Size() const override {
        return _size;
    }

    void Insert(K k, V v) override {
        Node<K,V>* y = _nil;
        Node<K,V>* x = _root;
        Node<K,V>* z = new Node<K,V>(k, std::move(v));
        while ( x != _nil ) {
             y = x;
             x = (k < x->key) ? x->left : x->right;
        }
        z->parent = y;

        _size += 1;

        if (y == _nil) {
            _root = z;
        } else {
            if ( k < y->key ) {
                y->left = z;
            } else {
                y->right = z;
            }
        }
        z->left = z->right = _nil;
        z->SetRed();
        FixupAfterInsert(z);
    }

    void Update(const K & k, V v) override {
        auto it = Search(k);
        if (it) {
            it->value = std::move(v);
        }
    }

    const V & Get(const K & k) const override {
        auto it = Search(k);
        if (it != _nil) {
            return it->value;
        } else {
            static V empty_value{};
            return empty_value;
        }
    }

    V & Get(const K & k) override {
    auto it = Search(k);
    if (it == _nil) {
        it = Insert(k, V{});
    }
    return it->value;
}


    std::vector<V> ValuesFromRange(const K & minbound, const K & maxbound) override {
        std::vector<V> result;
        auto komparator = [minbound, maxbound](const K & k) {
            return minbound <= k && k <= maxbound;
        };
        auto functor = [&result](const V & v) {
            result.push_back(v);
        };
        InorderWalk(komparator, functor);
        return result;
    }

    template<class Func>
    void InorderWalk(Func func) {
        InorderWalk(_root, func);
    }

    template<class Compare, class Func>
    void InorderWalk(Compare compare, Func func) {
        InorderWalk(_root, compare, func);
    }

    void Remove(const K & k) override {
        Node<K,V>* x = Search(k);
        if (x != _nil) Remove(x);
    }

    void Clear() override {
        DeleteNode(_root);
        _root = _nil;
        _size = 0;
    }

    bool Contains(const K & k) const override {
        auto n = Search(k);
        return (n != _nil && k == n->key);
    }

    bool IsEmpty() const override {
        return _root == _nil;
    }
}; 
