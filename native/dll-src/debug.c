#include "_package.h"
#include "utils.h"

nar_object_t debug_toString(nar_runtime_t rt, nar_object_t x) {
    return stringify(rt, x);
}

nar_object_t debug_log(nar_runtime_t rt, nar_object_t msg, nar_object_t a) {
    nar_string_t msg_ = nar->to_string(rt, msg);
    if (nar->get_object_kind(rt, a) != NAR_STRING) {
        a = stringify(rt, a);
    }
    nar_string_t a_ = nar->to_string(rt, stringify(rt, a));
    size_t len = wcslen(msg_) + wcslen(a_) + 1;
    nar_string_t str = nar->alloc(rt, len * sizeof(nar_char_t));
    wcscpy(str, msg_);
    wcscat(str, a_);
    nar->print(rt, str);
    return a;
}

nar_object_t debug_todo(nar_runtime_t rt, nar_object_t msg) {
    nar->fail(rt, nar->to_string(rt, msg));
    return nar->new_unit(rt);
}

nar_object_t debug_getType(nar_runtime_t rt, nar_object_t x) {
    nar_object_kind_t kind = nar->get_object_kind(rt, x);
    switch (kind) {
        case NAR_UNKNOWN:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeUnknown", 0, NULL);
        case NAR_UNIT:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeUnit", 0, NULL);
        case NAR_INT:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeInt", 0, NULL);
        case NAR_FLOAT:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeFloat", 0, NULL);
        case NAR_STRING:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeString", 0, NULL);
        case NAR_CHAR:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeChar", 0, NULL);
        case NAR_RECORD:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeRecord", 0, NULL);
        case NAR_TUPLE:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeTuple", 0, NULL);
        case NAR_LIST:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeList", 0, NULL);
        case NAR_OPTION:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeOption", 0, NULL);
        case NAR_FUNCTION:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeFunction", 0, NULL);
        case NAR_CLOSURE:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeClosure", 0, NULL);
        case NAR_NATIVE:
            return nar->new_option(rt, L"Nar.Base.Debug.Type#TypeNative", 0, NULL);
        default:
            nar->fail(rt, L"debug_getType: enum case is out of range");
            return nar->new_unit(rt);
    }
}

void register_debug(nar_runtime_t rt) {
    nar_string_t module_name = L"Nar.Base.Debug";
    nar->register_def(rt, module_name, L"toString", nar->new_func(rt, &debug_toString, 1));
    nar->register_def(rt, module_name, L"log", nar->new_func(rt, &debug_log, 2));
    nar->register_def(rt, module_name, L"todo", nar->new_func(rt, &debug_todo, 1));
    nar->register_def(rt, module_name, L"getType", nar->new_func(rt, &debug_getType, 1));
}
