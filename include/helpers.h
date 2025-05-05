#ifndef HELPERS_H
#define HELPERS_H

#include "manager.h"

int get_segment(uint32_t va);
int get_page(uint32_t va);
int get_offset(uint32_t va);
int get_page_offset(uint32_t va);

#endif