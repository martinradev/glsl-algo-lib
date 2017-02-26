#ifndef UTIL_HPP
#define UTIL_HPP

#define BENCHMARK_GPU(name, call) call; \
state.SetIterationTime(double(name##_Elapsed) / 1000000000.0);

#endif // UTIL_HPP