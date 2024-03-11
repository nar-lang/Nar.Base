#ifndef NAR_BASE_UTILS_H
#define NAR_BASE_UTILS_H

#include <nar.h>

#define DEFAULT_STRINGIFY_SIZE 2048

int cmp(nar_object_t a, nar_object_t b);
nar_string_t stringify(nar_object_t a);

#endif //NAR_BASE_UTILS_H
