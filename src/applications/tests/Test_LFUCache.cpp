#include <algorithm>
#include <gtest/gtest.h>
#include <cache/LFUCache.h>

namespace UT
{

TEST(TestLFUCache, CapacitySizeUnit)
{
    const size_t capacity = 16;
    
    caches::LFUCache<int, int> simplecache{capacity};
    
    EXPECT_EQ(simplecache.getCapacity(), capacity);
    EXPECT_EQ(simplecache.getSize(), 0);
    for (int i = 0; i < 20; ++i)
    {
        simplecache.lookup(i, i);
        EXPECT_EQ(simplecache.getSize(), std::min(i + 1, static_cast<int>(capacity)));
    } 
}

TEST(TestLFUCache, TrivialUnit)
{
    const size_t capacity = 100;
    const size_t repeat = 3;
    const size_t numOfRequests = 50;
    size_t hits = 0;
    caches::LFUCache<size_t, size_t> cache{capacity};
    
    for (size_t j = 0; j < repeat; ++j)
    {
        for (size_t i = 0; i < numOfRequests; ++i)
        {
            hits += static_cast<size_t>(cache.lookup(i, i));
        }
    }
    EXPECT_EQ(hits, 100);
}

TEST(TestLFUCache, StreamUnit)
{
    const size_t capacity = 100;
    const size_t repeat = 3;
    const size_t numOfRequests = 1000;
    size_t hits = 0;
    caches::LFUCache<size_t, size_t> cache{capacity};
    
    for (size_t j = 0; j < repeat; ++j)
    {
        for (size_t i = 0; i < numOfRequests; ++i)
        {
            hits += static_cast<size_t>(cache.lookup(i, i));
        }
    }
    EXPECT_EQ(hits, 0);
}

TEST(TestLFUCache, TrashingUnit)
{
    const size_t capacity = 100;
    const size_t repeat = 3;
    const size_t numOfRequests = 101;
    size_t hits = 0;
    caches::LFUCache<size_t, size_t> cache{capacity};
    
    for (size_t j = 0; j < repeat; ++j)
    {
        for (size_t i = 0; i < numOfRequests; ++i)
        {
            hits += static_cast<size_t>(cache.lookup(i, i));
        }
    }

    // Not trashing resistant like LRU
    EXPECT_EQ(hits, 0);
}

TEST(TestLFUCache, ScanUnit)
{
    const size_t capacity = 100;
    const size_t repeat = 3;
    const size_t numOfRequests = 100;
    const size_t scanSize = 10;
    size_t hits = 0;
    caches::LFUCache<size_t, size_t> cache{capacity};
    
    // Stable pattern fill the cache
    for (size_t j = 0; j < repeat; ++j)
    {
        for (size_t i = 0; i < numOfRequests; ++i)
        {
            hits += static_cast<size_t>(cache.lookup(i, i));
        }
    }

    // Scan pattern, which evicts 10 useful elements, 0 hits because new
    for (size_t i = 0; i < scanSize; ++i)
    {
        hits += static_cast<size_t>(cache.lookup(i + 10000, i));
    }

    // Must reveive 99 hits
    for (size_t i = 0; i < numOfRequests; ++i)
    {
        hits += static_cast<size_t>(cache.lookup(i, i));
    }

    // LFU is scan-resistant unlike LRU
    EXPECT_EQ(hits, 299);
}


} // namespace UT