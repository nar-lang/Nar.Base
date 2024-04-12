#include "_package.h"

nar_object_t char_toUpper(nar_runtime_t rt, nar_object_t c) {
    return nar->make_char(rt, towupper((wchar_t)nar->to_char(rt, c)));
}

nar_object_t char_toLower(nar_runtime_t rt, nar_object_t c) {
    return nar->make_char(rt, towlower((wchar_t)nar->to_char(rt, c)));
}

nar_object_t char_toCode(nar_runtime_t rt, nar_object_t c) {
    return nar->make_int(rt, nar->to_char(rt, c));
}

nar_object_t char_fromCode(nar_runtime_t rt, nar_object_t code) {
    return nar->make_char(rt, (nar_char_t) nar->to_int(rt, code));
}

void register_char(nar_runtime_t rt) {
    nar_string_t module_name = "Nar.Base.Char";
    nar->register_def_dynamic(rt, module_name, "toUpper", "char_toUpper", 1);
    nar->register_def_dynamic(rt, module_name, "toLower", "char_toLower", 1);
    nar->register_def_dynamic(rt, module_name, "toCode", "char_toCode", 1);
    nar->register_def_dynamic(rt, module_name, "fromCode", "char_fromCode", 1);
}