#include <wchar.h>
#include <nar-package.h>
#include <string.h>
#include <stdbool.h>
#include "_package.h"
#include "vector.h"

bool index_of(
        const fchar_t *str, size_t str_len, const fchar_t *sub, size_t sub_len, size_t start,
        size_t *out_index) {
    *out_index = 0;
    if (sub_len == 0) {
        return false;
    }
    for (size_t i = start; i < str_len; i++) {
        if (str[i] == sub[0]) {
            nar_bool_t found = nar_true;
            for (size_t j = 1; j < sub_len; j++) {
                if (str[i + j] != sub[j]) {
                    found = nar_false;
                    break;
                }
            }
            if (found) {
                *out_index = i;
                return true;
            }
        }
    }
    return false;
}

fchar_t *new_fstring_from_object(nar_runtime_t rt, nar_object_t string, size_t *out_len) {
    nar_cstring_t u8string = nar->to_string(rt, string);
    size_t u8srting_len = strlen(u8string);
    fchar_t *fstring = nar->alloc(sizeof(fchar_t) * (u8srting_len + 1));
    size_t fstring_len = u8sntofcs(fstring, u8string, u8srting_len, u8srting_len);
    if (out_len != NULL) {
        *out_len = fstring_len;
    }
    return fstring;
}

nar_object_t fstring_to_obj(nar_runtime_t rt, const fchar_t *str, size_t len) {
    size_t string_result_len = len * MAX_U8_SIZE;
    nar_string_t string_result = nar->alloc(sizeof(char) * (string_result_len + 1));
    fcsntou8s(string_result, str, string_result_len, len);
    nar_object_t result = nar->new_string(rt, string_result);
    nar->free(string_result);
    return result;
}

nar_object_t string_length(nar_runtime_t rt, nar_object_t string) {
    size_t len;
    nar->free(new_fstring_from_object(rt, string, &len));
    return nar->new_int(rt, (nar_int_t) len);
}

nar_object_t string_reverse(nar_runtime_t rt, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    size_t hlen = fstring_len / 2;
    for (size_t i = 0; i < hlen; i++) {
        fchar_t tmp = fstring[i];
        fstring[i] = fstring[fstring_len - i - 1];
        fstring[fstring_len - i - 1] = tmp;
    }

    nar_object_t result = fstring_to_obj(rt, fstring, fstring_len);
    nar->free(fstring);
    return result;
}

nar_object_t string_append(nar_runtime_t rt, nar_object_t a, nar_object_t b) {
    nar_cstring_t value_a = nar->to_string(rt, a);
    nar_cstring_t value_b = nar->to_string(rt, b);
    size_t len_a = strlen(value_a);
    size_t len_b = strlen(value_b);
    size_t len_total = len_a + len_b;
    nar_string_t appended = nar->alloc((len_total + 1) * sizeof(char));
    strcpy(appended, value_a);
    strcpy(appended + len_a, value_b);
    appended[len_total] = '\0';
    nar_object_t result = nar->new_string(rt, appended);
    nar->free(appended);
    return result;
}

nar_object_t string_split(nar_runtime_t rt, nar_object_t sep, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    size_t fsep_len;
    fchar_t *fsep = new_fstring_from_object(rt, sep, &fsep_len);

    vector_t *items = nvector_new(sizeof(nar_object_t), 0, nar);
    size_t last_index = 0;
    size_t index;
    bool found = index_of(fstring, fstring_len, fsep, fsep_len, 0, &index);
    while (found) {
        nar_object_t part = fstring_to_obj(rt, fstring + last_index, index - last_index);
        vector_push(items, 1, &part);
        last_index = index + fsep_len;
        found = index_of(fstring, fstring_len, fsep, fsep_len, last_index, &index);
    }

    nar_object_t part = fstring_to_obj(rt, fstring + last_index, fstring_len - last_index);
    vector_push(items, 1, &part);

    nar_object_t result = nar->new_list(rt, vector_size(items), vector_data(items));
    nar->free(fstring);
    nar->free(fsep);
    vector_free(items);
    return result;
}

nar_object_t string_join(nar_runtime_t rt, nar_object_t sep, nar_object_t strings) {
    nar_cstring_t sep_ = nar->to_string(rt, sep);
    size_t sep_len = strlen(sep_);
    nar_list_t list = nar->to_list(rt, strings);
    nar_cstring_t *ss = nar->alloc(list.size * sizeof(nar_cstring_t));
    size_t len = 0;
    for (int i = 0; i < list.size; i++) {
        nar_cstring_t s = nar->to_string(rt, list.items[i]);
        len += strlen(s);
        ss[i] = s;
    }
    len += (list.size - 1) * sep_len;
    nar_string_t joined = nar->alloc((len + 1) * sizeof(char));
    nar_string_t ptr = joined;
    for (int i = 0; i < list.size; i++) {
        size_t l = strlen(ss[i]);
        memcpy(ptr, ss[i], l);
        ptr += l;
        if (i < list.size - 1) {
            memcpy(ptr, sep_, sep_len);
            ptr += sep_len;
        }
    }
    *ptr = '\0';
    nar_object_t result = nar->new_string(rt, joined);
    nar->free(joined);
    nar->free(ss);
    return result;
}

nar_object_t string_words(nar_runtime_t rt, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    vector_t *words = nvector_new(sizeof(nar_object_t), 0, nar);

    size_t last_index = 0;
    for (size_t i = 0; i < fstring_len; i++) {
        if (iswspace((wchar_t)fstring[i])) {
            size_t len = i - last_index;
            if (len > 0) {
                nar_object_t word = fstring_to_obj(rt, fstring + last_index, len);
                vector_push(words, 1, &word);
            }
            last_index = i + 1;
        }
    }

    if (last_index < fstring_len) {
        nar_object_t word = fstring_to_obj(rt, fstring + last_index, fstring_len - last_index);
        vector_push(words, 1, &word);
    }

    nar_object_t result = nar->new_list(rt, vector_size(words), vector_data(words));
    nar->free(fstring);
    vector_free(words);
    return result;
}

nar_object_t string_lines(nar_runtime_t rt, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    vector_t *words = nvector_new(sizeof(nar_object_t), 0, nar);

    size_t last_index = 0;
    for (size_t i = 0; i < fstring_len; i++) {
        if (fstring[i] == '\n' || fstring[i] == '\r') {
            size_t len = i - last_index;
            if (len > 0) {
                nar_object_t word = fstring_to_obj(rt, fstring + last_index, len);
                vector_push(words, 1, &word);
            }
            last_index = i + 1;
        }
    }

    if (last_index < fstring_len) {
        nar_object_t word = fstring_to_obj(rt, fstring + last_index, fstring_len - last_index);
        vector_push(words, 1, &word);
    }

    nar_object_t result = nar->new_list(rt, vector_size(words), vector_data(words));
    nar->free(fstring);
    vector_free(words);
    return result;
}

nar_object_t string_slice(
        nar_runtime_t rt, nar_object_t from, nar_object_t to, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    nar_int_t from_ = nar->to_int(rt, from);
    nar_int_t to_ = nar->to_int(rt, to);
    if (from_ < 0) {
        from_ += (nar_int_t) fstring_len;
    }
    if (to_ < 0) {
        to_ += (nar_int_t) fstring_len;
    }
    if (from_ < 0) {
        from_ = 0;
    }
    if (to_ > fstring_len) {
        to_ = (nar_int_t) fstring_len;
    }

    nar_int_t slice_len = (to_ - from_);
    nar_object_t result = fstring_to_obj(rt, fstring + from_, slice_len);
    nar->free(fstring);
    return result;
}

nar_object_t string_contains(nar_runtime_t rt, nar_object_t sub, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    size_t fsub_len;
    fchar_t *fsub = new_fstring_from_object(rt, sub, &fsub_len);

    nar_bool_t found = nar_true;
    if (fstring_len >= fsub_len) {
        for (size_t i = 0; i < fsub_len; i++) {
            if (fstring[i] != fsub[i]) {
                found = nar_false;
                break;
            }
        }
    } else {
        found = nar_false;
    }

    nar->free(fstring);
    nar->free(fsub);
    return nar->new_bool(rt, found);
}

nar_object_t string_startsWith(nar_runtime_t rt, nar_object_t sub, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    size_t fsub_len;
    fchar_t *fsub = new_fstring_from_object(rt, sub, &fsub_len);

    nar_bool_t found = nar_true;
    if (fstring_len >= fsub_len) {
        for (size_t i = 0; i < fsub_len; i++) {
            if (fstring[i] != fsub[i]) {
                found = nar_false;
                break;
            }
        }
    } else {
        found = nar_false;
    }

    nar->free(fstring);
    nar->free(fsub);
    return nar->new_bool(rt, found);
}

nar_object_t string_endsWith(nar_runtime_t rt, nar_object_t sub, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    size_t fsub_len;
    fchar_t *fsub = new_fstring_from_object(rt, sub, &fsub_len);

    nar_bool_t found = nar_true;
    if (fstring_len >= fsub_len) {
        for (size_t i = 0; i < fsub_len; i++) {
            if (fstring[fstring_len - i - 1] != fsub[fsub_len - i - 1]) {
                found = nar_false;
                break;
            }
        }
    } else {
        found = nar_false;
    }

    nar->free(fstring);
    nar->free(fsub);
    return nar->new_bool(rt, found);
}

nar_object_t string_indices(nar_runtime_t rt, nar_object_t sub, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);
    size_t fsub_len;
    fchar_t *fsub = new_fstring_from_object(rt, sub, &fsub_len);

    vector_t *indices = nvector_new(sizeof(nar_object_t), 0, nar);
    size_t last_index;
    bool found = index_of(fstring, fstring_len, fsub, fsub_len, 0, &last_index);
    while (found) {
        nar_object_t index = nar->new_int(rt, (nar_int_t) last_index);
        vector_push(indices, 1, &index);
        found = index_of(fstring, fstring_len, fsub, fsub_len, last_index + 1, &last_index);
    }

    nar_object_t result = nar->new_list(rt, vector_size(indices), vector_data(indices));
    nar->free(fstring);
    nar->free(fsub);
    vector_free(indices);
    return result;
}

nar_object_t string_toUpper(nar_runtime_t rt, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    for (size_t i = 0; i < fstring_len; i++) {
        fstring[i] = (fchar_t)towupper((wchar_t)fstring[i]);
    }

    nar_object_t result = fstring_to_obj(rt, fstring, fstring_len);
    nar->free(fstring);
    return result;
}

nar_object_t string_toLower(nar_runtime_t rt, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    for (size_t i = 0; i < fstring_len; i++) {
        fstring[i] = (fchar_t)towlower((wchar_t)fstring[i]);
    }

    nar_object_t result = fstring_to_obj(rt, fstring, fstring_len);
    nar->free(fstring);
    return result;
}

nar_object_t string_trim(nar_runtime_t rt, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    size_t start = 0;
    while (start < fstring_len && iswspace((wchar_t)fstring[start])) {
        start++;
    }

    size_t end = fstring_len;
    while (end > start && iswspace((wchar_t)fstring[end - 1])) {
        end--;
    }

    nar_object_t result = fstring_to_obj(rt, fstring + start, end - start);
    nar->free(fstring);
    return result;
}

nar_object_t string_trimLeft(nar_runtime_t rt, nar_object_t s) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, s, &fstring_len);

    size_t start = 0;
    while (start < fstring_len && iswspace((wchar_t)fstring[start])) {
        start++;
    }

    nar_object_t result = fstring_to_obj(rt, fstring + start, fstring_len - start);
    nar->free(fstring);
    return result;
}

nar_object_t string_trimRight(nar_runtime_t rt, nar_object_t s) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, s, &fstring_len);

    size_t end = fstring_len;
    while (end > 0 && iswspace((wchar_t)fstring[end - 1])) {
        end--;
    }

    nar_object_t result = fstring_to_obj(rt, fstring, end);
    nar->free(fstring);
    return result;
}

nar_object_t string_toInt(nar_runtime_t rt, nar_object_t s) {
    nar_cstring_t value = nar->to_string(rt, s);
    nar_string_t end;
    nar_int_t result = strtol(value, &end, 10);
    if (end == value || end != value + strlen(value)) {
        return nar->new_option(rt, "Nar.Base.Maybe.Maybe#Nothing", 0, NULL);
    } else {
        return nar->new_option(rt, "Nar.Base.Maybe.Maybe#Just", 1,
                (nar_object_t[]) {nar->new_int(rt, result)});
    }
}

nar_object_t string_fromInt(nar_runtime_t rt, nar_object_t n) {
    nar_int_t value = nar->to_int(rt, n);
    nar_string_t buffer = nar->alloc(32 * sizeof(char));
    snprintf(buffer, 32, "%lld", value);
    nar_object_t result = nar->new_string(rt, buffer);
    nar->free(buffer);
    return result;
}

nar_object_t string_toFloat(nar_runtime_t rt, nar_object_t s) {
    nar_cstring_t value = nar->to_string(rt, s);
    nar_string_t end;
    nar_float_t result = strtod(value, &end);
    if (end == value || end != value + strlen(value)) {
        return nar->new_option(rt, "Nar.Base.Maybe.Maybe#Nothing", 0, NULL);
    } else {
        return nar->new_option(rt, "Nar.Base.Maybe.Maybe#Just", 1,
                (nar_object_t[]) {nar->new_float(rt, result)});
    }
}

nar_object_t string_fromFloat(nar_runtime_t rt, nar_object_t n) {
    nar_float_t value = nar->to_float(rt, n);
    nar_string_t buffer = nar->alloc(32 * sizeof(char));
    snprintf(buffer, 32, "%f", value);
    nar_object_t result = nar->new_string(rt, buffer);
    nar->free(buffer);
    return result;
}

nar_object_t string_cons(nar_runtime_t rt, nar_object_t head, nar_object_t tail) {
    nar_char_t value_c = nar->to_char(rt, head);
    nar_cstring_t value_s = nar->to_string(rt, tail);
    char h[MAX_U8_SIZE + 1];
    size_t hlen = fctou8(h, value_c);
    h[hlen] = '\0';
    nar_string_t consed = nar->alloc((strlen(value_s) + hlen + 1) * sizeof(char));
    strcpy(consed, h);
    strcat(consed, value_s);
    nar_object_t result = nar->new_string(rt, consed);
    nar->free(consed);
    return result;
}

nar_object_t string_uncons(nar_runtime_t rt, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    nar_object_t result;
    if (fstring_len == 0) {
        result = nar->new_option(rt, "Nar.Base.Maybe.Maybe#Nothing", 0, NULL);
    } else {
        size_t string_result_len = (fstring_len - 1) * MAX_U8_SIZE;
        nar_string_t string_result = nar->alloc(sizeof(char) * (string_result_len + 1));
        fcstou8s(string_result, fstring + 1, string_result_len);
        nar_object_t result_obj = nar->new_string(rt, string_result);
        nar->free(string_result);

        nar_object_t optValue = nar->new_tuple(rt, 2,
                (nar_object_t[]) {nar->new_char(rt, fstring[0]), result_obj});
        result = nar->new_option(rt, "Nar.Base.Maybe.Maybe#Just", 1, &optValue);
    }

    nar->free(fstring);
    return result;
}

nar_object_t string_map(nar_runtime_t rt, nar_object_t func, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    for (size_t i = 0; i < fstring_len; i++) {
        nar_object_t c = nar->new_char(rt, fstring[i]);
        c = nar->apply_func(rt, func, 1, &c);
        fstring[i] = nar->to_char(rt, c);
    }

    nar_object_t result = fstring_to_obj(rt, fstring, fstring_len);
    nar->free(fstring);
    return result;
}

nar_object_t string_filter(nar_runtime_t rt, nar_object_t f, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);
    size_t skipped = 0;
    for (size_t i = 0; i < fstring_len; i++) {
        nar_object_t arg = nar->new_char(rt, fstring[i]);
        nar_object_t result = nar->apply_func(rt, f, 1, &arg);
        if (!nar->to_bool(rt, result)) {
            skipped++;
        } else {
            fstring[i - skipped] = fstring[i];
        }
    }

    nar_object_t result = fstring_to_obj(rt, fstring, fstring_len - skipped);
    nar->free(fstring);
    return result;
}

nar_object_t string_foldl(
        nar_runtime_t rt, nar_object_t func, nar_object_t acc, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    nar_object_t result = acc;
    for (size_t i = 0; i < fstring_len; i++) {
        nar_object_t arg[2] = {nar->new_char(rt, fstring[i]), result};
        result = nar->apply_func(rt, func, 2, arg);
    }

    nar->free(fstring);
    return result;
}

nar_object_t string_foldr(
        nar_runtime_t rt, nar_object_t func, nar_object_t acc, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    nar_object_t result = acc;
    for (size_t i = fstring_len; i > 0; i--) {
        nar_object_t arg[2] = {nar->new_char(rt, fstring[i - 1]), result};
        result = nar->apply_func(rt, func, 2, arg);
    }

    nar->free(fstring);
    return result;
}

nar_object_t string_any(nar_runtime_t rt, nar_object_t func, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    nar_bool_t result = false;
    for (size_t i = 0; i < fstring_len; i++) {
        nar_object_t c = nar->new_char(rt, fstring[i]);
        result = nar->to_bool(rt, nar->apply_func(rt, func, 1, &c));
        if (result) {
            break;
        }
    }

    nar->free(fstring);
    return nar->new_bool(rt, result);
}

nar_object_t string_all(nar_runtime_t rt, nar_object_t func, nar_object_t string) {
    size_t fstring_len;
    fchar_t *fstring = new_fstring_from_object(rt, string, &fstring_len);

    nar_bool_t result = true;
    if (fstring_len == 0) {
        result = false;
    } else {
        for (size_t i = 0; i < fstring_len; i++) {
            nar_object_t c = nar->new_char(rt, fstring[i]);
            result = nar->to_bool(rt, nar->apply_func(rt, func, 1, &c));
            if (!result) {
                break;
            }
        }
    }

    nar->free(fstring);
    return nar->new_bool(rt, result);
}

void register_string(nar_runtime_t rt) {
    nar_string_t module_name = "Nar.Base.String";
    nar->register_def_dynamic(rt, module_name, "length", "string_length", 1);
    nar->register_def_dynamic(rt, module_name, "reverse", "string_reverse", 1);
    nar->register_def_dynamic(rt, module_name, "append", "string_append", 2);
    nar->register_def_dynamic(rt, module_name, "split", "string_split", 2);
    nar->register_def_dynamic(rt, module_name, "join", "string_join", 2);
    nar->register_def_dynamic(rt, module_name, "words", "string_words", 1);
    nar->register_def_dynamic(rt, module_name, "lines", "string_lines", 1);
    nar->register_def_dynamic(rt, module_name, "slice", "string_slice", 3);
    nar->register_def_dynamic(rt, module_name, "contains", "string_contains", 2);
    nar->register_def_dynamic(rt, module_name, "startsWith", "string_startsWith", 2);
    nar->register_def_dynamic(rt, module_name, "endsWith", "string_endsWith", 2);
    nar->register_def_dynamic(rt, module_name, "indices", "string_indices", 2);
    nar->register_def_dynamic(rt, module_name, "toUpper", "string_toUpper", 1);
    nar->register_def_dynamic(rt, module_name, "toLower", "string_toLower", 1);
    nar->register_def_dynamic(rt, module_name, "trim", "string_trim", 1);
    nar->register_def_dynamic(rt, module_name, "trimLeft", "string_trimLeft", 1);
    nar->register_def_dynamic(rt, module_name, "trimRight", "string_trimRight", 1);
    nar->register_def_dynamic(rt, module_name, "toInt", "string_toInt", 1);
    nar->register_def_dynamic(rt, module_name, "fromInt", "string_fromInt", 1);
    nar->register_def_dynamic(rt, module_name, "toFloat", "string_toFloat", 1);
    nar->register_def_dynamic(rt, module_name, "fromFloat", "string_fromFloat", 1);
    nar->register_def_dynamic(rt, module_name, "cons", "string_cons", 2);
    nar->register_def_dynamic(rt, module_name, "uncons", "string_uncons", 1);
    nar->register_def_dynamic(rt, module_name, "map", "string_map", 2);
    nar->register_def_dynamic(rt, module_name, "filter", "string_filter", 2);
    nar->register_def_dynamic(rt, module_name, "foldl", "string_foldl", 3);
    nar->register_def_dynamic(rt, module_name, "foldr", "string_foldr", 3);
    nar->register_def_dynamic(rt, module_name, "any", "string_any", 2);
    nar->register_def_dynamic(rt, module_name, "all", "string_all", 2);
}
