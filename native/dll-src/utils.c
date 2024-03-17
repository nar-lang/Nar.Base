#include "_package.h"
#include <string.h>
#include "utils.h"

nar_int_t cmp(nar_runtime_t rt, nar_object_t a, nar_object_t b) {
    nar_object_kind_t kind_a = nar->get_object_kind(rt, a);
    nar_object_kind_t kind_b = nar->get_object_kind(rt, b);
    if (kind_a == kind_b) {
        switch (kind_a) {
            case NAR_UNIT: {
                return 0;
            }
            case NAR_CHAR: {
                nar_char_t value_a = nar->to_char(rt, a);
                nar_char_t value_b = nar->to_char(rt, b);
                return (value_a < value_b) ? -1 : (value_a > value_b ? 1 : 0);
            }
            case NAR_INT: {
                nar_int_t value_a = nar->to_int(rt, a);
                nar_int_t value_b = nar->to_int(rt, b);
                return (value_a < value_b) ? -1 : (value_a > value_b ? 1 : 0);
            }
            case NAR_FLOAT: {
                nar_float_t value_a = nar->to_float(rt, a);
                nar_float_t value_b = nar->to_float(rt, b);
                return (value_a < value_b) ? -1 : (value_a > value_b ? 1 : 0);
            }
            case NAR_STRING: {
                nar_string_t value_a = nar->to_string(rt, a);
                nar_string_t value_b = nar->to_string(rt, b);
                nar_int_t eq = wcscmp(value_a, value_b);
                return eq;
            }
            case NAR_RECORD: {
                nar_record_t a_ = nar->to_record(rt, a);
                nar_record_t b_ = nar->to_record(rt, b);
                if (a_.size < b_.size) {
                    return -1;
                } else if (a_.size > b_.size) {
                    return 1;
                }
                for (size_t i = 0; i < a_.size; i++) {
                    nar_string_t key_a = a_.keys[i];
                    nar_bool_t found = nar_false;
                    for (size_t j = 0; j < b_.size; j++) {
                        nar_string_t key_b = b_.keys[j];
                        if (wcscmp(key_a, key_b) == 0) {
                            nar_int_t c = cmp(rt, a_.values[i], b_.values[j]);
                            if (c != 0) {
                                return c;
                            }
                            found = nar_true;
                            break;
                        }
                    }
                    if (!found) {
                        return (a < b) ? -1 : 1;
                    }
                }
                return 0;
            }
            case NAR_TUPLE: {
                nar_tuple_t a_ = nar->to_tuple(rt, a);
                nar_tuple_t b_ = nar->to_tuple(rt, b);
                if (a_.size == b_.size) {
                    for (int i = 0; i < a_.size; i++) {
                        nar_int_t c = cmp(rt, a_.items[i], b_.items[i]);
                        if (c != 0) {
                            return c;
                        }
                    }
                    return 0;
                }
                nar->fail(rt, L"cmp: tuple sizes are not equal");
            }
            case NAR_LIST: {
                nar_list_t a_ = nar->to_list(rt, a);
                nar_list_t b_ = nar->to_list(rt, b);
                if (a_.size < b_.size) {
                    return -1;
                } else if (a_.size > b_.size) {
                    return 1;
                }
                for (int i = 0; i < a_.size; i++) {
                    nar_int_t c = cmp(rt, a_.items[i], b_.items[i]);
                    if (c != 0) {
                        return c;
                    }
                }
                return 0;
            }
            case NAR_OPTION: {
                nar_option_t a_ = nar->to_option(rt, a);
                nar_option_t b_ = nar->to_option(rt, b);
                nar_int_t c = wcscmp(a_.name, b_.name);
                if (c != 0) {
                    return c;
                }
                if (a_.size == b_.size) {
                    for (size_t i = 0; i < a_.size; i++) {
                        c = cmp(rt, a_.values[i], b_.values[i]);
                        if (c != 0) {
                            return c;
                        }
                    }
                    return 0;
                }
                nar->fail(rt, L"cmp: option sizes are not equal");
            }
            case NAR_FUNCTION: {
                nar_func_t a_ = nar->to_func(rt, a);
                nar_func_t b_ = nar->to_func(rt, b);
                return (a_.arity < b_.arity) ? -1 :
                       (a_.arity > b_.arity ? 1 :
                        ((a_.ptr < b_.ptr) ? -1 : (a_.ptr > b_.ptr ? 1 : 0))
                       );
            }
            case NAR_CLOSURE:
            case NAR_NATIVE: {
                nar_native_t a_ = nar->to_native(rt, a);
                nar_native_t b_ = nar->to_native(rt, b);
                if ((void *) a_.cmp < (void *) b_.cmp) {
                    return -1;
                } else if ((void *) a_.cmp > (void *) b_.cmp) {
                    return 1;
                }
                return a_.cmp(rt, a_.ptr, b_.ptr);
            }
            default: {
                nar->fail(rt, L"cmp: enum case is out of range");
            }
        }
    } else {
        nar->fail(rt, L"cmp: types are not equal");
    }

    return a < b ? -1 : (a > b ? 1 : 0);
}

nar_string_t join_strings(nar_runtime_t rt, nar_object_t *strings, size_t size, nar_string_t separator) {
    if (size == 0) {
        return L"";
    }
    nar_string_t lines[size];
    nar_size_t total_size = 0;

    for (size_t i = 0; i < size; i++) {
        nar_string_t s = nar->to_string(rt, strings[i]);
        total_size += wcslen(s);
        lines[i] = s;
    }

    total_size += (size - 1) * wcslen(separator);
    nar_string_t result = nar->alloc(rt, (total_size + 1) * sizeof(nar_char_t));
    result[0] = L'\0';
    for (size_t i = 0; i < size; i++) {
        wcscat(result, lines[i]);
        if (i < size - 1) {
            wcscat(result, separator);
        }
    }
    return result;
}

nar_object_t stringify(nar_runtime_t rt, nar_object_t a) {
    nar_object_kind_t kind = nar->get_object_kind(rt, a);
    switch (kind) {
        case NAR_UNIT: {
            return nar->new_string(rt, L"()");
        }
        case NAR_CHAR: {
            nar_char_t c = nar->to_char(rt, a);
            nar_char_t str[2];
            str[0] = c;
            str[1] = L'\0';
            return nar->new_string(rt, str);
        }
        case NAR_INT: {
            nar_char_t str[32];
            swprintf(str, 32, L"%d", nar->to_int(rt, a));
            return nar->new_string(rt, str);
        }
        case NAR_FLOAT: {
            wchar_t buffer[32];
            swprintf(buffer, 32, L"%g", nar->to_float(rt, a));
            return nar->new_string(rt, buffer);
        }
        case NAR_STRING: {
            return a;
        }
        case NAR_RECORD: {
            nar_record_t record = nar->to_record(rt, a);
            nar_object_t values[record.size];
            for (nar_size_t i = 0; i < record.size; i++) {
                values[i] = stringify(rt, record.values[i]);
            }
            nar_string_t valueStrings[record.size];
            for (nar_size_t i = 0; i < record.size; i++) {
                valueStrings[i] = nar->to_string(rt, values[i]);
            }
            nar_size_t total_size = 2;
            for (nar_size_t i = 0; i < record.size; i++) {
                total_size += wcslen(record.keys[i]) + wcslen(valueStrings[i]) + 4;
            }
            nar_char_t result[total_size];
            nar_char_t *ptr = result;
            *ptr++ = L'{';
            for (nar_size_t i = 0; i < record.size; ++i) {
                memcpy(ptr, record.keys[i], wcslen(record.keys[i]) * sizeof(nar_char_t));
                ptr += wcslen(record.keys[i]);
                *ptr++ = L' ';
                *ptr++ = L'=';
                *ptr++ = L' ';
                memcpy(ptr, valueStrings[i], wcslen(valueStrings[i]) * sizeof(nar_char_t));
                ptr += wcslen(valueStrings[i]);
                if (i < record.size - 1) {
                    *ptr++ = L',';
                    *ptr++ = L' ';
                }
            }
            *ptr++ = L'}';
            *ptr = L'\0';

            return nar->new_string(rt, result);
        }
        case NAR_TUPLE: {
            nar_tuple_t tuple = nar->to_tuple(rt, a);
            nar_object_t strings[tuple.size];
            for (nar_size_t i = 0; i < tuple.size; i++) {
                strings[i] = stringify(rt, tuple.items[i]);
            }
            nar_string_t strings_str = join_strings(rt, strings, tuple.size, L", ");
            size_t len = wcslen(strings_str) + 3;
            nar_char_t result[len];
            swprintf(result, len, L"(%ls)", strings_str);
            return nar->new_string(rt, result);
        }
        case NAR_LIST: {
            nar_list_t list = nar->to_list(rt, a);
            nar_object_t strings[list.size];
            for (nar_size_t i = 0; i < list.size; i++) {
                strings[i] = stringify(rt, list.items[i]);
            }
            nar_string_t strings_str = join_strings(rt, strings, list.size, L", ");
            size_t len = wcslen(strings_str) + 3;
            nar_char_t result[len];
            swprintf(result, len, L"[%ls]", strings_str);
            return nar->new_string(rt, result);
        }
        case NAR_OPTION: {
            nar_option_t option = nar->to_option(rt, a);
            nar_object_t values[option.size];
            for (size_t i = 0; i < option.size; i++) {
                values[i] = stringify(rt, option.values[i]);
            }
            nar_string_t values_str = join_strings(rt, values, option.size, L", ");
            size_t len = wcslen(option.name) + wcslen(values_str) + 3;
            nar_char_t result[len];
            swprintf(result, len, L"%ls(%ls)", option.name, values_str);
            return nar->new_string(rt, result);
        }
        case NAR_FUNCTION: {
            nar_func_t func = nar->to_func(rt, a);
            nar_char_t result[32];
            swprintf(result, 32, L"function/%p(%lld)", func.ptr, func.arity);
            return nar->new_string(rt, result);
        }
        case NAR_CLOSURE: {
            return nar->new_string(rt, L"closure");
        }
        case NAR_NATIVE: {
            nar_native_t native = nar->to_native(rt, a);
            nar_char_t result[32];
            swprintf(result, 32, L"native/%p(%p)", native.ptr, native.cmp);
            return nar->new_string(rt, result);
        }
        default: {
            nar->fail(rt, L"stringify: enum case is out of range");
            return nar->new_string(rt, L"?");
        }
    }
}
