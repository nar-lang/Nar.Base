#include "_package.h"

nar_object_t char_toUpper(nar_runtime_t rt, nar_object_t c) {
    return nar->new_char(rt, towupper(nar->to_char(rt, c)));
}

nar_object_t char_toLower(nar_runtime_t rt, nar_object_t c) {
    return nar->new_char(rt, towlower(nar->to_char(rt, c)));
}

nar_object_t char_toCode(nar_runtime_t rt, nar_object_t c) {
    return nar->new_int(rt, nar->to_char(rt, c));
}

nar_object_t char_fromCode(nar_runtime_t rt, nar_object_t code) {
    return nar->new_char(rt, (nar_char_t) nar->to_int(rt, code));
}

void register_char(nar_runtime_t rt) {
    nar_string_t module_name = L"Nar.Base.Char";
    nar->register_def(rt, module_name, L"toUpper", nar->new_func(rt, char_toUpper, 1));
    nar->register_def(rt, module_name, L"toLower", nar->new_func(rt, char_toLower, 1));
    nar->register_def(rt, module_name, L"toCode", nar->new_func(rt, char_toCode, 1));
    nar->register_def(rt, module_name, L"fromCode", nar->new_func(rt, char_fromCode, 1));
}