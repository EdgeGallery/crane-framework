/*
 * @Descripttion: Crane observer abstract class.
 * @version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-09 16:53:13
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-12 19:35:43
 */
#ifndef __PLUGIN_CRANEOBSERVERIMPI_H__
#define __PLUGIN_CRANEOBSERVERIMPI_H__

#include <string>
#include <vector>
#include <thread>
#include "crane.h"
#include "craneMacro.h"
#include "Util.h"

#include "CraneMsgCenter.h"

using namespace std;
namespace NS_CRANE {

class CraneObserverImpi : public CraneObserver, public enable_shared_from_this<CraneObserverImpi> {
    public:
        CraneObserverImpi () {
            LOG_ENTER();
            _msgCenter = dynamic_cast<Itf_CraneMsgCenter*>(
                AbstractPluginFrame::getPluginFrame()->create("Itf_CraneMsgCenter", "CraneMsgCenter", ""));            
        }

        virtual unsigned subscribe(const CraneTopic& topic) override {
            LOG_ENTER();
            unsigned ret = _msgCenter->subscriber(topic, shared_from_this());
            if (ret != CRANE_SUCC) {
                LOG_ERROR("subscribe with topic { %s } failed with retCode: { %u }", topic.name().c_str(), ret);
                return ret;
            }

            _topics.push_back(topic);

            return CRANE_SUCC;
        }

        unsigned unsubscribe(const CraneTopic& topic) override {
            LOG_ENTER();
            unsigned ret = _msgCenter->unSubscriber(topic, dynamic_pointer_cast<CraneObserver>(shared_from_this()));
            if (ret != CRANE_SUCC) {
                LOG_ERROR("unsubscribe with topic { %s } failed with retCode: { %u }", topic.name().c_str(), ret);
                return ret;
            }

            _topics.remove(topic);

            return CRANE_SUCC;
        }

        Itf_CraneMsgCenter*         msgCenter() {
            return _msgCenter;
        }

        ~CraneObserverImpi() {
            LOG_ENTER();
            for (CraneTopic t : _topics) {
                _msgCenter->unSubscriber(t, dynamic_pointer_cast<CraneObserver>(shared_from_this()));
            }
        }       

    private:
        Itf_CraneMsgCenter*         _msgCenter;
        list<CraneTopic>            _topics;

};

}

#endif