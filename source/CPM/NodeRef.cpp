#include <simpleplan/CPM/NodeRef.hpp>

#include <simpleplan/CPM/Node.hpp>

namespace sp {
namespace cpm {

    NodeRef::NodeRef(int _index, std::vector<Node>& _nodes) : index (_index) , nodes (_nodes) {}

    Node& NodeRef::operator * () const {
        return nodes[index];
    }

    Node* NodeRef::operator -> () const {
        return get();
    }

    Node* NodeRef::get() const {
        return nodes.data() + index;
    }
}
}
