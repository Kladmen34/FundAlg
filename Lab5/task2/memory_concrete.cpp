#define _CRT_SECURE_NO_WARNINGS
#include "memory_concrete.h"
#include <sstream>
#include <cstring>

void memory_concrete::log(std::string const& target, logger::severity level) const {
	if (_log == nullptr)
		return;
	_log->log(target, level);
}

memory_concrete::memory_concrete(logger* log) {
	_log = log;
}

void* memory_concrete::allocate(size_t target_size) const {
	void* ptr = nullptr;
	try { ptr = ::operator new(sizeof(size_t) + target_size); }
	catch (std::bad_alloc) {
		log("Memory allocated error!", logger::error);
		throw;
	}
	*reinterpret_cast<size_t*>(ptr) = target_size;
	std::ostringstream stream;
	stream << "Allocated memory with address " << ptr;
	log(stream.str(), logger::information);
	return reinterpret_cast<void *>(reinterpret_cast<size_t *>(ptr) + 1);
}

void memory_concrete::deallocate(void const* const target_to_dealloc) const {
	unsigned char* mem = reinterpret_cast<unsigned char*>(const_cast<void *>(target_to_dealloc));
	size_t target_size = *(mem - sizeof(size_t));

	std::ostringstream log_stream;

	log_stream << "Memory deallocated at " << target_to_dealloc << " with contents: ";
	
	for (size_t i = 0; i < target_size; ++i) {
		log_stream << static_cast<unsigned short>(mem[i]) << " ";
	}
	
	log(log_stream.str(), logger::information);
	::operator delete(mem - sizeof(size_t));
	
}
