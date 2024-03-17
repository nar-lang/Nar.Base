#ifndef NAR_BASE_NATIVE_H
#define NAR_BASE_NATIVE_H

#include <nar.h>
#include <nar-package.h>

nar_t *nar;

void register_array(nar_runtime_t rt);
void register_basics(nar_runtime_t rt);
void register_bitwise(nar_runtime_t rt);
void register_char(nar_runtime_t rt);
void register_debug(nar_runtime_t rt);
void register_list(nar_runtime_t rt);
void register_math(nar_runtime_t rt);
void register_string(nar_runtime_t rt);

#endif //NAR_BASE_NATIVE_H
