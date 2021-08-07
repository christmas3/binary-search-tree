#include <gtest/gtest.h>
#include <random>

#include "Dop.h"

static const std::uint16_t kElemsSize = 10;
static const std::string delim(80, '=');

static void printElems(const std::vector<dop::Node>& nodes)
{
    std::cerr << delim << std::endl;

    for (std::uint16_t i = 0; i < kElemsSize; ++i) {
        std::cerr << nodes[i] << std::endl;
    }
}

static void generateNodes(std::vector<dop::Node>& nodes, std::uint16_t size)
{
    nodes.reserve(size);
    for (std::uint16_t i = 0; i < size; ++i) {
        nodes.push_back({ i, 1 });
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<std::uint16_t> uni(0, kElemsSize - 1);

    for (std::uint16_t i = 0, searchCount = kElemsSize * 2; i < searchCount; ++i) {
        auto number = uni(g);
        std::cerr << "number: " << number << std::endl;
        auto iter = std::lower_bound(nodes.begin(), nodes.end(), dop::Node{ number, 0 });
        iter->weight += 1;
    }

    //    std::shuffle(nodes.begin(), nodes.end(), g);
}

TEST(Dop, Algo1)
{
    std::vector<dop::Node> elems;

    generateNodes(elems, kElemsSize);

    printElems(elems);

    dop::DopBst dopTree;
    dop::createDop1(&dopTree, &elems[0], elems.size());

    printElems(elems);

    std::cerr << delim << std::endl;
    dopTree.print();
    std::cerr << "avgDepth: " << dopTree.calcAvgDepth() << std::endl;
}

TEST(Dop, Algo2)
{
    std::vector<dop::Node> elems;

    generateNodes(elems, kElemsSize);

    printElems(elems);

    dop::DopBst dopTree;
    dop::createDop2(&dopTree, &elems[0], elems.size());

    printElems(elems);

    std::cerr << delim << std::endl;
    dopTree.print();
    std::cerr << "avgDepth: " << dopTree.calcAvgDepth() << std::endl;
}
