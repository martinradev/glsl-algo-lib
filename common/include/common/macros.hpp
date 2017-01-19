#ifndef MACROS_HPP
#define MACROS_HPP

#define UNIMPLEMENTED(error_msg) \
fprintf(stderr, "Not implemented: %s\n", error_msg); \
exit(1);

#endif // MACROS_HPP