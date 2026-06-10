#pragma once
#include "vrpch.h"
#include "kernel.h"

namespace vray {

	class VRAYLIB Resource {
	protected:
		bool copy;
	public:
		Resource() : copy(false) {} // !!!
	};

	class VertexArray;
	class VRAYLIB Mesh : public Resource {
	private:
		VertexArray* vertexArray;
		bool copy;

	public:
		Mesh(const std::string& filename);
		Mesh(VertexArray* _vertexArray)
			: vertexArray(_vertexArray), copy(false) {
		}
		~Mesh();

		Mesh(const Mesh&);
		Mesh& operator=(const Mesh&);

		VertexArray* getVertexArray() const { return vertexArray; }
		bool isACopy() const { return copy; }
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