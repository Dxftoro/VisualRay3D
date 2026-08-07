#pragma once

#include <type_traits>

namespace vray {

	template <typename Signature> class FunctionRef;

	template <typename R, typename... Args>
	class FunctionRef<R(Args...)> {
	private:
		void* callable;
		R(*sign)(void*, Args...);

	public:
		template <typename F>
		FunctionRef(F&& func)
		:	callable(const_cast<void*>(static_cast<const void*>(&func))),
			sign([](void* clb, Args... args) -> R {
				return (*static_cast<std::remove_reference_t<F>*>(clb))(args...);
			})
		{}

		R operator()(Args... args) const { return sign(callable, args); }
	};

}