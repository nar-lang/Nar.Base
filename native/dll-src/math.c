#include "_package.h"
#include <math.h>

#define INT64_NAN INT64_MAX

nar_object_t math_add(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    nar_object_kind_t kind_x = nar->get_object_kind(rt, x);
    nar_object_kind_t kind_y = nar->get_object_kind(rt, y);
    if (kind_x != kind_y) {
        nar->fail(rt, L"Nar.Base.Math.add: types are not equal");
        return nar->new_unit(rt);
    }
    switch (kind_x) {
        case NAR_INT: {
            return nar->new_int(rt, nar->to_int(rt, x) + nar->to_int(rt, y));
        }
        case NAR_FLOAT: {
            return nar->new_float(rt, nar->to_float(rt, x) + nar->to_float(rt, y));
        }
        default: {
            nar->fail(rt, L"Nar.Base.Math.add: unsupported type");
            return nar->new_unit(rt);
        }
    }
}

nar_object_t math_sub(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    nar_object_kind_t kind_x = nar->get_object_kind(rt, x);
    nar_object_kind_t kind_y = nar->get_object_kind(rt, y);
    if (kind_x != kind_y) {
        nar->fail(rt, L"Nar.Base.Math.sub: types are not equal");
        return nar->new_unit(rt);
    }
    switch (kind_x) {
        case NAR_INT: {
            return nar->new_int(rt, nar->to_int(rt, x) - nar->to_int(rt, y));
        }
        case NAR_FLOAT: {
            return nar->new_float(rt, nar->to_float(rt, x) - nar->to_float(rt, y));
        }
        default: {
            nar->fail(rt, L"Nar.Base.Math.sub: unsupported type");
            return nar->new_unit(rt);
        }
    }
}

nar_object_t math_mul(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    nar_object_kind_t kind_x = nar->get_object_kind(rt, x);
    nar_object_kind_t kind_y = nar->get_object_kind(rt, y);
    if (kind_x != kind_y) {
        nar->fail(rt, L"Nar.Base.Math.mul: types are not equal");
        return nar->new_unit(rt);
    }
    switch (kind_x) {
        case NAR_INT: {
            return nar->new_int(rt, nar->to_int(rt, x) * nar->to_int(rt, y));
        }
        case NAR_FLOAT: {
            return nar->new_float(rt, nar->to_float(rt, x) * nar->to_float(rt, y));
        }
        default: {
            nar->fail(rt, L"Nar.Base.Math.mul: unsupported type");
            return nar->new_unit(rt);
        }
    }
}

nar_object_t math_div(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    nar_object_kind_t kind_x = nar->get_object_kind(rt, x);
    nar_object_kind_t kind_y = nar->get_object_kind(rt, y);
    if (kind_x != kind_y) {
        nar->fail(rt, L"Nar.Base.Math.div: types are not equal");
        return nar->new_unit(rt);
    }
    switch (kind_x) {
        case NAR_INT: {
            nar_int_t value_y = nar->to_int(rt, y);
            if (value_y == 0) {
                return nar->new_int(rt, INT64_NAN);
            }
            return nar->new_int(rt, nar->to_int(rt, x) / value_y);
        }
        case NAR_FLOAT: {
            return nar->new_float(rt, nar->to_float(rt, x) / nar->to_float(rt, y));
        }
        default: {
            nar->fail(rt, L"Nar.Base.Math.div: unsupported type");
            return nar->new_unit(rt);
        }
    }
}

nar_object_t math_neg(nar_runtime_t rt, nar_object_t x) {
    nar_object_kind_t kind_x = nar->get_object_kind(rt, x);
    switch (kind_x) {
        case NAR_INT: {
            return nar->new_int(rt, -nar->to_int(rt, x));
        }
        case NAR_FLOAT: {
            return nar->new_float(rt, -nar->to_float(rt, x));
        }
        default: {
            nar->fail(rt, L"Nar.Base.Math.neg: unsupported type");
            return nar->new_unit(rt);
        }
    }
}

nar_object_t math_abs(nar_runtime_t rt, nar_object_t x) {
    nar_object_kind_t kind_x = nar->get_object_kind(rt, x);
    switch (kind_x) {
        case NAR_INT: {
            nar_int_t value_x = nar->to_int(rt, x);
            return value_x >= 0 ? x : nar->new_int(rt, -value_x);
        }
        case NAR_FLOAT: {
            nar_float_t value_x = nar->to_float(rt, x);
            return value_x >= 0 ? x : nar->new_float(rt, -value_x);
        }
        default: {
            nar->fail(rt, L"Nar.Base.Math.abs: unsupported type");
            return nar->new_unit(rt);
        }
    }
}

nar_object_t math_toPower(nar_runtime_t rt, nar_object_t powr, nar_object_t num) {
    nar_object_kind_t kind_num = nar->get_object_kind(rt, num);
    nar_object_kind_t kind_pow = nar->get_object_kind(rt, powr);
    if (kind_num != kind_pow) {
        nar->fail(rt, L"Nar.Base.Math.toPower: types are not equal");
        return nar->new_unit(rt);
    }
    switch (kind_num) {
        case NAR_INT: {
            return nar->new_int(rt, lround(pow((nar_float_t) nar->to_int(rt, num), (nar_float_t) nar->to_int(rt, powr))));
        }
        case NAR_FLOAT: {
            return nar->new_float(rt, pow(nar->to_float(rt, num), nar->to_float(rt, powr)));
        }
        default: {
            nar->fail(rt, L"Nar.Base.Math.toPower: unsupported type");
            return nar->new_unit(rt);
        }
    }
}

nar_object_t math_isNan(nar_runtime_t rt, nar_object_t n) {
    nar_object_kind_t kind_n = nar->get_object_kind(rt, n);
    switch (kind_n) {
        case NAR_INT: {
            return nar->new_bool(rt, INT64_NAN == nar->to_int(rt, n));
        }
        case NAR_FLOAT: {
            return nar->new_bool(rt, isnan(nar->to_float(rt, n)));
        }
        default: {
            nar->fail(rt, L"Nar.Base.Math.isNan: unsupported type");
            return nar->new_unit(rt);
        }
    }
}

nar_object_t math_isInf(nar_runtime_t rt, nar_object_t n) {
    nar_object_kind_t kind_n = nar->get_object_kind(rt, n);
    switch (kind_n) {
        case NAR_INT: {
            return nar->new_bool(rt, nar_false);
        }
        case NAR_FLOAT: {
            return nar->new_bool(rt, isinf(nar->to_float(rt, n)));
        }
        default: {
            nar->fail(rt, L"Nar.Base.Math.isInf: unsupported type");
            return nar->new_unit(rt);
        }
    }
}

nar_object_t math_toFloat(nar_runtime_t rt, nar_object_t n) {
    return nar->new_float(rt, (nar_float_t) nar->to_int(rt, n));
}

nar_object_t math_round(nar_runtime_t rt, nar_object_t n) {
    return nar->new_int(rt, lround(nar->to_float(rt, n)));
}

nar_object_t math_floor(nar_runtime_t rt, nar_object_t n) {
    return nar->new_int(rt, floor(nar->to_float(rt, n)));
}

nar_object_t math_ceil(nar_runtime_t rt, nar_object_t n) {
    return nar->new_int(rt, ceil(nar->to_float(rt, n)));
}

nar_object_t math_trunc(nar_runtime_t rt, nar_object_t n) {
    return nar->new_int(rt, lround(trunc(nar->to_float(rt, n))));
}

nar_object_t math_sqrt(nar_runtime_t rt, nar_object_t n) {
    return nar->new_float(rt, sqrt(nar->to_float(rt, n)));
}

nar_object_t math_remainderBy(nar_runtime_t rt, nar_object_t n, nar_object_t x) {
    return nar->new_int(rt, nar->to_int(rt, x) % nar->to_int(rt, n));
}

nar_object_t math_modBy(nar_runtime_t rt, nar_object_t modulus, nar_object_t x) {
    nar_int_t value_modulus = nar->to_int(rt, modulus);
    if (value_modulus == 0) {
        return nar->new_int(rt, INT64_NAN);
    }
    const nar_int_t answer = nar->to_int(rt, x) % value_modulus;
    return nar->new_int(rt, ((answer > 0 && value_modulus < 0) || (answer < 0 && value_modulus > 0)) ? answer + value_modulus : answer);
}

nar_object_t math_logBase(nar_runtime_t rt, nar_object_t base, nar_object_t n) {
    return nar->new_float(rt, log(nar->to_float(rt, n)) / log(nar->to_float(rt, base)));
}

void register_math(nar_runtime_t rt) {
    nar_string_t module_name = L"Nar.Base.Math";
    nar->register_def(rt, module_name, L"add", nar->new_func(rt, &math_add, 2));
    nar->register_def(rt, module_name, L"sub", nar->new_func(rt, &math_sub, 2));
    nar->register_def(rt, module_name, L"mul", nar->new_func(rt, &math_mul, 2));
    nar->register_def(rt, module_name, L"div", nar->new_func(rt, &math_div, 2));
    nar->register_def(rt, module_name, L"neg", nar->new_func(rt, &math_neg, 1));
    nar->register_def(rt, module_name, L"abs", nar->new_func(rt, &math_abs, 1));
    nar->register_def(rt, module_name, L"toPower", nar->new_func(rt, &math_toPower, 2));
    nar->register_def(rt, module_name, L"isNan", nar->new_func(rt, &math_isNan, 1));
    nar->register_def(rt, module_name, L"isInf", nar->new_func(rt, &math_isInf, 1));
    nar->register_def(rt, module_name, L"toFloat", nar->new_func(rt, &math_toFloat, 1));
    nar->register_def(rt, module_name, L"round", nar->new_func(rt, &math_round, 1));
    nar->register_def(rt, module_name, L"floor", nar->new_func(rt, &math_floor, 1));
    nar->register_def(rt, module_name, L"ceil", nar->new_func(rt, &math_ceil, 1));
    nar->register_def(rt, module_name, L"trunc", nar->new_func(rt, &math_trunc, 1));
    nar->register_def(rt, module_name, L"sqrt", nar->new_func(rt, &math_sqrt, 1));
    nar->register_def(rt, module_name, L"remainderBy", nar->new_func(rt, &math_remainderBy, 2));
    nar->register_def(rt, module_name, L"modBy", nar->new_func(rt, &math_modBy, 2));
    nar->register_def(rt, module_name, L"logBase", nar->new_func(rt, &math_logBase, 2));
}