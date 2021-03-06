#pragma once

#include "types.h"
#include "linear_allocator.h"

struct FldAllocator;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERNAL HEADER ONLY
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct CommandBuffer {
    LinearAllocator allocator;
    int command_count;
    u8* read_ptr;
} CommandBuffer;

// Create a new command buffer to fill with commands
bool CommandBuffer_create(CommandBuffer* self, const char* id, struct FldAllocator* allocator, int len);

// Destroy the command buffer
void CommandBuffer_destroy(CommandBuffer* self);

// Rewind to start populating/allocating from the buffer again
void CommandBuffer_rewind(CommandBuffer* self);

// Allocates a command in the command buffer for the user to fill out
u8* CommandBuffer_alloc_cmd(CommandBuffer* self, u16 cmd, u32 size);

// Start reading commands from the command buffer
FLD_INLINE int CommandBuffer_begin_read_commands(CommandBuffer* self) {
    self->read_ptr = self->allocator.start_data;
    return self->command_count;
}

// Allocates a command in the command buffer for the user to fill out
u8* CommandBuffer_alloc_cmd(CommandBuffer* self, u16 cmd, u32 size);

// Read commands from the commandbuffer
FLD_INLINE u16 CommandBuffer_read_next_cmd(CommandBuffer* self, const u8** command_ptr) {
    const u8* data = (const u8*)self->read_ptr;
    // Data layout is as follwing:
    // 2 bytes command
    // 4 bytes size of command
    // x command data
    u16 cmd = *(u16*)(data + 0);
    u32 size = *(u32*)(data + 2);

    const u8* data_out = data + 6;
    self->read_ptr = (u8*)data_out + size;
    *command_ptr = data_out;
    return cmd;
}

#define CommandBuffer_alloc(state, id, type) (type*)CommandBuffer_alloc_cmd(state, id, sizeof(type))
