#include "BinarySearchTree.h"

#include <gtest/gtest.h>
#include <algorithm>
#include <random>

#define TimeLapse(code, time)                             \
    {                                                     \
        auto begin_t = std::clock();                      \
        code;                                             \
        auto end_t = std::clock();                        \
        time = (float)(end_t - begin_t) / CLOCKS_PER_SEC; \
    }

TEST(BST, SearchInsert)
{
    std::initializer_list<std::uint64_t> elems = { 20, 10, 4, 5, 16, 30, 24, 50 };

    BST<std::uint64_t> bst(elems);

    for (auto elem : elems) {
        EXPECT_TRUE(bst.search(elem));
    }
}

TEST(BST, Remove)
{
    std::initializer_list<std::uint64_t> elems = { 20, 10, 4, 5, 16, 30, 24, 50 };
    BST<std::uint64_t> bst(elems);

    for (auto elem : elems) {
        bst.remove(elem);
        std::cerr << "after remove: " << elem << std::endl;
        bst.print();
    }
}

using NumberType = std::uint32_t;

static const NumberType kNumberCount = 50000;

TEST(BST, CompareTwoTrees)
{
    BST<NumberType> sequenceTree, randomTree;
    double timeSpent;
    TimeLapse(
        {
            for (NumberType i = 0; i < kNumberCount; ++i) {
                sequenceTree.insert(i);
            }
        },
        timeSpent);

    std::cerr << "Fill sequence tree: " << timeSpent << std::endl;

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<NumberType> uni(0, kNumberCount - 1);
    std::vector<NumberType> randomNumbers(kNumberCount);
    std::generate(randomNumbers.begin(), randomNumbers.end(), [&]() { return uni(g); });
    TimeLapse(
        {
            for (auto i : randomNumbers) {
                randomTree.insert(i);
            }
        },
        timeSpent);
    std::cerr << "Fill random tree: " << timeSpent << std::endl;

    std::shuffle(randomNumbers.begin(), randomNumbers.end(), g);

    auto tenthPart = kNumberCount / 10;

    TimeLapse(
        {
            for (NumberType i = 0; i < tenthPart; ++i) {
                EXPECT_TRUE(sequenceTree.search(randomNumbers[i]));
            }
        },
        timeSpent);
    std::cerr << "Search sequence tree: " << timeSpent << std::endl;

    TimeLapse(
        {
            for (NumberType i = 0; i < tenthPart; ++i) {
                EXPECT_TRUE(randomTree.search(randomNumbers[i]));
            }
        },
        timeSpent);

    std::cerr << "Search random tree: " << timeSpent << std::endl;

    TimeLapse(
        {
            for (NumberType i = 0; i < tenthPart; ++i) {
                sequenceTree.remove(randomNumbers[i]);
            }
        },
        timeSpent);
    std::cerr << "Remove sequence tree: " << timeSpent << std::endl;

    TimeLapse(
        {
            for (NumberType i = 0; i < tenthPart; ++i) {
                randomTree.remove(randomNumbers[i]);
            }
        },
        timeSpent);

    std::cerr << "Remove random tree: " << timeSpent << std::endl;
}
