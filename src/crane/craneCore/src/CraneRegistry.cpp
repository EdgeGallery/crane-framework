/*
 * @Descripttion: Crane插件注册中心
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-10 10:26:19
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-12 16:10:21
 */ 
#include "CraneRegistry.h"

namespace NS_CRANE {
    const string CraneRegistry::_CACHE_FILE = ".crane_cache";
    const string CraneRegistry::_CACHE_FILE_TMP = ".crane_cache_tmp";

    /**
     * @Descripttion: 将插件接口信息和其下的插件类的工厂对象添加到插件注册中心
     * @Param: 
     * @Return: 
     */   
    //TODO:需要在看一下这个函数 
    unsigned CraneRegistry::registry_(shared_ptr<PluginInterfaceInfo> spItfInfo) {
        bool insertFactoryAndPluginNameFlags = false;
        shared_ptr<PluginInterfaceInfo> tmpItfInfo;
        const string& type = spItfInfo->type();

        PluginInterfaceMap::const_iterator itr =  _pluginItfMap.find(type);
        if (itr !=  _pluginItfMap.end()) {
            LOG_DEBUG("This plugin interface[ %s ] has existed in the registry.", type.c_str());
            LOG_INFO("Check the plugin interface[ %s ] version...", type.c_str());
            auto oItfInfo = (*itr).second;
            int result = Util::compareVersion(oItfInfo->curVersion(), spItfInfo->curVersion());
            if (result >= CRANE_EQUAL) {
                //map中的版本大于等于要插入的接口版本，因此无需新增插件接口，而只用新增该接口下的插件工厂 和 接口实现的类名称。
                LOG_INFO("The current version{ %s } in the registry is same or higher than the new plugin interface version{ % }",
                                        oItfInfo->curVersion().c_str(), spItfInfo->curVersion().c_str());
                LOG_INFO("So we should not update the interface in registry.");
                tmpItfInfo = spItfInfo; //tmpItfInfo will be update into the interfaceInfo which has already existed in the registry.
                insertFactoryAndPluginNameFlags = true;
            } else {
                //更新插件接口
                LOG_INFO("New plugin interface version[ %s ] is higher than current version[ %s ] in the registry.",
                    spItfInfo->curVersion().c_str(), oItfInfo->curVersion().c_str());
                LOG_INFO("So we will update the interface of the registry.");

                tmpItfInfo = itr->second;
                insertFactoryAndPluginNameFlags = true;
                _pluginItfMap.erase(itr); //TODO: !!!老版本插件下的工厂和实现类名称应该要迁移到新的插件接口对象下。

                auto ret = _pluginItfMap.insert(make_pair(type, spItfInfo));
                if (ret.second != true) {
                    LOG_ERROR("Update the interface{ %s } into registry failed.", type.c_str());
                    return CRANE_FAIL;
                }
                itr = ret.first; 
                //The pair::first set to an iterator pointing to either the newly inserted element or to the element with an equivalent key in the map. 
                //The pair::second element in the pair is set to true if a new element was inserted or false if an equivalent key already existed.
            }

            if (insertFactoryAndPluginNameFlags) {
                //添加插件接口实现类名称
                LOG_INFO("Add the pair of plugin name and plugin library filename into the registry.");
                (*itr).second->addPluginLibFileMap(tmpItfInfo->pluginLibFileMap());
                //(*itr).second->addPluginNames(spItfInfo->getPluginNameList());

                //添加插件类型的工厂对象
                LOG_INFO("Add Plugin factory into the registry.");
                (*itr).second->addPluginFactory(tmpItfInfo->pluginFactoryList());
            }

            return CRANE_SUCC;
        } else {
            //新增的插件接口，即其下的插件类工厂信息
            LOG_INFO("The interface is not exist, so insert the interface into the registry.");
            auto ret = _pluginItfMap.insert(make_pair(type, spItfInfo));
            if (ret.second == false) {
                //ret.first 是当前位置的迭代器；
                //插入失败；
                LOG_ERROR("Insert a new interface { %s } into the map failure.", spItfInfo->type().c_str());
                return CRANE_FAIL;
            }
            LOG_INFO("Inserted a new interface { %s } into the map.", ret.first->first.c_str());

            return CRANE_SUCC;
        }
    }

    unsigned CraneRegistry::_addPluginLibMap(unique_ptr<DlLibrary> library) {
        //getDlFileDesc = reinterpret_cast<Func_Crane_Dl_Name_Desc>(library->symbol("crane_dl_" + -lib-file-name- + "_desc"));
        //DlLibrary& dlLibrary = dynamic_cast<DlLibrary>(library);
        const PluginDesc& pluginDesc = library->pluginDesc();
        unused(pluginDesc);

        //pluginMap.insert(pluginDlDesc->name, library);
        const string& filename = library->name(); //absolate filename of the plugin library.
        PluginImplementMap::const_iterator itr = _pluginLibMap.find(filename);
        if (itr != _pluginLibMap.cend()) {
            LOG_ERROR("This plugin{ %s } has existed in plugin library map.", filename.c_str());
            return CRANE_SUCC;
        }
        _pluginLibMap[library->name()] = std::move(library);
        LOG_INFO("Add plugin{ %s } into the plugin library map.", filename.c_str());
        return CRANE_SUCC;
    }

    unsigned CraneRegistry::_delPluginLibMap(const string& filename) {
        if (filename.empty()) {
            LOG_ERROR("filename is empty.");
            return CRANE_FAIL;
        }
        PluginImplementMap::const_iterator itr = _pluginLibMap.find(filename);
        if (itr == _pluginLibMap.cend()) {
            LOG_ERROR("This plugin{ %s } has not existed in plugin library map.", filename.c_str());
            return CRANE_SUCC;
        }
        LOG_DEBUG("===========Erase { %s %s }==============", itr->first.c_str(), itr->second->pluginDesc().pluginName.c_str());

        //Pointer of DlLibrary instance is swapped by unique_ptr instance which is ONLY holded by _pluginLibMap.
        //So when shared_ptr instance is removed, the ~DlLibrary will be invoked.
        _pluginLibMap.erase(itr);
        LOG_DEBUG("=================_pluginLibMap.erase(itr);=================");
        return CRANE_SUCC;
    }

    /**
     * @Descripttion: Crane插件系统初始化
     * @Param: 
     * @Return: 
     */    
    unsigned CraneRegistry::craneInit(int argc, char** argv) {
        LOG_INFO("Crane: Init CRANE plugin system...... ");

        unused(argc, argv);
        
        _initPluginPath();

        if (_initMode() == _InitMode::CACHE_FILE) {
            LOG_INFO("Init Crane from the cache file.");
            lock_guard<mutex> lock(_mtx);
            return _parseCacheFile();
        } else {
            LOG_INFO("Init Crane through scan library files on library path.");
            lock_guard<mutex> lock(_mtx);
            _scan();
            _loadFromLib();
            _createCacheFile();
            return CRANE_SUCC;
        }
    }

    shared_ptr<PluginInterfaceInfo> CraneRegistry::findPluginItfInfo(const string& itfType) {

        PluginInterfaceMap::const_iterator itr = _pluginItfMap.find(itfType);
        if (itr == _pluginItfMap.end()) {
            //返回一个空的PluginInterfaceInfo对象；
            LOG_ERROR("Cannot find itf type{ %s } in the plugin map.", itfType.c_str());
            return shared_ptr<PluginInterfaceInfo>(nullptr);
        }
        return itr->second;
    }

    void CraneRegistry::_initPluginPath() {
        const char* sysPath = getenv("CRANE_SYS_PLUGIN_PATH");
        const char* appPath = getenv("CRANE_APP_PLUGIN_PATH");
        if ((sysPath == NULL) || (appPath == NULL)) {
            throw std::runtime_error("Obtain the plugin path from environment variable failed.");
            return;
        }
        _crane_sys_plugin_path = string(sysPath);
        _crane_app_plugin_path = string(appPath);
        return;
    }

    CraneRegistry::_InitMode CraneRegistry::_initMode() {
        const char *mode = getenv ("CRANE_INIT_MODE");

        if (mode == NULL) {
            //Initialize Crane through scan the library files.
            return _InitMode::LIB_FILE;
        }

        if (string(CRANE_INIT_CACHE_FILE).compare(string(mode)) == 0) {
            return _InitMode::CACHE_FILE;
        }

        if (string(CRANE_INIT_LIB_FILE).compare(string(mode)) == 0) {
            return _InitMode::LIB_FILE;
        }

        return _InitMode::LIB_FILE;
    }
    
    bool CraneRegistry::_isValidLibName(const string& path) {
        //path = /usr/local/lib/crane/libcranepluginABCDEFG.so
        string::size_type pos = path.find_last_of('/');
        string filename(path.substr(++pos));
        LOG_DEBUG("Lib filename: { %s }", filename.c_str());
        //filename = libcranepluginABCDEFG.so
        //libcraneABCDEFG.so--->ABCDEFG
        string::size_type bPos= filename.find(CRANE_PLUGIN_LIB_PREFIX);
        string::size_type ePos= filename.rfind(CRANE_PLUGIN_LIB_SUFFIX);
        if (((bPos != filename.npos) && (ePos != filename.npos)) && (ePos > bPos)) {
            return true;
        } else {
            LOG_ERROR("Plugin library file name { %s } is invalid.", filename.c_str());
            return false;
        }
    }

    void CraneRegistry::_listFiles(list<string>& list, const string& folder, const string& extension, bool recursive) {
        DIR* dir;
        DIR* subDir;
        struct dirent *ent;
        // try to open top folder
        dir = opendir(folder.c_str());
        if (dir == NULL){
            // could not open directory
            LOG_ERROR("Could not open \"%s\" directory.", folder.c_str());
          return;
        }else{
            // close, we'll process it next
            LOG_DEBUG("Crane: Librarys directory \"%s\".", folder.c_str());
            closedir(dir);
        }
        // enqueue top folder
        queue<string> folders;
        folders.push(folder);

        // run while has queued folders
        while (!folders.empty()){
            string currFolder = folders.front();
            LOG_DEBUG("Current folder is [%s].", currFolder.c_str());
            folders.pop();
            dir = opendir(currFolder.c_str());
            if (dir == NULL) continue;
            // iterate through all the files and directories
            while ((ent = readdir (dir)) != NULL) {
                std::string name(ent->d_name);
                // ignore "." and ".." directories
                if ( name.compare(".") == 0 || name.compare("..") == 0) continue;
                // add path to the file name
                std::string path = currFolder;
                path.append("/");
                path.append(name);
                // check if it's a folder by trying to open it
                subDir = opendir(path.c_str());
                if (subDir != NULL){
                    // it's a folder: close, we can process it later
                    closedir(subDir);
                    if (recursive) folders.push(path);
                }else{
                    // it's a file
                    if (extension.empty()){
                        LOG_DEBUG("Push path{ %s } into the library list.", path.c_str());
                        list.push_back(path);
                    }else{
                        // check file extension
                        size_t lastDot = name.find_last_of('.');
                        std::string ext = name.substr(lastDot+1);
                        if (ext.compare(extension) == 0){
                            if (!_isValidLibName(path)) {
                                continue; 
                            }
                            // match
                            LOG_DEBUG("Push plugin { %s } into the library list.", path.c_str());
                            list.push_back(path);
                        }
                    } // endif (extension test)
                } // endif (folder test)
            } // endwhile (nextFile)
            closedir(dir);
        } // endwhile (queued folders)
        
    }

    unsigned CraneRegistry::_scan() {
        _listFiles(_dlFiles, _crane_sys_plugin_path, CRANE_PLUGIN_LIB_SUFFIX, true);
        _listFiles(_dlFiles, _crane_app_plugin_path, CRANE_PLUGIN_LIB_SUFFIX, true);
        return CRANE_SUCC;
    }

    unsigned CraneRegistry::_loadFromLib() {
        list<string>::const_iterator itr;
        for(itr = _dlFiles.begin(); itr != _dlFiles.end(); ++itr) {
            LOG_DEBUG("Loading plugin library { %s }", itr->c_str());
            shared_ptr<PluginInterfaceInfo> spItfInfo = createItfInfo(*itr);
            if (!spItfInfo) {
                LOG_ERROR("Create ItfInfo with library file { %s }failed",*itr->c_str());
                return CRANE_FAIL;
            }

            //Registe the plugin information which include plugin interface, implemetation of the plugin interface and factory class of the implemetation.
            if (registry_(spItfInfo) == CRANE_SUCC) {
                LOG_INFO("Add plugin interface{ %s } into the registry successfully.", spItfInfo->type().c_str());       
            } else {
                LOG_ERROR("Add plugin interface{ %s } into the registry failed.", spItfInfo->type().c_str());
            }
        }
        return CRANE_SUCC;
    }

    shared_ptr<PluginInterfaceInfo> CraneRegistry::createItfInfo(const string& filename) {
        unique_ptr<DlLibrary> lib = DlLibrary::Load(filename);
        if (lib == nullptr) {
            LOG_ERROR("Load plugin library { %s } Failed", filename.c_str());
            return shared_ptr<PluginInterfaceInfo>(nullptr);
        }
        //read the description of the plugin and write it into the lib object.
        lib->getPluginDesc();
        const PluginDesc& pluginDesc = lib->pluginDesc();

        //Check whether the version of the plugin framework meet the need of the plugin interface.
        if (Util::compareVersion(pluginDesc.fwVer, CRANE_FRAMEWORK_VERION) == CRANE_HIGHER) {
            LOG_ERROR("Current Crane framework version[%s] is below than the version[%s] plugin library required.", 
                        CRANE_FRAMEWORK_VERION, pluginDesc.fwVer.c_str());
            return shared_ptr<PluginInterfaceInfo>(nullptr);
        } 

        //Create plugin interface instance.
        shared_ptr<PluginInterfaceInfo> spItfInfo = make_shared<PluginInterfaceInfo>(pluginDesc.itfType, 
                                                                                     pluginDesc.itfVer, 
                                                                                     pluginDesc.fwVer);
        LOG_DEBUG("PluginItf info: Itf type[%s], Itf version[%s], Framework version[%s].",
                        spItfInfo->type().c_str(), spItfInfo->curVersion().c_str(), spItfInfo->requireFwVer().c_str());

        //Insert the pluginName(class name of the implement of plugin interface) into the PluginNameList.
        spItfInfo->addPluginLibFileMap(pluginDesc.pluginName, lib->name());
        //在接口下添加插件工厂对象
        shared_ptr<AbstractPluginFactory> spPluginFactory = lib->createPluginFactory();
        if (spPluginFactory) {
            LOG_DEBUG("Create plugin factory successfully with info: Itf type{ %s }, Itf version{ %s }, Plugin name{ %s }.",
                        spPluginFactory->getInterfaceType().c_str(),
                        spPluginFactory->getVersion().c_str(),
                        spPluginFactory->getPluginName().c_str());
            spItfInfo->addPluginFactory(spPluginFactory);
        } else {
            LOG_ERROR("Create factory of plugin interface{ %s } failed.", spItfInfo->type().c_str());
            return shared_ptr<PluginInterfaceInfo>(nullptr);
        }

        //Add lib instance of the plugin into the map of the plugin lib with the pair of absolute filename and lib instance.
        if (CRANE_SUCC != _addPluginLibMap(std::move(lib))) {
            return shared_ptr<PluginInterfaceInfo>(nullptr);
        }
        return spItfInfo;
    }

    unsigned CraneRegistry::clearPlugin(const string& type, const string& pluginName) {
        shared_ptr<PluginInterfaceInfo> itf =  findPluginItfInfo(type);
        if (!itf) {
            LOG_ERROR("Cannot find the interface type:{ %s } in the registry.", type.c_str());
            return CRANE_FAIL;
        }

        string filename = itf->findPluginLibFileMap(pluginName);
        if (filename.empty()) { 
            LOG_ERROR("Plugin { %s } is not exist in _pluginLibFileMap.", pluginName);
            return CRANE_FAIL; 
        }
        itf->delPluginLibFileMap(pluginName);
        itf->delPluginFactory(pluginName);

        _delPluginLibMap(filename);
        return CRANE_SUCC;
    }
    
    unsigned CraneRegistry::_createCacheFile() {
        cout<<"_createCacheFile()"<<endl;
        //Open the cachefile
        const char* homepath = getenv("HOME");
        if (homepath == NULL) {
            LOG_ERROR("Cannot get HOME env.");
            return CRANE_FAIL;
        }
        string oldFile = string(homepath) + "/" + _CACHE_FILE;
        string tmpFile = string(homepath) + "/" + _CACHE_FILE_TMP;

        Document d;
        Document::AllocatorType& a = d.GetAllocator();
        d.SetObject();
        Value itfArray(kArrayType);
        for (PluginInterfaceMap::const_iterator itr = _pluginItfMap.begin(); itr != _pluginItfMap.end(); ++itr) {
            Value itf(kObjectType);
                Value libraryArray(kArrayType);
                    Value library(kObjectType);
            itf.AddMember("name", Value().SetString(itr->second->type().c_str(), a).Move(), a);
            itf.AddMember("ver", Value().SetString(itr->second->curVersion().c_str(), a).Move(), a);
            itf.AddMember("reqFwVer", Value().SetString(itr->second->requireFwVer().c_str(), a).Move(), a);
            
            PluginInterfaceInfo::PluginLibFileMap libFiles = itr->second->pluginLibFileMap();
            for (auto f : libFiles) {
                library.AddMember("pluginName", Value().SetString(f.first.c_str(), a).Move(), a);
                library.AddMember("absoluteFilename", Value().SetString(f.second.c_str(), a).Move(), a);

                libraryArray.PushBack(library.Move(), a);
                //library.Clear();
            }
            
            itf.AddMember("librarys", libraryArray.Move(), a);
            //libraryArray.Clear();

            itfArray.PushBack(itf.Move(), a);
            //itf.Clear();
        }
        d.AddMember("itfs", itfArray.Move(), a);
        
        FILE* tmpFp = fopen(tmpFile.c_str(), "w");
        char writeBuffer[65536];
        FileWriteStream os(tmpFp, writeBuffer, sizeof(writeBuffer));
        Writer<FileWriteStream> writer(os);
        d.Accept(writer);
        fclose(tmpFp);  

        FILE* pOldFile = fopen(oldFile.c_str(), "r");
        if (pOldFile != NULL) {
            fclose(pOldFile);
            //If old cache file is exist, then remove it.
            if (remove(oldFile.c_str()) != 0) {
                LOG_ERROR("Remove the old cache file { %s } failed.", oldFile.c_str());
                return CRANE_FAIL;
            }
        }

        if (rename(tmpFile.c_str(), oldFile.c_str()) != 0) {
            LOG_ERROR("Rename the tmp cache file name from { %s } to { %s } failed.",tmpFile.c_str(), oldFile.c_str());
            return CRANE_FAIL;
        }
 
        return CRANE_SUCC;        
    }

    unsigned CraneRegistry::_parseCacheFile() {
        const char* homepath = getenv("HOME");
        if (homepath == NULL) { 
            LOG_ERROR("Cannot find the HOME env");
            return CRANE_FAIL; 
        }
        string file = string(homepath) + "/" + _CACHE_FILE;

        Document d;
        if (Util::parseJsonFile(file, d) != CRANE_SUCC) {
            LOG_ERROR("Parse cache file { %s } failed.", file.c_str());
            return CRANE_FAIL;
        }

        Value* v = nullptr;
        list<string> h;

        //fatch app server config info.
        h = {"itfs"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) { return CRANE_FAIL; }
        if (!v->IsArray()) {
            LOG_ERROR("Type of v is not array.");
            return CRANE_FAIL;
        }

        auto itfArray = v->GetArray();
        for(auto itr = itfArray.Begin(); itr != itfArray.End(); ++itr) {
            Value* v;
            h = {"name"};
            v = Util::fatchJsonValue(*itr, h);
            if (v == nullptr) { return CRANE_FAIL; }
            string name = string(v->GetString());
            
            h = {"ver"};
            v = Util::fatchJsonValue(*itr, h);
            if (v == nullptr) { return CRANE_FAIL; }
            string ver = string(v->GetString());

            h = {"reqFwVer"};
            v = Util::fatchJsonValue(*itr, h);
            if (v == nullptr) { return CRANE_FAIL; }
            string reqFwVer = string(v->GetString());


            h = {"librarys"};
            v = Util::fatchJsonValue(*itr, h);
            if (v == nullptr) { return CRANE_FAIL; }
            PluginInterfaceInfo::PluginLibFileMap pluginLibs;
            for(auto& f : v->GetArray()) {
                string pluginName, absoluteLibFilename;
                auto itr1 = f.FindMember("pluginName");
                auto itr2 = f.FindMember("absoluteFilename");
                if ((itr1 != f.MemberEnd()) && (itr2 != f.MemberEnd())) {
                    LOG_DEBUG("Insert pair of { %s } and { %s } into ItfInfo's _libfileMap.", itr1->value.GetString(), itr2->value.GetString());
                    pluginLibs.insert(make_pair(string(itr1->value.GetString()), string(itr2->value.GetString())));
                }
            }

            //Construct the PluginInterfaceInfo instance and fill the key info of plugin and its interface.
            shared_ptr<PluginInterfaceInfo> spItfInfo = make_shared<PluginInterfaceInfo>(name, ver, reqFwVer);
            spItfInfo->addPluginLibFileMap(pluginLibs);

            LOG_DEBUG("Insert ItfInfo { %s } into registry's _pluginIntMap", name.c_str());
            _pluginItfMap.insert(make_pair(name, spItfInfo));
        }

        return CRANE_SUCC;
    }
}