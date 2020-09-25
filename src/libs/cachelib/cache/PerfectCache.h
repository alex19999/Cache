#ifndef PERFECT_CACHE_H
#define PERFECT_CACHE_H

#include <iostream>
#include <unordered_map>
#include <set>
#include <chrono>
#include <thread>

#include "CacheInterface.h"

namespace caches
{

template <typename T, typename KeyT>
class PerfectCache: public CacheInterface<T, KeyT>
{
    public:
        explicit PerfectCache(size_t cap, const std::vector<KeyT> seqOfKeys)
            : capacity(cap)
            , size(0)
            , fullSeqLen(seqOfKeys.size())
            , currentStep(0)
            {
                for (size_t i = 0; i < seqOfKeys.size(); ++i)
                {
                    const auto key = seqOfKeys[i];
                    if (linesUses.find(key) != linesUses.end())
                    {
                        linesUses[key].insert(i);
                    }
                    else
                    {
                        linesUses[key] = std::set{ i };
                    }
                }
            }
        
        size_t getCapacity() const { return capacity; }
        size_t getSize() const { return size; }
        bool full() const { return capacity == size; }
        
        bool lookup(const KeyT& key) override
        {
            if (values.find(key) != values.end())
            {
                currentStep++;
                return true;
            }
            
            // Erase if needed
            if (full())
            {
                evict();
            }
            insert(key);

            currentStep++;
            return false;
        }
    
    private:
        void touch(const KeyT& key, bool isfill = false) override
        {
        }
        
        void insert(const KeyT& key) override
        {
            values[key] = slowLoad(key);
            size++;
        }
        
        void evict() override
        {
            auto victim = values.begin()->first;
            size_t victimLastUse = 0;
            for (auto& line : values)
            {
                const auto currentKey = line.first;
                const auto allLineUses = linesUses[currentKey];
                const auto nextLineUse = allLineUses.upper_bound(currentStep);
                
                // Delete never used in future key
                if (nextLineUse == allLineUses.end())
                {
                    victim = currentKey;
                    break;
                }

                if (*nextLineUse > victimLastUse)
                {
                    victimLastUse = *nextLineUse;
                    victim = currentKey;
                }
            }

            values.erase(victim);
            size--;
        }

        T slowLoad([[maybe_unused]] const KeyT& key) const override
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            return static_cast<T>(1);
        }

        size_t capacity;
        size_t size;

        size_t fullSeqLen;
        size_t currentStep;
        
        // Info about uses
        std::unordered_map<KeyT, std::set<size_t>> linesUses;

        // Storage
        std::unordered_map<KeyT, T> values;
};

} // namespace caches

#endif