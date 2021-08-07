#pragma once

#include "BinarySearchTree.h"

namespace dop {

struct Node
{
    std::uint32_t key;
    std::uint32_t weight;
};

inline bool operator<(const Node& lho, const Node& rho)
{
    return lho.key < rho.key;
}

inline bool operator==(const Node& lho, const Node& rho)
{
    return lho.key == rho.key;
}

inline std::ostream& operator<<(std::ostream& out, const Node& val)
{
    return out << "key: " << val.key << " weight: " << val.weight;
}

struct DopBst : public BST<Node>
{
    DopBst* parent;
    std::uint32_t depth;

    DopBst()
        : parent(nullptr)
        , depth(1)
    {
    }

    [[nodiscard]] double calcAvgDepth() const
    {
        std::uint32_t avgDepth = 0;
        std::uint32_t treeWeight = 0;
        walk(this, [&avgDepth, &treeWeight](const BST<Node>* node) mutable {
            auto dopNode = toDopBst(node);
            treeWeight += dopNode->elems[0].weight;
            avgDepth += dopNode->elems[0].weight * dopNode->depth;
        });

        return double(avgDepth) / treeWeight;
    }

protected:
    BST<Node>* create() override { return new DopBst; }
    void setR(BST* node) override
    {
        setChild(node);
        BST::setR(node);
    }
    void setL(BST* node) override
    {
        setChild(node);
        BST::setL(node);
    }
    virtual void printImpl() const { walk(this, printElems); }

private:
    void setChild(BST* node)
    {
        auto child = toDopBst(node);
        child->parent = this;
        child->depth = child->parent->depth + 1;
    }
    static DopBst* toDopBst(BST* node) { return static_cast<DopBst*>(node); }
    static const DopBst* toDopBst(const BST* node) { return static_cast<const DopBst*>(node); }
    static void printElems(const BST* node)
    {
        for (const auto& elem : node->elems) {
            std::cerr << elem << " ";
        }
        std::cerr << "depth: " << toDopBst(node)->depth << std::endl;
    }
};

void createDop1(DopBst*, Node*, size_t);
void createDop2(DopBst*, Node*, size_t);
} // namespace dop
