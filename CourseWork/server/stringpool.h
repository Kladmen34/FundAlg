#pragma once

#include <unordered_set>
#include <unordered_map>

#ifndef PSTR
typedef const char * PSTR;
#endif

class StringPool {
    std::unordered_set<std::string> _set;
    std::unordered_map<PSTR, size_t> _len;

    StringPool() {}
    ~StringPool() {}

public:

    static StringPool& instance() {
        static StringPool sp;
        return sp;
    }

    const char* find(const std::string & str) {
        auto it = _set.find(str);
        if (it != _set.end()) {
            return it->c_str();
        }
        const char *ptr = _set.insert(str).first->c_str();
        _len.insert(std::make_pair(ptr, str.size()));
        return ptr;
    }

    size_t length(const char* str_ptr) {
        auto it = _len.find(str_ptr);
        if (it == _len.end()) {
            return 0;
        }
        return it->second;
    }
};
