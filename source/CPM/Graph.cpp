#include <simpleplan/CPM/Graph.hpp>

#include <algorithm>

namespace sp {
namespace cpm {

    Graph::Graph() : nodes (), starts (), ends () {}

    NodeRef Graph::getRef(const Node& node) {
        auto it = std::find(nodes.begin(), nodes.end(), node);

        return { std::distance(nodes.begin(), it), nodes };
    }

    NodeRef Graph::addNode(const Node& node) {
        nodes.emplace_back(node);
        return NodeRef(nodes.size() - 1, nodes);
    }

    void Graph::checkGraph() const {
        // If this returns without throwing, doWork should always complete

        // TODO: cycle detection
        // Start nodes should have a set early start value
        // ?...
    }

    void Graph::doWork() {
        // OK, here we go.
        // First, we need to find all our starts and ends.
        int i = 0;

        for (const auto& node: nodes) {
            if (node.parents.empty()) {
                starts.emplace_back(i, nodes);
            } else if (node.children.empty()) {
                ends.emplace_back(i, nodes);
            }

            i++;
        }

        // Now, we need to start our "left-to-right" analysis.
        // We start from our starts, propagate early start values using the arrows.
        // If a Node has multiple parents, it needs to wait and get values from all of them and chose the maximum propagated early start.

        std::vector<std::vector<int>> candidates(nodes.size());

        // Start propagating
        for (auto start: starts) {
            propagateRight(start, candidates);
        }

        // TODO: Does this actually always work in one pass?
        // I think it should if there is no cycle

        // Let's do right to left!
        // Start out by clearing our candidates vector
        for (auto& vec: candidates) {
            vec.clear();
        }

        // Now, for our end points, we need to set the lateStart to be the same as the earlyStart
        for (auto end: ends) {
            end->lateStart = end->earlyStart;
        }

        // Now, let's propagate backwards!
        for (auto end: ends) {
            propagateLeft(end, candidates);
        }

        // We're done!
    }

    void Graph::propagateRight(NodeRef start, std::vector<std::vector<int>>& candidates) {
        for (auto& arrow: start->children) {
            auto& vec = candidates[arrow.to.index];
            vec.push_back(arrow.calculate(true));

            if (vec.size() == arrow.to->parents.size()) {
                // We have all candidates!

                arrow.to->earlyStart = *std::max_element(vec.begin(), vec.end());

                propagateRight(arrow.to, candidates);
            }
        }
    }

    void Graph::propagateLeft(NodeRef start, std::vector<std::vector<int>>& candidates) {
        for (auto& arrow: start->parents) {
            auto& vec = candidates[arrow.to.index];
            vec.push_back(arrow.calculate(false));

            if (vec.size() == arrow.to->children.size()) {
                // We have all candidates!

                arrow.to->lateStart = *std::min_element(vec.begin(), vec.end());

                propagateLeft(arrow.to, candidates);
            }
        }
    }
}
}
