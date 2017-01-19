#ifndef UTIL_HPP
#define UTIL_HPP

#define BENCHMARK_GPU(call, queryObject) glBeginQuery(GL_TIME_ELAPSED, queryObject);\
call;\
glEndQuery(GL_TIME_ELAPSED);\
int stopTimerAvailable = 0;\
while (!stopTimerAvailable) {\
     glGetQueryObjectiv(queryObject,\
     GL_QUERY_RESULT_AVAILABLE,\
     &stopTimerAvailable);\
}\
GLuint64 elapsedTime;\
glGetQueryObjectui64v(queryObject, GL_QUERY_RESULT, &elapsedTime);\
state.SetIterationTime(double(elapsedTime) / 1000000000.0);

#endif // UTIL_HPP