#include "native.h"
#include "utils.h"

nar_object_t basics_eq(nar_object_t a, nar_object_t b) {
    return nar_bool(cmp(a, b) == 0);
}

nar_object_t basics_neq(nar_object_t a, nar_object_t b) {
    return nar_bool(cmp(a, b) != 0);
}

nar_object_t basics_lt(nar_object_t a, nar_object_t b) {
    return nar_bool(cmp(a, b) < 0);
}

nar_object_t basics_gt(nar_object_t a, nar_object_t b) {
    return nar_bool(cmp(a, b) > 0);
}

nar_object_t basics_le(nar_object_t a, nar_object_t b) {
    return nar_bool(cmp(a, b) <= 0);
}

nar_object_t basics_ge(nar_object_t a, nar_object_t b) {
    return nar_bool(cmp(a, b) >= 0);
}

nar_object_t basics_not(nar_object_t x) {
    return nar_bool(!nar_to_bool(x));
}

nar_object_t basics_and(nar_object_t x, nar_object_t y) {
    return nar_bool(nar_to_bool(x) && nar_to_bool(y));
}

nar_object_t basics_or(nar_object_t x, nar_object_t y) {
    return nar_bool(nar_to_bool(x) || nar_to_bool(y));
}

nar_object_t basics_xor(nar_object_t x, nar_object_t y) {
    return nar_bool(nar_to_bool(x) ^ nar_to_bool(y));
}

void register_basics(void) {
    nar_string_t module_name = L"Nar.Base.Basics";
    nar_register_def(module_name, L"eq", nar_func(&basics_eq, 2));
    nar_register_def(module_name, L"neq", nar_func(&basics_neq, 2));
    nar_register_def(module_name, L"lt", nar_func(&basics_lt, 2));
    nar_register_def(module_name, L"gt", nar_func(&basics_gt, 2));
    nar_register_def(module_name, L"le", nar_func(&basics_le, 2));
    nar_register_def(module_name, L"ge", nar_func(&basics_ge, 2));
    nar_register_def(module_name, L"not", nar_func(&basics_not, 1));
    nar_register_def(module_name, L"and", nar_func(&basics_and, 2));
    nar_register_def(module_name, L"or", nar_func(&basics_or, 2));
    nar_register_def(module_name, L"xor", nar_func(&basics_xor, 2));
}
