#include <gtest/gtest.h>
#include <cache/LFUCache.h>

namespace UT
{

TEST(TestLFUCache, CapacitySizeUnit)
{
    const size_t capacity = 16;
    
    caches::LFUCache<int, int> simplecache(capacity); 
}

} // namespace UT