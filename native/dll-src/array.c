#include <memory.h>
#include "native.h"

nar_object_t array_empty() {
    return nar_native(NULL);
}

nar_object_t array_singleton(nar_object_t item) {
    void *result = nar_alloc(sizeof(nar_size_t) + sizeof(nar_object_t));
    *(nar_size_t *) result = 1;
    *(nar_object_t *) (result + sizeof(nar_size_t)) = item;
    return nar_native(result);
}

nar_object_t array_length(nar_object_t array) {
    nar_native_t *ptr = nar_to_native(array);
    if (ptr == NULL) {
        return nar_int(0);
    }
    return nar_int((nar_int_t) *(nar_size_t *) ptr);
}

nar_object_t array_initialize(nar_object_t size, nar_object_t offset, nar_object_t func) {
    nar_int_t size_value = nar_to_int(size);
    nar_int_t offset_value = nar_to_int(offset);
    if (size_value <= 0) {
        return array_empty();
    }
    void *result = nar_alloc(sizeof(nar_size_t) + size_value * sizeof(nar_object_t));
    *(nar_size_t *) result = size_value;
    nar_object_t *ptr = (nar_object_t *) (result + sizeof(nar_size_t));
    for (nar_size_t i = 0; i < size_value; i++) {
        nar_object_t arg = nar_int(offset_value + (nar_int_t) i);
        ptr[i] = nar_apply(func, 1, &arg);
    }
    return nar_native(result);
}

nar_object_t array_initializeFromList(nar_object_t max, nar_object_t ls) {
    nar_int_t max_value = nar_to_int(max);
    if (max_value <= 0) {
        nar_object_t arg[2] = {array_empty(), ls};
        return nar_tuple(2, arg);
    }
    nar_list_t ls_ = nar_to_list(ls);
    nar_size_t result_size = ls_.size < max_value ? ls_.size : (nar_size_t) max_value;
    nar_size_t mem_size = sizeof(nar_size_t) + result_size * sizeof(nar_object_t);
    void *result = nar_alloc(mem_size);
    *(nar_size_t *) result = result_size;
    nar_object_t *ptr = (nar_object_t *) (result + sizeof(nar_size_t));
    memcpy(ptr, ls_.items, result_size * sizeof(nar_object_t));
    nar_object_t rest = nar_list(ls_.size - result_size, ls_.items + result_size);
    return nar_tuple(2, (nar_object_t[]) {nar_native(result), rest});
}

nar_object_t array_unsafeGet(nar_object_t index, nar_object_t array) {
    nar_int_t index_value = nar_to_int(index);
    nar_native_t *ptr = nar_to_native(array);
    if (ptr == NULL) {
        return nar_fail(L"Nar.Base.NativeArray.unsafeGet: array is empty");
    }
    nar_size_t size = *(nar_size_t *) ptr;
    if (index_value < 0 || index_value >= size) {
        return nar_fail(L"Nar.Base.NativeArray.unsafeGet: index out of bounds");
    }
    return ((nar_object_t *) (ptr + sizeof(nar_size_t)))[index_value];
}

nar_object_t array_unsafeSet(nar_object_t index, nar_object_t value, nar_object_t array) {
    nar_int_t index_value = nar_to_int(index);
    nar_native_t *ptr = nar_to_native(array);
    if (ptr == NULL) {
        return nar_fail(L"Nar.Base.NativeArray.unsafeSet: array is empty");
    }
    nar_size_t size = *(nar_size_t *) ptr;
    if (index_value < 0 || index_value >= size) {
        return nar_fail(L"Nar.Base.NativeArray.unsafeSet: index out of bounds");
    }
    ((nar_object_t *) (ptr + sizeof(nar_size_t)))[index_value] = value;
    return array;
}

nar_object_t array_push(nar_object_t value, nar_object_t array) {
    nar_native_t *ptr = nar_to_native(array);
    nar_size_t size = ptr == NULL ? 0 : *(nar_size_t *) ptr;
    void *result = nar_alloc(sizeof(nar_size_t) + (size + 1) * sizeof(nar_object_t));
    *(nar_size_t *) result = size + 1;
    if (size > 0) {
        memcpy(result + sizeof(nar_size_t), ptr + sizeof(nar_size_t), size * sizeof(nar_object_t));
    }
    ((nar_object_t *) (result + sizeof(nar_size_t)))[size] = value;
    return nar_native(result);
}

nar_object_t array_foldl(nar_object_t func, nar_object_t acc, nar_object_t array) {
    nar_native_t *ptr = nar_to_native(array);
    if (ptr == NULL) {
        return acc;
    }
    nar_size_t size = *(nar_size_t *) ptr;
    nar_object_t *values = (nar_object_t *) (ptr + sizeof(nar_size_t));
    for (nar_size_t i = 0; i < size; i++) {
        nar_object_t args[2] = {values[i], acc};
        acc = nar_apply(func, 2, args);
    }
    return acc;
}

nar_object_t array_foldr(nar_object_t func, nar_object_t acc, nar_object_t array) {
    nar_native_t *ptr = nar_to_native(array);
    if (ptr == NULL) {
        return acc;
    }
    nar_size_t size = *(nar_size_t *) ptr;
    nar_object_t *values = (nar_object_t *) (ptr + sizeof(nar_size_t));
    for (nar_size_t i = size; i > 0; i--) {
        nar_object_t args[2] = {values[i - 1], acc};
        acc = nar_apply(func, 2, args);
    }
    return acc;
}

nar_object_t array_map(nar_object_t func, nar_object_t array) {
    nar_native_t *ptr = nar_to_native(array);
    if (ptr == NULL) {
        return array_empty();
    }
    nar_size_t size = *(nar_size_t *) ptr;
    nar_object_t *values = (nar_object_t *) (ptr + sizeof(nar_size_t));
    void *result = nar_alloc(sizeof(nar_size_t) + size * sizeof(nar_object_t));
    *(nar_size_t *) result = size;
    nar_object_t *result_values = (nar_object_t *) (result + sizeof(nar_size_t));
    for (nar_size_t i = 0; i < size; i++) {
        result_values[i] = nar_apply(func, 1, &values[i]);
    }
    return nar_native(result);
}

nar_object_t array_indexedMap(nar_object_t func, nar_object_t offset, nar_object_t array) {
    nar_int_t offset_value = nar_to_int(offset);
    nar_native_t *ptr = nar_to_native(array);
    if (ptr == NULL) {
        return array_empty();
    }
    nar_size_t size = *(nar_size_t *) ptr;
    nar_object_t *values = (nar_object_t *) (ptr + sizeof(nar_size_t));
    void *result = nar_alloc(sizeof(nar_size_t) + size * sizeof(nar_object_t));
    *(nar_size_t *) result = size;
    nar_object_t *result_values = (nar_object_t *) (result + sizeof(nar_size_t));
    for (nar_size_t i = 0; i < size; i++) {
        nar_object_t args[2] = {nar_int(offset_value + (nar_int_t) i), values[i]};
        result_values[i] = nar_apply(func, 2, args);
    }
    return nar_native(result);
}

nar_object_t array_slice(nar_object_t from, nar_object_t to, nar_object_t array) {
    nar_int_t from_value = nar_to_int(from);
    nar_int_t to_value = nar_to_int(to);
    nar_native_t *ptr = nar_to_native(array);
    if (ptr == NULL) {
        return array_empty();
    }
    nar_size_t size = *(nar_size_t *) ptr;
    printf(" from: %lld, to: %lld, size: %lld\n", from_value, to_value, size);
    if (from_value < 0) {
        from_value = (nar_int_t) size + from_value;
    }
    if (to_value < 0) {
        to_value = (nar_int_t) size + to_value;
    }
    if (from_value < 0 || from_value > size || to_value < 0 || to_value > size || from_value > to_value) {
        return nar_fail(L"Nar.Base.NativeArray.slice: invalid range");
    }
    printf("`from: %lld, to: %lld, size: %lld\n", from_value, to_value, size);
    nar_size_t len = to_value - from_value;
    void *result = nar_alloc(sizeof(nar_size_t) + len * sizeof(nar_object_t));
    *(nar_size_t *) result = len;
    nar_object_t *result_values = (nar_object_t *) (result + sizeof(nar_size_t));
    memcpy(result_values, (nar_object_t *) (ptr + sizeof(nar_size_t) + (from_value * sizeof(nar_object_t))), len * sizeof(nar_object_t));
    return nar_native(result);
}

nar_object_t array_appendN(nar_object_t n, nar_object_t dest, nar_object_t source) {
    nar_int_t n_value = nar_to_int(n);
    nar_native_t *dest_ptr = nar_to_native(dest);
    if (dest_ptr == NULL) {
        return source;
    }
    nar_size_t dest_size = *(nar_size_t *) dest_ptr;
    nar_native_t *source_ptr = nar_to_native(source);
    if (source_ptr == NULL) {
        return dest;
    }
    nar_size_t source_size = *(nar_size_t *) source_ptr;
    nar_size_t result_size = dest_size + n_value;
    if (result_size > source_size) {
        result_size = source_size;
    }
    void *result = nar_alloc(sizeof(nar_size_t) + result_size * sizeof(nar_object_t));
    *(nar_size_t *) result = result_size;
    nar_object_t *result_values = (nar_object_t *) (result + sizeof(nar_size_t));
    memcpy(result_values, (nar_object_t *) (dest_ptr + sizeof(nar_size_t)), dest_size * sizeof(nar_object_t));
    memcpy(result_values + dest_size, (nar_object_t *) (source_ptr + sizeof(nar_size_t)), result_size * sizeof(nar_object_t));
    return nar_native(result);
}

void register_array(void) {
    nar_string_t module_name = L"Nar.Base.NativeArray";
    nar_register_def(module_name, L"empty", nar_func(&array_empty, 0));
    nar_register_def(module_name, L"singleton", nar_func(&array_singleton, 1));
    nar_register_def(module_name, L"length", nar_func(&array_length, 1));
    nar_register_def(module_name, L"initialize", nar_func(&array_initialize, 3));
    nar_register_def(module_name, L"initializeFromList", nar_func(&array_initializeFromList, 2));
    nar_register_def(module_name, L"unsafeGet", nar_func(&array_unsafeGet, 2));
    nar_register_def(module_name, L"unsafeSet", nar_func(&array_unsafeSet, 3));
    nar_register_def(module_name, L"push", nar_func(&array_push, 2));
    nar_register_def(module_name, L"foldl", nar_func(&array_foldl, 3));
    nar_register_def(module_name, L"foldr", nar_func(&array_foldr, 3));
    nar_register_def(module_name, L"map", nar_func(&array_map, 2));
    nar_register_def(module_name, L"indexedMap", nar_func(&array_indexedMap, 3));
    nar_register_def(module_name, L"slice", nar_func(&array_slice, 3));
    nar_register_def(module_name, L"appendN", nar_func(&array_appendN, 3));
}
