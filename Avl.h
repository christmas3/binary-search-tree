#pragma once

#include <cassert>
#include "BinarySearchTree.h"

template<typename T>
struct AVL : public BST<T>
{
    size_t height;
    AVL* parentNode;

    AVL()
        : BST<T>()
        , height(1)
        , parentNode(nullptr)
    {
    }

    AVL(std::initializer_list<T> init)
        : BST<T>()
        , height(1)
        , parentNode(nullptr)
    {
        for (const auto& elem : init) {
            BST<T>::insert(elem);
        }
    }

    void reBalance()
    {
        bool change;
        do {
            change = false;
            reBalanceImpl(this, change);
        } while (change);
    }

    friend void swap(AVL& lho, AVL& rho)
    {
        using std::swap;
        swap(lho.height, rho.height);
        swap(lho.parentNode, rho.parentNode);
        swap(lho.elems, rho.elems);
        swap(lho.l, rho.l);
        swap(lho.r, rho.r);
    }

protected:
    BST<T>* create() override { return new AVL<T>; }
    void setR(BST<T>* node) override
    {
        BST<T>::r = node;
        getAVL(node)->parentNode = this;
        height = calcHeight(this);
    }
    void setL(BST<T>* node) override
    {
        BST<T>::l = node;
        getAVL(node)->parentNode = this;
        height = calcHeight(this);
    }

    void reBalanceImpl(AVL* node, bool& change)
    {
        if (!node || node->height < 2) {
            return;
        }
        if (balance(node)) {
            change = true;
        }

        reBalanceImpl(getAVL(node->l), change);
        reBalanceImpl(getAVL(node->r), change);
    }

    bool balance(AVL* node)
    {
        int64_t lHeight = 0, rHeight = 0;

        if (node->l) {
            lHeight = getAVL(node->l)->height;
        }
        if (node->r) {
            rHeight = getAVL(node->r)->height;
        }

        if (lHeight - rHeight > 1) {
            rightRotate(node);
            //            std::cerr << "===============================" << std::endl;
            //            BST<T>::print();
            return true;
        }
        else if (rHeight - lHeight > 1) {
            leftRotate(node);
            //            std::cerr << "===============================" << std::endl;
            //            BST<T>::print();
            return true;
        }
        return false;
    }

    void rightRotate(AVL* node)
    {
        //        std::cerr << "need right rotate: ";
        //        printElems(node);
        int64_t lHeight = 0, rHeight = 0;
        auto leftChild = node->l;

        if (leftChild->l) {
            lHeight = getAVL(leftChild->l)->height;
        }
        if (leftChild->r) {
            rHeight = getAVL(leftChild->r)->height;
        }
        if (lHeight >= rHeight) {
            //            std::cerr << "small right rotation: " << std::endl;
            smallRightRotation(node);
            nodeChanged(node->r);
        }
        else {
            bigRightRotation(node);
        }
    }

    void bigRightRotation(AVL* node)
    {
        leftRotate(getAVL(node->l));
        rightRotate(node);
    }

    void leftRotate(AVL* node)
    {
        //        std::cerr << "need left rotate: ";
        //        printElems(node);
        int64_t cHeight = 0, rHeight = 0;
        auto rightChild = node->r;

        if (rightChild->l) {
            cHeight = getAVL(rightChild->l)->height;
        }
        if (rightChild->r) {
            rHeight = getAVL(rightChild->r)->height;
        }
        if (rHeight >= cHeight) {
            //            std::cerr << "small left rotation: " << std::endl;
            smallLeftRotation(node);
            nodeChanged(node->l);
        }
        else {
            bigLeftRotate(node);
        }
    }

    void bigLeftRotate(AVL* node)
    {
        rightRotate(getAVL(node->r));
        leftRotate(node);
    }

    void printImpl() const override { BST<T>::walk(this, printElems); }

    void nodeChanged(BST<T>* node) override
    {
        if (!node) {
            return;
        }
        auto avlNode = getAVL(node);
        auto newHeight = calcHeight(avlNode);
        if (newHeight != avlNode->height) {
            avlNode->height = newHeight;
        }
        nodeChanged(avlNode->parentNode);
    }

    void smallRightRotation(AVL* node)
    {
        auto tmp = node->l;
        auto tmpAvl = getAVL(tmp);
        swap(*node, *tmpAvl);
        std::swap(node->parentNode, tmpAvl->parentNode);
        tmp->l = node->r;
        node->r = tmp;
        if (node->l) {
            getAVL(node->l)->parentNode = node;
        }
    }

    void smallLeftRotation(AVL* node)
    {
        auto tmp = node->r;
        auto tmpAvl = getAVL(tmp);
        swap(*node, *tmpAvl);
        std::swap(node->parentNode, tmpAvl->parentNode);
        tmp->r = node->l;
        node->l = tmp;
        if (node->r) {
            getAVL(node->r)->parentNode = node;
        }
    }

private:
    size_t calcHeight(AVL<T>* current)
    {
        auto newHeight = 0;
        if (!current) {
            return newHeight;
        }
        for (auto ch : { current->r, current->l }) {
            auto child = getAVL(ch);
            if (child && child->height > newHeight) {
                newHeight = child->height;
            }
        }
        if (!newHeight && !current->elems.empty()) {
            return 1;
        }
        return newHeight > 0 ? newHeight + 1 : newHeight;
    }

    static void printElems(const BST<T>* node)
    {
        for (const auto& elem : node->elems) {
            std::cerr << elem << " ";
        }
        std::cerr << "height: " << getAVL(node)->height << std::endl;
    }

    static AVL* getAVL(BST<T>* node) { return static_cast<AVL<T>*>(node); }
    static const AVL* getAVL(const BST<T>* node) { return static_cast<const AVL<T>*>(node); }
};
