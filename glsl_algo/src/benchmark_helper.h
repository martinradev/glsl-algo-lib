#ifndef BENCHMARK_HELPER_H
#define BENCHMARK_HELPER_H

#ifdef IS_BENCHMARK_BUILD

#define BENCHMARK_GPU_CALL(gl, call, output) gl->glBeginQuery(GL_TIME_ELAPSED, output##_Query);\
call;\
gl->glEndQuery(GL_TIME_ELAPSED);\
int stopTimerAvailable = 0;\
while (!stopTimerAvailable) {\
     gl->glGetQueryObjectiv(output##_Query,\
     GL_QUERY_RESULT_AVAILABLE,\
     &stopTimerAvailable);\
}\
gl->glGetQueryObjectui64v(output##_Query, GL_QUERY_RESULT, &output##_Elapsed);

#else

#define BENCHMARK_GPU_CALL(gl, call, output) call

#endif

#endif // BENCHMARK_HELPER_H