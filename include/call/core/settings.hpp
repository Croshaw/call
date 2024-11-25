//
// Created by croshaw on 11/20/24.
//

#ifndef CALL_SETTINGS_HPP
#define CALL_SETTINGS_HPP

#include <vector>
#include <string>
#include <map>
#include <algorithm>

namespace call {

    typedef std::vector<std::string> table;
    enum table_type {
        RESERVED,
        SEPARATORS,
        NUMBERS,
        IDENTIFIERS
    };

    struct token {
        const table_type table;
        const size_t id;
        const std::pair<size_t, size_t> pos;
    };

    class settings {
    private:
        std::map<table_type, table> _tables;
    public:
        settings(const table &reserved, const table &separators) {
            _tables[RESERVED] = reserved;
            _tables[SEPARATORS] = separators;
            _tables[NUMBERS] = {};
            _tables[IDENTIFIERS] = {};
        }

        void reset() {
            _tables[NUMBERS].clear();
            _tables[IDENTIFIERS].clear();
        }

        const table &get(table_type type) const {
            return _tables.at(type);
        }

        size_t find(table_type table, const std::string &value) {
            auto &t = get(table);
            auto it = std::find(t.begin(), t.end(), value);
            if (it == t.end())
                return -1;
            return std::distance(t.begin(), it);
        }

        size_t write(table_type table, const std::string &value) {
            int index = find(table, value);
            if (index != -1 || table < 2)
                return index;
            auto &t = _tables[table];
            t.push_back(value);
            return t.size() - 1;
        }
    };

} // call

#endif //CALL_SETTINGS_HPP
