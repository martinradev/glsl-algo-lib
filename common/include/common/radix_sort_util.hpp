#ifndef RADIX_SORT_UTIL_HPP
#define RADIX_SORT_UTIL_HPP

#include "scan_utilities.hpp"

#include <cassert>

inline int decode(int el, unsigned radixOffset, unsigned radixSize)
{
    int t = (el >> radixOffset) & ((1<<radixSize)-1);
    return t;
}

inline void radix_sort_gather(const int *input, int *output, unsigned n, unsigned block_size, unsigned radixOffset, unsigned radixSize)
{
    const unsigned numBlocks = (n + block_size - 1) / block_size;
    for (unsigned i = 0u, b = 0u; i < n; i+=block_size, ++b)
    {
        unsigned cnt[1<<radixSize];
        for (unsigned j = 0u; j < (1<<radixSize); ++j)
        {
            cnt[j] = 0u;
        }
        for(unsigned j = 0u; j < block_size && i+j < n; ++j)
        {
            cnt[decode(input[i+j], radixOffset, radixSize)] += 1;
        }
        for (unsigned j = 0u; j < (1<<radixSize); ++j)
        {
            output[b+j*numBlocks] = cnt[j];
        }
    }
}

#endif // RADIX_SORT_UTIL_HPP