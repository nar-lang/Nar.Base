#include <memory.h>
#include "native.h"
#include "utils.h"

typedef struct {
    nar_size_t size;
    nar_object_t *items;
} chunk_t;

chunk_t *alloc_chunk(nar_size_t size) {
    nar_size_t mem_size = sizeof(chunk_t) + size * sizeof(nar_object_t);
    chunk_t *chunk = nar_alloc(mem_size);
    chunk->size = size;
    if (size > 0) {
        chunk->items = (nar_object_t *) (chunk + 1);
        memset(chunk->items, 0, size * sizeof(nar_object_t));
    } else {
        chunk->items = NULL;
    }
    return chunk;
}

nar_int_t cmp_chunks(void *pa, void *pb) {
    chunk_t *a = pa;
    chunk_t *b = pb;
    if (a->size < b->size) {
        return -1;
    } else if (a->size > b->size) {
        return 1;
    }
    for (nar_size_t i = 0; i < a->size; i++) {
        nar_int_t c = cmp(a->items[i], b->items[i]);
        if (c != 0) {
            return c;
        }
    }
    return 0;
}

nar_object_t chunk_to_object(chunk_t *chunk) {
    return nar_native(chunk, &cmp_chunks);
}

chunk_t *object_to_chunk(nar_object_t array) {
    return (chunk_t *) nar_to_native(array).ptr;
}

nar_object_t array_empty() {
    return chunk_to_object(alloc_chunk(0));
}

nar_object_t array_singleton(nar_object_t item) {
    chunk_t *chunk = alloc_chunk(1);
    chunk->items[0] = item;
    return chunk_to_object(chunk);
}

nar_object_t array_length(nar_object_t array) {
    chunk_t *chunk = object_to_chunk(array);
    return nar_int((nar_int_t) chunk->size);
}

nar_object_t array_initialize(nar_object_t size, nar_object_t offset, nar_object_t func) {
    nar_int_t size_ = nar_to_int(size);
    nar_int_t offset_ = nar_to_int(offset);
    if (size_ <= 0) {
        return array_empty();
    }
    chunk_t *chunk = alloc_chunk((nar_size_t) size_);
    for (nar_int_t i = 0; i < size_; i++) {
        nar_object_t arg = nar_int(offset_ + i);
        chunk->items[i] = nar_apply(func, 1, &arg);
    }
    return chunk_to_object(chunk);
}

nar_object_t array_initializeFromList(nar_object_t max, nar_object_t ls) {
    nar_int_t max_ = nar_to_int(max);
    nar_list_t ls_ = nar_to_list(ls);
    if (max_ <= 0) {
        nar_object_t arg[2] = {array_empty(), ls};
        return nar_tuple(2, arg);
    }
    chunk_t *chunk = alloc_chunk(ls_.size < max_ ? ls_.size : (nar_size_t) max_);
    memcpy(chunk->items, ls_.items, chunk->size * sizeof(nar_object_t));
    nar_size_t rest_size = ls_.size - chunk->size;
    nar_object_t rest;
    if (rest_size == 0) {
        rest = nar_list(0, NULL);
    } else {
        rest = nar_list(rest_size, ls_.items + chunk->size);
    }
    return nar_tuple(2, (nar_object_t[]) {chunk_to_object(chunk), rest});
}

nar_object_t array_unsafeGet(nar_object_t index, nar_object_t array) {
    nar_int_t index_ = nar_to_int(index);
    chunk_t *chunk = object_to_chunk(array);
    if (index_ < 0 || index_ >= chunk->size) {
        return nar_fail(L"Nar.Base.NativeArray.unsafeGet: index out of bounds");
    }
    return chunk->items[index_];
}

nar_object_t array_unsafeSet(nar_object_t index, nar_object_t value, nar_object_t array) {
    nar_int_t index_ = nar_to_int(index);
    chunk_t *chunk = object_to_chunk(array);
    if (index_ < 0 || index_ >= chunk->size) {
        return nar_fail(L"Nar.Base.NativeArray.unsafeSet: index out of bounds");
    }
    chunk_t *result = alloc_chunk(chunk->size);
    memcpy(result->items, chunk->items, chunk->size * sizeof(nar_object_t));
    result->items[index_] = value;
    return chunk_to_object(result);
}

nar_object_t array_push(nar_object_t value, nar_object_t array) {
    chunk_t *chunk = object_to_chunk(array);
    chunk_t *result = alloc_chunk(chunk->size + 1);
    memcpy(result->items, chunk->items, chunk->size * sizeof(nar_object_t));
    result->items[chunk->size] = value;
    return chunk_to_object(result);
}

nar_object_t array_foldl(nar_object_t func, nar_object_t acc, nar_object_t array) {
    chunk_t *chunk = object_to_chunk(array);
    for (nar_size_t i = 0; i < chunk->size; i++) {
        nar_object_t args[2] = {chunk->items[i], acc};
        acc = nar_apply(func, 2, args);
    }
    return acc;
}

nar_object_t array_foldr(nar_object_t func, nar_object_t acc, nar_object_t array) {
    chunk_t *chunk = object_to_chunk(array);
    for (nar_size_t i = chunk->size; i > 0;) {
        nar_object_t args[2] = {chunk->items[--i], acc};
        acc = nar_apply(func, 2, args);
    }
    return acc;
}

nar_object_t array_map(nar_object_t func, nar_object_t array) {
    chunk_t *chunk = object_to_chunk(array);
    chunk_t *result = alloc_chunk(chunk->size);
    for (nar_size_t i = 0; i < chunk->size; i++) {
        result->items[i] = nar_apply(func, 1, chunk->items + i);
    }
    return chunk_to_object(result);
}

nar_object_t array_indexedMap(nar_object_t func, nar_object_t offset, nar_object_t array) {
    nar_int_t offset_ = nar_to_int(offset);
    chunk_t *chunk = object_to_chunk(array);
    chunk_t *result = alloc_chunk(chunk->size);
    for (nar_int_t i = 0; i < chunk->size; i++) {
        nar_object_t args[2] = {nar_int(offset_ + i), chunk->items[i]};
        result->items[i] = nar_apply(func, 2, args);
    }
    return chunk_to_object(result);
}

nar_object_t array_slice(nar_object_t from, nar_object_t to, nar_object_t array) {
    nar_int_t from_ = nar_to_int(from);
    nar_int_t to_ = nar_to_int(to);
    chunk_t *chunk = object_to_chunk(array);
    nar_int_t size = (nar_int_t)chunk->size;

    if ((-size <= from_) && (from_ < 0)) {
        from_ += size;
    }
    if (from_ < -size) {
        from_ = 0;
    }
    if (from_ > size) {
        return array_empty();
    }

    if ((-size <= to_) && (to_ < 0)) {
        to_ += (nar_int_t)size;
    }
    if (to_ < -size) {
        to_ = 0;
    }
    if (to_ > size) {
        to_ = size;
    }
    if (to_ == from_) {
        return array_empty();
    }
    chunk_t *result = alloc_chunk(to_ - from_);
    memcpy(result->items, chunk->items + from_, result->size * sizeof(nar_object_t));
    return chunk_to_object(result);
}

nar_object_t array_appendN(nar_object_t n, nar_object_t dest, nar_object_t source) {
    nar_int_t n_ = nar_to_int(n);
    chunk_t *dest_ = object_to_chunk(dest);
    chunk_t *source_ = object_to_chunk(source);
    nar_size_t dest_len = dest_->size;

    nar_size_t items_to_copy = n_ - dest_len;
    if (items_to_copy > source_->size) {
        items_to_copy = source_->size;
    }

    nar_size_t size = dest_len + items_to_copy;
    chunk_t *result = alloc_chunk(size);
    memcpy(result->items, dest_->items, dest_len * sizeof(nar_object_t));
    memcpy(result->items + dest_len, source_->items, items_to_copy * sizeof(nar_object_t));
    return chunk_to_object(result);
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
