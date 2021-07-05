#pragma once

#include <functional>
#include <iostream>
#include <vector>

template<typename T>
struct BST
{
    BST *l, *r;
    std::vector<T> elems;
    int height;

    BST()
        : l(nullptr)
        , r(nullptr)
        , height(0)
    {
    }

    BST(std::initializer_list<T> init)
        : l(nullptr)
        , r(nullptr)
        , height(0)
    {
        for (const auto& elem : init) {
            insert(elem);
        }
    }

    ~BST()
    {
        //        std::cerr << "~BST: " << std::endl;
        delete l;
        delete r;
    }

    void insert(const T& elem) { insertImpl(this, elem); }
    void remove(const T& elem)
    {
        NodeParent forRemove{ this, nullptr };
        removeImpl(forRemove, elem);
    }

    bool search(const T& elem) const
    {
        NodeParentConst current{ this, nullptr };
        return findVal(elem, current);
    }
    void print() const { walk(this, printElems); }

private:
    using NodeParentConst = std::pair<const BST*, const BST*>;
    using NodeParent = std::pair<BST*, BST*>;

    void removeImpl(NodeParent& removeNode, const T& elem)
    {
        auto* node = removeNode.first;
        if (!node) {
            return;
        }
        if (!findVal(elem, removeNode)) {
            return;
        }
        auto* currentNode = removeNode.first;
        if (currentNode->elems.size() > 1) {
            currentNode->elems.pop_back();
            return;
        }
        if (!currentNode->l && !currentNode->r) {
            removeLeaf(currentNode, removeNode.second);
            return;
        }
        if (!currentNode->l || !currentNode->r) {
            removeOneChild(currentNode, removeNode.second);
            return;
        }
        NodeParent maxLessChildPair{ removeNode.first->l, removeNode.first };
        findMaxNode(maxLessChildPair);
        std::swap(maxLessChildPair.first->elems, removeNode.first->elems);
        removeImpl(maxLessChildPair, elem);
    }

    void assignNodeChild(BST* node, BST* parent, BST* val)
    {
        if (!parent) {
            return;
        }
        if (parent->l == node) {
            parent->l = val;
        }
        else {
            parent->r = val;
        }
    }

    void removeOneChild(BST* node, BST* parent)
    {
        if (!parent) {
            BST* forRemove;
            if (node->l) {
                forRemove = node->l;
                *node = *node->l;
            }
            else {
                forRemove = node->r;
                *node = *node->r;
            }
            forRemove->l = forRemove->r = nullptr;
            delete forRemove;
            return;
        }
        if (node->l) {
            assignNodeChild(node, parent, node->l);
        }
        else {
            assignNodeChild(node, parent, node->r);
        }
        node->l = node->r = nullptr;
        delete node;
    }

    void removeLeaf(BST* node, BST* parent)
    {
        if (this == node) {
            elems.pop_back();
            return;
        }
        assignNodeChild(node, parent, nullptr);
        delete node;
    }

    void findMaxNode(NodeParentConst& nodeParent) const
    {
        auto* node = nodeParent.first;
        if (!node || !node->r) {
            return;
        }
        nodeParent.first = node->r;
        nodeParent.second = node;
        return findMaxNode(nodeParent);
    }

    void findMaxNode(NodeParent& nodeParent)
    {
        NodeParentConst nodeParentConst = nodeParent;
        findMaxNode(static_cast<NodeParentConst&>(nodeParentConst));
        nodeParent.first = const_cast<BST*>(nodeParentConst.first);
        nodeParent.second = const_cast<BST*>(nodeParentConst.second);
    }

    bool findVal(const T& val, NodeParentConst& nodeParent) const
    {
        auto* node = nodeParent.first;
        if (!node) {
            return false;
        }
        if (!node->elems.empty()) {
            if (node->elems[0] == val) {
                return true;
            }
            nodeParent.second = node;
            if (node->elems[0] > val) {
                nodeParent.first = node->l;
            }
            else {
                nodeParent.first = node->r;
            }
            return findVal(val, nodeParent);
        }
        return false;
    }
    bool findVal(const T& val, NodeParent& nodeParent)
    {
        NodeParentConst nodeParentConst = nodeParent;
        auto res = findVal(val, nodeParentConst);
        nodeParent.first = const_cast<BST*>(nodeParentConst.first);
        nodeParent.second = const_cast<BST*>(nodeParentConst.second);
        return res;
    }

    BST* insertImpl(BST* current, const T& elem)
    {
        if (!current) {
            current = new BST<T>;
        }

        if (current->elems.empty() || current->elems[0] == elem) {
            current->elems.push_back(elem);
        }
        else if (current->elems[0] < elem) {
            current->r = insertImpl(current->r, elem);
        }
        else {
            current->l = insertImpl(current->l, elem);
        }

        return current;
    }

    void walk(const BST* node, std::function<void(const BST*)> func) const
    {
        if (!node) {
            return;
        }
        walk(node->l, func);
        func(node);
        walk(node->r, func);
    }

    static void printElems(const BST* node)
    {
        for (const auto& elem : node->elems) {
            std::cerr << elem << " ";
        }
        std::cerr << std::endl;
    }
};