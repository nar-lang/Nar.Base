#include "_package.h"
#include "utils.h"

nar_object_t basics_eq(nar_runtime_t rt, nar_object_t a, nar_object_t b) {
    return nar->new_bool(rt, cmp(rt, a, b) == 0);
}

nar_object_t basics_neq(nar_runtime_t rt, nar_object_t a, nar_object_t b) {
    return nar->new_bool(rt, cmp(rt, a, b) != 0);
}

nar_object_t basics_lt(nar_runtime_t rt, nar_object_t a, nar_object_t b) {
    return nar->new_bool(rt, cmp(rt, a, b) < 0);
}

nar_object_t basics_gt(nar_runtime_t rt, nar_object_t a, nar_object_t b) {
    return nar->new_bool(rt, cmp(rt, a, b) > 0);
}

nar_object_t basics_le(nar_runtime_t rt, nar_object_t a, nar_object_t b) {
    return nar->new_bool(rt, cmp(rt, a, b) <= 0);
}

nar_object_t basics_ge(nar_runtime_t rt, nar_object_t a, nar_object_t b) {
    return nar->new_bool(rt, cmp(rt, a, b) >= 0);
}

nar_object_t basics_not(nar_runtime_t rt, nar_object_t x) {
    return nar->new_bool(rt, !nar->to_bool(rt, x));
}

nar_object_t basics_and(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    return nar->new_bool(rt, nar->to_bool(rt, x) && nar->to_bool(rt, y));
}

nar_object_t basics_or(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    return nar->new_bool(rt, nar->to_bool(rt, x) || nar->to_bool(rt, y));
}

nar_object_t basics_xor(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    return nar->new_bool(rt, nar->to_bool(rt, x) ^ nar->to_bool(rt, y));
}

void register_basics(nar_runtime_t rt) {
    nar_string_t module_name = "Nar.Base.Basics";
    nar->register_def(rt, module_name, "eq", nar->new_func(rt, &basics_eq, 2));
    nar->register_def(rt, module_name, "neq", nar->new_func(rt, &basics_neq, 2));
    nar->register_def(rt, module_name, "lt", nar->new_func(rt, &basics_lt, 2));
    nar->register_def(rt, module_name, "gt", nar->new_func(rt, &basics_gt, 2));
    nar->register_def(rt, module_name, "le", nar->new_func(rt, &basics_le, 2));
    nar->register_def(rt, module_name, "ge", nar->new_func(rt, &basics_ge, 2));
    nar->register_def(rt, module_name, "not", nar->new_func(rt, &basics_not, 1));
    nar->register_def(rt, module_name, "and", nar->new_func(rt, &basics_and, 2));
    nar->register_def(rt, module_name, "or", nar->new_func(rt, &basics_or, 2));
    nar->register_def(rt, module_name, "xor", nar->new_func(rt, &basics_xor, 2));
}
