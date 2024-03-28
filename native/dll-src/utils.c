#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include "_package.h"
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include "utils.h"

nar_int_t cmp(nar_runtime_t rt, nar_object_t a, nar_object_t b) {
    nar_object_kind_t kind_a = nar->object_get_kind(rt, a);
    nar_object_kind_t kind_b = nar->object_get_kind(rt, b);
    if (kind_a == kind_b) {
        switch (kind_a) {
            case NAR_OBJECT_KIND_UNIT: {
                return 0;
            }
            case NAR_OBJECT_KIND_CHAR: {
                nar_char_t value_a = nar->to_char(rt, a);
                nar_char_t value_b = nar->to_char(rt, b);
                return (value_a < value_b) ? -1 : (value_a > value_b ? 1 : 0);
            }
            case NAR_OBJECT_KIND_INT: {
                nar_int_t value_a = nar->to_int(rt, a);
                nar_int_t value_b = nar->to_int(rt, b);
                return (value_a < value_b) ? -1 : (value_a > value_b ? 1 : 0);
            }
            case NAR_OBJECT_KIND_FLOAT: {
                nar_float_t value_a = nar->to_float(rt, a);
                nar_float_t value_b = nar->to_float(rt, b);
                return (value_a < value_b) ? -1 : (value_a > value_b ? 1 : 0);
            }
            case NAR_OBJECT_KIND_STRING: {
                nar_cstring_t value_a = nar->to_string(rt, a);
                nar_cstring_t value_b = nar->to_string(rt, b);
                nar_int_t eq = strcmp(value_a, value_b);
                return eq;
            }
            case NAR_OBJECT_KIND_RECORD: {
                nar_record_t a_ = nar->to_record(rt, a);
                nar_record_t b_ = nar->to_record(rt, b);
                if (a_.size < b_.size) {
                    return -1;
                } else if (a_.size > b_.size) {
                    return 1;
                }
                for (size_t i = 0; i < a_.size; i++) {
                    nar_cstring_t key_a = a_.keys[i];
                    nar_bool_t found = nar_false;
                    for (size_t j = 0; j < b_.size; j++) {
                        nar_cstring_t key_b = b_.keys[j];
                        if (strcmp(key_a, key_b) == 0) {
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
            case NAR_OBJECT_KIND_TUPLE: {
                nar_tuple_t a_ = nar->to_tuple(rt, a);
                nar_tuple_t b_ = nar->to_tuple(rt, b);
                if (a_.size == b_.size) {
                    for (size_t i = 0; i < a_.size; i++) {
                        nar_int_t c = cmp(rt, a_.values[i], b_.values[i]);
                        if (c != 0) {
                            return c;
                        }
                    }
                    return 0;
                }
                nar->fail(rt, "cmp: tuple sizes are not equal");
            }
            case NAR_OBJECT_KIND_LIST: {
                //todo: iterate linked list, do not allocate it
                nar_list_t a_ = nar->to_list(rt, a);
                nar_list_t b_ = nar->to_list(rt, b);
                if (a_.size < b_.size) {
                    return -1;
                } else if (a_.size > b_.size) {
                    return 1;
                }
                for (size_t i = 0; i < a_.size; i++) {
                    nar_int_t c = cmp(rt, a_.items[i], b_.items[i]);
                    if (c != 0) {
                        return c;
                    }
                }
                return 0;
            }
            case NAR_OBJECT_KIND_OPTION: {
                nar_option_t a_ = nar->to_option(rt, a);
                nar_option_t b_ = nar->to_option(rt, b);
                nar_int_t c = strcmp(a_.name, b_.name);
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
                nar->fail(rt, "cmp: option sizes are not equal");
            }
            case NAR_OBJECT_KIND_FUNCTION: {
                nar_func_t a_ = nar->to_func(rt, a);
                nar_func_t b_ = nar->to_func(rt, b);
                return (a_.arity < b_.arity) ? -1 :
                       (a_.arity > b_.arity ? 1 :
                        ((a_.ptr < b_.ptr) ? -1 : (a_.ptr > b_.ptr ? 1 : 0))
                       );
            }
            case NAR_OBJECT_KIND_CLOSURE:
            case NAR_OBJECT_KIND_NATIVE: {
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
                nar->fail(rt, "cmp: enum case is out of range");
            }
        }
    } else {
        nar->fail(rt, "cmp: types are not equal");
    }

    return a < b ? -1 : (a > b ? 1 : 0);
}

nar_string_t new_joined_strings(
        nar_runtime_t rt, size_t size, const nar_object_t *strings, nar_cstring_t separator,
        nar_cstring_t prefix, nar_cstring_t suffix) {
    nar_cstring_t *lines = nar->alloc(size * sizeof(nar_cstring_t));
    nar_size_t len_totoal = 0;

    for (size_t i = 0; i < size; i++) {
        nar_cstring_t s = nar->to_string(rt, strings[i]);
        len_totoal += strlen(s);
        lines[i] = s;
    }
    size_t len_separator = strlen(separator);
    len_totoal += (size - 1) * len_separator;
    len_totoal += strlen(prefix) + strlen(suffix);
    nar_string_t result = nar->alloc((len_totoal + 1) * sizeof(char));
    nar_string_t ptr = result;
    size_t len_prefix = strlen(prefix);
    memcpy(ptr, prefix, len_prefix);
    ptr += len_prefix;
    for (size_t i = 0; i < size; i++) {
        size_t len_i = strlen(lines[i]);
        memcpy(ptr, lines[i], len_i);
        ptr += len_i;

        if (i < size - 1) {
            memcpy(ptr, separator, len_separator);
            ptr += len_separator;
        }
    }
    size_t len_suffix = strlen(suffix);
    memcpy(ptr, suffix, len_suffix);
    ptr += len_suffix;
    *ptr = '\0';
    nar->free(lines);
    return result;
}

nar_object_t stringify(nar_runtime_t rt, nar_object_t a) {
    nar_object_kind_t kind = nar->object_get_kind(rt, a);
    switch (kind) {
        case NAR_OBJECT_KIND_UNIT: {
            return nar->new_string(rt, "()");
        }
        case NAR_OBJECT_KIND_CHAR: {
            nar_char_t c = nar->to_char(rt, a);
            char str[MAX_U8_SIZE + 1];
            size_t len = fctou8(str, c);
            str[len] = '\0';
            return nar->new_string(rt, str);
        }
        case NAR_OBJECT_KIND_INT: {
            char str[32];
            snprintf(str, 32, "%lld", nar->to_int(rt, a));
            return nar->new_string(rt, str);
        }
        case NAR_OBJECT_KIND_FLOAT: {
            char buffer[32];
            snprintf(buffer, 32, "%g", nar->to_float(rt, a));
            return nar->new_string(rt, buffer);
        }
        case NAR_OBJECT_KIND_STRING: {
            return a;
        }
        case NAR_OBJECT_KIND_RECORD: {
            nar_record_t record = nar->to_record(rt, a);
            nar_object_t *values = nar->alloc(record.size * sizeof(nar_object_t));
            for (nar_size_t i = 0; i < record.size; i++) {
                values[i] = stringify(rt, record.values[i]);
            }
            nar_cstring_t *valueStrings = nar->alloc(record.size * sizeof(nar_cstring_t));
            for (nar_size_t i = 0; i < record.size; i++) {
                valueStrings[i] = nar->to_string(rt, values[i]);
            }
            nar_size_t total_size = 2;
            for (nar_size_t i = 0; i < record.size; i++) {
                total_size += strlen(record.keys[i]) + strlen(valueStrings[i]) + 4;
            }
            nar_string_t result = nar->alloc(total_size * sizeof(char));
            nar_string_t ptr = result;
            *ptr++ = '{';
            for (nar_size_t i = 0; i < record.size; ++i) {
                memcpy(ptr, record.keys[i], strlen(record.keys[i]));
                ptr += strlen(record.keys[i]);
                *ptr++ = ' ';
                *ptr++ = '=';
                *ptr++ = ' ';
                memcpy(ptr, valueStrings[i], strlen(valueStrings[i]));
                ptr += strlen(valueStrings[i]);
                if (i < record.size - 1) {
                    *ptr++ = ',';
                    *ptr++ = ' ';
                }
            }
            *ptr++ = '}';
            *ptr = '\0';
            nar_object_t str = nar->new_string(rt, result);

            nar->free(result);
            nar->free(valueStrings);
            nar->free(values);
            return str;
        }
        case NAR_OBJECT_KIND_TUPLE: {
            nar_tuple_t tuple = nar->to_tuple(rt, a);
            nar_object_t *strings = nar->alloc(tuple.size * sizeof(nar_object_t));
            for (nar_size_t i = 0; i < tuple.size; i++) {
                strings[i] = stringify(rt, tuple.values[i]);
            }
            nar_string_t result = new_joined_strings(rt, tuple.size, strings, ", ", "(", ")");
            nar_object_t str = nar->new_string(rt, result);
            nar->free(result);
            nar->free(strings);
            return str;
        }
        case NAR_OBJECT_KIND_LIST: {
            nar_list_t list = nar->to_list(rt, a);
            nar_object_t *strings = nar->alloc(list.size * sizeof(nar_object_t));
            for (nar_size_t i = 0; i < list.size; i++) {
                strings[i] = stringify(rt, list.items[i]);
            }
            nar_string_t result = new_joined_strings(rt, list.size, strings, ", ", "[", "]");
            nar_object_t str = nar->new_string(rt, result);
            nar->free(strings);
            nar->free(result);
            return str;
        }
        case NAR_OBJECT_KIND_OPTION: {
            nar_option_t option = nar->to_option(rt, a);
            nar_object_t *values = nar->alloc(option.size * sizeof(nar_object_t));
            for (size_t i = 0; i < option.size; i++) {
                values[i] = stringify(rt, option.values[i]);
            }
            size_t len_name = strlen(option.name);
            nar_string_t name = nar->alloc((len_name + 2));
            memcpy(name, option.name, len_name);
            name[len_name] = '(';
            name[len_name + 1] = '\0';
            nar_string_t result = new_joined_strings(rt, option.size, values, ", ", name, ")");
            nar_object_t str = nar->new_string(rt, result);
            nar->free(result);
            nar->free(values);
            nar->free(name);
            return str;
        }
        case NAR_OBJECT_KIND_FUNCTION: {
            nar_func_t func = nar->to_func(rt, a);
            char result[32];
            snprintf(result, 32, "function/%p(%ld)", func.ptr, func.arity);
            return nar->new_string(rt, result);
        }
        case NAR_OBJECT_KIND_CLOSURE: {
            return nar->new_string(rt, "closure");
        }
        case NAR_OBJECT_KIND_NATIVE: {
            nar_native_t native = nar->to_native(rt, a);
            char result[32];
            snprintf(result, 32, "native/%p(%p)", native.ptr, native.cmp);
            return nar->new_string(rt, result);
        }
        default: {
            nar->fail(rt, "stringify: enum case is out of range");
            return nar->new_string(rt, "?");
        }
    }
}

#pragma clang diagnostic pop
