#pragma once
#include "vrpch.h"
#include "kernel.h"
#include "../thirdparty/glm/glm.hpp"

namespace vray {

	class VRAYLIB Resource {
	protected:
		bool copy;
		Resource(bool _copy) : copy(_copy) {}

	public:
		Resource() : copy(false) {} // !!!
	};

	class VertexArray;
	class VRAYLIB Mesh : public Resource {
	private:
		VertexArray* vertexArray;
		glm::vec3 baseSize, aabbMin, aabbMax;

	public:
		Mesh(const std::string& filename);
		Mesh(VertexArray* _vertexArray)
			: Resource(false), vertexArray(_vertexArray), baseSize(0.0f), aabbMin(0.0f), aabbMax(0.0f) {
		}
		~Mesh();

		Mesh(const Mesh&);
		Mesh& operator=(const Mesh&);

		VertexArray* getVertexArray() const { return vertexArray; }
		const glm::vec3 getBaseSize() const { return baseSize; }
		const glm::vec3 getAabbMin() const { return aabbMin; }
		const glm::vec3 getAaabbMax() const { return aabbMax; }
	};

	class VRAYLIB Texture : public Resource {
	private:
		int width, height, cCount;
		unsigned int handle;

	public:
		Texture(const std::string& filename);
		~Texture();

		void bind() const;
		void unbind() const;

		int getWidth() const { return width; }
		int getHeight() const { return height; }
		int getChannelCount() const { return cCount; }
		unsigned int getHandle() const { return handle; }
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