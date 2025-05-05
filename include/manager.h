#ifndef MANAGER_H
#define MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define PAGE_SIZE 512
#define FRAME_SIZE 1024
#define PHYSICAL_MEMORY_SIZE

extern int physical_memory[PHYSICAL_MEMORY_SIZE];
extern int disk[FRAME_SIZE][PAGE_SIZE];
extern int free_frames[FRAME_SIZE];
extern int free_frames_count;

void init(const char* init_file);
int translate_address(uint32_t va);
void process_addresses(const char* va_file, const char* output_file);
int allocate_free_frame();
void read_block(int block, int frame_start);

#endif