#include "Dop.h"

#include <algorithm>
#include <cassert>
#include <numeric>

namespace dop {

void createDop1(DopBst* tree, Node* elems, size_t size)
{
    std::sort(elems, elems + size, [](const auto& lho, const auto& rho) { return lho.weight > rho.weight; });
    for (size_t i = 0; i < size; ++i) {
        tree->insert(elems[i]);
    }
}

static int findRoot(Node* elems, int left, int right)
{
    if (left >= right) {
        return left;
    }
    NumbersType treeWeightHalf = std::accumulate(elems + left,
                                                 elems + right + 1,
                                                 0,
                                                 [](auto& cur, const auto& val) { return cur += val.weight; }) /
                                 2;
    //    std::cerr << "treeWeightHalf: " << treeWeightHalf << std::endl;
    NumbersType currentWeight = 0;
    int i = left;
    while (currentWeight < treeWeightHalf && i <= right) {
        //        std::cerr << "i: " << i << std::endl;
        currentWeight += elems[i++].weight;
        //        std::cerr << "currentWeight: " << currentWeight << std::endl;
    }

    assert(i - 1 <= right);
    return i - 1;
}

static void createDop2Impl(DopBst* tree, Node* elems, int left, int right)
{
    if (left > right) {
        return;
    }
    auto root = findRoot(elems, left, right);
    //    std::cerr << "root: " << root << std::endl;
    tree->insert(elems[root]);
    createDop2Impl(tree, elems, left, root - 1);
    createDop2Impl(tree, elems, root + 1, right);
}

void createDop2(DopBst* root, Node* elems, size_t size)
{
    if (!std::is_sorted(elems, elems + size)) {
        std::cerr << "need to sort" << std::endl;
        std::sort(elems, elems + size);
    }
    createDop2Impl(root, elems, 0, size - 1);
}

} // namespace dop
