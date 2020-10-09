#pragma once

struct RefCount
{
    int SmartPtrCount;
    int WeakPtrCount;

    RefCount(const int smartPtrCount, const int weakPtrCount) : 
        SmartPtrCount(smartPtrCount), WeakPtrCount(weakPtrCount)
    {}
};