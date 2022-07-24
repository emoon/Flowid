#include "linear_allocator.h"
#include "include/flowid.h"
#include "flowid_private.h"
#include "types.h"

// TODO: Configure stdlib
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LinearAllocator_create(LinearAllocator* self, const char* id, u8* data, int len) {
    self->id = id;
    self->start_data = data;
    self->end_data = data + len;
    self->current_data = data;
    self->allocator = NULL;
    self->allow_realloc = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool LinearAllocator_create_with_allocator(LinearAllocator* self, const char* id, FldAllocator* allocator, int len,
                                           bool allow_realloc) {
    u8* mem = Allocator_alloc_array(allocator, len, u8);

    LinearAllocator_create(self, id, mem, len);
    self->allocator = allocator;
    self->allow_realloc = allow_realloc;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LinearAllocator_destroy(LinearAllocator* self) {
    if (self->allocator) {
        Allocator_free(self->allocator, self->start_data);
        self->allocator = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FLD_INLINE u8* internal_alloc_unchecked(LinearAllocator* self, int size, int alignment) {
    u8* start = self->current_data;

    // Align the pointer up to correct alignment
    start = (u8*)(((uintptr_t)start + (alignment - 1)) & -alignment);

    u8* end = start + size;
    self->current_data = end;

    return start;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static u8* handle_out_of_memory(LinearAllocator* self, int size, int alignment) {
    if (!self->allocator || !self->allow_realloc) {
        //ERROR_ADD(FlError_Memory, "Ran out of memory in fixed LinearAllocator: %s", self->id);
        return NULL;
    }

    // calculate the the original size and double it for the new alloc
    int old_size = (int)((uintptr_t)self->end_data - (uintptr_t)self->start_data);
    int new_size = old_size * 2; 
    u8* data = Allocator_realloc_array(self->allocator, u8, self->start_data, old_size, new_size);

    LinearAllocator_update_resize(self, data, new_size);
    return internal_alloc_unchecked(self, size, alignment);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8* LinearAllocator_internal_alloc(LinearAllocator* self, int size, int alignment) {
    u8* start = self->current_data;

    // Align the pointer up to correct alignment
    start = (u8*)(((uintptr_t)start + (alignment - 1)) & -alignment);

    u8* end = start + size;

    if (FLD_UNLIKELY(end > self->end_data)) {
        return handle_out_of_memory(self, size, alignment);
    }

    self->current_data = end;

    return start;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8* LinearAllocator_internal_alloc_unaligned(LinearAllocator* self, int size) {
    u8* start = self->current_data;
    u8* end = start + size;

    if (FLD_UNLIKELY(end > self->end_data)) {
        return handle_out_of_memory(self, size, 1);
    }

    self->current_data = end;

    return start;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8* LinearAllocator_internal_alloc_zero(LinearAllocator* self, int size, int alignment) {
    u8* data = LinearAllocator_internal_alloc(self, size, alignment);
    // TODO: configure stdlib
    memset(data, 0, size);
    return data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// If we ran out of memory it's possible to set new pointers and continue on from where we were

void LinearAllocator_update_resize(LinearAllocator* self, u8* data, int new_len) {
    int current_location = LinearAllocator_current_position(self);
    self->start_data = data;
    self->end_data = data + new_len;
    self->current_data = data + current_location;
}
