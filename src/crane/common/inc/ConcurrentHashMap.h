#ifndef __CRANE_CONCURRENTHASHMAP_H__
#define __CRANE_CONCURRENTHASHMAP_H__

#include <memory>
#include <unordered_map>
#include <mutex>
#include <condition_variable>

#include "craneMacro.h"

using namespace std;

namespace NS_CRANE {
    template<typename K, typename T>
    class ConcurrentHashMap {
        private:
            unordered_map<K, T> _data;
            mutable mutex _mtx;
            T _null_entry = T();

        public:
            ConcurrentHashMap() {}

            ConcurrentHashMap(const ConcurrentHashMap& o) {
                lock_guard<mutex> lock(o._mtx);
                _data = o._data;
            }

            ConcurrentHashMap& operator=(const ConcurrentHashMap&) = delete;

            /**
             * @Descripttion:   get the value of the key.
             * @Param:          key: key of data.
             * @Param[out]:     null 
             * @Return:         T&
             */    
            const T& get(const K& key) {
                lock_guard<mutex> lock(_mtx);
                auto itr = _data.find(key);
                if (itr != _data.end()) {
                    return itr->second;
                } else {
                    return _null_entry;
                }
            }

            /**
             * @Descripttion:   Push ref of data into the queue.
             * @Param:          val: reference of data.
             * @Param[out]:     null 
             * @Return:         void
             */   
            void put(const K& key, const T& data) {
                lock_guard<mutex> lock(_mtx);
                _data.insert({key, data});
                //_data.insert(make_pair<K, T>(key, data));
            }

            /**
             * @Descripttion:   Put right ref of data into the map.
             * @Param:          val: right reference of data.
             * @Param[out]:     null 
             * @Return:         void
             */    
            void put(const K& key, const T&& data) {
                lock_guard<mutex> lock(_mtx);
                _data.insert({key, data});
                //_data.insert(make_pair<K, T>(key, data));
            }

            /**
             * @Descripttion:   Del data in the map by key.
             * @Param:          key: key of data.
             * @Param[out]:     null 
             * @Return:         this is 1 if an element with a key value of k existed (and thus was subsequently erased), and zero otherwise. 
             */    
            typename unordered_map<K, T>::size_type del(const K& key) {
                lock_guard<mutex> lock(_mtx);
                return _data.erase(key);
            }

            bool empty() const {
                lock_guard<mutex> lock(_mtx);
                return _data.empty();
            }

            typename unordered_map<string, T>::size_type size() const {
                lock_guard<mutex> lock(_mtx);
                return _data.size();
            }
    };
}

#endif