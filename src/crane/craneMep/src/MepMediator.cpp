/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-01 20:21:43
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-07-16 17:40:43
 */ 

#include "MepMediator.h"

namespace NS_CRANE {
    MepMediator::~MepMediator() {
        std::cout<<"~MepMediator()"<<std::endl;
    }
    
    MepMediator::MepMediator() {
        //获取appInstanceId信息；
        _appInstanceId = string("");
        char* id = getenv("APP_INSTANCE_ID");
        if (id == NULL) {
            _appInstanceId = string("appInstanceId_123456");
            LOG_ERROR("Unable get APP_INSTANCE_ID env.");
            //TODO: 抛出异常，以便MepMediator在创建该对象的时候能够获取到；
        } else {
            _appInstanceId = string(id);        
            LOG_DEBUG("APP instance id: %s", _appInstanceId.c_str());
        }

        //创建http client
        //auto opts = Pistache::Http::Client::options().threads(1).maxConnectionsPerHost(8).maxResponseSize(1024*8);
        //client.init(opts);
        //http_client_config config;
        //config.set_validate_certificates(false);
        //client = make_shared<http_client>();
    }

    void MepMediator::MepServiceResult(shared_ptr<AbstractMepService> mepService) {
        LOG_DEBUG("enter MepServiceResult()");
        MepClazzType type = mepService->ClazzType();
        switch (type) {
            case MepClazzType::ServiceReady:
                if (CRANE_SUCC == mepService->result()) {
                    LOG_INFO("Service ready successfully.");
                    //启动服务发现过程；
                    _srvDsv->doInteraction();
                } else {
                    LOG_ERROR("Service ready process failed.");
                    //TODO Service Ready fail.
                    return;
                }
                break;

            case MepClazzType::ServiceDiscover:
                if (CRANE_SUCC == mepService->result()) {
                    LOG_INFO("Service discover successfully.");
                    //获取服务发现的结构信息；
                    _discoverServicesResult = (dynamic_pointer_cast<MepServiceDiscover>(mepService))->getDiscoverResult();
                    LOG_DEBUG("------Discover Services Result: ------");
                    for (auto it = _discoverServicesResult.begin(); it != _discoverServicesResult.end(); ++it) {
                        LOG_DEBUG("Service Id:          [ %s ]", it->second->srvId.c_str());
                        LOG_DEBUG("Service Name:        [ %s ]", it->second->srvName.c_str());
                        for (auto ep : it->second->endpoints) {
                        LOG_DEBUG("Service Endpoint:    [ %s ]", ep.c_str());
                        }
                    }
                    
                    //启动服务注册过程；
                    _srvReg->doInteraction();
                } else {
                    //TODO Service Ready fail.
                    LOG_ERROR("Service discover failed!");
                    return;
                }
                break;

            case MepClazzType::ServiceRegister:
                if (CRANE_SUCC == mepService->result()) {
                    LOG_INFO("Service register successfully.");
                    //如果ServiceRegister对象返回的结果，则。。。
                    _registerServicesResult = dynamic_pointer_cast<MepServiceRegister>(mepService)->getRegisterResult();
                    LOG_DEBUG("------Register Services Result: ------");
                    for (auto it = _registerServicesResult.begin(); it != _registerServicesResult.end(); ++it) {
                        LOG_DEBUG("Service Id:          [ %s ]", it->second->srvId.c_str());
                        LOG_DEBUG("Service Name         [ %s ]", it->second->srvName.c_str());
                        LOG_DEBUG("Service Location     [ %s ]", it->second->location.c_str());
                    }

                    //Start put service route producedure.
                    _srvRte->doInteraction();
                } else {
                    //TODO Servivce register fail.
                    LOG_ERROR("Service register failed!");
                    return;
                }
                break;

            case MepClazzType::ServiceRoute:
                if (CRANE_SUCC == mepService->result() || CRANE_FAIL == mepService->result()) {
                    //如果ServiceRoute对象返回的结果，则。。。
                    LOG_INFO("Put service route successfully.");

                    //启动服务状态订阅之前，先启动订阅通知的服务端；
                    _svrPrc->Init();
                    /*
                    for (auto itr = servicesSubInfo.begin(); itr != servicesSubInfo.end(); ++itr) {
                        _svrPrc->SetRoutes(HttpMethod::POST, itr->second.callbackRef, &MepServiceSubscriber::DoServiceNotify, _srvSub);
                    }
                    */
                    _svrPrc->SetRoutes(HttpMethod::POST, _srvSub->getCallbackRefResource(), &MepServiceSubscriber::DoServiceNotify, _srvSub);
                    thread serverThread(&MepServerProcess::Start, move(_svrPrc));
                    serverThread.detach();
                    //_svrPrc->Start();
                    LOG_INFO("Start server interact with API-GW for subscription notify.");

                    //Start service subscriber interaction producedure.
                    _srvSub->doInteraction();
                } else {
                    //TODO Servivce register fail.
                    LOG_ERROR("Service route failed!");
                    return;
                }
                break;


            case MepClazzType::ServiceSubscirber:
                if (CRANE_SUCC == mepService->result()) {
                    LOG_INFO("Service subscriber successfully.");
                    _servicesSubscriberInfo = dynamic_pointer_cast<MepServiceSubscriber>(mepService)->getServicesInfo();
                    LOG_DEBUG("------Services Subscriber Info: ------");
                    for (auto it = _servicesSubscriberInfo.begin(); it != _servicesSubscriberInfo.end(); ++it) {
                        LOG_DEBUG("Service Name:    [ %s ]", it->second->serviceName.c_str());
                        LOG_DEBUG("Callback Ref:    [ %s ]", it->second->callbackRef.c_str());
                        LOG_DEBUG("Status:          [ %u ]", it->second->status); //TODO: may use serviceStatusCvs() to convert to string types.
                    }
                } else {
                    //TODO Service subscriber fail.
                    LOG_ERROR("Service subscriber failed!");
                    return;
                }
                break;
            
            default:
                LOG_ERROR("Invalid Clazz type{ %d }", static_cast<int>(type));
                return;
        }
    }

    void MepMediator::CreateMepServices() {
        _srvRdy =  make_shared<MepServiceReady>(shared_from_this(), _appInstanceId);
        _srvDsv =  make_shared<MepServiceDiscover>(shared_from_this(), _appInstanceId, _dsvServices);
        _srvReg =  make_shared<MepServiceRegister>(shared_from_this(), _appInstanceId, _regServices);
        _srvRte =  make_shared<MepServiceRoute>(shared_from_this(), _appInstanceId, _regServices);
        _srvSub =  make_shared<MepServiceSubscriber>(shared_from_this(), _appInstanceId, _subServices);

        _svrPrc =   make_shared<MepServerProcess>(shared_from_this(), _appInstanceId);
    }

    void MepMediator::doAction() {
        _srvRdy->doInteraction();
    }

    void MepMediator::feedServicesSubscriberInfo(void* servicesInfo) {
        _servicesSubscriberInfo = *(static_cast<ServicesSubscriberInfo*>(servicesInfo));
    }
/*
    void  MepMediator::feedServicesSubscriberInfo(const ServicesSubscriberInfo& servicesInfo) {
        _servicesSubscriberInfo = servicesInfo;
    }
*/    
    /*
    const Pistache::Http::Client& MepMediator::getClient() {
        return _client;      
    }
    */
}