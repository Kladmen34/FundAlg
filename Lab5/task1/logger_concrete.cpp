#define _CRT_SECURE_NO_WARNINGS
#include "logger_concrete.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>


std::map<std::string, std::pair<std::ofstream*, size_t> > logger_concrete::_streams =
std::map<std::string, std::pair<std::ofstream*, size_t> >();

logger_concrete::logger_concrete(
    std::map<std::string, logger::severity> const& targets)
{
    for (auto& target : targets)
    {
        auto global_stream = _streams.find(target.first);
        std::ofstream* stream = nullptr;

        if (global_stream == _streams.end())
        {
            if (target.first != "console") 
            {
                stream = new std::ofstream;
                stream->open(target.first);
            }

            _streams.insert(std::make_pair(target.first, std::make_pair(stream, 1)));
        }
        else
        {
            stream = global_stream->second.first;
            global_stream->second.second++;
        }

        _logger_streams.insert(std::make_pair(target.first, std::make_pair(stream, target.second)));
    }
}

logger_concrete::~logger_concrete()
{
    for (auto& logger_stream : _logger_streams)
    {
        auto global_stream = _streams.find(logger_stream.first);
        if (--(global_stream->second.second) == 0)
        {
            if (global_stream->second.first != nullptr)
            {
                global_stream->second.first->flush();
                global_stream->second.first->close();
                delete global_stream->second.first;
            }

            _streams.erase(global_stream);
        }
    }
}

logger const* logger_concrete::log(const std::string& target, severity level) const{
    auto t_now = std::time(nullptr);
    auto t_local = *std::localtime(&t_now);
    for (auto strm : _logger_streams) {
        if (level >= strm.second.second && strm.second.first) {
            *(strm.second.first) << "[" << std::put_time(&t_local, "%d/%m/%Y %H:%M:%S") << "]";
            switch (level) {
            case trace:
                *(strm.second.first) << "[TRACE] ";
                break;
            case debug:
                *(strm.second.first) << "[DEBUG] ";
                break;
            case information:
                *(strm.second.first) << "[INFO] ";
                break;
            case warning:
                *(strm.second.first) << "[WARNING] ";
                break;
            case error:
                *(strm.second.first) << "[ERROR] ";
                break;
            case critical:
                *(strm.second.first) << "[CRITICAL] ";
                break;
            default:
                break;
            }
            *(strm.second.first) << target << std::endl;
        }
    }
    return const_cast<logger_concrete*>(this);
}
