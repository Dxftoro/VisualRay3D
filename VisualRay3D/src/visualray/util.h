#pragma once
#include "vrpch.h"
#include "kernel.h"

namespace vray {

	std::string sizeToString(size_t bytes);

	float VRAYLIB frand(const float a, const float b);

	/* Strong typedef for numeric types. Use to avoid implicit type cast on assignment */
	template <typename T>
	class Strong {
	protected:
		T value;

	public:
		explicit constexpr Strong(T _value) : value(_value) {}
		
		explicit operator T() const { return value; }
		T& operator=(T value) {
			this->value = value;
			return *this;
		}

		/* Prefix increment */
		T& operator++() { ++value; return *this; }

		/* Postfix increment */
		T& operator++(int) {
			T res = value;
			++value;
			return res;
		}

		T& operator--() { --value; return *this; }
		T& operator--(int) {
			T res = *this;
			--value;
			return res;
		}

		bool operator==(const T& value) { return this->value == value; }
		bool operator!=(const T& value) { return this->value != value; }
		bool operator<(const T& value) { return this->value < value; }
		bool operator>(const T& value) { return this->value > value; }
		bool operator<=(const T& value) { return this->value <= value; }
		bool operator>=(const T& value) { return this->value >= value; }

		T get() const { return value; }
	};

}