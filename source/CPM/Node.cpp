#include <simpleplan/CPM/Node.hpp>

#include <cassert>

namespace sp {
namespace cpm {

    Node::Node(int _duration, std::string _name) : children (), parents (), earlyStart (-1), duration (_duration), lateStart(-1), name (_name) {}

    void Node::addArrow(Arrow arrow) {
        assert(arrow.from.get() == this);

        children.emplace_back(arrow);

        arrow.to->parents.emplace_back(arrow.reverse());
    }

    // Check by address, should do the trick
    bool Node::operator == (const Node& other) const {
        return this == &other;
    }
}
}
