#include "_package.h"

nar_int_t init(nar_t *n, nar_runtime_t rt) {
    nar = n;
    register_array(rt);
    register_basics(rt);
    register_bitwise(rt);
    register_char(rt);
    register_debug(rt);
    register_list(rt);
    register_math(rt);
    register_string(rt);
    return 0;
}
