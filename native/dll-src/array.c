#include <memory.h>
#include "_package.h"
#include "utils.h"

typedef struct {
    nar_size_t size;
    nar_object_t *items;
} chunk_t;

chunk_t *alloc_chunk(nar_runtime_t rt, nar_size_t size) {
    nar_size_t mem_size = sizeof(chunk_t) + size * sizeof(nar_object_t);
    chunk_t *chunk = nar->alloc(rt, mem_size);
    chunk->size = size;
    if (size > 0) {
        chunk->items = (nar_object_t *) (chunk + 1);
        memset(chunk->items, 0, size * sizeof(nar_object_t));
    } else {
        chunk->items = NULL;
    }
    return chunk;
}

nar_int_t cmp_chunks(nar_runtime_t rt, void *pa, void *pb) {
    chunk_t *a = pa;
    chunk_t *b = pb;
    if (a->size < b->size) {
        return -1;
    } else if (a->size > b->size) {
        return 1;
    }
    for (nar_size_t i = 0; i < a->size; i++) {
        nar_int_t c = cmp(rt, a->items[i], b->items[i]);
        if (c != 0) {
            return c;
        }
    }
    return 0;
}

nar_object_t chunk_to_object(nar_runtime_t rt, chunk_t *chunk) {
    return nar->new_native(rt, chunk, &cmp_chunks);
}

chunk_t *object_to_chunk(nar_runtime_t rt, nar_object_t array) {
    return (chunk_t *)nar->to_native(rt, array).ptr;
}

nar_object_t array_empty(nar_runtime_t rt) {
    return chunk_to_object(rt, alloc_chunk(rt, 0));
}

nar_object_t array_singleton(nar_runtime_t rt, nar_object_t item) {
    chunk_t *chunk = alloc_chunk(rt, 1);
    chunk->items[0] = item;
    return chunk_to_object(rt, chunk);
}

nar_object_t array_length(nar_runtime_t rt, nar_object_t array) {
    chunk_t *chunk = object_to_chunk(rt, array);
    return nar->new_int(rt, (nar_int_t) chunk->size);
}

nar_object_t array_initialize(nar_runtime_t rt, nar_object_t size, nar_object_t offset, nar_object_t func) {
    nar_int_t size_ = nar->to_int(rt, size);
    nar_int_t offset_ = nar->to_int(rt, offset);
    if (size_ <= 0) {
        return array_empty(rt);
    }
    chunk_t *chunk = alloc_chunk(rt, (nar_size_t) size_);
    for (nar_int_t i = 0; i < size_; i++) {
        nar_object_t arg = nar->new_int(rt, offset_ + i);
        chunk->items[i] = nar->apply_func(rt, func, 1, &arg);
    }
    return chunk_to_object(rt, chunk);
}

nar_object_t array_initializeFromList(nar_runtime_t rt, nar_object_t max, nar_object_t ls) {
    nar_int_t max_ = nar->to_int(rt, max);
    nar_list_t ls_ = nar->to_list(rt, ls);
    if (max_ <= 0) {
        nar_object_t arg[2] = {array_empty(rt), ls};
        return nar->new_tuple(rt, 2, arg);
    }
    chunk_t *chunk = alloc_chunk(rt, ls_.size < max_ ? ls_.size : (nar_size_t) max_);
    memcpy(chunk->items, ls_.items, chunk->size * sizeof(nar_object_t));
    nar_size_t rest_size = ls_.size - chunk->size;
    nar_object_t rest;
    if (rest_size == 0) {
        rest = nar->new_list(rt, 0, NULL);
    } else {
        rest = nar->new_list(rt, rest_size, ls_.items + chunk->size);
    }
    return nar->new_tuple(rt, 2, (nar_object_t[]) {chunk_to_object(rt, chunk), rest});
}

nar_object_t array_unsafeGet(nar_runtime_t rt, nar_object_t index, nar_object_t array) {
    nar_int_t index_ = nar->to_int(rt, index);
    chunk_t *chunk = object_to_chunk(rt, array);
    //TODO: remove check
    if (index_ < 0 || index_ >= chunk->size) {
        nar->fail(rt, L"Nar.Base.NativeArray.unsafeGet: index out of bounds");
        return nar->new_unit(rt);
    }
    return chunk->items[index_];
}

nar_object_t array_unsafeSet(nar_runtime_t rt, nar_object_t index, nar_object_t value, nar_object_t array) {
    nar_int_t index_ = nar->to_int(rt, index);
    chunk_t *chunk = object_to_chunk(rt, array);
    //TODO: remove check
    if (index_ < 0 || index_ >= chunk->size) {
        nar->fail(rt, L"Nar.Base.NativeArray.unsafeSet: index out of bounds");
        return nar->new_unit(rt);
    }
    chunk_t *result = alloc_chunk(rt, chunk->size);
    memcpy(result->items, chunk->items, chunk->size * sizeof(nar_object_t));
    result->items[index_] = value;
    return chunk_to_object(rt, result);
}

nar_object_t array_push(nar_runtime_t rt, nar_object_t value, nar_object_t array) {
    chunk_t *chunk = object_to_chunk(rt, array);
    chunk_t *result = alloc_chunk(rt, chunk->size + 1);
    memcpy(result->items, chunk->items, chunk->size * sizeof(nar_object_t));
    result->items[chunk->size] = value;
    return chunk_to_object(rt, result);
}

nar_object_t array_foldl(nar_runtime_t rt, nar_object_t func, nar_object_t acc, nar_object_t array) {
    chunk_t *chunk = object_to_chunk(rt, array);
    for (nar_size_t i = 0; i < chunk->size; i++) {
        nar_object_t args[2] = {chunk->items[i], acc};
        acc = nar->apply_func(rt, func, 2, args);
    }
    return acc;
}

nar_object_t array_foldr(nar_runtime_t rt, nar_object_t func, nar_object_t acc, nar_object_t array) {
    chunk_t *chunk = object_to_chunk(rt, array);
    for (nar_size_t i = chunk->size; i > 0;) {
        nar_object_t args[2] = {chunk->items[--i], acc};
        acc = nar->apply_func(rt, func, 2, args);
    }
    return acc;
}

nar_object_t array_map(nar_runtime_t rt, nar_object_t func, nar_object_t array) {
    chunk_t *chunk = object_to_chunk(rt, array);
    chunk_t *result = alloc_chunk(rt, chunk->size);
    for (nar_size_t i = 0; i < chunk->size; i++) {
        result->items[i] = nar->apply_func(rt, func, 1, chunk->items + i);
    }
    return chunk_to_object(rt, result);
}

nar_object_t array_indexedMap(nar_runtime_t rt, nar_object_t func, nar_object_t offset, nar_object_t array) {
    nar_int_t offset_ = nar->to_int(rt, offset);
    chunk_t *chunk = object_to_chunk(rt, array);
    chunk_t *result = alloc_chunk(rt, chunk->size);
    for (nar_int_t i = 0; i < chunk->size; i++) {
        nar_object_t args[2] = {nar->new_int(rt, offset_ + i), chunk->items[i]};
        result->items[i] = nar->apply_func(rt, func, 2, args);
    }
    return chunk_to_object(rt, result);
}

nar_object_t array_slice(nar_runtime_t rt, nar_object_t from, nar_object_t to, nar_object_t array) {
    nar_int_t from_ = nar->to_int(rt, from);
    nar_int_t to_ = nar->to_int(rt, to);
    chunk_t *chunk = object_to_chunk(rt, array);
    nar_int_t size = (nar_int_t)chunk->size;

    if ((-size <= from_) && (from_ < 0)) {
        from_ += size;
    }
    if (from_ < -size) {
        from_ = 0;
    }
    if (from_ > size) {
        return array_empty(rt);
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
        return array_empty(rt);
    }
    chunk_t *result = alloc_chunk(rt, to_ - from_);
    memcpy(result->items, chunk->items + from_, result->size * sizeof(nar_object_t));
    return chunk_to_object(rt, result);
}

nar_object_t array_appendN(nar_runtime_t rt, nar_object_t n, nar_object_t dest, nar_object_t source) {
    nar_int_t n_ = nar->to_int(rt, n);
    chunk_t *dest_ = object_to_chunk(rt, dest);
    chunk_t *source_ = object_to_chunk(rt, source);
    nar_size_t dest_len = dest_->size;

    nar_size_t items_to_copy = n_ - dest_len;
    if (items_to_copy > source_->size) {
        items_to_copy = source_->size;
    }

    nar_size_t size = dest_len + items_to_copy;
    chunk_t *result = alloc_chunk(rt, size);
    memcpy(result->items, dest_->items, dest_len * sizeof(nar_object_t));
    memcpy(result->items + dest_len, source_->items, items_to_copy * sizeof(nar_object_t));
    return chunk_to_object(rt, result);
}

void register_array(nar_runtime_t rt) {
    nar_string_t module_name = L"Nar.Base.NativeArray";
    nar->register_def(rt, module_name, L"empty", nar->new_func(rt, &array_empty, 0));
    nar->register_def(rt, module_name, L"singleton", nar->new_func(rt, &array_singleton, 1));
    nar->register_def(rt, module_name, L"length", nar->new_func(rt, &array_length, 1));
    nar->register_def(rt, module_name, L"initialize", nar->new_func(rt, &array_initialize, 3));
    nar->register_def(rt, module_name, L"initializeFromList", nar->new_func(rt, &array_initializeFromList, 2));
    nar->register_def(rt, module_name, L"unsafeGet", nar->new_func(rt, &array_unsafeGet, 2));
    nar->register_def(rt, module_name, L"unsafeSet", nar->new_func(rt, &array_unsafeSet, 3));
    nar->register_def(rt, module_name, L"push", nar->new_func(rt, &array_push, 2));
    nar->register_def(rt, module_name, L"foldl", nar->new_func(rt, &array_foldl, 3));
    nar->register_def(rt, module_name, L"foldr", nar->new_func(rt, &array_foldr, 3));
    nar->register_def(rt, module_name, L"map", nar->new_func(rt, &array_map, 2));
    nar->register_def(rt, module_name, L"indexedMap", nar->new_func(rt, &array_indexedMap, 3));
    nar->register_def(rt, module_name, L"slice", nar->new_func(rt, &array_slice, 3));
    nar->register_def(rt, module_name, L"appendN", nar->new_func(rt, &array_appendN, 3));
}
