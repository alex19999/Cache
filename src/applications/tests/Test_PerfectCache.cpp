#include <algorithm>
#include <numeric>
#include <gtest/gtest.h>
#include <cache/PerfectCache.h>

namespace UT
{

TEST(TestPerfectCache, CapacitySizeUnit)
{
    const size_t capacity = 16;
    caches::PerfectCache<int, int> simplecache(capacity, std::vector<int>(20, 1));
    
    EXPECT_EQ(simplecache.getCapacity(), capacity);
    EXPECT_EQ(simplecache.getSize(), 0);
    for (int i = 0; i < 20; ++i)
    {
        simplecache.lookup(i);
        EXPECT_EQ(simplecache.getSize(), std::min(i + 1, static_cast<int>(capacity)));
    } 
}


TEST(TestPerfectCache, TrivialUnit)
{
    const size_t capacity = 100;
    const size_t repeat = 3;
    const size_t numOfRequests = 50;
    size_t hits = 0;
    std::vector<size_t> seqOfKeys(repeat * numOfRequests);
    caches::PerfectCache<size_t, size_t> cache{capacity, seqOfKeys};
    
    for (size_t j = 0; j < repeat; ++j)
    {
        for (size_t i = 0; i < numOfRequests; ++i)
        {
            hits += static_cast<size_t>(cache.lookup(i));
        }
    }
    EXPECT_EQ(hits, 100);
}


TEST(TestPerfectCache, StreamUnit)
{
    const size_t capacity = 100;
    const size_t repeat = 3;
    const size_t numOfRequests = 1000;
    size_t hits = 0;
    // Sequence
    std::vector<size_t> seqOfKeys(repeat * numOfRequests);
    std::iota(seqOfKeys.begin(), seqOfKeys.begin() + numOfRequests, 0);
    std::iota(seqOfKeys.begin() + numOfRequests, seqOfKeys.begin() + 2 * numOfRequests, 0);
    std::iota(seqOfKeys.begin() + 2 * numOfRequests, seqOfKeys.end(), 0);
    caches::PerfectCache<size_t, size_t> cache{ capacity, seqOfKeys };
    
    for (size_t j = 0; j < repeat; ++j)
    {
        for (size_t i = 0; i < numOfRequests; ++i)
        {
            hits += static_cast<size_t>(cache.lookup(i));
        }
    }
    EXPECT_EQ(hits, 200);
}


TEST(TestPerfectCache, TrashingUnit)
{
    const size_t capacity = 100;
    const size_t repeat = 3;
    const size_t numOfRequests = 101;
    size_t hits = 0;
     // Sequence
    std::vector<size_t> seqOfKeys(repeat * numOfRequests);
    std::iota(seqOfKeys.begin(), seqOfKeys.begin() + numOfRequests, 0);
    std::iota(seqOfKeys.begin() + numOfRequests, seqOfKeys.begin() + 2 * numOfRequests, 0);
    std::iota(seqOfKeys.begin() + 2 * numOfRequests, seqOfKeys.end(), 0);
    caches::PerfectCache<size_t, size_t> cache{ capacity, seqOfKeys };
    
    for (size_t j = 0; j < repeat; ++j)
    {
        for (size_t i = 0; i < numOfRequests; ++i)
        {
            hits += static_cast<size_t>(cache.lookup(i));
        }
    }

    EXPECT_EQ(hits, 200);
}

TEST(TestPerfectCache, ScanUnit)
{
    const size_t capacity = 100;
    const size_t repeat = 3;
    const size_t numOfRequests = 100;
    const size_t scanSize = 10;
    size_t hits = 0;
    // Sequence
    std::vector<size_t> seqOfKeys((repeat + 1) * numOfRequests + scanSize);
    std::iota(seqOfKeys.begin(), seqOfKeys.begin() + repeat * numOfRequests, 0);
    std::iota(seqOfKeys.begin() + repeat * numOfRequests, seqOfKeys.begin() + repeat * numOfRequests + scanSize, 10000);
    std::iota(seqOfKeys.begin() + repeat * numOfRequests + scanSize, seqOfKeys.end(), 0);
    caches::PerfectCache<size_t, size_t> cache{ capacity, seqOfKeys };
    
    // Stable pattern fill the cache
    for (size_t j = 0; j < repeat; ++j)
    {
        for (size_t i = 0; i < numOfRequests; ++i)
        {
            hits += static_cast<size_t>(cache.lookup(i));
        }
    }

    // Scan pattern, which evicts 10 useful elements, 0 hits because new
    for (size_t i = 0; i < scanSize; ++i)
    {
        hits += static_cast<size_t>(cache.lookup(10000 + i));
    }

    // Must reveive 99 hits
    for (size_t i = 0; i < numOfRequests; ++i)
    {
        hits += static_cast<size_t>(cache.lookup(i));
    }

    EXPECT_EQ(hits, 299);
}

} // namespace UT