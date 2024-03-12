#include "native.h"
#include "utils.h"

nar_object_t debug_toString(nar_object_t x) {
    return stringify(x);
}

nar_object_t debug_log(nar_object_t msg, nar_object_t a) {
    nar_string_t msg_ = nar_to_string(msg);
    if (nar_object_kind(a) != NAR_STRING) {
        a = stringify(a);
    }
    nar_string_t a_ = nar_to_string(stringify(a));
    size_t len = wcslen(msg_) + wcslen(a_) + 1;
    nar_string_t str = nar_alloc(len * sizeof(nar_char_t));
    wcscpy(str, msg_);
    wcscat(str, a_);
    nar_print(str);
    return a;
}

nar_object_t debug_todo(nar_object_t msg) {
    return nar_fail(nar_to_string(msg));
}

nar_object_t debug_getType(nar_object_t x) {
    nar_object_kind_t kind = nar_object_kind(x);
    switch (kind) {
        case NAR_UNKNOWN:
            return nar_option(L"Nar.Base.Debug.Type#TypeUnknown", 0, NULL);
        case NAR_UNIT:
            return nar_option(L"Nar.Base.Debug.Type#TypeUnit", 0, NULL);
        case NAR_INT:
            return nar_option(L"Nar.Base.Debug.Type#TypeInt", 0, NULL);
        case NAR_FLOAT:
            return nar_option(L"Nar.Base.Debug.Type#TypeFloat", 0, NULL);
        case NAR_STRING:
            return nar_option(L"Nar.Base.Debug.Type#TypeString", 0, NULL);
        case NAR_CHAR:
            return nar_option(L"Nar.Base.Debug.Type#TypeChar", 0, NULL);
        case NAR_RECORD:
            return nar_option(L"Nar.Base.Debug.Type#TypeRecord", 0, NULL);
        case NAR_TUPLE:
            return nar_option(L"Nar.Base.Debug.Type#TypeTuple", 0, NULL);
        case NAR_LIST:
            return nar_option(L"Nar.Base.Debug.Type#TypeList", 0, NULL);
        case NAR_OPTION:
            return nar_option(L"Nar.Base.Debug.Type#TypeOption", 0, NULL);
        case NAR_FUNCTION:
            return nar_option(L"Nar.Base.Debug.Type#TypeFunction", 0, NULL);
        case NAR_CLOSURE:
            return nar_option(L"Nar.Base.Debug.Type#TypeClosure", 0, NULL);
        case NAR_NATIVE:
            return nar_option(L"Nar.Base.Debug.Type#TypeNative", 0, NULL);
        default:
            return nar_fail(L"debug_getType: enum case is out of range");
    }
}

void register_debug(void) {
    nar_string_t module_name = L"Nar.Base.Debug";
    nar_register_def(module_name, L"toString", nar_func(&debug_toString, 1));
    nar_register_def(module_name, L"log", nar_func(&debug_log, 2));
    nar_register_def(module_name, L"todo", nar_func(&debug_todo, 1));
    nar_register_def(module_name, L"getType", nar_func(&debug_getType, 1));
}
