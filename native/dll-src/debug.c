#include "native.h"
#include "utils.h"

nar_object_t debug_toString(nar_object_t x) {
    return nar_string(stringify(x));
}

nar_object_t debug_log(nar_object_t msg, nar_object_t a) {
    nar_string_t msg_ = nar_to_string(msg);
    nar_string_t a_ = nar_to_string(a);
    size_t len = wcslen(msg_) + wcslen(a_) + 1;
    nar_string_t str = nar_alloc(len * sizeof(wchar_t));
    wcscpy(str, msg_);
    wcscat(str, a_);
    nar_print(str);
    return a;
}

nar_object_t debug_todo(nar_object_t msg) {
    return nar_fail(nar_to_string(msg));
}

void register_debug(void) {
    nar_string_t module_name = L"Nar.Base.Debug";
    nar_register_def(module_name, L"toString", nar_func(&debug_toString, 1));
    nar_register_def(module_name, L"log", nar_func(&debug_log, 2));
    nar_register_def(module_name, L"todo", nar_func(&debug_todo, 1));
}
