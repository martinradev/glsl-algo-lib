#ifndef SCAN_UTILITIES_HPP
#define SCAN_UTILITIES_HPP

#include <random>
#include <vector>

template<typename T>
void scan(const T *input, T *output, unsigned n, unsigned blockSize, bool isInclusive)
{
    unsigned i = 0u, k = 0u;
    T tmp = static_cast<T>(0);
    while(i < n)
    {
        if (k == blockSize)
        {
            tmp = static_cast<T>(0);
            k = 0u;
        }
        if (isInclusive) tmp += input[i];
        output[i] = tmp;
        if (!isInclusive) tmp += input[i];
        ++i;
        ++k;
    } 
}

template<typename T>
void reduce(const T *input, T *output, unsigned n, unsigned blockSize)
{
    T sum = static_cast<T>(0);
    unsigned k = 0u, i = 0u;
    while(i < n)
    {
        sum += input[i];
        ++i;
        ++k;
        if (k == blockSize || i == n)
        {
            k = 0;
            *output = sum;
            ++output;
            sum = static_cast<T>(0);
        }
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

template<typename T>
std::vector<T> generateIntegralRandomVector(unsigned n, T minValue, T maxValue)
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

template<typename T>
std::vector<T> generateFloatRandomVector(unsigned n, T minValue, T maxValue)
{
    std::vector<T> result(n);
    std::random_device r;
    std::default_random_engine eng(r());
    std::uniform_real_distribution<T> dist(minValue, maxValue);
    for (unsigned i = 0u; i < n; ++i)
    {
        result[i] = dist(eng);
    }
    return result;
}

#endif // SCAN_UTILITIES_HPP