#ifndef GRAPH__HPP
#define GRAPH__HPP

#include <simpleplan/CPM/Node.hpp>

#include <vector>

// TODO: add field functions (user defined JIT-ed functions to add any variable to Nodes)

namespace sp {
namespace cpm {

    struct Graph {
        std::vector<Node> nodes;

        // starts have no parents, ends have no children
        std::vector<NodeRef> starts;
        std::vector<NodeRef> ends;

        Graph();

        NodeRef getRef(const Node& node);
        NodeRef addNode(const Node& node);

        // Precomputes some stuff (at the moment, starts and ends)
        void finalize();
        void check() const;
        void doWork();

    private:
        void propagateRight(NodeRef start, std::vector<std::vector<int>>& candidates);
        void propagateLeft(NodeRef start, std::vector<std::vector<int>>& candidates);

        void cycleCheck(NodeRef start, std::vector<NodeRef>& stack) const;
    };

}
}

#endif
