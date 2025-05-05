#include "helpers.h"
#include "manager.h"

// extract segment number (s)
int get_segment(uint32_t va){
    // right shift  by 18 and keep lowest 9 bits
    return (va >> 18) & 0x1FF;
}

// extract page number (p)
int get_page(uint32_t va){
    // right shift  by 9 and keep lowest 9 bits
    return (va >> 9) & 0x1FF;
}

// extract offset (w)
int get_offset(uint32_t va){
    // keep lowest 9 bits
    return va & 0x1FF;
}

// extract page offset (pw)
int get_page_offset(uint32_t va){
    // keep lowest 18 bits
    return va & 0x3FFFF;
}