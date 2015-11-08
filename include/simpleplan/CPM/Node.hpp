#ifndef NODE__HPP
#define NODE__HPP

#include <simpleplan/CPM/Arrow.hpp>

#include <vector>
#include <string>

namespace sp {
namespace cpm {

    struct Node {
        std::vector<Arrow> children;
        std::vector<Arrow> parents;

        int earlyStart;
        int duration;
        int lateStart;

        std::string name;

        Node(int _duration, std::string _name);

        void addArrow(Arrow arrow);
        bool operator == (const Node& other) const;
    };

}
}

#endif
