#pragma once
#include "../vrpch.h"
#include "../kernel.h"

namespace vray {

	float VRAYLIB frand(const float a, const float b);

	template <typename T>
	constexpr auto bitsOf(T x) { return sizeof(x) * 8; }

	/* Strong typedef for numeric types. Use to avoid implicit type cast on assignment */
	template <typename T>
	class Strong {
	protected:
		T value;

	public:
		explicit constexpr Strong(T _value) : value(_value) {}
		constexpr Strong(const Strong<T>& other) : value(other.value) {}
		
		explicit operator T() const { return value; }
		Strong& operator=(T value) {
			this->value = value;
			return *this;
		}

		/* Prefix increment */
		Strong& operator++() { ++value; return *this; }

		/* Postfix increment */
		Strong operator++(int) {
			Strong res(value);
			++value;
			return res;
		}

		Strong& operator--() { --value; return *this; }
		Strong operator--(int) {
			Strong res(*this);
			--value;
			return res;
		}

		bool operator==(const T& value) const { return this->value == value; }
		bool operator!=(const T& value) const { return this->value != value; }
		bool operator<(const T& value) const { return this->value < value; }
		bool operator>(const T& value) const { return this->value > value; }
		bool operator<=(const T& value) const { return this->value <= value; }
		bool operator>=(const T& value) const { return this->value >= value; }

		constexpr T get() const { return value; }
	};

}

namespace std {

	template <typename T>
	struct hash<vray::Strong<T>> {
		size_t operator()(const vray::Strong<T>& value) const {
			return value.get();
		}
	};

}