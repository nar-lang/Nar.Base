#include <stdlib.h>
#include <string.h>
#include "_package.h"
#include "utils.h"

//TODO: iterate lists instead of using nar_un_list

nar_object_t list_cons(nar_runtime_t rt, nar_object_t head, nar_object_t tail) {
    return nar->new_list_cons(rt, head, tail);
}

nar_object_t list_map2(nar_runtime_t rt, nar_object_t fn, nar_object_t a, nar_object_t b) {
    nar_list_t a_ = nar->to_list(rt, a);
    nar_list_t b_ = nar->to_list(rt, b);
    nar_size_t size = a_.size < b_.size ? a_.size : b_.size;
    nar_object_t *items = nar->alloc(size * sizeof(nar_object_t));
    for (int i = 0; i < size; i++) {
        nar_object_t arg[2] = {a_.items[i], b_.items[i]};
        items[i] = nar->apply_func(rt, fn, 2, arg);
    }
    nar_object_t result = nar->new_list(rt, size, items);
    nar->free(items);
    return result;
}

nar_object_t list_map3(
        nar_runtime_t rt, nar_object_t fn, nar_object_t a, nar_object_t b, nar_object_t c) {
    nar_list_t a_ = nar->to_list(rt, a);
    nar_list_t b_ = nar->to_list(rt, b);
    nar_list_t c_ = nar->to_list(rt, c);
    nar_size_t size = a_.size < b_.size ? a_.size : b_.size;
    size = size < c_.size ? size : c_.size;
    nar_object_t *items = nar->alloc(size * sizeof(nar_object_t));
    for (int i = 0; i < size; i++) {
        nar_object_t arg[3] = {a_.items[i], b_.items[i], c_.items[i]};
        items[i] = nar->apply_func(rt, fn, 3, arg);
    }
    nar_object_t result = nar->new_list(rt, size, items);
    nar->free(items);
    return result;
}

nar_object_t list_map4(
        nar_runtime_t rt, nar_object_t fn, nar_object_t a, nar_object_t b, nar_object_t c,
        nar_object_t d) {
    nar_list_t a_ = nar->to_list(rt, a);
    nar_list_t b_ = nar->to_list(rt, b);
    nar_list_t c_ = nar->to_list(rt, c);
    nar_list_t d_ = nar->to_list(rt, d);
    nar_size_t size = a_.size < b_.size ? a_.size : b_.size;
    size = size < c_.size ? size : c_.size;
    size = size < d_.size ? size : d_.size;
    nar_object_t *items = nar->alloc(size * sizeof(nar_object_t));
    for (int i = 0; i < size; i++) {
        nar_object_t arg[4] = {a_.items[i], b_.items[i], c_.items[i], d_.items[i]};
        items[i] = nar->apply_func(rt, fn, 4, arg);
    }
    nar_object_t result = nar->new_list(rt, size, items);
    nar->free(items);
    return result;
}

nar_object_t list_map5(
        nar_runtime_t rt, nar_object_t fn, nar_object_t a, nar_object_t b, nar_object_t c,
        nar_object_t d, nar_object_t e) {
    nar_list_t a_ = nar->to_list(rt, a);
    nar_list_t b_ = nar->to_list(rt, b);
    nar_list_t c_ = nar->to_list(rt, c);
    nar_list_t d_ = nar->to_list(rt, d);
    nar_list_t e_ = nar->to_list(rt, e);
    nar_size_t size = a_.size < b_.size ? a_.size : b_.size;
    size = size < c_.size ? size : c_.size;
    size = size < d_.size ? size : d_.size;
    size = size < e_.size ? size : e_.size;
    nar_object_t *items = nar->alloc(size * sizeof(nar_object_t));
    for (int i = 0; i < size; i++) {
        nar_object_t arg[5] = {a_.items[i], b_.items[i], c_.items[i], d_.items[i], e_.items[i]};
        items[i] = nar->apply_func(rt, fn, 5, arg);
    }
    nar_object_t result = nar->new_list(rt, size, items);
    nar->free(items);
    return result;
}

typedef struct {
    nar_runtime_t rt;
    nar_object_t cmp_fn;
} rt_and_fn;

int with_fn(void *data, const void *a, const void *b) {
    nar_runtime_t rt = ((rt_and_fn *) data)->rt;
    nar_object_t fn = ((rt_and_fn *) data)->cmp_fn;
    nar_object_t arg[2] = {*(nar_object_t *) a, *(nar_object_t *) b};
    nar_object_t res = nar->apply_func(rt, fn, 2, arg);
    nar_option_t res_ = nar->to_option(rt, res);
    if (strcmp(res_.name, "Nar.Base.Basics.Order#LT") == 0) {
        return -1;
    }
    if (strcmp(res_.name, "Nar.Base.Basics.Order#GT") == 0) {
        return 1;
    }
    if (strcmp(res_.name, "Nar.Base.Basics.Order#EQ") == 0) {
        return 0;
    }
    nar->fail(rt, "Nar.Base.List.sortWith: expected that predicate returns Nar.Base.Basics.Order");
    return -1;
}

nar_object_t list_sortWith(nar_runtime_t rt, nar_object_t fn, nar_object_t xs) {
    nar_list_t xs_ = nar->to_list(rt, xs);
    rt_and_fn data = {rt, fn};
    qsort_r(xs_.items, xs_.size, sizeof(nar_object_t), &data, &with_fn);
    return nar->new_list(rt, xs_.size, xs_.items);
}

int by_fn(void *data, const void *a, const void *b) {
    nar_runtime_t rt = ((rt_and_fn *) data)->rt;
    nar_object_t fn = ((rt_and_fn *) data)->cmp_fn;
    nar_object_t value_a = nar->apply_func(rt, fn, 1, (nar_object_t *) a);
    nar_object_t value_b = nar->apply_func(rt, fn, 1, (nar_object_t *) b);
    return (int) cmp(rt, value_a, value_b);
}

nar_object_t list_sortBy(nar_runtime_t rt, nar_object_t fn, nar_object_t xs) {
    nar_list_t xs_ = nar->to_list(rt, xs);
    rt_and_fn data = {rt, fn};
    qsort_r(xs_.items, xs_.size, sizeof(nar_object_t), &data, &by_fn);
    return nar->new_list(rt, xs_.size, xs_.items);
}

void register_list(nar_runtime_t rt) {
    nar_string_t module_name = "Nar.Base.List";
    nar->register_def_dynamic(rt, module_name, "cons", "list_cons", 2);
    nar->register_def_dynamic(rt, module_name, "map2", "list_map2", 3);
    nar->register_def_dynamic(rt, module_name, "map3", "list_map3", 4);
    nar->register_def_dynamic(rt, module_name, "map4", "list_map4", 5);
    nar->register_def_dynamic(rt, module_name, "map5", "list_map5", 6);
    nar->register_def_dynamic(rt, module_name, "sortWith", "list_sortWith", 2);
    nar->register_def_dynamic(rt, module_name, "sortBy", "list_sortBy", 2);
}
