#pragma once
#include "vrpch.h"
#include "kernel.h"

namespace vray {

	template <typename T>
	class ResourceManager {
	private:
		static_assert(std::is_base_of_v<Resource, T>, "T must be inherited from Resource!");
		std::unordered_map<std::string, std::unique_ptr<T>> resourceMap;

	public:
		ResourceManager() {}
		~ResourceManager() { clear(); }

		T* load(const std::string& filename, const std::string& resourceName = "");
		T* get(const std::string& resourceName);
		void clear() { resourceMap.clear(); }
	};

	template<typename T>
	T* ResourceManager<T>::load(const std::string& filename, const std::string& resourceName) {
		if (!std::filesystem::exists(filename)) {
			throw std::runtime_error("\"" + filename + "\" - File not found!");
		}

		std::string finalName = resourceName == "" ? filename : resourceName;
		std::unique_ptr<T> resourcePtr = std::make_unique<T>(filename);

		T* rawPtr = resourcePtr.get();
		resourceMap[finalName] = std::move(resourcePtr);

		return rawPtr;
	}

	template<typename T>
	T* ResourceManager<T>::get(const std::string& resourceName) {
		auto it = resourceMap.find(resourceName);
		if (it != resourceMap.end()) return it->second.get();
		else return load(resourceName);
	}

}