#include "_package.h"

nar_object_t debug_toString(nar_runtime_t rt, nar_object_t x) {
    return stringify(rt, x);
}

nar_object_t debug_log(nar_runtime_t rt, nar_object_t msg, nar_object_t a) {
    if (nar->object_get_kind(rt, a) != NAR_OBJECT_KIND_STRING) {
        a = stringify(rt, a);
    }
    nar_cstring_t msg_str = nar->to_string(rt, msg);
    nar_cstring_t a_str = nar->to_string(rt, a);

    size_t len = strlen(msg_str) + strlen(a_str) + 1;
    nar_string_t str = nar->alloc(len * sizeof(char));
    strcpy(str, msg_str);
    strcat(str, a_str);
    nar->print(rt, str);
    nar->free(str);
    return a;
}

nar_object_t debug_todo(nar_runtime_t rt, nar_object_t msg) {
    nar->fail(rt, nar->to_string(rt, msg));
    return nar->make_unit(rt);
}

nar_object_t debug_getType(nar_runtime_t rt, nar_object_t x) {
    nar_object_kind_t kind = nar->object_get_kind(rt, x);
    switch (kind) {
        case NAR_OBJECT_KIND_UNKNOWN:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeUnknown, 0, NULL);
        case NAR_OBJECT_KIND_UNIT:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeUnit, 0, NULL);
        case NAR_OBJECT_KIND_INT:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeInt, 0, NULL);
        case NAR_OBJECT_KIND_FLOAT:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeFloat, 0, NULL);
        case NAR_OBJECT_KIND_STRING:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeString, 0, NULL);
        case NAR_OBJECT_KIND_CHAR:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeChar, 0, NULL);
        case NAR_OBJECT_KIND_RECORD:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeRecord, 0, NULL);
        case NAR_OBJECT_KIND_TUPLE:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeTuple, 0, NULL);
        case NAR_OBJECT_KIND_LIST:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeList, 0, NULL);
        case NAR_OBJECT_KIND_OPTION:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeOption, 0, NULL);
        case NAR_OBJECT_KIND_FUNCTION:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeFunction, 0, NULL);
        case NAR_OBJECT_KIND_CLOSURE:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeClosure, 0, NULL);
        case NAR_OBJECT_KIND_NATIVE:
            return nar->make_option(rt, Nar_Base_Debug_Type__TypeNative, 0, NULL);
        default:
            nar->fail(rt, "debug_getType: enum case is out of range");
            return nar->make_unit(rt);
    }
}

void register_debug(nar_runtime_t rt) {
    nar_string_t module_name = "Nar.Base.Debug";
    nar->register_def_dynamic(rt, module_name, "toString", "debug_toString", 1);
    nar->register_def_dynamic(rt, module_name, "log", "debug_log", 2);
    nar->register_def_dynamic(rt, module_name, "todo", "debug_todo", 1);
    nar->register_def_dynamic(rt, module_name, "getType", "debug_getType", 1);
}
