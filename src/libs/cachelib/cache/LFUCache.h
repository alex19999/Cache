
#ifndef LFU_CACHE_H
#define LFU_CACHE_H

#include <iostream>
#include <map>
#include <unordered_map>
#include <optional>

namespace caches
{

template <typename T, typename KeyT = size_t>
class LFUCache
{
    public:
        explicit LFUCache(size_t cap);
        
        size_t getCapacity() const { return capacity; }
        bool full() const { return capacity == size; }
        
        bool lookup(const KeyT& key, const T& value);
    
    private:
        void touch(const KeyT& key, bool isfill = false);
        void insert(const KeyT& key);
        void evict();

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