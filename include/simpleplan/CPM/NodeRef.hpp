#ifndef NODEREF__HPP
#define NODEREF__HPP


#include <vector>


namespace sp {
namespace cpm {
    struct Node;

    struct NodeRef {

        int index;
        std::vector<Node>& nodes;

        NodeRef(int _index, std::vector<Node>& _nodes);

        Node& operator *  () const;
        Node* operator -> () const;

        Node* get() const;

        bool operator == (const NodeRef& other) const;
    };

}
}

#endif
