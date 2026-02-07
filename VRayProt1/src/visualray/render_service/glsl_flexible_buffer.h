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

		static void setSubData(GLuint handle, GLenum type, const void* data, size_t offset, size_t size);
		static void getSubData(GLuint handle, GLenum type, void* data, size_t offset, size_t size);
	};

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	class GlslFlexibleBuffer {
	private:
		size_t
			/* The max capacity of the buffer until it needs to resize */
			capacity, 
			/* Extent is an amount of buffer elements on which it's capacity will be increased */
			extent, 
			/* Current count of elements */
			_size;
		GLuint handle;
		GlslUsage usage;

		GlslFlexibleBuffer(const GlslFlexibleBuffer&) = delete;
		GlslFlexibleBuffer(GlslFlexibleBuffer&&) noexcept = delete;
		GlslFlexibleBuffer& operator=(const GlslFlexibleBuffer&) = delete;

		void increaseCapacity(size_t size);
		void decreaseCapacity(size_t size);

	public:
		static constexpr size_t defaultExtent = 10;

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

		void set(size_t index, const DataType& data);
		DataType get(size_t index) const;
		void push(const DataType& data);
		void pop();
	};

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	GlslFlexibleBuffer<BufferType, DataType>::GlslFlexibleBuffer(GlslUsage _usage) 
		: usage(_usage), capacity(defaultExtent), extent(defaultExtent), _size(0) {
		GlslBufferController::init(handle, BufferType, nullptr, capacity, usage);
	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	GlslFlexibleBuffer<BufferType, DataType>::GlslFlexibleBuffer(size_t size_, GlslUsage usage) 
		: _size(size_), extent(defaultExtent) {
		capacity = _size > 0 ? _size : defaultExtent;
		GlslBufferController::init(handle, BufferType, nullptr, capacity, usage);
	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	GlslFlexibleBuffer<BufferType, DataType>::~GlslFlexibleBuffer() {
		GlslBufferController::free(handle);
	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	void GlslFlexibleBuffer<BufferType, DataType>::set(size_t index, const DataType& data) {
		GlslBufferController::setSubData(handle, BufferType, &data, sizeof(DataType) * index, sizeof(DataType));
	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	DataType GlslFlexibleBuffer<BufferType, DataType>::get(size_t index) const {
		DataType data;
		GlslBufferController::getSubData(handle, BufferType, &data, sizeof(DataType) * index, sizeof(DataType));
		return data;
	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	void GlslFlexibleBuffer<BufferType, DataType>::push(const DataType& data) {

	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	void GlslFlexibleBuffer<BufferType, DataType>::pop() {

	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	void GlslFlexibleBuffer<BufferType, DataType>::increaseCapacity(size_t size) {

	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	void GlslFlexibleBuffer<BufferType, DataType>::decreaseCapacity(size_t size) {

	}

}