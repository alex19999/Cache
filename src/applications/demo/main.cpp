#include <iostream>
#include <string>

#include <cache/LFUCache.h>
#include <cache/PerfectCache.h>
#include <tclap/CmdLine.h>

//#define NAIVE_PARSE

struct cmdParam
{
    size_t mCacheSize;
    std::vector<size_t> mKeys;
};

cmdParam parseCMD(int argc, char* argv[])
{
    cmdParam res;
    TCLAP::CmdLine cmd("", ' ', "1.0", false);

    // Read size
    TCLAP::ValueArg<size_t> cacheSize("", "cache_size", "Cache size", false, 0, "size_t");
    cmd.add(cacheSize);
    
    // And all other numbers will be considered as sequence of keys
    TCLAP::UnlabeledMultiArg<size_t> keys("keys", "Sequence of keys", false, "Vector of keys", cmd);

    cmd.parse(argc, argv);

    res.mCacheSize = cacheSize.getValue();
    res.mKeys = keys.getValue();
    return res;
}

#ifdef NAIVE_PARSE
int main(int argc, char* argv[])
{
    size_t cacheSize = 0;
    size_t numKeys = 0;
    size_t key = 0;
    
    std::cout << "Enter capacity of cache" << std::endl;
    std::cin >> cacheSize;

    caches::LFUCache<size_t, size_t> lfuCache{ cacheSize };

    std::cout << "Enter number of keys" << std::endl;
    std::cin >> numKeys;
    
    size_t lfuHits = 0;
    size_t perfectHits = 0;
    std::vector<size_t> seqOfKeys(numKeys);
    for (size_t i = 0; i < numKeys; ++i)
    {
        std::cin >> key;
        lfuHits += lfuCache.lookup(key);
        seqOfKeys[i] = key;

    }

    caches::PerfectCache<size_t, size_t> perfectCache{ cacheSize, seqOfKeys };
    for (size_t i = 0; i < numKeys; ++i)
    {
        perfectHits += perfectCache.lookup(seqOfKeys[i]);
    }
    std::cout << "Number of hits in LFU cache = " << lfuHits << std::endl;
    std::cout << "Number of hits in Perfect cache = " << perfectHits << std::endl;
    return 0;
}
#else
int main(int argc, char* argv[])
{
    auto params = parseCMD(argc, argv);
    size_t lfuHits = 0;
    size_t perfectHits = 0;
    caches::LFUCache<size_t, size_t> lfuCache{ params.mCacheSize };
    caches::PerfectCache<size_t, size_t> perfectCache{ params.mCacheSize, params.mKeys };
    for (size_t i = 0; i < params.mKeys.size(); ++i)
    {
        lfuHits += lfuCache.lookup(params.mKeys[i]);
        perfectHits += perfectCache.lookup(params.mKeys[i]);
    }
    std::cout << "Number of hits in LFU cache = " << lfuHits << std::endl;
    std::cout << "Number of hits in Perfect cache = " << perfectHits << std::endl;
    return 0;
}
#endif
