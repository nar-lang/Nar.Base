#include "native.h"

nar_object_t char_toUpper(nar_object_t c) {
    return nar_char(towupper(nar_to_char(c)));
}

nar_object_t char_toLower(nar_object_t c) {
    return nar_char(towlower(nar_to_char(c)));
}

nar_object_t char_toCode(nar_object_t c) {
    return nar_int(nar_to_char(c));
}

nar_object_t char_fromCode(nar_object_t code) {
    return nar_char((nar_char_t) nar_to_int(code));
}

void register_char() {
    nar_string_t module_name = L"Nar.Base.Char";
    nar_register_def(module_name, L"toUpper", nar_func(char_toUpper, 1));
    nar_register_def(module_name, L"toLower", nar_func(char_toLower, 1));
    nar_register_def(module_name, L"toCode", nar_func(char_toCode, 1));
    nar_register_def(module_name, L"fromCode", nar_func(char_fromCode, 1));
}