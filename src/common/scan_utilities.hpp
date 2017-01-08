#ifndef SCAN_UTILITIES_HPP
#define SCAN_UTILITIES_HPP

#include <random>
#include <vector>

template<typename T>
void scan(const T *input, T *output, unsigned n, bool isInclusive)
{
    unsigned i = 0u;
    T tmp = isInclusive ? input[i] : static_cast<T>(0);
    while(i < n)
    {
        output[i] = tmp;
        tmp += input[i];
        ++i;
    } 
}

template<typename T>
std::vector<T> generateRandomVector(unsigned n, T minValue, T maxValue)
{
    std::vector<T> result(n);
    
    std::random_device r;
    std::default_random_engine eng(r());
    std::uniform_int_distribution<T> dist(minValue, maxValue);
    for (unsigned i = 0u; i < n; ++i)
    {
        result[i] = dist(eng);
    }
    
    return result;
}

#endif // SCAN_UTILITIES_HPP