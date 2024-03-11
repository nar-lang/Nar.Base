#include "native.h"

nar_object_t bitwise_and(nar_object_t x, nar_object_t y) {
    return nar_int(nar_to_int(x) & nar_to_int(y));
}

nar_object_t bitwise_or(nar_object_t x, nar_object_t y) {
    return nar_int(nar_to_int(x) | nar_to_int(y));
}

nar_object_t bitwise_xor(nar_object_t x, nar_object_t y) {
    return nar_int(nar_to_int(x) ^nar_to_int(y));
}

nar_object_t bitwise_complement(nar_object_t x) {
    return nar_int(~nar_to_int(x));
}

nar_object_t bitwise_shiftLeftBy(nar_object_t x, nar_object_t y) {
    return nar_int(nar_to_int(y) << nar_to_int(x));
}

nar_object_t bitwise_shiftRightBy(nar_object_t x, nar_object_t y) {
    return nar_int(nar_to_int(y) >> nar_to_int(x));
}

nar_object_t bitwise_shiftRightZfBy(nar_object_t x, nar_object_t y) {
    return nar_int((nar_int_t) (((nar_uint_t) nar_to_int(y)) >> ((nar_uint_t) nar_to_int(x))));
}

void register_bitwise(void) {
    nar_string_t module_name = L"Nar.Base.Bitwise";
    nar_register_def(module_name, L"and", nar_func(&bitwise_and, 2));
    nar_register_def(module_name, L"or", nar_func(&bitwise_or, 2));
    nar_register_def(module_name, L"xor", nar_func(&bitwise_xor, 2));
    nar_register_def(module_name, L"complement", nar_func(&bitwise_complement, 1));
    nar_register_def(module_name, L"shiftLeftBy", nar_func(&bitwise_shiftLeftBy, 2));
    nar_register_def(module_name, L"shiftRightBy", nar_func(&bitwise_shiftRightBy, 2));
    nar_register_def(module_name, L"shiftRightZfBy", nar_func(&bitwise_shiftRightZfBy, 2));
}
