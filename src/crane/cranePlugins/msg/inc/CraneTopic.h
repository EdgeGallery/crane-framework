#ifndef __PLUGIN_CRANETOPIC_H__
#define __PLUGIN_CRANETOPIC_H__

#include <string>

#include "craneMacro.h"
#include "Log.h"

using namespace std;
namespace NS_CRANE {
class CraneTopic final {
    public: 
        enum class Type {
            SYNC = 0,       //同步消息，通过函数调用方式，用于线程内方式；
            ASYNC,          //异步消息，通过线程安全队列方式，用于跨线程场景；       
        };

        CraneTopic(string name, CraneTopic::Type type ) : _name(name), _type(type) {
            LOG_ENTER();
        }

        CraneTopic(const CraneTopic& o): _name(o.name()), _type(o.type()) {} //It may be redundant.

        const string& name() const { return _name; }

        const Type& type() const { return _type; }

        bool operator < (const CraneTopic& topic) const {
            return ((_name < topic.name()) 
                || ((_name == topic.name()) && (_type < topic.type())));
        }

        bool operator == (const CraneTopic& topic) const {
            return (_name == topic.name()) && (_type == topic.type());
        }

        ~CraneTopic() {}
    private:
        string              _name;
        Type                _type;
};

}
#endif