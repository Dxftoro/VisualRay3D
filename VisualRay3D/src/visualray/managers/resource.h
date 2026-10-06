#pragma once
#include "../vrpch.h"
#include "../kernel.h"
#include "../thirdparty/glm/glm.hpp"
#include "../render_service/gpu_handle.h"

namespace vray {

	class VRAYLIB Resource {
	protected:
		bool copy;
		Resource(bool _copy) : copy(_copy) {}

	public:
		Resource() : copy(false) {} // !!!
	};

	class VRAYLIB Mesh : public Resource {
	private:
		std::vector<float> vertexData;
		std::vector<int> elements;
		glm::vec3 baseSize, aabbMin, aabbMax;
		GpuHandle handle;

	public:
		Mesh(const std::string& filename);

		const glm::vec3 getBaseSize() const { return baseSize; }
		const glm::vec3 getAabbMin() const { return aabbMin; }
		const glm::vec3 getAaabbMax() const { return aabbMax; }
		GpuHandle getHandle() const { return handle; }

		void setHandle(GpuHandle handle) { this->handle = handle; }
	};

	class VRAYLIB Texture : public Resource {
	private:
		std::vector<unsigned char> pixels;
		int width, height, cCount;
		GpuHandle handle;

	public:
		Texture(const std::string& filename);

		int getWidth() const { return width; }
		int getHeight() const { return height; }
		int getChannelCount() const { return cCount; }
		GpuHandle getHandle() const { return handle; }
		std::vector<unsigned char>& getPixels() { return pixels; }

		void setHandle(GpuHandle handle) { this->handle = handle; }
	};

	class VRAYLIB Sound : public Resource {
	private:
		int cCount, sampleRate, sampleCount;
		unsigned int handle;

	public:
		Sound(const std::string& filename);
		~Sound();

		int getChannelCount() const { return cCount; }
		int getSampleRate() const { return sampleRate; }
		int getSampleCount() const { return sampleCount; }
		unsigned int getHandle() const { return handle; }
	};

}