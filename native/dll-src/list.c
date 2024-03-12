#include <stdlib.h>
#include "native.h"
#include "utils.h"

//TODO: iterate lists instead of using nar_un_list

nar_object_t list_cons(nar_object_t head, nar_object_t tail) {
    return nar_list_cons(head, tail);
}

nar_object_t list_map2(nar_object_t fn, nar_object_t a, nar_object_t b) {
    nar_list_t a_ = nar_to_list(a);
    nar_list_t b_ = nar_to_list(b);
    nar_size_t size = a_.size < b_.size ? a_.size : b_.size;
    nar_object_t *items = malloc(size * sizeof(nar_object_t));
    for (int i = 0; i < size; i++) {
        nar_object_t arg[2] = {a_.items[i], b_.items[i]};
        items[i] = nar_apply(fn, 2, arg);
    }
    nar_object_t result = nar_list(size, items);
    free(items);
    return result;
}

nar_object_t list_map3(nar_object_t fn, nar_object_t a, nar_object_t b, nar_object_t c) {
    nar_list_t a_ = nar_to_list(a);
    nar_list_t b_ = nar_to_list(b);
    nar_list_t c_ = nar_to_list(c);
    nar_size_t size = a_.size < b_.size ? a_.size : b_.size;
    size = size < c_.size ? size : c_.size;
    nar_object_t *items = malloc(size * sizeof(nar_object_t));
    for (int i = 0; i < size; i++) {
        nar_object_t arg[3] = {a_.items[i], b_.items[i], c_.items[i]};
        items[i] = nar_apply(fn, 3, arg);
    }
    nar_object_t result = nar_list(size, items);
    free(items);
    return result;
}

nar_object_t list_map4(nar_object_t fn, nar_object_t a, nar_object_t b, nar_object_t c, nar_object_t d) {
    nar_list_t a_ = nar_to_list(a);
    nar_list_t b_ = nar_to_list(b);
    nar_list_t c_ = nar_to_list(c);
    nar_list_t d_ = nar_to_list(d);
    nar_size_t size = a_.size < b_.size ? a_.size : b_.size;
    size = size < c_.size ? size : c_.size;
    size = size < d_.size ? size : d_.size;
    nar_object_t *items = malloc(size * sizeof(nar_object_t));
    for (int i = 0; i < size; i++) {
        nar_object_t arg[4] = {a_.items[i], b_.items[i], c_.items[i], d_.items[i]};
        items[i] = nar_apply(fn, 4, arg);
    }
    nar_object_t result = nar_list(size, items);
    free(items);
    return result;
}

nar_object_t list_map5(nar_object_t fn, nar_object_t a, nar_object_t b, nar_object_t c, nar_object_t d, nar_object_t e) {
    nar_list_t a_ = nar_to_list(a);
    nar_list_t b_ = nar_to_list(b);
    nar_list_t c_ = nar_to_list(c);
    nar_list_t d_ = nar_to_list(d);
    nar_list_t e_ = nar_to_list(e);
    nar_size_t size = a_.size < b_.size ? a_.size : b_.size;
    size = size < c_.size ? size : c_.size;
    size = size < d_.size ? size : d_.size;
    size = size < e_.size ? size : e_.size;
    nar_object_t *items = malloc(size * sizeof(nar_object_t));
    for (int i = 0; i < size; i++) {
        nar_object_t arg[5] = {a_.items[i], b_.items[i], c_.items[i], d_.items[i], e_.items[i]};
        items[i] = nar_apply(fn, 5, arg);
    }
    nar_object_t result = nar_list(size, items);
    free(items);
    return result;
}

int with_fn(void *cmp_fn, const void *a, const void *b) {
    nar_object_t fn = *(nar_object_t *) cmp_fn;
    nar_object_t arg[2] = {*(nar_object_t *) a, *(nar_object_t *) b};
    nar_object_t res = nar_apply(fn, 2, arg);
    nar_option_t res_ = nar_to_option(res);
    if (wcscmp(res_.name, L"Nar.Base.Basics.Order#LT") == 0) {
        return -1;
    }
    if (wcscmp(res_.name, L"Nar.Base.Basics.Order#GT") == 0) {
        return 1;
    }
    if (wcscmp(res_.name, L"Nar.Base.Basics.Order#EQ") == 0) {
        return 0;
    }
    nar_fail(L"Nar.Base.List.sortWith: expected that predicate returns Nar.Base.Basics.Order");
    return -1;
}

nar_object_t list_sortWith(nar_object_t fn, nar_object_t xs) {
    nar_list_t xs_ = nar_to_list(xs);
    qsort_r(xs_.items, xs_.size, sizeof(nar_object_t), &fn, &with_fn);
    return nar_list(xs_.size, xs_.items);
}

int by_fn(void *sel_fn, const void *a, const void *b) {
    nar_object_t fn = *(nar_object_t *) sel_fn;
    nar_object_t value_a = nar_apply(fn, 1, (nar_object_t *) a);
    nar_object_t value_b = nar_apply(fn, 1, (nar_object_t *) b);
    return (int)cmp(value_a, value_b);
}

nar_object_t list_sortBy(nar_object_t fn, nar_object_t xs) {
    nar_list_t xs_ = nar_to_list(xs);
    qsort_r(xs_.items, xs_.size, sizeof(nar_object_t), &fn, &by_fn);
    return nar_list(xs_.size, xs_.items);
}

void register_list(void) {
    nar_string_t module_name = L"Nar.Base.List";
    nar_register_def(module_name, L"cons", nar_func(&list_cons, 2));
    nar_register_def(module_name, L"map2", nar_func(&list_map2, 3));
    nar_register_def(module_name, L"map3", nar_func(&list_map3, 4));
    nar_register_def(module_name, L"map4", nar_func(&list_map4, 5));
    nar_register_def(module_name, L"map5", nar_func(&list_map5, 6));
    nar_register_def(module_name, L"sortWith", nar_func(&list_sortWith, 2));
    nar_register_def(module_name, L"sortBy", nar_func(&list_sortBy, 2));
}
