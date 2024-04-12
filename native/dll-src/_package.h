#ifndef NAR_BASE_NATIVE_H
#define NAR_BASE_NATIVE_H

#include <math.h>
#include <memory.h>
#include <nar-package.h>
#include <nar.h>
#include <stdbool.h>
#include <vector.h>
#include <wchar.h>

nar_t *nar;

void register_array(nar_runtime_t rt);
void register_basics(nar_runtime_t rt);
void register_bitwise(nar_runtime_t rt);
void register_char(nar_runtime_t rt);
void register_debug(nar_runtime_t rt);
void register_list(nar_runtime_t rt);
void register_math(nar_runtime_t rt);
void register_string(nar_runtime_t rt);

nar_int_t cmp(nar_runtime_t rt, nar_object_t a, nar_object_t b);
nar_object_t stringify(nar_runtime_t rt, nar_object_t a);

#endif //NAR_BASE_NATIVE_H
