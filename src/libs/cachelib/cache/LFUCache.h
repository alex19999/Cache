
#ifndef LFU_CACHE_H
#define LFU_CACHE_H

#include <iostream>
#include <map>
#include <unordered_map>
#include <optional>

namespace caches
{

template <typename T, typename KeyT>
class LFUCache
{
    public:
        explicit LFUCache(size_t cap)
            : capacity(cap)
            , size(0)
            {
            }
        
        size_t getCapacity() const { return capacity; }
        size_t getSize() const { return size; }
        bool full() const { return capacity == size; }
        
        bool lookup(const KeyT& key, const T& value)
        {
            // Update value
            if (keyToVal.find(key) != keyToVal.end())
            {
                touch(key);
                keyToVal[key] = value;
                return true;
            }
            
            // Erase if needed
            if (full())
            {
                evict();
            }
            touch(key, true);
            keyToVal.emplace(key, value);
            return false;
        }
    
    private:
        void touch(const KeyT& key, bool isfill = false)
        {
            if (isfill)
            {
                insert(key);
                return;
            }
            
            if (keyToFreq.find(key) == keyToFreq.end())
            {
                throw std::runtime_error("LFUCache[touch]: no key to increment");
            }
            
            // Update keys
            auto elementToUpdate = keyToFreq[key];
            const auto newFrequency = elementToUpdate->first + 1;
            freqToKey.erase(elementToUpdate);
            keyToFreq[key] = freqToKey.emplace(newFrequency, key);
        }
        
        void insert(const KeyT& key)
        {
            if (size >= capacity)
            {
                throw std::runtime_error("LFUCache[insert]: overflow");
            }
            keyToFreq.emplace(key, freqToKey.emplace(0, key));
            size++;
        }
        
        void evict()
        {
            auto elementToEvict = freqToKey.begin()->second;
            freqToKey.erase(keyToFreq[elementToEvict]);
            keyToVal.erase(elementToEvict);
            keyToFreq.erase(elementToEvict);

            if (size <= 0)
            {
                throw std::runtime_error("LFUCache[evict]: size should be at least zero");
            }
            size--;
        }

        size_t capacity;
        size_t size;

        using cacheIter = typename std::multimap<size_t, KeyT>::iterator;
        // Data
        std::unordered_map<KeyT, T> keyToVal;
        // Info to make evicting decisions
        std::multimap<size_t, KeyT> freqToKey;
        std::unordered_map<KeyT, cacheIter> keyToFreq;
};

} // namespace caches

#endif