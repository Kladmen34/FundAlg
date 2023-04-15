#ifndef SANDBOX_CPP_MEMORY_H
#define SANDBOX_CPP_MEMORY_H

#include <iostream>

class memory {

public:

	virtual void* allocate(size_t target_size) const = 0;

	virtual void deallocate(void const* const target_to_dealloc) const = 0;

	virtual ~memory();

};

#endif 
