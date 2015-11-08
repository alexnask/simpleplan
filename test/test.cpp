#include <iostream>

#include <simpleplan/CPM/Graph.hpp>

using namespace sp::cpm;

std::string typeToString(Arrow::Type type) {
    std::string result;
    switch (type) {
        case Arrow::Type::FS:
            result = "FS";
        break;
        case Arrow::Type::SF:
            result = "SF";
        break;
        case Arrow::Type::SS:
            result = "SS";
        break;
        case Arrow::Type::FF:
            result = "FF";
        break;
    }
    return result;
}

void uglyPrint(const Graph& graph) {
    for (const auto& node: graph.nodes) {
        std::cout << "---------------" << std::endl;
        std::cout << node.name << " (duration: " << node.duration << ")" << std::endl;
        std::cout << "---------------" << std::endl;
        std::cout << "ES: " << node.earlyStart << std::endl << "LS: " << node.lateStart << std::endl;

        for (auto arrow: node.children) {
            std::cout << typeToString(arrow.type) << ' ' << arrow.value << " -> " << arrow.to->name << std::endl;
        }
        std::cout << std::endl << std::endl;
    }
}

int main(int argc, char *argv[]) {

    Graph graph = {};

    // Let's build something simple by hand

    auto nodeA = graph.addNode({2, "A"});
    auto nodeB = graph.addNode({4, "B"});
    auto nodeC = graph.addNode({3, "C"});
    auto nodeD = graph.addNode({5, "D"});

    nodeA->earlyStart = 0;

    nodeA->addArrow({ Arrow::Type::FS, 2, nodeA, nodeB });
    nodeA->addArrow({ Arrow::Type::FF, -1, nodeA, nodeC });

    nodeB->addArrow({ Arrow::Type::SS, 1, nodeB, nodeD });
    nodeC->addArrow({ Arrow::Type::FS, 1, nodeC, nodeD });

    // This does nothing atm
    graph.checkGraph();

    graph.doWork();

    uglyPrint(graph);

    return 0;
}
