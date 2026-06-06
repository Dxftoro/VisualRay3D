#include <stdexcept>
#include "vrpch.h"

namespace vray {

	class AudioException : public std::exception {
	private:
		std::string message;

	public:
		explicit AudioException(const std::string& _message) : message(_message) {}
		const char* what() const noexcept override;
	};

}