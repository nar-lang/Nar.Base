#include "native.h"
#include <narc.c>

int init(init_data_t *init_data) {
    nar_init_lib(init_data);
    register_array();
    register_basics();
    register_bitwise();
    register_char();
    register_debug();
    register_list();
    register_math();
    register_string();
    return 0;
}
