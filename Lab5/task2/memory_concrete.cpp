#define _CRT_SECURE_NO_WARNINGS
#include "memory_concrete.h"
#include <sstream>


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
	try { ptr = ::operator new(target_size); }
	catch (std::bad_alloc) {
		log("Memory allocated error!", logger::error);
		throw;
	}
	std::ostringstream stream;
	stream << "Allocated memory with adress " << ptr;
	log(stream.str(), logger::information);
	return ptr;
}

void memory_concrete::deallocate(void const* const target_to_dealloc) const {
	unsigned char const* mem = static_cast<unsigned char const*>(target_to_dealloc);

	std::ostringstream log_stream;

	log_stream << "Memory deallocated at " << target_to_dealloc << " with contents: ";

	for (size_t i = 0; i < 65; ++i) {
		log_stream << static_cast<int>(mem[i]) << " ";
	}
	
	log(log_stream.str(), logger::information);
	::operator delete(const_cast<void*>(target_to_dealloc));
	
}
