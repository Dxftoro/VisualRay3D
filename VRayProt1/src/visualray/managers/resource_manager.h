#pragma once
#include "vrpch.h"
#include "kernel.h"

namespace vray {

	class Resource {
	protected:
		std::string name;

	public:
		Resource(const std::string& resourceName) : name(resourceName) {}
		std::string getName() const { return name; }
	};

	class IResourceManager {
	private:
	public:
		virtual Resource& load(const std::string& filename, const std::string& resourceName = "") = 0;
		virtual Resource& get(const std::string& resourceName) = 0;
		virtual void clear() = 0;
	};

}