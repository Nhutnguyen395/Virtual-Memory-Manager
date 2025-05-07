#include "helpers.h"
#include "manager.h"

int physical_memory[PHYSICAL_MEMORY_SIZE];
int disk[FRAME_SIZE][PAGE_SIZE];
int free_frames[FRAME_SIZE];
int free_frames_count;

void init(const char* init_file){
    FILE* file = fopen(init_file, "r");
    if (file == NULL){
        exit(1);
    }
    
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

    char* token = strtok(line, " \t\n");
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

        if (frame_or_block > 1){
            for (i = 0; i < free_frames_count; i++){
                if (free_frames[i] == frame_or_block){
                    free_frames[i] = free_frames[free_frames_count - 1];
                    free_frames_count--;
                    break;
                }
            }
        }
        token = strtok(NULL, " \t\n");
    }

    if (fgets(line, sizeof(line), file) == NULL){
        fclose(file);
        return;
    }

    token = strtok(line, " \t\n");
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
            physical_memory[page_table_frame_or_block * PAGE_SIZE + page] = frame_or_block;
        } else {
            disk[abs(page_table_frame_or_block)][page] = frame_or_block;
        }

        if (frame_or_block > 1) {
            for (i = 0; i < free_frames_count; i++) {
                if (free_frames[i] == frame_or_block) {
                    free_frames[i] = free_frames[free_frames_count - 1];
                    free_frames_count--;
                    break;
                }
            }
        }

        token = strtok(NULL, " \t\n");
    }
    fclose(file);
}

int translate_address(uint32_t va){
    int s = get_segment(va);
    int p = get_page(va);
    int w = get_offset(va);
    int pw = get_page_offset(va);

    if (physical_memory[2 * s] == 0 || pw >= physical_memory[2 * s]){
        return -1;
    }

    if (physical_memory[2 * s + 1] < 0){
        int page_table_block = abs(physical_memory[2 * s + 1]);
        int free_frame = allocate_free_frame();
        read_block(page_table_block, free_frame * PAGE_SIZE);
        physical_memory[2 * s + 1] = free_frame;
    }

    int page_table_entry = physical_memory[physical_memory[2 * s + 1] * PAGE_SIZE + p];
    if (page_table_entry < 0){
        int page_block = abs(page_table_entry);
        int free_frame = allocate_free_frame();
        read_block(page_block, free_frame * PAGE_SIZE);
        physical_memory[physical_memory[2 * s + 1] * PAGE_SIZE + p] = free_frame;
    }
    return physical_memory[physical_memory[2 * s + 1] * PAGE_SIZE + p] * PAGE_SIZE + w;
}

void process_addresses(const char* va_file, const char* output_file){
    FILE* input = fopen(va_file, "r");
    FILE* output = fopen(output_file, "w");
    if (input == NULL || output == NULL){
        exit(1);
    }

    uint32_t va;
    int is_first_flag = 1;
    while (fscanf(input, "%u", &va) != EOF){
        int pa = translate_address(va);
        if (is_first_flag){
            fprintf(output, "%d ", pa);
        } else {
            fprintf(output, "%d", pa);
        }
    }
    fclose(input);
    fclose(output);
}

int allocate_free_frame(){
    if (free_frames_count <= 0){
        exit(1);
    }
    
    int frame = free_frames[0];
    int i;
    for(i = 0; i < free_frames_count - 1; i++){
        free_frames[i] = free_frames[i + 1];
    }
    free_frames_count--;
    return frame;
}

void read_block(int block, int frame_start){
    if (block < 0 || block >= FRAME_SIZE) {
        exit(1);
    }

    int i;
    for (i = 0; i < PAGE_SIZE; i++){
        physical_memory[frame_start + i] = disk[block][i];
    }
}

int main(int argc, char* argv[]){
    init(argv[1]);
    process_addresses(argv[2], argv[3]);
    return 0;
}