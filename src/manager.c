#include "helpers.h"
#include "manager.h"

int physical_memory[PHYSICAL_MEMORY_SIZE];
int disk[FRAME_SIZE][PAGE_SIZE];
int free_frames[FRAME_SIZE];
int free_frames_count;

void init(const char* init_file){
    FILE* file = fopen(init_file, "r");
    
    // initialize all physical memory to 0
    int i;
    for (i = 0; i < PHYSICAL_MEMORY_SIZE; i++){
        physical_memory[i] = 0;
    }

    // initialize disk array
    int j;
    for (i = 0; i < FRAME_SIZE; i++){
        for (j = 0; j < PAGE_SIZE; j++){
            disk[i][j] = 0;
        }
    }

    // reserve fram 0 and 1 for segment table
    free_frames_count = FRAME_SIZE - 2;
    for (i = 0; i < free_frames_count; i++){
        free_frames[i] = i + 2;
    }

    char line[4096];
    if (fgets(line, sizeof(line), file) == NULL){
        exit(1);
    }

    char* token = strtok(line, "\t\n");
    while (token != NULL){
        int segment = atoi(token);

        token = strtok(NULL, " \t\n");
        if (token == NULL) break;

        int size = atoi(token);
        token = strtok(NULL, " \t\n");
        if (token == NULL) break;

        int frame_or_block = atoi(token);

        physical_memory[2 * segment] = size; // size of segment
        physical_memory[2 * segment + 1] = frame_or_block; // frame or disk block

        token = strtok(NULL, " \t\n");
    }

    if (fgets(line, sizeof(line), file) == NULL){
        fclose(file);
        return;
    }

    token = strtok(line, "\t\n");
    while (token != NULL){
        int segment = atoi(token);

        token = strtok(NULL, " \t\n");
        if (token == NULL) break;
        int page = atoi(token);

        token = strtok(NULL, " \t\n");
        if (token == NULL) break;
        int frame_or_block = atoi(token);

        int page_table_frame_or_block = physical_memory[2 * segment + 1];

        if (page_table_frame_or_block >= 0){
            physical_memory[page_table_frame_or_block * PAGE_SIZE + page];
        } else {
            disk[abs(page_table_frame_or_block)][page] = frame_or_block;
        }

        token = strtok(NULL, " \t\n");
    }
    fclose(file);
}


int translate_address(uint32_t va);
void process_addresses(const char* va_file, const char* output_file);

int allocate_free_frame(){
    if (free_frames_count <= 0){
        exit(1);
    }
    return free_frames[--free_frames_count];
}

void read_block(int block, int frame_start){
    int i;
    for (i = 0; i < PAGE_SIZE; i++){
        physical_memory[frame_start + i] = disk[block][i];
    }
}

int main(int argc, char* argv[]){
    return 1;
}