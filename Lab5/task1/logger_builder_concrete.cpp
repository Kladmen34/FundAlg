#include "logger_builder_concrete.h"
#include "logger_concrete.h"
#include <map>
#include <fstream>


logger_builder* logger_builder_concrete::add_stream(
    std::string const& path,
    logger::severity severity)
{
    _construction_info[path] = severity;

    return this;
}

logger* logger_builder_concrete::construct() const
{
    return new logger_concrete(_construction_info);
}

logger_builder* logger_builder_concrete::parse_config(std::string const& path) {
    std::map<std::string, logger::severity> _types = { {"trace", logger::severity::trace},
                                                      {"debug", logger::severity::debug},
                                                      {"information", logger::severity::information},
                                                      {"warning", logger::severity::warning},
                                                      {"error", logger::severity::error},
                                                      {"critical", logger::severity::critical} };
    std::ifstream file;
    file.open(path);

    if (!file) {
        return nullptr;
    }

    std::string file_path, level_severity;
    while (file >> file_path >> level_severity) {
        add_stream(file_path, _types[level_severity]);
    }
    file.close();
    return this;
}