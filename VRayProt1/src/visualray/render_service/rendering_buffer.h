#pragma once
#include "vrpch.h"
#include "kernel.h"
#include "buffer_layout.h"

namespace vray {

	class RenderingBuffer {
	protected:
		unsigned int handle;

	public:
		RenderingBuffer();
		~RenderingBuffer();

		virtual void bind() = 0;
		virtual void unbind() = 0;
	};

	class VertexBuffer : public RenderingBuffer {
	private:
		BufferLayout bufferLayout;

	public:
		VertexBuffer(float* verticies, unsigned int size, const BufferLayout& _bufferLayout);

		void bind() override;
		void unbind() override;

		const BufferLayout& getBufferLayout() const { return bufferLayout; }
		void setBufferLayout(const BufferLayout& _bufferLayout) { bufferLayout = _bufferLayout; }
	};

	class ElementBuffer : public RenderingBuffer {
	private:
		unsigned int count;

	public:
		ElementBuffer(int* elements, unsigned int size);

		void bind() override;
		void unbind() override;

		unsigned int getCount() const { return count; }
	};

}