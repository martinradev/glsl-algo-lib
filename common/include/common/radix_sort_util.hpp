#ifndef RADIX_SORT_UTIL_HPP
#define RADIX_SORT_UTIL_HPP

#include "scan_utilities.hpp"

#include <cassert>
#include <vector>

inline unsigned decode(unsigned el, unsigned radixOffset, unsigned radixSize)
{
	unsigned t = (el >> radixOffset) & ((1<<radixSize)-1);
    return t;
}

inline void radix_sort_gather(const unsigned *input, unsigned *output, unsigned n, unsigned block_size, unsigned radixOffset, unsigned radixSize)
{
    const unsigned numBlocks = (n + block_size - 1) / block_size;
    for (unsigned i = 0u, b = 0u; i < n; i+=block_size, ++b)
    {
        std::vector<unsigned> cnt(1<<radixSize, 0u);
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

inline void radix_sort_single_pass(const unsigned *input, unsigned *output, unsigned n, unsigned radixOffset, unsigned radixSize)
{
    std::vector<unsigned> cnt(1<<radixSize, 0u);
    for (unsigned j = 0u; j < n; ++j)
    {
        cnt[decode(input[j], radixOffset, radixSize)] += 1;
    }
    unsigned prev = 0u;
    for (unsigned j = 0u; j < (1<<radixSize); ++j)
    {
        unsigned tmp = cnt[j];
        cnt[j] = prev;
        prev += tmp;
    }
    for (unsigned j = 0u; j < n; ++j)
    {
        unsigned key = decode(input[j], radixOffset, radixSize);
        output[cnt[key]] = input[j];
        ++cnt[key];
    }
}

#endif // RADIX_SORT_UTIL_HPP