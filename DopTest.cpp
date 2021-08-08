#include <gtest/gtest.h>
#include <random>

#include "Dop.h"

#define TimeLapse(code, time)                             \
    {                                                     \
        auto begin_t = std::clock();                      \
        code;                                             \
        auto end_t = std::clock();                        \
        time = (float)(end_t - begin_t) / CLOCKS_PER_SEC; \
    }

static const NumbersType kElemsSize = 10000000;
static const std::string delim(80, '=');

static void printElems(const std::vector<dop::Node>& nodes)
{
    std::cerr << delim << std::endl;

    for (NumbersType i = 0; i < kElemsSize; ++i) {
        std::cerr << nodes[i] << std::endl;
    }
}

static std::random_device rd;
static std::mt19937 g(rd());

static void generateNodes(std::vector<dop::Node>& nodes, NumbersType size)
{
    nodes.reserve(size);
    for (NumbersType i = 0; i < size; ++i) {
        nodes.push_back({ i, 1 });
    }

    std::uniform_int_distribution<NumbersType> uni(0, kElemsSize - 1);

    for (NumbersType i = 0, searchCount = kElemsSize; i < searchCount; ++i) {
        auto number = uni(g);
        auto iter = std::lower_bound(nodes.begin(), nodes.end(), dop::Node{ number, 0 });
        iter->weight += 1;
    }

    std::shuffle(nodes.begin(), nodes.end(), g);
}

TEST(Dop, Algo1)
{
    std::vector<dop::Node> elems;

    generateNodes(elems, kElemsSize);

    //    printElems(elems);

    dop::DopBst dopTree;

    double timeSpent = 0;
    TimeLapse({ dop::createDop1(&dopTree, &elems[0], elems.size()); }, timeSpent);

    std::cerr << "Fill dop1 tree: " << timeSpent << std::endl;
    std::cerr << "avgDepth: " << dopTree.calcAvgDepth() << std::endl;

    std::shuffle(elems.begin(), elems.end(), g);

    NumbersType tenthPart = kElemsSize / 10;

    TimeLapse(
        {
            for (NumbersType i = 0; i < tenthPart; ++i) {
                EXPECT_TRUE(dopTree.search(elems[i]));
            }
        },
        timeSpent);
    std::cerr << "Search dop1 tree: " << timeSpent << std::endl;

    //    printElems(elems);

    //    std::cerr << delim << std::endl;
    //    dopTree.print();
}

TEST(Dop, Algo2)
{
    std::vector<dop::Node> elems;

    generateNodes(elems, kElemsSize);

    //    printElems(elems);

    dop::DopBst dopTree;
    double timeSpent = 0;
    TimeLapse({ dop::createDop2(&dopTree, &elems[0], elems.size()); }, timeSpent);

    std::cerr << "Fill dop2 tree: " << timeSpent << std::endl;
    std::cerr << "avgDepth: " << dopTree.calcAvgDepth() << std::endl;

    std::shuffle(elems.begin(), elems.end(), g);

    NumbersType tenthPart = kElemsSize / 10;

    TimeLapse(
        {
            for (NumbersType i = 0; i < tenthPart; ++i) {
                EXPECT_TRUE(dopTree.search(elems[i]));
            }
        },
        timeSpent);
    std::cerr << "Search dop2 tree: " << timeSpent << std::endl;

    //    printElems(elems);
    //    std::cerr << delim << std::endl;
    //    dopTree.print();
}
