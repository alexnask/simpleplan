#include <iostream>

#include <cstdio>
#include <climits>

#include <simpleplan/CPM/Graph.hpp>

using namespace sp::cpm;

int getChar(FILE *fhandle) {
    int c = fgetc(fhandle);

    return c > CHAR_MAX ? (c - (UCHAR_MAX + 1)) : c;
}

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

Arrow::Type stringToType(std::string str) {
    if (str == "FS") {
        return Arrow::Type::FS;
    } else if (str == "SF") {
        return Arrow::Type::SF;
    } else if(str == "SS") {
        return Arrow::Type::SS;
    } else if(str == "FF") {
        return Arrow::Type::FF;
    }

    // We never should get here
    return Arrow::Type::FS;
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

    std::string filename;

    if (argc > 1) {
        filename = argv[1];
    } else {
        filename = "test.bin";
    }

    FILE *fhandle = fopen(filename.c_str(), "rb");

    // Ok, simple format we will be reading (don't feel like dealing with endianess right now):
    // 1 byte gives us the length of our node vector
    // For each node, 1 byte gives us the length of the name, followed by the name itself, 1 byte duration and 1 byte early start value
    // Then, for each node again, 1 byte gives us the arrow amount and for each arrow, we have a 2 byte type ("FF", "SS", "FS" or "SF"), 1 byte value and 1 byte dest node index

    int nodeLen = getChar(fhandle);

    {
        std::vector<NodeRef> newNodes;

        for (int i = 0; i < nodeLen; i++) {
            int nameLen = getChar(fhandle);

            std::string name(nameLen, 0);
            fread(&name[0], 1, nameLen, fhandle);

            int duration = getChar(fhandle);
            int earlyStart = getChar(fhandle);

            newNodes.emplace_back(graph.addNode({ duration, name }));
            newNodes.back()->earlyStart = earlyStart;
        }

        for (int i = 0; i < nodeLen - 1; i++) {
            int arrowLen = getChar(fhandle);

            for (int j = 0; j < arrowLen; j++) {
                std::string typeStr(2, 0);
                fread(&typeStr[0], 1, 2, fhandle);

                Arrow::Type type = stringToType(typeStr);

                int value = getChar(fhandle);
                int destIndex = getChar(fhandle);

                newNodes[i]->addArrow({ type, value, newNodes[i], newNodes[destIndex] });
            }
        }
    }

    fclose(fhandle);

    graph.finalize();
    graph.check();
    graph.doWork();

    uglyPrint(graph);

    return 0;
}
