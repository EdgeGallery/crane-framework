#ifndef __SINGLETONIMPL_H__
#define __SINGLETONIMPL_H__

#include <iostream>
#include "cranePlugin.h"
#include "singletonInterface.h"

class SingletonImpl : public Itf_Singleton {
    public:
        SingletonImpl () { std::cout<<"SingletonImpl()"<<std::endl; };
        SingletonImpl (std::string name) : _name(name) { std::cout<<"SingletonImpl(string)"<<std::endl;};
        unsigned int init() override;
        unsigned setName(std::string&) override;
        unsigned showName() override;
        ~SingletonImpl() {std::cout<<"~SingletonImpl()"<<std::endl;};

    private:
        std::string     _name = "dongyin";
};

#endif