#ifndef SANDBOX_CPP_LOGGER_BUILDER_H
#define SANDBOX_CPP_LOGGER_BUILDER_H

#include <iostream>
#include "logger.h"

class logger_builder
{

public:

    virtual logger_builder* add_stream(std::string const&, logger::severity) = 0;

    virtual logger* construct() const = 0;

    virtual logger_builder* parse_config(std::string const& path) = 0;

public:

    virtual ~logger_builder();

};

#endif //SANDBOX_CPP_LOGGER_BUILDER_H
