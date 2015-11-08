#include <simpleplan/CPM/Arrow.hpp>

#include <simpleplan/CPM/Node.hpp>

namespace sp {
namespace cpm {

    Arrow::Arrow(Type _type, int _value, NodeRef _from, NodeRef _to) : type (_type), value (_value), from (_from), to (_to) {}

    Arrow Arrow::reverse() const {
        Type newType = type;

        switch (type) {
            case Type::FS:
                newType = Type::SF;
            break;
            case Type::SF:
                newType = Type::FS;
            break;
            default:
            break;
        }

        return { newType, -value, to, from };
    }

    int Arrow::calculate(bool early) const {
        int start;
        if (early) {
            start = from->earlyStart;
        } else {
            start = from->lateStart;
        }
            
        switch (type) {
            case Type::SF:
                return start + value - to->duration;
            break;
            case Type::FS:
                return start + from->duration + value;
            break;
            case Type::SS:
                return start + value;
            break;
            case Type::FF:
                return start + from->duration + value - to->duration;
            break;
        }

        return -1;
    }

}
}
