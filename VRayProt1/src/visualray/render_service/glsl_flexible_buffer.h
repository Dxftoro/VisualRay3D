#pragma once
#include "kernel.h"
#include <type_traits>

namespace vray {

	enum class GlslUsage : GLenum {
		STREAM_DRAW = 0x88E0,
		STREAM_READ = 0x88E1,
		STREAM_COPY = 0x88E2,
		STATIC_DRAW = 0x88E4,
		STATIC_READ = 0x88E5,
		STATIC_COPY = 0x88E6,
		DYNAMIC_DRAW = 0x88E8,
		DYNAMIC_READ = 0x88E9,
		DYNAMIC_COPY = 0x88EA
	};

	template <typename T>
	concept GlslTrivial = std::is_trivially_copyable_v<T>;

	class GlslBufferController {
		template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
		friend class GlslFlexibleBuffer;

	private:
		static void init(GLuint& handle, GLenum type, const void* data, size_t size, GlslUsage usage);
		static void resize(GLuint& handle, GLenum type, size_t size, GlslUsage usage);
		static void free(GLuint& handle);

		static void bind(const GLuint handle, GLenum type);
		static void unbind(GLenum type);

		static void setSubData(GLuint handle, GLenum type, const void* data, size_t size, GlslUsage usage);
	};

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	class GlslFlexibleBuffer {
	private:
		size_t capacity, extent, _size;
		GLuint handle;
		GlslUsage usage;

		GlslFlexibleBuffer(const GlslFlexibleBuffer&) = delete;
		GlslFlexibleBuffer(GlslFlexibleBuffer&&) noexcept = delete;
		GlslFlexibleBuffer& operator=(const GlslFlexibleBuffer&) = delete;

	public:
		static constexpr size_t defaultCapacity		= 10;
		static constexpr size_t defaultExtent		= 10;

		explicit GlslFlexibleBuffer(GlslUsage usage = GlslUsage::DYNAMIC_DRAW);
		explicit GlslFlexibleBuffer(size_t initialSize, GlslUsage usage = GlslUsage::DYNAMIC_DRAW);
		~GlslFlexibleBuffer();

		size_t getCapacity() const { return capacity; }
		size_t getExtent() const { return extent; }
		size_t getSize() const { return _size; }
		GlslUsage getUsage() const { return usage; }

		inline size_t size() const { return getSize(); }

		void setExtent(size_t extent) { this->extent = extent; }
		void setUsage(GlslUsage usage) { this->usage = usage; }
	};

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	GlslFlexibleBuffer<BufferType, DataType>::GlslFlexibleBuffer(GlslUsage usage) {

	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	GlslFlexibleBuffer<BufferType, DataType>::GlslFlexibleBuffer(size_t initialSize, GlslUsage usage) {

	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	GlslFlexibleBuffer<BufferType, DataType>::~GlslFlexibleBuffer() {

	}

}