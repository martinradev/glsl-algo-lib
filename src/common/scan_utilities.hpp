#ifndef SCAN_UTILITIES_HPP
#define SCAN_UTILITIES_HPP

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

#endif // SCAN_UTILITIES_HPP