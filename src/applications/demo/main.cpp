#include <iostream>
#include <string>

#include <cache/LFUCache.h>
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

    caches::LFUCache<size_t, size_t> cache{ cacheSize };

    std::cout << "Enter number of keys" << std::endl;
    std::cin >> numKeys;
    
    size_t hits = 0;
    for (size_t i = 0; i < numKeys; ++i)
    {
        std::cin >> key;
        hits += cache.lookup(key, 1);

    }
    std::cout << "Number of hits = " << hits << std::endl;
    return 0;
}
#else
int main(int argc, char* argv[])
{
    auto params = parseCMD(argc, argv);
    size_t hits = 0;
    caches::LFUCache<size_t, size_t> cache{ params.mCacheSize };
    for (size_t i = 0; i < params.mKeys.size(); ++i)
    {
        hits += cache.lookup(params.mKeys[i], 1);
    }
    std::cout << "Number of hits = " << hits << std::endl;
    return 0;
}
#endif
