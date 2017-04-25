#ifndef SCAN_UTILITIES_HPP
#define SCAN_UTILITIES_HPP

#include <glsl_algo/init.h>

#include <random>
#include <vector>
#include <cassert>
#include <cmath>
#include <limits>

namespace
{
  
template<typename T>
T apply_scan_operator(T a, T b, GLSL_AlGO_SCAN_OPERATOR scan_op)
{
  
    if (scan_op == GASOadd)
    {
        return a+b;
    }
    else if (scan_op == GASOmin)
    {
        return std::min(a,b);
    }
    else if (scan_op == GASOmax)
    {
        return std::max(a,b);
    }
    assert(false && "undefined operator");
    return static_cast<T>(0);
}

template<typename T>
T get_initial_value(GLSL_AlGO_SCAN_OPERATOR scan_op)
{
      if (scan_op == GASOadd)
      {
          return static_cast<T>(0);
      }
      else if (scan_op == GASOmin)
      {
          return std::numeric_limits<T>::max();
      }
      else if(scan_op == GASOmax)
      {
          return std::numeric_limits<T>::lowest();
      }
      assert(false && "undefined operator");
      return static_cast<T>(0);
}

} // namespace

template<typename T>
void scan(const T *input, T *output, unsigned n, unsigned blockSize, bool isInclusive, GLSL_AlGO_SCAN_OPERATOR scan_op = GASOadd)
{
    unsigned i = 0u, k = 0u;
    T tmp = ::get_initial_value<T>(scan_op);
    while(i < n)
    {
        if (k == blockSize)
        {
            tmp = ::get_initial_value<T>(scan_op);
            k = 0u;
        }
        if (isInclusive) tmp = ::apply_scan_operator(tmp, input[i], scan_op);
        output[i] = tmp;
        if (!isInclusive) tmp = ::apply_scan_operator(tmp, input[i], scan_op);
        ++i;
        ++k;
    } 
}

template<typename T>
void reduce(const T *input, T *output, unsigned n, unsigned blockSize, GLSL_AlGO_SCAN_OPERATOR scan_op = GASOadd)
{
    T sum = ::get_initial_value<T>(scan_op);
    unsigned k = 0u, i = 0u;
    while(i < n)
    {
        sum = ::apply_scan_operator(sum, input[i], scan_op);
        ++i;
        ++k;
        if (k == blockSize || i == n)
        {
            k = 0;
            *output = sum;
            ++output;
            sum = ::get_initial_value<T>(scan_op);
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