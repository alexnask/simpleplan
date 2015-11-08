#include <simpleplan/CPM/Graph.hpp>

#include <algorithm>
#include <stdexcept>

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

    void Graph::finalize() {
        int i = 0;

        for (const auto& node: nodes) {
            if (node.parents.empty()) {
                starts.emplace_back(i, nodes);
            } else if (node.children.empty()) {
                ends.emplace_back(i, nodes);
            }

            i++;
        }
    }

    void Graph::check() const {
        // If this returns without throwing, doWork should always complete

        // Build a stack while recursively traversing the graph
        // (pop the stack when going back)
        // If we ever get to a value that is already in the stack, we have a cycle

        std::vector<NodeRef> stack;
        for (auto start: starts) {
            cycleCheck(start, stack);

            if (start->earlyStart < 0) {
                throw std::logic_error("Starting node " + start->name + " has invalid early start value");
            }
        }
    }

    // This could blow up our stack, we should find a way to do it iteratively
    void Graph::cycleCheck(NodeRef start, std::vector<NodeRef>& stack) const {
        stack.emplace_back(start);

        for (auto& arrow: start->children) {
            if (std::find(stack.begin(), stack.end(), arrow.to) != stack.end()) {
                // Cycle detected!
                throw std::logic_error("Cycle detected around node " + arrow.to->name);
            }

            cycleCheck(arrow.to, stack);
        }

        stack.pop_back();
    }

    void Graph::doWork() {
        // Now, we need to start our "left-to-right" analysis.
        // We start from our starts, propagate early start values using the arrows.
        // If a Node has multiple parents, it needs to wait and get values from all of them and chose the maximum propagated early start.

        std::vector<std::vector<int>> candidates(nodes.size());

        // Start propagating
        for (auto start: starts) {
            propagateRight(start, candidates);
        }

        // Let's do right to left!
        // Start out by clearing our candidate vectors
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
