#include <narc.h>
#include "utils.h"

int cmp(nar_object_t a, nar_object_t b) {
    nar_object_kind_t kind_a = nar_object_kind(a);
    nar_object_kind_t kind_b = nar_object_kind(b);
    if (kind_a == kind_b) {
        switch (kind_a) {
            case NAR_UNIT: {
                return 0;
            }
            case NAR_CHAR: {
                nar_char_t value_a = nar_to_char(a);
                nar_char_t value_b = nar_to_char(b);
                return (value_a < value_b) ? -1 : (value_a > value_b ? 1 : 0);
            }
            case NAR_INT: {
                nar_int_t value_a = nar_to_int(a);
                nar_int_t value_b = nar_to_int(b);
                return (value_a < value_b) ? -1 : (value_a > value_b ? 1 : 0);
            }
            case NAR_FLOAT: {
                nar_float_t value_a = nar_to_float(a);
                nar_float_t value_b = nar_to_float(b);
                return (value_a < value_b) ? -1 : (value_a > value_b ? 1 : 0);
            }
            case NAR_STRING: {
                nar_string_t value_a = nar_to_string(a);
                nar_string_t value_b = nar_to_string(b);
                return wcscmp(value_a, value_b);
            }
            case NAR_RECORD: {
                return (a < b) ? -1 : (a > b ? 1 : 0); //TODO: make good comparison
            }
            case NAR_TUPLE: {
                nar_tuple_t a_ = nar_to_tuple(a);
                nar_tuple_t b_ = nar_to_tuple(b);
                if (a_.size == b_.size) {
                    for (int i = 0; i < a_.size; i++) {
                        int c = cmp(a_.items[i], b_.items[i]);
                        if (c != 0) {
                            return c;
                        }
                    }
                    return 0;
                }
                nar_fail(L"cmp: tuple sizes are not equal");
            }
            case NAR_LIST: {
                nar_list_t a_ = nar_to_list(a);
                nar_list_t b_ = nar_to_list(b);
                if (a_.size < b_.size) {
                    return -1;
                } else if (a_.size > b_.size) {
                    return 1;
                }
                for (int i = 0; i < a_.size; i++) {
                    int c = cmp(a_.items[i], b_.items[i]);
                    if (c != 0) {
                        return c;
                    }
                }
                return 0;
            }
            case NAR_OPTION: {
                nar_option_t a_ = nar_to_option(a);
                nar_option_t b_ = nar_to_option(b);
                int c = wcscmp(a_.name, b_.name);
                if (c != 0) {
                    return c;
                }
                if (a_.size == b_.size) {
                    for (int i = 0; i < a_.size; i++) {
                        c = cmp(a_.values[i], b_.values[i]);
                        if (c != 0) {
                            return c;
                        }
                    }
                    return 0;
                }
                nar_fail(L"cmp: option sizes are not equal");
            }
            case NAR_FUNCTION: {
                nar_func_t a_ = nar_to_func(a);
                nar_func_t b_ = nar_to_func(b);
                return (a_.arity < b_.arity) ? -1 :
                       (a_.arity > b_.arity ? 1 :
                        ((a_.ptr < b_.ptr) ? -1 : (a_.ptr > b_.ptr ? 1 : 0))
                       );
            }
            case NAR_CLOSURE:
            case NAR_NATIVE: {
                return (a < b) ? -1 : (a > b ? 1 : 0);
            }
            default: {
                nar_fail(L"cmp: enum case is out of range");
            }
        }
    } else {
        nar_fail(L"cmp: types are not equal");
    }

    return a < b ? -1 : (a > b ? 1 : 0);
}

nar_string_t stringify(nar_object_t a) {
    nar_object_kind_t kind = nar_object_kind(a);
    switch (kind) {
        case NAR_UNIT: {
            return L"()";
        }
        case NAR_CHAR: {
            wchar_t *str = nar_alloc(2 * sizeof(wchar_t));
            str[0] = nar_to_char(a);
            str[1] = L'\0';
            return str;
        }
        case NAR_INT: {
            wchar_t buffer[32 * sizeof(wchar_t)];
            swprintf(buffer, 32, L"%d", nar_to_int(a));
            wchar_t *str = nar_alloc((wcslen(buffer) + 1) * sizeof(wchar_t));
            wcscpy(str, buffer);
            return str;
        }
        case NAR_FLOAT: {
            wchar_t buffer[32 * sizeof(wchar_t)];
            swprintf(buffer, 32, L"%f", nar_to_float(a));
            wchar_t *str = nar_alloc((wcslen(buffer) + 1) * sizeof(wchar_t));
            wcscpy(str, buffer);
            return str;
        }
        case NAR_STRING: {
            return nar_to_string(a);
        }
        case NAR_RECORD: {
            return L"record"; //TODO: show fields
        }
        case NAR_TUPLE: {
            return L"tuple"; //TODO: show items
        }
        case NAR_LIST: {
            return L"list"; //TODO: show items
        }
        case NAR_OPTION: {
            return L"option"; //TODO: show name and values
        }
        case NAR_FUNCTION: {
            return L"function"; //TODO: show arity and pointer
        }
        case NAR_CLOSURE: {
            return L"closure";
        }
        case NAR_NATIVE: {
            return L"native";
        }
        default:
            nar_fail(L"stringify: enum case is out of range");
            return L"?";
    }
}

wchar_t *join_strings(wchar_t **strings, int size, wchar_t *separator) {
    size_t total_size = 0;
    for (int i = 0; i < size; i++) {
        total_size += wcslen(strings[i]);
    }
    total_size += (size - 1) * wcslen(separator);
    wchar_t *result = nar_alloc((total_size + 1) * sizeof(wchar_t));
    result[0] = L'\0';
    for (int i = 0; i < size; i++) {
        wcscat(result, strings[i]);
        if (i < size - 1) {
            wcscat(result, separator);
        }
    }
    return result;
}
