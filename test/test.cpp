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

    auto nodeA = graph.addNode({3, "A"});
    auto nodeB = graph.addNode({4, "B"});
    auto nodeC = graph.addNode({2, "C"});
    auto nodeD = graph.addNode({1, "D"});
    auto nodeE = graph.addNode({5, "E"});
    auto nodeF = graph.addNode({3, "F"});
    auto nodeG = graph.addNode({4, "G"});
    auto nodeH = graph.addNode({3, "H"});
    auto nodeI = graph.addNode({6, "I"});

    nodeA->earlyStart = 0;

    nodeA->addArrow({ Arrow::Type::FF, 3, nodeA, nodeB });
    nodeA->addArrow({ Arrow::Type::SS, 2, nodeA, nodeC });
    nodeA->addArrow({ Arrow::Type::FS, 0, nodeA, nodeD });

    nodeB->addArrow({ Arrow::Type::FS, 3, nodeB, nodeG });
    nodeB->addArrow({ Arrow::Type::SF, -1, nodeB, nodeF });

    nodeC->addArrow({ Arrow::Type::FS, 1, nodeC, nodeB });
    nodeC->addArrow({ Arrow::Type::SF, 3, nodeC, nodeE });

    nodeD->addArrow({ Arrow::Type::SS, 4, nodeD, nodeE });

    nodeE->addArrow({ Arrow::Type::SS, 0, nodeE, nodeB });
    nodeE->addArrow({ Arrow::Type::FF, 2, nodeE, nodeF }); // When combined with commenting this,
    nodeE->addArrow({ Arrow::Type::SF, 1, nodeE, nodeH });

    nodeF->addArrow({ Arrow::Type::FF, 3, nodeF, nodeI });
    //nodeF->addArrow({ Arrow::Type::FF, 2, nodeF, nodeE }); // uncommenting this is a cycle

    nodeG->addArrow({ Arrow::Type::FS, 0, nodeG, nodeI });

    nodeH->addArrow({ Arrow::Type::SS, -1, nodeH, nodeI });

    graph.finalize();
    graph.check();
    graph.doWork();

    uglyPrint(graph);

    return 0;
}
