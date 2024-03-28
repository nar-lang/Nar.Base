#include "_package.h"

nar_object_t bitwise_and(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    return nar->new_int(rt, nar->to_int(rt, x) & nar->to_int(rt, y));
}

nar_object_t bitwise_or(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    return nar->new_int(rt, nar->to_int(rt, x) | nar->to_int(rt, y));
}

nar_object_t bitwise_xor(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    return nar->new_int(rt, nar->to_int(rt, x) ^ nar->to_int(rt, y));
}

nar_object_t bitwise_complement(nar_runtime_t rt, nar_object_t x) {
    return nar->new_int(rt, ~nar->to_int(rt, x));
}

nar_object_t bitwise_shiftLeftBy(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    return nar->new_int(rt, nar->to_int(rt, y) << nar->to_int(rt, x));
}

nar_object_t bitwise_shiftRightBy(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    return nar->new_int(rt, nar->to_int(rt, y) >> nar->to_int(rt, x));
}

nar_object_t bitwise_shiftRightZfBy(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    nar_uint_t ux = ((nar_uint_t) nar->to_int(rt, x));
    nar_uint_t uy = ((nar_uint_t) nar->to_int(rt, y));
    return nar->new_int(rt, (nar_int_t) (uy >> ux));
}

void register_bitwise(nar_runtime_t rt) {
    nar_string_t module_name = "Nar.Base.Bitwise";
    nar->register_def(rt, module_name, "and", nar->new_func(rt, &bitwise_and, 2));
    nar->register_def(rt, module_name, "or", nar->new_func(rt, &bitwise_or, 2));
    nar->register_def(rt, module_name, "xor", nar->new_func(rt, &bitwise_xor, 2));
    nar->register_def(rt, module_name, "complement", nar->new_func(rt, &bitwise_complement, 1));
    nar->register_def(rt, module_name, "shiftLeftBy", nar->new_func(rt, &bitwise_shiftLeftBy, 2));
    nar->register_def(rt, module_name, "shiftRightBy",
            nar->new_func(rt, &bitwise_shiftRightBy, 2));
    nar->register_def(rt, module_name, "shiftRightZfBy",
            nar->new_func(rt, &bitwise_shiftRightZfBy, 2));
}
