#include <unordered_map>
#include <cstdint>

#ifndef FREQUENCY_TABLE_H
#define FREQUENCY_TABLE_H

namespace data {
	template<typename T>
	class FrequencyTable {
	private:
		std::unordered_map<T, std::uint64_t> frequency_table;

	public:
		void increment(const T& x) {
			++frequency_table[x];
		}

		std::uint64_t operator [](const T& x) {
			auto it = frequency_table.find(x);
			if (it == frequency_table.end()) {
				return 0;
			}
			else {
				return it->second;
			}
		}

		std::vector<T> values() const {
			std::vector<T> values;
			for (const auto& kv : frequency_table) {
				values.push_back(kv.first);
			}
			return values;
		}

	};

	template<typename T>
	FrequencyTable<T> count_frequencies(const std::vector<T>& v) {
		FrequencyTable<T> table;
		for (const auto& x : v) {
			table.increment(x);
		}
		return table;
	}
}

#endif
