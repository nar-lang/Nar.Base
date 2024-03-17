#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include "_package.h"

nar_object_t string_length(nar_runtime_t rt, nar_object_t s) {
    return nar->new_int(rt, (nar_int_t) wcslen(nar->to_string(rt, s)));
}

nar_object_t string_reverse(nar_runtime_t rt, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    size_t len = wcslen(value);
    nar_string_t reversed = malloc((len + 1) * sizeof(nar_char_t));
    for (size_t i = 0; i < len; i++) {
        reversed[i] = value[len - i - 1];
    }
    reversed[len] = L'\0';
    nar_object_t result = nar->new_string(rt, reversed);
    free(reversed);
    return result;
}

nar_object_t string_append(nar_runtime_t rt, nar_object_t a, nar_object_t b) {
    nar_string_t value_a = nar->to_string(rt, a);
    nar_string_t value_b = nar->to_string(rt, b);
    size_t len_a = wcslen(value_a);
    size_t len_b = wcslen(value_b);
    nar_string_t appended = malloc((len_a + len_b + 1) * sizeof(nar_char_t));
    wcscpy(appended, value_a);
    wcscpy(appended + len_a, value_b);
    appended[len_a + len_b] = L'\0';
    nar_object_t result = nar->new_string(rt, appended);
    free(appended);
    return result;
}

nar_object_t string_split(nar_runtime_t rt, nar_object_t sep, nar_object_t string) {
    nar_string_t sep_ = nar->to_string(rt, sep);
    nar_string_t string_ = nar->to_string(rt, string);
    nar_size_t parts = 1;
    nar_string_t token = string_;
    while ((token = wcsstr(token, sep_)) != NULL) {
        parts++;
        token += wcslen(sep_);
    }
    if (parts == 1) {
        return nar->new_list(rt, 1, (nar_object_t[]) {string});
    }

    nar_object_t items[parts];
    token = string_;
    for (nar_size_t i = 0; i < parts; i++) {
        nar_string_t next = wcsstr(token, sep_);
        nar_size_t len = next ? next - token : wcslen(token);
        nar_char_t sub[len + 1];
        wcsncpy(sub, token, len);
        sub[len] = L'\0';
        items[i] = nar->new_string(rt, sub);
        if (!next) {
            break;
        }
        token = next + wcslen(sep_);
    }

    return nar->new_list(rt, parts, items);
}

nar_object_t string_join(nar_runtime_t rt, nar_object_t sep, nar_object_t strings) {
    nar_string_t value_sep = nar->to_string(rt, sep);
    nar_list_t strings_ = nar->to_list(rt, strings);
    size_t len = 0;
    for (int i = 0; i < strings_.size; i++) {
        len += wcslen(nar->to_string(rt, strings_.items[i]));
    }
    len += (strings_.size - 1) * wcslen(value_sep);
    nar_string_t joined = malloc((len + 1) * sizeof(nar_char_t));
    joined[0] = L'\0';
    for (int i = 0; i < strings_.size; i++) {
        wcscat(joined, nar->to_string(rt, strings_.items[i]));
        if (i < strings_.size - 1) {
            wcscat(joined, value_sep);
        }
    }
    nar_object_t result = nar->new_string(rt, joined);
    free(joined);
    return result;
}

nar_object_t string_words(nar_runtime_t rt, nar_object_t string) {
    nar_string_t value = nar->to_string(rt, string);
    nar_string_t str = wcsdup(value);
    nar_string_t token = wcstok(str, L" \t\n\r", &str);
    int size = 0;
    while (token) {
        size++;
        token = wcstok(NULL, L" \t\n\r", &str);
    }
    free(str);
    str = wcsdup(value);
    nar_object_t *items = malloc(size * sizeof(nar_object_t));
    token = wcstok(str, L" \t\n\r", &str);
    for (int i = 0; i < size; i++) {
        items[i] = nar->new_string(rt, token);
        token = wcstok(NULL, L" \t\n\r", &str);
    }
    free(str);
    nar_object_t result = nar->new_list(rt, size, items);
    free(items);
    return result;
}

nar_object_t string_lines(nar_runtime_t rt, nar_object_t string) {
    nar_string_t value = nar->to_string(rt, string);
    nar_string_t str = wcsdup(value);
    nar_string_t token = wcstok(str, L"\n\r", &str);
    int size = 0;
    while (token) {
        size++;
        token = wcstok(NULL, L"\n\r", &str);
    }
    free(str);
    str = wcsdup(value);
    nar_object_t *items = malloc(size * sizeof(nar_object_t));
    token = wcstok(str, L"\n\r", &str);
    for (int i = 0; i < size; i++) {
        items[i] = nar->new_string(rt, token);
        token = wcstok(NULL, L"\n\r", &str);
    }
    free(str);
    nar_object_t result = nar->new_list(rt, size, items);
    free(items);
    return result;
}

nar_object_t string_slice(nar_runtime_t rt, nar_object_t from, nar_object_t to, nar_object_t array) {
    nar_string_t value = nar->to_string(rt, array);
    nar_int_t value_begin = nar->to_int(rt, from);
    nar_int_t value_end = nar->to_int(rt, to);
    nar_size_t len = wcslen(value);
    if (value_begin < 0) {
        value_begin = (nar_int_t) len + value_begin;
    }
    if (value_end < 0) {
        value_end = (nar_int_t) len + value_end;
    }
    nar_string_t sliced = wcsdup(value + value_begin);
    sliced[value_end - value_begin] = L'\0';
    nar_object_t result = nar->new_string(rt, sliced);
    free(sliced);
    return result;
}

nar_object_t string_contains(nar_runtime_t rt, nar_object_t sub, nar_object_t string) {
    nar_string_t value_sub = nar->to_string(rt, sub);
    nar_string_t value_string = nar->to_string(rt, string);
    return nar->new_bool(rt, wcsstr(value_string, value_sub) != NULL);
}

nar_object_t string_startsWith(nar_runtime_t rt, nar_object_t sub, nar_object_t string) {
    nar_string_t value_sub = nar->to_string(rt, sub);
    nar_string_t value_string = nar->to_string(rt, string);
    return nar->new_bool(rt, wcsncmp(value_string, value_sub, wcslen(value_sub)) == 0);
}

nar_object_t string_endsWith(nar_runtime_t rt, nar_object_t sub, nar_object_t string) {
    nar_string_t value_sub = nar->to_string(rt, sub);
    nar_string_t value_string = nar->to_string(rt, string);
    size_t len_sub = wcslen(value_sub);
    size_t len_string = wcslen(value_string);
    return nar->new_bool(rt, wcsncmp(value_string + len_string - len_sub, value_sub, len_sub) == 0);
}

nar_object_t string_indices(nar_runtime_t rt, nar_object_t sub, nar_object_t string) {
    nar_string_t value_sub = nar->to_string(rt, sub);
    if (wcslen(value_sub) == 0) {
        return nar->new_list(rt, 0, NULL);
    }
    nar_string_t value_string = nar->to_string(rt, string);
    int size = 0;
    nar_string_t pos = value_string;
    while (*pos != L'\0' && ((pos = wcsstr(pos, value_sub)) != NULL)) {
        size++;
        pos++;
    }
    nar_object_t *items = malloc(size * sizeof(nar_object_t));
    pos = value_string;
    for (int i = 0; i < size; i++) {
        pos = wcsstr(pos, value_sub);
        items[i] = nar->new_int(rt, (nar_int_t)(pos - value_string));
        pos++;
    }
    nar_object_t result = nar->new_list(rt, size, items);
    free(items);
    return result;
}

nar_object_t string_toUpper(nar_runtime_t rt, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    size_t len = wcslen(value);
    nar_string_t upper = malloc((len + 1) * sizeof(nar_char_t));
    for (size_t i = 0; i < len; i++) {
        upper[i] = towupper(value[i]);
    }
    upper[len] = L'\0';
    nar_object_t result = nar->new_string(rt, upper);
    free(upper);
    return result;
}

nar_object_t string_toLower(nar_runtime_t rt, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    size_t len = wcslen(value);
    nar_string_t lower = malloc((len + 1) * sizeof(nar_char_t));
    for (size_t i = 0; i < len; i++) {
        lower[i] = towlower(value[i]);
    }
    lower[len] = L'\0';
    nar_object_t result = nar->new_string(rt, lower);
    free(lower);
    return result;
}

nar_object_t string_trim(nar_runtime_t rt, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    size_t len = wcslen(value);
    size_t begin = 0;
    size_t end = len;
    while (iswspace(value[begin])) {
        begin++;
    }
    while (end > begin && iswspace(value[end - 1])) {
        end--;
    }
    nar_string_t trimmed = malloc((end - begin + 1) * sizeof(nar_char_t));
    wcsncpy(trimmed, value + begin, end - begin);
    trimmed[end - begin] = L'\0';
    nar_object_t result = nar->new_string(rt, trimmed);
    free(trimmed);
    return result;
}

nar_object_t string_trimLeft(nar_runtime_t rt, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    size_t len = wcslen(value);
    size_t begin = 0;
    while (iswspace(value[begin])) {
        begin++;
    }
    nar_string_t trimmed = malloc((len - begin + 1) * sizeof(nar_char_t));
    wcscpy(trimmed, value + begin);
    nar_object_t result = nar->new_string(rt, trimmed);
    free(trimmed);
    return result;
}

nar_object_t string_trimRight(nar_runtime_t rt, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    size_t len = wcslen(value);
    size_t end = len;
    while (end > 0 && iswspace(value[end - 1])) {
        end--;
    }
    nar_string_t trimmed = malloc((end + 1) * sizeof(nar_char_t));
    wcsncpy(trimmed, value, end);
    trimmed[end] = L'\0';
    nar_object_t result = nar->new_string(rt, trimmed);
    free(trimmed);
    return result;
}

nar_object_t string_toInt(nar_runtime_t rt, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    nar_char_t *end;
    nar_int_t result = wcstol(value, &end, 10);
    if (end == value || end != value + wcslen(value)) {
        return nar->new_option(rt, L"Nar.Base.Maybe.Maybe#Nothing", 0, NULL);
    } else {
        return nar->new_option(rt, L"Nar.Base.Maybe.Maybe#Just", 1, (nar_object_t[]) {nar->new_int(rt, result)});
    }
}

nar_object_t string_fromInt(nar_runtime_t rt, nar_object_t n) {
    nar_int_t value = nar->to_int(rt, n);
    nar_string_t buffer = malloc(32 * sizeof(nar_char_t));
    swprintf(buffer, 32, L"%d", value);
    nar_object_t result = nar->new_string(rt, buffer);
    free(buffer);
    return result;
}

nar_object_t string_toFloat(nar_runtime_t rt, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    nar_char_t *end;
    nar_float_t result = wcstod(value, &end);
    if (end == value || end != value + wcslen(value)) {
        return nar->new_option(rt, L"Nar.Base.Maybe.Maybe#Nothing", 0, NULL);
    } else {
        return nar->new_option(rt, L"Nar.Base.Maybe.Maybe#Just", 1, (nar_object_t[]) {nar->new_float(rt, result)});
    }
}

nar_object_t string_fromFloat(nar_runtime_t rt, nar_object_t n) {
    nar_float_t value = nar->to_float(rt, n);
    nar_string_t buffer = malloc(32 * sizeof(nar_char_t));
    swprintf(buffer, 32, L"%f", value);
    nar_object_t result = nar->new_string(rt, buffer);
    free(buffer);
    return result;
}

nar_object_t string_cons(nar_runtime_t rt, nar_object_t head, nar_object_t tail) {
    nar_char_t value_c = nar->to_char(rt, head);
    nar_string_t value_s = nar->to_string(rt, tail);
    nar_string_t consed = malloc((wcslen(value_s) + 2) * sizeof(nar_char_t));
    consed[0] = value_c;
    wcscpy(consed + 1, value_s);
    nar_object_t result = nar->new_string(rt, consed);
    free(consed);
    return result;
}

nar_object_t string_uncons(nar_runtime_t rt, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    if (wcslen(value) == 0) {
        return nar->new_option(rt, L"Nar.Base.Maybe.Maybe#Nothing", 0, NULL);
    } else {
        nar_char_t c = value[0];
        nar_object_t optValue = nar->new_tuple(rt, 2, (nar_object_t[]) {nar->new_char(rt, c), nar->new_string(rt, value + 1)});
        nar_object_t result = nar->new_option(rt, L"Nar.Base.Maybe.Maybe#Just", 1, &optValue);
        return result;
    }
}

nar_object_t string_map(nar_runtime_t rt, nar_object_t func, nar_object_t array) {
    nar_string_t value = nar->to_string(rt, array);
    size_t len = wcslen(value);
    nar_string_t mapped = malloc((len + 1) * sizeof(nar_char_t));
    for (size_t i = 0; i < len; i++) {
        nar_char_t c = value[i];
        nar_object_t arg = nar->new_char(rt, c);
        nar_object_t result = nar->apply_func(rt, func, 1, &arg);
        nar_char_t mapped_c = nar->to_char(rt, result);
        mapped[i] = mapped_c;
    }
    mapped[len] = L'\0';
    nar_object_t result = nar->new_string(rt, mapped);
    free(mapped);
    return result;
}

nar_object_t string_filter(nar_runtime_t rt, nar_object_t f, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    size_t len = wcslen(value);
    nar_string_t filtered = malloc((len + 1) * sizeof(nar_char_t));
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        nar_char_t c = value[i];
        nar_object_t arg = nar->new_char(rt, c);
        nar_object_t result = nar->apply_func(rt, f, 1, &arg);
        nar_bool_t keep = nar->to_bool(rt, result);
        if (keep) {
            filtered[j++] = c;
        }
    }
    filtered[j] = L'\0';
    nar_object_t result = nar->new_string(rt, filtered);
    free(filtered);
    return result;
}

nar_object_t string_foldl(nar_runtime_t rt, nar_object_t func, nar_object_t acc, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    nar_object_t result = acc;
    for (size_t i = 0; i < wcslen(value); i++) {
        nar_char_t c = value[i];
        nar_object_t arg[2] = {nar->new_char(rt, c), result};
        result = nar->apply_func(rt, func, 2, arg);
    }
    return result;
}

nar_object_t string_foldr(nar_runtime_t rt, nar_object_t func, nar_object_t acc, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    nar_object_t result = acc;
    for (size_t i = wcslen(value); i > 0;) {
        nar_char_t c = value[--i];
        nar_object_t arg[2] = {nar->new_char(rt, c), result};
        result = nar->apply_func(rt, func, 2, arg);
    }
    return result;
}

nar_object_t string_any(nar_runtime_t rt, nar_object_t f, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    for (size_t i = 0; i < wcslen(value); i++) {
        nar_char_t c = value[i];
        nar_object_t arg = nar->new_char(rt, c);
        nar_object_t result = nar->apply_func(rt, f, 1, &arg);
        nar_bool_t keep = nar->to_bool(rt, result);
        if (keep) {
            return nar->new_bool(rt, nar_true);
        }
    }
    return nar->new_bool(rt, nar_false);
}

nar_object_t string_all(nar_runtime_t rt, nar_object_t f, nar_object_t s) {
    nar_string_t value = nar->to_string(rt, s);
    for (size_t i = 0; i < wcslen(value); i++) {
        nar_char_t c = value[i];
        nar_object_t arg = nar->new_char(rt, c);
        nar_object_t result = nar->apply_func(rt, f, 1, &arg);
        nar_bool_t keep = nar->to_bool(rt, result);
        if (!keep) {
            return nar->new_bool(rt, nar_false);
        }
    }
    return nar->new_bool(rt, nar_true);
}

void register_string(nar_runtime_t rt) {
    nar_string_t module_name = L"Nar.Base.String";
    nar->register_def(rt, module_name, L"length", nar->new_func(rt, &string_length, 1));
    nar->register_def(rt, module_name, L"reverse", nar->new_func(rt, &string_reverse, 1));
    nar->register_def(rt, module_name, L"append", nar->new_func(rt, &string_append, 2));
    nar->register_def(rt, module_name, L"split", nar->new_func(rt, &string_split, 2));
    nar->register_def(rt, module_name, L"join", nar->new_func(rt, &string_join, 2));
    nar->register_def(rt, module_name, L"words", nar->new_func(rt, &string_words, 1));
    nar->register_def(rt, module_name, L"lines", nar->new_func(rt, &string_lines, 1));
    nar->register_def(rt, module_name, L"slice", nar->new_func(rt, &string_slice, 3));
    nar->register_def(rt, module_name, L"contains", nar->new_func(rt, &string_contains, 2));
    nar->register_def(rt, module_name, L"startsWith", nar->new_func(rt, &string_startsWith, 2));
    nar->register_def(rt, module_name, L"endsWith", nar->new_func(rt, &string_endsWith, 2));
    nar->register_def(rt, module_name, L"indices", nar->new_func(rt, &string_indices, 2));
    nar->register_def(rt, module_name, L"toUpper", nar->new_func(rt, &string_toUpper, 1));
    nar->register_def(rt, module_name, L"toLower", nar->new_func(rt, &string_toLower, 1));
    nar->register_def(rt, module_name, L"trim", nar->new_func(rt, &string_trim, 1));
    nar->register_def(rt, module_name, L"trimLeft", nar->new_func(rt, &string_trimLeft, 1));
    nar->register_def(rt, module_name, L"trimRight", nar->new_func(rt, &string_trimRight, 1));
    nar->register_def(rt, module_name, L"toInt", nar->new_func(rt, &string_toInt, 1));
    nar->register_def(rt, module_name, L"fromInt", nar->new_func(rt, &string_fromInt, 1));
    nar->register_def(rt, module_name, L"toFloat", nar->new_func(rt, &string_toFloat, 1));
    nar->register_def(rt, module_name, L"fromFloat", nar->new_func(rt, &string_fromFloat, 1));
    nar->register_def(rt, module_name, L"cons", nar->new_func(rt, &string_cons, 2));
    nar->register_def(rt, module_name, L"uncons", nar->new_func(rt, &string_uncons, 1));
    nar->register_def(rt, module_name, L"map", nar->new_func(rt, &string_map, 2));
    nar->register_def(rt, module_name, L"filter", nar->new_func(rt, &string_filter, 2));
    nar->register_def(rt, module_name, L"foldl", nar->new_func(rt, &string_foldl, 3));
    nar->register_def(rt, module_name, L"foldr", nar->new_func(rt, &string_foldr, 3));
    nar->register_def(rt, module_name, L"any", nar->new_func(rt, &string_any, 2));
    nar->register_def(rt, module_name, L"all", nar->new_func(rt, &string_all, 2));
}
