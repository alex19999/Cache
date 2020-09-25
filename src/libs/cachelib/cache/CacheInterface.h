#ifndef CACHE_INTERFACE_H
#define CACHE_INTERFACE_H

namespace caches
{

template <typename T, typename KeyT>
class CacheInterface
{

public:
    CacheInterface() = default;
    virtual ~CacheInterface() = default;

    CacheInterface(const CacheInterface& other) = delete;
    CacheInterface(const CacheInterface&& other) = delete;

    virtual bool lookup(const KeyT& key) = 0;

private:
    virtual void touch(const KeyT& key, bool isfill = false) = 0;
    virtual void insert(const KeyT& key) = 0;
    virtual void evict() = 0;
    virtual T slowLoad([[maybe_unused]] const KeyT& key) const = 0;
};

}

#endif