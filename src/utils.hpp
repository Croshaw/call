#pragma once
#include <vector>
#include <algorithm>

namespace hufv {
	template<typename T>
	long long find(const std::vector<T>& table, const T& value) {
		auto it = std::find(table.begin(), table.end(), value);
		if (it == table.end())
			return -1;
		return std::distance(table.begin(), it);
	}
	template<typename T>
	unsigned long long write(std::vector<T>& table, const T& value) {
		int index = find(table, value);
		if (index != -1)
			return index;
		table.push_back(value);
		return table.size() - 1;
	}
}