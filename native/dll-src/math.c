#include "_package.h"

#define INT64_NAN INT64_MAX

nar_object_t math_add(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    nar_object_kind_t kind_x = nar->object_get_kind(rt, x);
    nar_object_kind_t kind_y = nar->object_get_kind(rt, y);
    if (kind_x != kind_y) {
        nar->fail(rt, "Nar.Base.Math.add: types are not equal");
        return nar->make_unit(rt);
    }
    switch (kind_x) {
        case NAR_OBJECT_KIND_INT: {
            return nar->make_int(rt, nar->to_int(rt, x) + nar->to_int(rt, y));
        }
        case NAR_OBJECT_KIND_FLOAT: {
            return nar->make_float(rt, nar->to_float(rt, x) + nar->to_float(rt, y));
        }
        default: {
            nar->fail(rt, "Nar.Base.Math.add: unsupported type");
            return nar->make_unit(rt);
        }
    }
}

nar_object_t math_sub(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    nar_object_kind_t kind_x = nar->object_get_kind(rt, x);
    nar_object_kind_t kind_y = nar->object_get_kind(rt, y);
    if (kind_x != kind_y) {
        nar->fail(rt, "Nar.Base.Math.sub: types are not equal");
        return nar->make_unit(rt);
    }
    switch (kind_x) {
        case NAR_OBJECT_KIND_INT: {
            return nar->make_int(rt, nar->to_int(rt, x) - nar->to_int(rt, y));
        }
        case NAR_OBJECT_KIND_FLOAT: {
            return nar->make_float(rt, nar->to_float(rt, x) - nar->to_float(rt, y));
        }
        default: {
            nar->fail(rt, "Nar.Base.Math.sub: unsupported type");
            return nar->make_unit(rt);
        }
    }
}

nar_object_t math_mul(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    nar_object_kind_t kind_x = nar->object_get_kind(rt, x);
    nar_object_kind_t kind_y = nar->object_get_kind(rt, y);
    if (kind_x != kind_y) {
        nar->fail(rt, "Nar.Base.Math.mul: types are not equal");
        return nar->make_unit(rt);
    }
    switch (kind_x) {
        case NAR_OBJECT_KIND_INT: {
            return nar->make_int(rt, nar->to_int(rt, x) * nar->to_int(rt, y));
        }
        case NAR_OBJECT_KIND_FLOAT: {
            return nar->make_float(rt, nar->to_float(rt, x) * nar->to_float(rt, y));
        }
        default: {
            nar->fail(rt, "Nar.Base.Math.mul: unsupported type");
            return nar->make_unit(rt);
        }
    }
}

nar_object_t math_div(nar_runtime_t rt, nar_object_t x, nar_object_t y) {
    nar_object_kind_t kind_x = nar->object_get_kind(rt, x);
    nar_object_kind_t kind_y = nar->object_get_kind(rt, y);
    if (kind_x != kind_y) {
        nar->fail(rt, "Nar.Base.Math.div: types are not equal");
        return nar->make_unit(rt);
    }
    switch (kind_x) {
        case NAR_OBJECT_KIND_INT: {
            nar_int_t value_y = nar->to_int(rt, y);
            if (value_y == 0) {
                return nar->make_int(rt, INT64_NAN);
            }
            return nar->make_int(rt, nar->to_int(rt, x) / value_y);
        }
        case NAR_OBJECT_KIND_FLOAT: {
            return nar->make_float(rt, nar->to_float(rt, x) / nar->to_float(rt, y));
        }
        default: {
            nar->fail(rt, "Nar.Base.Math.div: unsupported type");
            return nar->make_unit(rt);
        }
    }
}

nar_object_t math_neg(nar_runtime_t rt, nar_object_t x) {
    nar_object_kind_t kind_x = nar->object_get_kind(rt, x);
    switch (kind_x) {
        case NAR_OBJECT_KIND_INT: {
            return nar->make_int(rt, -nar->to_int(rt, x));
        }
        case NAR_OBJECT_KIND_FLOAT: {
            return nar->make_float(rt, -nar->to_float(rt, x));
        }
        default: {
            nar->fail(rt, "Nar.Base.Math.neg: unsupported type");
            return nar->make_unit(rt);
        }
    }
}

nar_object_t math_abs(nar_runtime_t rt, nar_object_t x) {
    nar_object_kind_t kind_x = nar->object_get_kind(rt, x);
    switch (kind_x) {
        case NAR_OBJECT_KIND_INT: {
            nar_int_t value_x = nar->to_int(rt, x);
            return value_x >= 0 ? x : nar->make_int(rt, -value_x);
        }
        case NAR_OBJECT_KIND_FLOAT: {
            nar_float_t value_x = nar->to_float(rt, x);
            return value_x >= 0 ? x : nar->make_float(rt, -value_x);
        }
        default: {
            nar->fail(rt, "Nar.Base.Math.abs: unsupported type");
            return nar->make_unit(rt);
        }
    }
}

nar_object_t math_toPower(nar_runtime_t rt, nar_object_t powr, nar_object_t num) {
    nar_object_kind_t kind_num = nar->object_get_kind(rt, num);
    nar_object_kind_t kind_pow = nar->object_get_kind(rt, powr);
    if (kind_num != kind_pow) {
        nar->fail(rt, "Nar.Base.Math.toPower: types are not equal");
        return nar->make_unit(rt);
    }
    switch (kind_num) {
        case NAR_OBJECT_KIND_INT: {
            nar_float_t fx = (nar_float_t) nar->to_int(rt, num);
            nar_float_t fpowr = (nar_float_t) nar->to_int(rt, powr);
            return nar->make_int(rt, lround(pow(fx, fpowr)));
        }
        case NAR_OBJECT_KIND_FLOAT: {
            return nar->make_float(rt, pow(nar->to_float(rt, num), nar->to_float(rt, powr)));
        }
        default: {
            nar->fail(rt, "Nar.Base.Math.toPower: unsupported type");
            return nar->make_unit(rt);
        }
    }
}

nar_object_t math_isNan(nar_runtime_t rt, nar_object_t n) {
    nar_object_kind_t kind_n = nar->object_get_kind(rt, n);
    switch (kind_n) {
        case NAR_OBJECT_KIND_INT: {
            return nar->make_bool(rt, INT64_NAN == nar->to_int(rt, n));
        }
        case NAR_OBJECT_KIND_FLOAT: {
            return nar->make_bool(rt, isnan(nar->to_float(rt, n)));
        }
        default: {
            nar->fail(rt, "Nar.Base.Math.isNan: unsupported type");
            return nar->make_unit(rt);
        }
    }
}

nar_object_t math_isInf(nar_runtime_t rt, nar_object_t n) {
    nar_object_kind_t kind_n = nar->object_get_kind(rt, n);
    switch (kind_n) {
        case NAR_OBJECT_KIND_INT: {
            return nar->make_bool(rt, nar_false);
        }
        case NAR_OBJECT_KIND_FLOAT: {
            return nar->make_bool(rt, isinf(nar->to_float(rt, n)));
        }
        default: {
            nar->fail(rt, "Nar.Base.Math.isInf: unsupported type");
            return nar->make_unit(rt);
        }
    }
}

nar_object_t math_toFloat(nar_runtime_t rt, nar_object_t n) {
    return nar->make_float(rt, (nar_float_t) nar->to_int(rt, n));
}

nar_object_t math_round(nar_runtime_t rt, nar_object_t n) {
    return nar->make_int(rt, lround(nar->to_float(rt, n)));
}

nar_object_t math_floor(nar_runtime_t rt, nar_object_t n) {
    return nar->make_int(rt, floor(nar->to_float(rt, n)));
}

nar_object_t math_ceil(nar_runtime_t rt, nar_object_t n) {
    return nar->make_int(rt, ceil(nar->to_float(rt, n)));
}

nar_object_t math_trunc(nar_runtime_t rt, nar_object_t n) {
    return nar->make_int(rt, lround(trunc(nar->to_float(rt, n))));
}

nar_object_t math_sqrt(nar_runtime_t rt, nar_object_t n) {
    return nar->make_float(rt, sqrt(nar->to_float(rt, n)));
}

nar_object_t math_remainderBy(nar_runtime_t rt, nar_object_t n, nar_object_t x) {
    return nar->make_int(rt, nar->to_int(rt, x) % nar->to_int(rt, n));
}

nar_object_t math_modBy(nar_runtime_t rt, nar_object_t modulus, nar_object_t x) {
    nar_int_t value_modulus = nar->to_int(rt, modulus);
    if (value_modulus == 0) {
        return nar->make_int(rt, INT64_NAN);
    }
    const nar_int_t answer = nar->to_int(rt, x) % value_modulus;
    bool add_mod = ((answer > 0 && value_modulus < 0) || (answer < 0 && value_modulus > 0));
    return nar->make_int(rt, add_mod ? answer + value_modulus : answer);
}

nar_object_t math_logBase(nar_runtime_t rt, nar_object_t base, nar_object_t n) {
    return nar->make_float(rt, log(nar->to_float(rt, n)) / log(nar->to_float(rt, base)));
}

void register_math(nar_runtime_t rt) {
    nar_string_t module_name = "Nar.Base.Math";
    nar->register_def_dynamic(rt, module_name, "add", "math_add", 2);
    nar->register_def_dynamic(rt, module_name, "sub", "math_sub", 2);
    nar->register_def_dynamic(rt, module_name, "mul", "math_mul", 2);
    nar->register_def_dynamic(rt, module_name, "div", "math_div", 2);
    nar->register_def_dynamic(rt, module_name, "neg", "math_neg", 1);
    nar->register_def_dynamic(rt, module_name, "abs", "math_abs", 1);
    nar->register_def_dynamic(rt, module_name, "toPower", "math_toPower", 2);
    nar->register_def_dynamic(rt, module_name, "isNan", "math_isNan", 1);
    nar->register_def_dynamic(rt, module_name, "isInf", "math_isInf", 1);
    nar->register_def_dynamic(rt, module_name, "toFloat", "math_toFloat", 1);
    nar->register_def_dynamic(rt, module_name, "round", "math_round", 1);
    nar->register_def_dynamic(rt, module_name, "floor", "math_floor", 1);
    nar->register_def_dynamic(rt, module_name, "ceil", "math_ceil", 1);
    nar->register_def_dynamic(rt, module_name, "trunc", "math_trunc", 1);
    nar->register_def_dynamic(rt, module_name, "sqrt", "math_sqrt", 1);
    nar->register_def_dynamic(rt, module_name, "remainderBy", "math_remainderBy", 2);
    nar->register_def_dynamic(rt, module_name, "modBy", "math_modBy", 2);
    nar->register_def_dynamic(rt, module_name, "logBase", "math_logBase", 2);
}