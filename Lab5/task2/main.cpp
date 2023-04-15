#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include "memory.h"
#include "memory_concrete.h"


int main() {
	logger_builder* builder = new logger_builder_concrete();
	logger* constructed_logger = builder->add_stream("info.txt", logger::information)
		->add_stream("errors.txt", logger::error)->construct();
	memory_concrete allocator(constructed_logger);
	int* ptr1 = static_cast<int*>(allocator.allocate(sizeof(int)));
	*ptr1 = 54;
	allocator.deallocate(ptr1);

	const char* str = "your bunny wrote";
	char* ptr2 = static_cast<char*>(allocator.allocate(strlen(str) + 1));
	std::strcpy(ptr2, str);
	allocator.deallocate(ptr2);

	struct random_struct {
		int x;
		double y;
		char z;
	};
	random_struct* ptr3 = static_cast<random_struct*>(allocator.allocate(sizeof(random_struct)));
	ptr3->x = 69;
	ptr3->y = 14.88;
	ptr3->z = 'Z';
	allocator.deallocate(ptr3);

	delete constructed_logger;

	return 0;
}
