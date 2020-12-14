/*************************************************************************************|
|   1. YOU ARE NOT ALLOWED TO SHARE/PUBLISH YOUR CODE (e.g., post on piazza or online)|
|   2. Fill memory_hierarchy.c                                                        |
|   3. Do not use any other .c files neither alter mipssim.h or parser.h              |
|   4. Do not include any other library files                                         |
|*************************************************************************************/

#include "mipssim.h"

uint32_t cache_type = 0;
         

struct block_direct{
    int32_t array[4];
    int tag ;
    int valid;
};
        
int blocks_number;  
int index_bits;
int number_of_tag_bits;

//cache_type ==1 ->direct mapped
//cache_type ==2 ->fully associative
//cache_type ==3 ->2-way associative


struct block_direct* cache;   



//addresses 32 bits
//Memory byte addressable
//Block size = 16 bytes-> offset bits =4
//cache holds intruction and data
//cache uses write through --------write to both cache and memory
//write no allocate policy ----------dont brong block in cache ->modify it only in memory
//LRu replacement policy
void memory_state_init(struct architectural_state *arch_state_ptr) {
    arch_state_ptr->memory = (uint32_t *) malloc(sizeof(uint32_t) * MEMORY_WORD_NUM);
    memset(arch_state_ptr->memory, 0, sizeof(uint32_t) * MEMORY_WORD_NUM);
    if (cache_size == 0) {
        // CACHE DISABLED
        memory_stats_init(arch_state_ptr, 0); // WARNING: we initialize for no cache 0
    } else {
        // CACHE ENABLED
         /// @students: remove assert and initialize cache
        
        /// @students: memory_stats_init(arch_state_ptr, X); <-- fill # of tag bits for cache 'X' correctly
        
        switch(cache_type) {
            case CACHE_TYPE_DIRECT: // direct mapped
                blocks_number = cache_size/16;
                index_bits = log2(blocks_number);
                int number_of_offsetbits = 4;
                int number_of_tag_bits = 32-4-index_bits;
                memory_stats_init(arch_state_ptr ,number_of_tag_bits);
                cache = (struct block_direct*) calloc(blocks_number ,sizeof(struct block_direct));
                printf("number of tag = %u \n", number_of_tag_bits);
                printf("number of index bits = %u \n" , index_bits);
                printf("size of cache : %i\n" , sizeof(cache));
                printf("%i" , cache[3].tag);

                break;

            case CACHE_TYPE_FULLY_ASSOC: // fully associative
                break;
            case CACHE_TYPE_2_WAY: // 2-way associative
                break;
        }
    }
}

// returns data on memory[address / 4]
int memory_read(int address){
    int i =1;
    uint32_t block_in_memory = (address>>4)<<2;
    arch_state.mem_stats.lw_total++;
    check_address_is_word_aligned(address);
    //printf("entered the memory read command\n");
    int word_number_in_block = (address>>28)/4;
    int address_index = get_piece_of_a_word(address , 4 , index_bits);
    int address_tag = get_piece_of_a_word(address , (4+index_bits) , number_of_tag_bits);
    if (cache_size == 0) {
        // CACHE DISABLED
        return (int) arch_state.memory[address / 4];
        //printf("entered cache size equals 0\n");
    } else {
        // CACHE ENABLED
        /// @students: Remove assert(0); and implement Memory hierarchy w/ cache
        
        /// @students: your implementation must properly increment: arch_state_ptr->mem_stats.lw_cache_hits


        
        switch(cache_type) {
        case CACHE_TYPE_DIRECT: // direct mapped
            printf("entered the case of type direct\n");
            printf("address tag :%i\n" , address_tag);
            printf("address : %i\n" , address);
            printf("address_index : %i\n" , address_index);
            printf("caaches tag : %i\n" , cache[0].tag);
        
            if (address_tag == cache[address_index].tag && cache[address_index].valid ==1)
            {
                //printf("i got in the if statement\n");
                arch_state.mem_stats.lw_cache_hits ++;
                
                 switch(word_number_in_block){
                     case 0:
                         return cache[address_index].array[0];
                         printf("%i" , cache[address_index].array[0]);
                         break;
                     case 1:
                         return cache[address_index].array[1];
                         printf("%i" , cache[address_index].array[1]);
                         break;
                     case 2:
                         return cache[address_index].array[2];
                         printf("%i" , cache[address_index].array[2]);
                         break;
                     case 3:
                         return cache[address_index].array[3];
                         printf("%i" , cache[address_index].array[3]);
                         break;
                /* code */
            }
            }else{
                 printf("just entered the else statement\n");
                 cache[address_index].array[0]=arch_state.memory[block_in_memory];
                 cache[address_index].array[1]=arch_state.memory[block_in_memory+4];
                 cache[address_index].array[2]=arch_state.memory[block_in_memory+8];
                 cache[address_index].array[3]=arch_state.memory[block_in_memory+12];

                
                
                
                 cache[address_index].valid = 1;
                 cache[address_index].tag = address_tag;
                 printf("block added \n number 0 : %i\n number 1 : %i \n number2 : %i \n number3 : %i\n" , cache[address_index].array[0],cache[address_index].array[1],cache[address_index].array[2],cache[address_index].array[3]);
                 //printf("in memory second integer which must be 0 : %i \n" , arch_state.memory[4]);
                 //printf("in memory second integer which must be 1280000 : %i \n" , arch_state.memory[0]);
                 return arch_state.memory[address/4];
                 
            }
            
            
                
               
            printf("went out\n ");
            

            break;
        case CACHE_TYPE_FULLY_ASSOC: // fully associative
            break;
        case CACHE_TYPE_2_WAY: // 2-way associative
            break;
        }
    }
    printf("went out of cases");
    
}

// writes data on memory[address / 4]
void memory_write(int address, int write_data) {
    arch_state.mem_stats.sw_total++;
    check_address_is_word_aligned(address);

    if (cache_size == 0) {
        // CACHE DISABLED
        arch_state.memory[address / 4] = (uint32_t) write_data;
    } else {
        // CACHE ENABLED
        assert(0); /// @students: Remove assert(0); and implement Memory hierarchy w/ cache
        
        /// @students: your implementation must properly increment: arch_state_ptr->mem_stats.sw_cache_hits
        
        switch(cache_type) {
        case CACHE_TYPE_DIRECT: // direct mapped
            break;
        case CACHE_TYPE_FULLY_ASSOC: // fully associative
            break;
        case CACHE_TYPE_2_WAY: // 2-way associative
            break;
        }
    }
}
