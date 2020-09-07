#include "LFUCache.h"

namespace caches
{

template <typename T, typename KeyT>
LFUCache<T, KeyT>::LFUCache(size_t cap)
    : capacity(cap)
    , size(0)
{
} 

template <typename T, typename KeyT>
bool LFUCache<T, KeyT>::lookup(const KeyT& key, const T& value)
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

template <typename T, typename KeyT>
void LFUCache<T, KeyT>::touch(const KeyT& key, bool isfill)
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
    freqToKey.erase(elementToUpdate);
    keyToFreq(key, 
        freqToKey.emplace(elementToUpdate.first + 1, elementToUpdate.second));
}

template <typename T, typename KeyT>
void LFUCache<T, KeyT>::insert(const KeyT& key)
{
    if (size >= capacity)
    {
        throw std::runtime_error("LFUCache[insert]: overflow")
    }
    keyToFreq.emplace(key, freqToKey.emplace(0, key));
    size++;
}

template <typename T, typename KeyT>
void LFUCache<T, KeyT>::evict()
{
    auto elementToEvict = freqToKey.begin()->seond();
    freqToKey.erase(keyToFreq[elementToEvict]);
    keyToVal.erase(elementToUpdate);
    keyToFreq.erase(elementToEvict);

    if (size <= 0)
    {
        throw std::runtime_error("LFUCache[evict]: size should be at least zero")
    }
    size--;
}

} // namespace caches