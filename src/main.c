#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // Para tipos exatos: uint8_t, uintptr_t, size_t
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>

typedef struct {
    uint8_t *buffer;
    size_t capacity;
    size_t offset;
} Arena;



static inline uintptr_t align_forward(uintptr_t ptr, size_t alignment) {
    uintptr_t aligned = (ptr + (alignment - 1)) & ~(alignment - 1);
    return aligned;
}
bool arena_init(Arena *arena, size_t capacity) {
    if (arena == NULL) return false;

    arena->buffer = mmap(NULL, capacity, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (arena->buffer == MAP_FAILED) {
        perror("[ERRO] Falha ao alocar buffer da Arena!");
        return false;
    }

    arena->capacity = capacity;
    arena->offset = 0;
    return true;
}

void arena_destroy(Arena *arena) {
    if (arena != NULL && arena->buffer != NULL) {
        munmap(arena->buffer, arena->capacity);
        arena->buffer = NULL;
        arena->capacity = 0;
        arena->offset = 0;
    }
}


void* arena_alloc(Arena *arena, size_t size) {
    if (arena == NULL || size == 0) return NULL;

    // Calcula o endereço atual absoluto
    uintptr_t curr_ptr = (uintptr_t) arena->buffer + arena->offset;
    // Força o alinhamento para 8 bytes (múltiplo de 8)
    uintptr_t aligned_ptr = align_forward(curr_ptr, 8);

    // Calcula quantos bytes foram consumidos com o padding de alinhamento
    size_t new_offset = (aligned_ptr - (uintptr_t) arena->buffer) + size;

    if (new_offset > arena->capacity) {
        fprintf(stderr, "[ERRO] Arena Out of Memory!\n");
        return NULL;
    }

    arena->offset = new_offset;
    return (void*) aligned_ptr;
}

void arena_reset(Arena *arena) {
    if (arena != NULL) {
        arena->offset = 0;
    }
}

void arena_dump(const Arena *arena, const char *label) {
    printf("\n\033[1;36m=== [ MEMORY SNAPSHOT: %s ] ===\033[0m\n", label);
    printf(" Base: %p | Offset: %zu / %zu bytes\n", 
           (void*)arena->buffer, arena->offset, arena->capacity);

    // 1. Barra gráfica percentual
    const int BAR_WIDTH = 32;
    int used_chars = (int)((arena->offset * BAR_WIDTH) / arena->capacity);
    if (arena->offset > 0 && used_chars == 0) used_chars = 1; // Pelo menos 1 bloco se houver bytes

    printf(" [");
    for (int i = 0; i < BAR_WIDTH; i++) {
        if (i < used_chars) {
            printf("\033[1;32m█\033[0m"); // Verde = Usado
        } else {
            printf("\033[0;37m░\033[0m"); // Cinza = Livre
        }
    }
    printf("] (%.1f%% alocado)\n", (double)arena->offset / arena->capacity * 100.0);

    // 2. Hexdump dos primeiros 64 bytes
    printf(" Raw Bytes na RAM (Hex):\n ");
    size_t preview_len = 64;
    if (preview_len > arena->capacity) preview_len = arena->capacity;

    for (size_t i = 0; i < preview_len; i++) {
        if (i < arena->offset) {
            // Byte ocupado: verde
            printf("\033[1;32m%02X \033[0m", arena->buffer[i]);
        } else {
            // Byte livre: cinza escuro
            printf("\033[0;90m.. \033[0m");
        }
        if ((i + 1) % 16 == 0) printf("\n ");
    }
    printf("\033[1;36m=============================================\033[0m\n");
}




int main(void) {
    Arena min_arena;
    if (!arena_init(&min_arena, 256)) { // Arena de 256 bytes para vermos bem a proporção
        return 1;
    }

    // Estado 0: Virgem (recém-alocada pelo Kernel)
    arena_dump(&min_arena, "0. Arena Recém-Criada via mmap");

    // Estado 1: Alocando 5 inteiros (5 * 4 = 20 bytes)
    int *nums = (int*) arena_alloc(&min_arena, 5 * sizeof(int));
    for (int i = 0; i < 5; i++) {
        nums[i] = 100 + i; // Grava números na RAM
    }
    arena_dump(&min_arena, "1. Alocou Array de Inteiros (5 ints)");

    // Estado 2: Alocando uma string com texto
    char *msg = (char*) arena_alloc(&min_arena, 24);
    strcpy(msg, "Kernel Linux!");
    arena_dump(&min_arena, "2. Alocou String 'Kernel Linux!'");

    // Estado 3: Reset Instantâneo em O(1)
    arena_reset(&min_arena);
    arena_dump(&min_arena, "3. Após arena_reset() (Reciclada!)");

    arena_destroy(&min_arena);
    return 0;
} 

