#pragma once
#include "kernel.h"
#include <type_traits>
#include <cassert>

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

		static size_t bytes(size_t count) { return sizeof(DataType) * count; }

	public:
		static constexpr size_t defaultExtent	= 30;
		static constexpr size_t multiplier		= 2;

		explicit GlslFlexibleBuffer(GlslUsage usage = GlslUsage::DYNAMIC_DRAW);
		explicit GlslFlexibleBuffer(size_t size, GlslUsage usage = GlslUsage::DYNAMIC_DRAW, 
									size_t extent = GlslFlexibleBuffer::defaultExtent);
		~GlslFlexibleBuffer();

		size_t getCapacity() const { return capacity; }
		size_t getExtent() const { return extent; }
		size_t getSize() const { return _size; }
		GlslUsage getUsage() const { return usage; }

		inline size_t size() const { return getSize(); }

		void setUsage(GlslUsage usage) { this->usage = usage; }

		void set(size_t index, const DataType& data);
		DataType get(size_t index) const;
		void push(const DataType& data);
		void pop();

		bool isEmpty() const { return (_size == 0); }
	};

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	GlslFlexibleBuffer<BufferType, DataType>::GlslFlexibleBuffer(GlslUsage _usage) 
		: usage(_usage), capacity(defaultExtent), extent(defaultExtent), _size(0) {
		assert((capacity && extent) || "Capacity and extent cannot be 0!");

		//GlslBufferController::init(handle, BufferType, nullptr, bytes(capacity), usage);
		VR_ENGINE_LOGIMPORTANT(std::format("Buffer created! Capacity: {0}, bytes: {1}", capacity, bytes(capacity)));
	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	GlslFlexibleBuffer<BufferType, DataType>::GlslFlexibleBuffer(size_t size_, GlslUsage usage, size_t _extent) 
		: _size(size_), extent(_extent) {
		
		capacity = _size > 0
					? extent * ((_size / extent) + (_size % extent))
					: defaultExtent;
		assert((capacity && extent) || "Capacity and extent cannot be 0!");

		//GlslBufferController::init(handle, BufferType, nullptr, bytes(capacity), usage);
		VR_ENGINE_LOGIMPORTANT(std::format("Buffer created! Capacity: {0}, bytes: {1}", capacity, bytes(capacity)));
	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	GlslFlexibleBuffer<BufferType, DataType>::~GlslFlexibleBuffer() {
		GlslBufferController::free(handle); // !!!
	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	void GlslFlexibleBuffer<BufferType, DataType>::set(size_t index, const DataType& data) {
		GlslBufferController::setSubData(handle, BufferType, &data, bytes(index), sizeof(DataType));
	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	DataType GlslFlexibleBuffer<BufferType, DataType>::get(size_t index) const {
		DataType data;
		GlslBufferController::getSubData(handle, BufferType, &data, bytes(index), sizeof(DataType));
		return data;
	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	void GlslFlexibleBuffer<BufferType, DataType>::push(const DataType& data) {
		if (_size >= capacity) {
			capacity += extent;
			VR_ENGINE_LOGIMPORTANT(std::format("Resizing up: size: {0} capacity: {1}, capacity (bytes): {2}", _size, capacity, bytes(capacity)));
			//GlslBufferController::resize(handle, BufferType, bytes(capacity), usage);
		}
		//GlslBufferController::setSubData(handle, BufferType, &data, bytes(_size), sizeof(DataType));
		_size++;
	}

	template <GLenum BufferType, typename DataType> requires GlslTrivial<DataType>
	void GlslFlexibleBuffer<BufferType, DataType>::pop() {
		if (isEmpty()) throw std::runtime_error("Buffer is empty!");

		size_t halfCapacity = (capacity / extent / 2) * extent;
		if (capacity > extent
			/*	If an element count is less than a half of capacity aligned to the specific extent size,
				it will be the second condition to resize our buffer */
			&& _size <= halfCapacity) {
			capacity -= halfCapacity;
			VR_ENGINE_LOGIMPORTANT(std::format("Resizing down: size: {0} capacity: {1}, capacity (bytes): {2}", _size, capacity, bytes(capacity)));
			//GlslBufferController::resize(handle, BufferType, bytes(capacity), usage);
		}

		_size--;
	}

}