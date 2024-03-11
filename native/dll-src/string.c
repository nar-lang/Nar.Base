#include <wchar.h>
#include <stdlib.h>
#include "native.h"

nar_object_t string_length(nar_object_t s) {
    return nar_int((nar_int_t) wcslen(nar_to_string(s)));
}

nar_object_t string_reverse(nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    size_t len = wcslen(value);
    wchar_t *reversed = malloc((len + 1) * sizeof(wchar_t));
    for (size_t i = 0; i < len; i++) {
        reversed[i] = value[len - i - 1];
    }
    reversed[len] = L'\0';
    nar_object_t result = nar_string(reversed);
    free(reversed);
    return result;
}

nar_object_t string_append(nar_object_t a, nar_object_t b) {
    nar_string_t value_a = nar_to_string(a);
    nar_string_t value_b = nar_to_string(b);
    size_t len_a = wcslen(value_a);
    size_t len_b = wcslen(value_b);
    wchar_t *appended = malloc((len_a + len_b + 1) * sizeof(wchar_t));
    wcscpy(appended, value_a);
    wcscpy(appended + len_a, value_b);
    appended[len_a + len_b] = L'\0';
    nar_object_t result = nar_string(appended);
    free(appended);
    return result;
}

nar_object_t string_split(nar_object_t sep, nar_object_t string) {
    nar_string_t value_sep = nar_to_string(sep);
    nar_string_t value_string = nar_to_string(string);
    wchar_t *str = wcsdup(value_string);
    wchar_t *token = wcstok(str, value_sep, &str);
    int size = 0;
    while (token) {
        size++;
        token = wcstok(NULL, value_sep, &str);
    }
    free(str);
    str = wcsdup(value_string);
    nar_object_t *items = malloc(size * sizeof(nar_object_t));
    token = wcstok(str, value_sep, &str);
    for (int i = 0; i < size; i++) {
        items[i] = nar_string(token);
        token = wcstok(NULL, value_sep, &str);
    }
    free(str);
    nar_object_t result = nar_list(size, items);
    free(items);
    return result;
}

nar_object_t string_join(nar_object_t sep, nar_object_t strings) {
    nar_string_t value_sep = nar_to_string(sep);
    nar_list_t strings_ = nar_to_list(strings);
    size_t len = 0;
    for (int i = 0; i < strings_.size; i++) {
        len += wcslen(nar_to_string(strings_.items[i]));
    }
    len += (strings_.size - 1) * wcslen(value_sep);
    wchar_t *joined = malloc((len + 1) * sizeof(wchar_t));
    joined[0] = L'\0';
    for (int i = 0; i < strings_.size; i++) {
        wcscat(joined, nar_to_string(strings_.items[i]));
        if (i < strings_.size - 1) {
            wcscat(joined, value_sep);
        }
    }
    nar_object_t result = nar_string(joined);
    free(joined);
    return result;
}

nar_object_t string_words(nar_object_t string) {
    nar_string_t value = nar_to_string(string);
    wchar_t *str = wcsdup(value);
    wchar_t *token = wcstok(str, L" \t\n\r", &str);
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
        items[i] = nar_string(token);
        token = wcstok(NULL, L" \t\n\r", &str);
    }
    free(str);
    nar_object_t result = nar_list(size, items);
    free(items);
    return result;
}

nar_object_t string_lines(nar_object_t string) {
    nar_string_t value = nar_to_string(string);
    wchar_t *str = wcsdup(value);
    wchar_t *token = wcstok(str, L"\n\r", &str);
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
        items[i] = nar_string(token);
        token = wcstok(NULL, L"\n\r", &str);
    }
    free(str);
    nar_object_t result = nar_list(size, items);
    free(items);
    return result;
}

nar_object_t string_slice(nar_object_t from, nar_object_t to, nar_object_t array) {
    nar_string_t value = nar_to_string(array);
    nar_int_t value_begin = nar_to_int(from);
    nar_int_t value_end = nar_to_int(to);
    wchar_t *sliced = wcsdup(value + value_begin);
    sliced[value_end - value_begin] = L'\0';
    nar_object_t result = nar_string(sliced);
    free(sliced);
    return result;
}

nar_object_t string_contains(nar_object_t sub, nar_object_t string) {
    nar_string_t value_sub = nar_to_string(sub);
    nar_string_t value_string = nar_to_string(string);
    return nar_bool(wcsstr(value_string, value_sub) != NULL);
}

nar_object_t string_startsWith(nar_object_t sub, nar_object_t string) {
    nar_string_t value_sub = nar_to_string(sub);
    nar_string_t value_string = nar_to_string(string);
    return nar_bool(wcsncmp(value_string, value_sub, wcslen(value_sub)) == 0);
}

nar_object_t string_endsWith(nar_object_t sub, nar_object_t string) {
    nar_string_t value_sub = nar_to_string(sub);
    nar_string_t value_string = nar_to_string(string);
    size_t len_sub = wcslen(value_sub);
    size_t len_string = wcslen(value_string);
    return nar_bool(wcsncmp(value_string + len_string - len_sub, value_sub, len_sub) == 0);
}

nar_object_t string_indices(nar_object_t sub, nar_object_t string) {
    nar_string_t value_sub = nar_to_string(sub);
    nar_string_t value_string = nar_to_string(string);
    int size = 0;
    wchar_t *pos = value_string;
    while (*pos != L'\0' && ((pos = wcsstr(pos, value_sub)) != NULL)) {
        size++;
        pos++;
    }
    nar_object_t *items = malloc(size * sizeof(nar_object_t));
    pos = value_string;
    for (int i = 0; i < size; i++) {
        pos = wcsstr(pos, value_sub);
        items[i] = nar_int((nar_int_t) (pos - value_string));
        pos++;
    }
    nar_object_t result = nar_list(size, items);
    free(items);
    return result;
}

nar_object_t string_toUpper(nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    size_t len = wcslen(value);
    wchar_t *upper = malloc((len + 1) * sizeof(wchar_t));
    for (size_t i = 0; i < len; i++) {
        upper[i] = towupper(value[i]);
    }
    upper[len] = L'\0';
    nar_object_t result = nar_string(upper);
    free(upper);
    return result;
}

nar_object_t string_toLower(nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    size_t len = wcslen(value);
    wchar_t *lower = malloc((len + 1) * sizeof(wchar_t));
    for (size_t i = 0; i < len; i++) {
        lower[i] = towlower(value[i]);
    }
    lower[len] = L'\0';
    nar_object_t result = nar_string(lower);
    free(lower);
    return result;
}

nar_object_t string_trim(nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    size_t len = wcslen(value);
    size_t begin = 0;
    size_t end = len;
    while (iswspace(value[begin])) {
        begin++;
    }
    while (end > begin && iswspace(value[end - 1])) {
        end--;
    }
    wchar_t *trimmed = malloc((end - begin + 1) * sizeof(wchar_t));
    wcsncpy(trimmed, value + begin, end - begin);
    trimmed[end - begin] = L'\0';
    nar_object_t result = nar_string(trimmed);
    free(trimmed);
    return result;
}

nar_object_t string_trimLeft(nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    size_t len = wcslen(value);
    size_t begin = 0;
    while (iswspace(value[begin])) {
        begin++;
    }
    wchar_t *trimmed = malloc((len - begin + 1) * sizeof(wchar_t));
    wcscpy(trimmed, value + begin);
    nar_object_t result = nar_string(trimmed);
    free(trimmed);
    return result;
}

nar_object_t string_trimRight(nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    size_t len = wcslen(value);
    size_t end = len;
    while (end > 0 && iswspace(value[end - 1])) {
        end--;
    }
    wchar_t *trimmed = malloc((end + 1) * sizeof(wchar_t));
    wcsncpy(trimmed, value, end);
    trimmed[end] = L'\0';
    nar_object_t result = nar_string(trimmed);
    free(trimmed);
    return result;
}

nar_object_t string_toInt(nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    wchar_t *end;
    nar_int_t result = wcstol(value, &end, 10);
    if (end == value) {
        return nar_option(L"Nar.Base.Maybe.Maybe#Nothing", 0, NULL);
    } else {
        return nar_option(L"Nar.Base.Maybe.Maybe#Just", 1, (nar_object_t[]) {nar_int(result)});
    }
}

nar_object_t string_fromInt(nar_object_t n) {
    nar_int_t value = nar_to_int(n);
    wchar_t *buffer = malloc(32 * sizeof(wchar_t));
    swprintf(buffer, 32, L"%d", value);
    nar_object_t result = nar_string(buffer);
    free(buffer);
    return result;
}

nar_object_t string_toFloat(nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    wchar_t *end;
    nar_float_t result = wcstod(value, &end);
    if (end == value) {
        return nar_option(L"Nar.Base.Maybe.Maybe#Nothing", 0, NULL);
    } else {
        return nar_option(L"Nar.Base.Maybe.Maybe#Just", 1, (nar_object_t[]) {nar_float(result)});
    }
}

nar_object_t string_fromFloat(nar_object_t n) {
    nar_float_t value = nar_to_float(n);
    wchar_t *buffer = malloc(32 * sizeof(wchar_t));
    swprintf(buffer, 32, L"%f", value);
    nar_object_t result = nar_string(buffer);
    free(buffer);
    return result;
}

nar_object_t string_cons(nar_object_t head, nar_object_t tail) {
    nar_char_t value_c = nar_to_char(head);
    nar_string_t value_s = nar_to_string(tail);
    wchar_t *consed = malloc((wcslen(value_s) + 2) * sizeof(wchar_t));
    consed[0] = value_c;
    wcscpy(consed + 1, value_s);
    nar_object_t result = nar_string(consed);
    free(consed);
    return result;
}

nar_object_t string_uncons(nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    if (wcslen(value) == 0) {
        return nar_option(L"Nar.Base.Maybe.Maybe#Nothing", 0, NULL);
    } else {
        nar_char_t c = value[0];
        nar_object_t result = nar_option(L"Nar.Base.Maybe.Maybe#Just", 1, (nar_object_t[]) {nar_tuple(2, (nar_object_t[]) {nar_char(c), nar_string(value + 1)})});
        return result;
    }
}

nar_object_t string_map(nar_object_t func, nar_object_t array) {
    nar_string_t value = nar_to_string(array);
    size_t len = wcslen(value);
    wchar_t *mapped = malloc((len + 1) * sizeof(wchar_t));
    for (size_t i = 0; i < len; i++) {
        nar_char_t c = value[i];
        nar_object_t arg = nar_char(c);
        nar_object_t result = nar_apply(func, 1, &arg);
        nar_char_t mapped_c = nar_to_char(result);
        mapped[i] = mapped_c;
    }
    mapped[len] = L'\0';
    nar_object_t result = nar_string(mapped);
    free(mapped);
    return result;
}

nar_object_t string_filter(nar_object_t f, nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    size_t len = wcslen(value);
    wchar_t *filtered = malloc((len + 1) * sizeof(wchar_t));
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        nar_char_t c = value[i];
        nar_object_t arg = nar_char(c);
        nar_object_t result = nar_apply(f, 1, &arg);
        nar_bool_t keep = nar_to_bool(result);
        if (keep) {
            filtered[j++] = c;
        }
    }
    filtered[j] = L'\0';
    nar_object_t result = nar_string(filtered);
    free(filtered);
    return result;
}

nar_object_t string_foldl(nar_object_t func, nar_object_t acc, nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    nar_object_t result = acc;
    for (size_t i = 0; i < wcslen(value); i++) {
        nar_char_t c = value[i];
        nar_object_t arg[2] = {nar_char(c), result};
        result = nar_apply(func, 2, arg);
    }
    return result;
}

nar_object_t string_foldr(nar_object_t func, nar_object_t acc, nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    nar_object_t result = acc;
    for (size_t i = wcslen(value); i > 0; i--) {
        nar_char_t c = value[i - 1];
        nar_object_t arg[2] = {nar_char(c), result};
        result = nar_apply(func, 2, arg);
    }
    return result;
}

nar_object_t string_any(nar_object_t f, nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    for (size_t i = 0; i < wcslen(value); i++) {
        nar_char_t c = value[i];
        nar_object_t arg = nar_char(c);
        nar_object_t result = nar_apply(f, 1, &arg);
        nar_bool_t keep = nar_to_bool(result);
        if (keep) {
            return nar_bool(nar_true);
        }
    }
    return nar_bool(nar_false);
}

nar_object_t string_all(nar_object_t f, nar_object_t s) {
    nar_string_t value = nar_to_string(s);
    for (size_t i = 0; i < wcslen(value); i++) {
        nar_char_t c = value[i];
        nar_object_t arg = nar_char(c);
        nar_object_t result = nar_apply(f, 1, &arg);
        nar_bool_t keep = nar_to_bool(result);
        if (!keep) {
            return nar_bool(nar_false);
        }
    }
    return nar_bool(nar_true);
}

void register_string(void) {
    nar_string_t module_name = L"Nar.Base.String";
    nar_register_def(module_name, L"length", nar_func(&string_length, 1));
    nar_register_def(module_name, L"reverse", nar_func(&string_reverse, 1));
    nar_register_def(module_name, L"append", nar_func(&string_append, 2));
    nar_register_def(module_name, L"split", nar_func(&string_split, 2));
    nar_register_def(module_name, L"join", nar_func(&string_join, 2));
    nar_register_def(module_name, L"words", nar_func(&string_words, 1));
    nar_register_def(module_name, L"lines", nar_func(&string_lines, 1));
    nar_register_def(module_name, L"slice", nar_func(&string_slice, 3));
    nar_register_def(module_name, L"contains", nar_func(&string_contains, 2));
    nar_register_def(module_name, L"startsWith", nar_func(&string_startsWith, 2));
    nar_register_def(module_name, L"endsWith", nar_func(&string_endsWith, 2));
    nar_register_def(module_name, L"indices", nar_func(&string_indices, 2));
    nar_register_def(module_name, L"toUpper", nar_func(&string_toUpper, 1));
    nar_register_def(module_name, L"toLower", nar_func(&string_toLower, 1));
    nar_register_def(module_name, L"trim", nar_func(&string_trim, 1));
    nar_register_def(module_name, L"trimLeft", nar_func(&string_trimLeft, 1));
    nar_register_def(module_name, L"trimRight", nar_func(&string_trimRight, 1));
    nar_register_def(module_name, L"toInt", nar_func(&string_toInt, 1));
    nar_register_def(module_name, L"fromInt", nar_func(&string_fromInt, 1));
    nar_register_def(module_name, L"toFloat", nar_func(&string_toFloat, 1));
    nar_register_def(module_name, L"fromFloat", nar_func(&string_fromFloat, 1));
    nar_register_def(module_name, L"cons", nar_func(&string_cons, 2));
    nar_register_def(module_name, L"uncons", nar_func(&string_uncons, 1));
    nar_register_def(module_name, L"map", nar_func(&string_map, 2));
    nar_register_def(module_name, L"filter", nar_func(&string_filter, 2));
    nar_register_def(module_name, L"foldl", nar_func(&string_foldl, 3));
    nar_register_def(module_name, L"foldr", nar_func(&string_foldr, 3));
    nar_register_def(module_name, L"any", nar_func(&string_any, 2));
    nar_register_def(module_name, L"all", nar_func(&string_all, 2));
}
