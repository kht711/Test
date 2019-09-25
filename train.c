#include <stdio.h>

#define RED 0
#define GREEN 1

struct block_info{
    struct train_info* train;
    struct block_info* next[2];
    int signal;
};

struct train_info{
    int number;
    struct block_info* dest;
};

void print_block(struct block_info blocks[]){
    char bi[9]; // block_info
    char si[9]; // signal_info
    
    int i;
    struct block_info* block;
    
    for (i = 0; i < 9; i++){
        block = &blocks[i];
        if (block->train == NULL){
            bi[i] = '\0';
        }else{
            bi[i] = '0' + block->train->number;
        }
    }
    
    for (i = 0; i < 9; i++){
        block = &blocks[i];
        if (block->signal == RED){
            si[i] = 'R';
        }else{
            si[i] = 'G';
        }
    }
    
    printf("                                    %c\n", bi[5]);
    printf("                              %c ┌ ─ ─ ─ ┤\n", si[5]);
    printf("  %c %c                       %c   │ %c %c\n", si[0], bi[0], bi[4], si[6], bi[6]);
    printf("├ ─ ─ ─ ┐             %c ┌ ─ ─ ─ ┼ ─ ─ ─ ┤\n", si[4]);
    printf("  %c %c   │ %c %c     %c %c   │ %c %c     %c %c\n", si[1], bi[1], si[2], bi[2], si[3], bi[3], si[7], bi[7], si[8], bi[8]);
    printf("├ ─ ─ ─ ┼ ─ ─ ─ ┼ ─ ─ ─ ┼ ─ ─ ─ ┼ ─ ─ ─ ┤\n");
}

void set_signals(struct block_info blocks[], int nblocks){
    int i;
    struct block_info* block;
    for (i = 0; i < nblocks; i++){
        block = &blocks[i];
        if (block->train == NULL){
            block->signal = GREEN;
        }else{
            block->signal = RED;
        }
    }
    
    print_block(blocks);
}

int find_block(struct block_info* block, struct block_info* dest){
    int i;
    if (block == dest){
        return 1;
    }
    for (i = 0; i < 2; i++){
        if (block->next[i] == NULL){
            continue;
        }
        if (find_block(block->next[i], dest) == 1){
            return 1;
        }
    }
    return 0;
}

void proceed(struct block_info blocks[], int nblocks){
    int i, j;
    struct block_info* block;
    for (i = nblocks - 1; i >= 0; i--){
        block = &blocks[i];
        if (block->train == NULL){
            continue;
        }
        if (block == block->train->dest){
            block->train = NULL;
            continue;
        }
        for (j = 0; j < 2; j++){
            if (block->next[j] == NULL){
                continue;
            }
            if (block->next[j]->signal == RED){
                continue;
            }
            if (find_block(block->next[j], block->train->dest) == 1){
                block->next[j]->train = block->train;
                block->next[j]->signal = RED;
                block->train = NULL;
                break;
            }
        }
    }
}

int main(void){
    struct block_info blocks[9];
    struct train_info trains[5];
    
    blocks[0].train = &trains[4];
    blocks[0].next[0] = &blocks[2];
    blocks[0].next[1] = NULL;
    blocks[0].signal = RED;
    
    blocks[1].train = &trains[3];
    blocks[1].next[0] = &blocks[2];
    blocks[1].next[1] = NULL;
    blocks[1].signal = RED;
    
    blocks[2].train = NULL;
    blocks[2].next[0] = &blocks[3];
    blocks[2].next[1] = NULL;
    blocks[2].signal = GREEN;
    
    blocks[3].train = &trains[2];
    blocks[3].next[0] = &blocks[4];
    blocks[3].next[1] = &blocks[7];
    blocks[3].signal = RED;
    
    blocks[4].train = &trains[1];
    blocks[4].next[0] = &blocks[5];
    blocks[4].next[1] = &blocks[6];
    blocks[4].signal = RED;
    
    blocks[5].train = NULL;
    blocks[5].next[0] = NULL;
    blocks[5].next[1] = NULL;
    blocks[5].signal = GREEN;
    
    blocks[6].train = NULL;
    blocks[6].next[0] = NULL;
    blocks[6].next[1] = NULL;
    blocks[6].signal = GREEN;
    
    blocks[7].train = NULL;
    blocks[7].next[0] = &blocks[8];
    blocks[7].next[1] = NULL;
    blocks[7].signal = GREEN;
    
    blocks[8].train = &trains[0];
    blocks[8].next[0] = NULL;
    blocks[8].next[1] = NULL;
    blocks[8].signal = RED;
    
    //----------------------------------
    
    trains[0].number = 0;
    trains[0].dest = &blocks[8];
    
    trains[1].number = 1;
    trains[1].dest = &blocks[5];
    
    trains[2].number = 2;
    trains[2].dest = &blocks[6];
    
    trains[3].number = 3;
    trains[3].dest = &blocks[5];
    
    trains[4].number = 4;
    trains[4].dest = &blocks[8];
    
    int nblocks = 9;
    print_block(blocks);
    
    int i;
    set_signals(blocks, nblocks);
    for (i = 0; i < 4; i++){
        proceed(blocks, nblocks);
        set_signals(blocks, nblocks);
    }
}
