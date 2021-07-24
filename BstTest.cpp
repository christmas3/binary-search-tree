#include "Avl.h"

#include <gtest/gtest.h>
#include <algorithm>
#include <chrono>
#include <random>

#define TimeLapse(code, time)                             \
    {                                                     \
        auto begin_t = std::clock();                      \
        code;                                             \
        auto end_t = std::clock();                        \
        time = (float)(end_t - begin_t) / CLOCKS_PER_SEC; \
    }

static const std::initializer_list<std::uint64_t> kElems = { 20, 10, 4, 5, 16, 30, 24, 50 };
static const std::initializer_list<std::uint64_t> kUnbalancedRight = { 10, 12, 11, 5, 7, 6, 8, 9, 3, 2 };
static const std::initializer_list<std::uint64_t> kUnbalancedLeft = { 5, 4, 3, 11, 9, 10, 8, 7, 12, 13 };

template<typename T, typename E>
std::unique_ptr<T> testSearchInsert(E elems)
{
    std::unique_ptr<T> bst(new T{ elems });
    for (auto elem : elems) {
        EXPECT_TRUE(bst->search(elem));
    }

    return bst;
}

TEST(BST, SearchInsert)
{
    testSearchInsert<BST<std::uint64_t>>(kElems);
}

TEST(AVL, SearchInsert)
{
    testSearchInsert<AVL<std::uint64_t>>(kElems);
}

TEST(AVL, SearchInsertUnbalancedRight)
{
    auto bst = testSearchInsert<AVL<std::uint64_t>>(kUnbalancedRight);
    std::cerr << "-------------------------------------------------------: " << std::endl;
    bst->print();
    bst->reBalance();
    std::cerr << "-------------------------------------------------------: " << std::endl;
    bst->print();
}

TEST(AVL, SearchInsertUnbalancedLeft)
{
    auto bst = testSearchInsert<AVL<std::uint64_t>>(kUnbalancedLeft);
    std::cerr << "-------------------------------------------------------: " << std::endl;
    bst->print();
    bst->reBalance();
    std::cerr << "-------------------------------------------------------: " << std::endl;
    bst->print();
}

template<typename T>
void testRemove()
{
    std::unique_ptr<T> bst(new T{ kElems });
    bst->print();
    for (auto elem : kElems) {
        bst->remove(elem);
        std::cerr << "after remove: " << elem << std::endl;
        bst->print();
    }
}

TEST(BST, Remove)
{
    testRemove<BST<std::uint64_t>>();
}

TEST(AVL, Remove)
{
    testRemove<AVL<std::uint64_t>>();
}

using NumberType = std::uint16_t;

static const NumberType kNumberCount = 54000;

TEST(BST, CompareTrees)
{
    BST<NumberType> sequenceTree, randomTree;

    double timeSpent = 0;
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

TEST(BST, CompareTwoTreesAvl)
{
    AVL<NumberType> sequenceTree;

    double timeSpent = 0;

    TimeLapse(
        {
            for (NumberType i = 1; i <= kNumberCount; ++i) {
                sequenceTree.insert(i);
                sequenceTree.reBalance();
            }
        },
        timeSpent);

    std::cerr << "Fill sequence1 tree: " << timeSpent << " val: " << sequenceTree.elems[0] << " height: " << sequenceTree.height
              << std::endl;

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<NumberType> uni(0, kNumberCount - 1);
    std::vector<NumberType> randomNumbers(kNumberCount);
    std::generate(randomNumbers.begin(), randomNumbers.end(), [&]() { return uni(g); });
    AVL<NumberType> randomTree;
    TimeLapse(
        {
            for (auto i : randomNumbers) {
                randomTree.insert(i);
            }
            randomTree.reBalance();
        },
        timeSpent);
    std::cerr << "Fill random tree: " << timeSpent << " val: " << randomTree.elems[0] << " height: " << randomTree.height << std::endl;

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
