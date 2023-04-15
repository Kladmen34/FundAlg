#ifndef SANDBOX_CPP_MEMORY_CONCRETE_H
#define SANDBOX_CPP_MEMORY_CONCRETE_H

#include "memory.h"
#include "logger.h"
#include "logger_builder.h"
#include "logger_builder_concrete.h"
#include "logger_concrete.h"


class memory_concrete final : public memory {
private:
	logger* _log;
	void log(std::string const& target, logger::severity level) const;

public:

	void* allocate(size_t target_size) const override;

	void deallocate(void const* const target_to_dealloc) const override;

	memory_concrete(logger* log = nullptr);

	memory_concrete(const memory_concrete& other) = delete;

	memory_concrete& operator=(const memory_concrete& other) = delete;
};

#endif