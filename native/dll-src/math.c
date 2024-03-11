#include "native.h"
#include <math.h>

#define INT64_NAN INT64_MAX

nar_object_t math_add(nar_object_t x, nar_object_t y) {
    nar_object_kind_t kind_x = nar_object_kind(x);
    nar_object_kind_t kind_y = nar_object_kind(y);
    if (kind_x != kind_y) {
        return nar_fail(L"Nar.Base.Math.add: types are not equal");
    }
    switch (kind_x) {
        case NAR_INT: {
            return nar_int(nar_to_int(x) + nar_to_int(y));
        }
        case NAR_FLOAT: {
            return nar_float(nar_to_float(x) + nar_to_float(y));
        }
        default: {
            return nar_fail(L"Nar.Base.Math.add: unsupported type");
        }
    }
}

nar_object_t math_sub(nar_object_t x, nar_object_t y) {
    nar_object_kind_t kind_x = nar_object_kind(x);
    nar_object_kind_t kind_y = nar_object_kind(y);
    if (kind_x != kind_y) {
        return nar_fail(L"Nar.Base.Math.sub: types are not equal");
    }
    switch (kind_x) {
        case NAR_INT: {
            return nar_int(nar_to_int(x) - nar_to_int(y));
        }
        case NAR_FLOAT: {
            return nar_float(nar_to_float(x) - nar_to_float(y));
        }
        default: {
            return nar_fail(L"Nar.Base.Math.sub: unsupported type");
        }
    }
}

nar_object_t math_mul(nar_object_t x, nar_object_t y) {
    nar_object_kind_t kind_x = nar_object_kind(x);
    nar_object_kind_t kind_y = nar_object_kind(y);
    if (kind_x != kind_y) {
        return nar_fail(L"Nar.Base.Math.mul: types are not equal");
    }
    switch (kind_x) {
        case NAR_INT: {
            return nar_int(nar_to_int(x) * nar_to_int(y));
        }
        case NAR_FLOAT: {
            return nar_float(nar_to_float(x) * nar_to_float(y));
        }
        default: {
            return nar_fail(L"Nar.Base.Math.mul: unsupported type");
        }
    }
}

nar_object_t math_div(nar_object_t x, nar_object_t y) {
    nar_object_kind_t kind_x = nar_object_kind(x);
    nar_object_kind_t kind_y = nar_object_kind(y);
    if (kind_x != kind_y) {
        return nar_fail(L"Nar.Base.Math.div: types are not equal");
    }
    switch (kind_x) {
        case NAR_INT: {
            nar_int_t value_y = nar_to_int(y);
            if (value_y == 0) {
                return nar_int(INT64_NAN);
            }
            return nar_int(nar_to_int(x) / value_y);
        }
        case NAR_FLOAT: {
            return nar_float(nar_to_float(x) / nar_to_float(y));
        }
        default: {
            return nar_fail(L"Nar.Base.Math.div: unsupported type");
        }
    }
}

nar_object_t math_neg(nar_object_t x) {
    nar_object_kind_t kind_x = nar_object_kind(x);
    switch (kind_x) {
        case NAR_INT: {
            return nar_int(-nar_to_int(x));
        }
        case NAR_FLOAT: {
            return nar_float(-nar_to_float(x));
        }
        default: {
            return nar_fail(L"Nar.Base.Math.neg: unsupported type");
        }
    }
}

nar_object_t math_abs(nar_object_t x) {
    nar_object_kind_t kind_x = nar_object_kind(x);
    switch (kind_x) {
        case NAR_INT: {
            nar_int_t value_x = nar_to_int(x);
            return value_x >= 0 ? x : nar_int(-value_x);
        }
        case NAR_FLOAT: {
            nar_float_t value_x = nar_to_float(x);
            return value_x >= 0 ? x : nar_float(-value_x);
        }
        default: {
            return nar_fail(L"Nar.Base.Math.abs: unsupported type");
        }
    }
}

nar_object_t math_toPower(nar_object_t powr, nar_object_t num) {
    nar_object_kind_t kind_num = nar_object_kind(num);
    nar_object_kind_t  kind_pow = nar_object_kind(powr);
    if (kind_num != kind_pow) {
        return nar_fail(L"Nar.Base.Math.toPower: types are not equal");
    }
    switch (kind_num) {
        case NAR_INT: {
            return nar_int(lround(pow((nar_float_t) nar_to_int(num), (nar_float_t) nar_to_int(powr)) + 0.5));
        }
        case NAR_FLOAT: {
            return nar_float(pow(nar_to_float(num), nar_to_float(powr)));
        }
        default: {
            return nar_fail(L"Nar.Base.Math.toPower: unsupported type");
        }
    }
}

nar_object_t math_isNan(nar_object_t n) {
    nar_object_kind_t kind_n = nar_object_kind(n);
    switch (kind_n) {
        case NAR_INT: {
            return nar_bool(INT64_NAN == nar_to_int(n));
        }
        case NAR_FLOAT: {
            return nar_bool(isnan(nar_to_float(n)));
        }
        default: {
            return nar_fail(L"Nar.Base.Math.isNan: unsupported type");
        }
    }
}

nar_object_t math_isInf(nar_object_t n) {
    nar_object_kind_t kind_n = nar_object_kind(n);
    switch (kind_n) {
        case NAR_INT: {
            return nar_bool(nar_false);
        }
        case NAR_FLOAT: {
            return nar_bool(isinf(nar_to_float(n)));
        }
        default: {
            return nar_fail(L"Nar.Base.Math.isInf: unsupported type");
        }
    }
}

nar_object_t math_toFloat(nar_object_t n) {
    return nar_float((nar_float_t) nar_to_int(n));
}

nar_object_t math_round(nar_object_t n) {
    return nar_int(lround(nar_to_float(n)));
}

nar_object_t math_floor(nar_object_t n) {
    return nar_int(floor(nar_to_float(n)));
}

nar_object_t math_ceil(nar_object_t n) {
    return nar_int(ceil(nar_to_float(n)));
}

nar_object_t math_trunc(nar_object_t n) {
    return nar_int(lround(trunc(nar_to_float(n))));
}

nar_object_t math_sqrt(nar_object_t n) {
    return nar_float(sqrt(nar_to_float(n)));
}

nar_object_t math_remainderBy(nar_object_t n, nar_object_t x) {
    return nar_int(nar_to_int(x) % nar_to_int(n));
}

nar_object_t math_modBy(nar_object_t modulus, nar_object_t x) {
    nar_int_t value_modulus = nar_to_int(modulus);
    if (value_modulus == 0) {
        return nar_int(INT64_NAN);
    }
    const nar_int_t answer = nar_to_int(x) % value_modulus;
    return nar_int(((answer > 0 && value_modulus < 0) || (answer < 0 && value_modulus > 0)) ? answer + value_modulus : answer);
}

nar_object_t math_logBase(nar_object_t base, nar_object_t n) {
    return nar_float(log(nar_to_float(n)) / log(nar_to_float(base)));
}

void register_math(void) {
    nar_string_t module_name = L"Nar.Base.Math";
    nar_register_def(module_name, L"add", nar_func(&math_add, 2));
    nar_register_def(module_name, L"sub", nar_func(&math_sub, 2));
    nar_register_def(module_name, L"mul", nar_func(&math_mul, 2));
    nar_register_def(module_name, L"div", nar_func(&math_div, 2));
    nar_register_def(module_name, L"neg", nar_func(&math_neg, 1));
    nar_register_def(module_name, L"abs", nar_func(&math_abs, 1));
    nar_register_def(module_name, L"toPower", nar_func(&math_toPower, 2));
    nar_register_def(module_name, L"isNan", nar_func(&math_isNan, 1));
    nar_register_def(module_name, L"isInf", nar_func(&math_isInf, 1));
    nar_register_def(module_name, L"toFloat", nar_func(&math_toFloat, 1));
    nar_register_def(module_name, L"round", nar_func(&math_round, 1));
    nar_register_def(module_name, L"floor", nar_func(&math_floor, 1));
    nar_register_def(module_name, L"ceil", nar_func(&math_ceil, 1));
    nar_register_def(module_name, L"trunc", nar_func(&math_trunc, 1));
    nar_register_def(module_name, L"sqrt", nar_func(&math_sqrt, 1));
    nar_register_def(module_name, L"remainderBy", nar_func(&math_remainderBy, 2));
    nar_register_def(module_name, L"modBy", nar_func(&math_modBy, 2));
    nar_register_def(module_name, L"logBase", nar_func(&math_logBase, 2));
}