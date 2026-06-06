#include <stdexcept>

namespace vray {

	class AudioException : public std::runtime_error {
	public:
		using std::runtime_error::runtime_error;
	};

}