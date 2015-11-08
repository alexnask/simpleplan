#ifndef ARROW__HPP
#define ARROW__HPP

#include <simpleplan/CPM/NodeRef.hpp>

namespace sp {
namespace cpm {

    struct Arrow {
        enum class Type {
            FS,
            SS,
            SF,
            FF
        };

        Type type;
        int value;

        NodeRef from;
        NodeRef to;

        Arrow reverse() const;

        // Returns a start candidate for 'to'
        int calculate(bool early) const;

        Arrow(Type _type, int _value, NodeRef _from, NodeRef _to);
    };

}
}

#endif
