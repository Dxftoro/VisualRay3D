#pragma once
#include "vrpch.h"
#include "kernel.h"

namespace vray {

	enum class BufferObjectType {
		BOOL,	BOOL2,	BOOL3,	BOOL4,
		FLOAT,	FLOAT2, FLOAT3, FLOAT4,
		INT,	INT2,	INT3,	INT4,
				MAT2,	MAT3,	MAT4
	};

	struct BufferObjectTypeInfo {
		GLuint count;
		GLuint size;
		GLenum glType;
	};

	class BufferLayout;
	class BufferObject {
		friend BufferLayout;
	private:
		std::string name;
		BufferObjectType type;
		bool normalized;
		GLuint offset;

		static std::unordered_map<BufferObjectType, BufferObjectTypeInfo> typeMap;

	public:
		BufferObject(const std::string& _name, BufferObjectType _type, bool _normalized = false)
			: name(_name), type(_type), normalized(_normalized), offset(0) {}

		std::string getName() const { return name; }
		GLuint getCount() const;
		GLuint getTypeSize() const;
		GLenum getGlType() const;
		GLuint getOffset() const { return offset; }

		bool isNormalized() const { return normalized; }
	};

	class BufferLayout {
	private:
		std::vector<BufferObject> objects;
		GLuint stride;

	public:
		BufferLayout(const std::initializer_list<BufferObject>& _objects);
		BufferLayout(const std::vector<BufferObject>& _objects);

		GLuint getStride() const { return stride; }

		GLuint size() const { return objects.size(); }
		inline BufferObject& operator[] (size_t index) { return objects[index]; }
	};

}