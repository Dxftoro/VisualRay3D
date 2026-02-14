#include "vrpch.h"
#include "kernel.h"
#include "buffer_layout.h"
#include "glad/glad.h"

namespace vray {

	std::unordered_map<BufferObjectType, BufferObjectTypeInfo> BufferObject::typeMap{
		{BufferObjectType::BOOL,	{1, sizeof(bool), GL_BOOL}},
		{BufferObjectType::FLOAT,	{1, sizeof(float), GL_FLOAT}},
		{BufferObjectType::FLOAT2,	{2, sizeof(float), GL_FLOAT}},
		{BufferObjectType::FLOAT3,	{3, sizeof(float), GL_FLOAT}},
		{BufferObjectType::FLOAT4,	{4, sizeof(float), GL_FLOAT}},
		{BufferObjectType::INT,		{1, sizeof(int), GL_INT}},
		{BufferObjectType::INT2,	{2, sizeof(int), GL_INT}},
		{BufferObjectType::INT3,	{3, sizeof(int), GL_INT}},
		{BufferObjectType::INT4,	{4, sizeof(int), GL_INT}},
	};

	GLuint BufferObject::getCount() const {
		return BufferObject::typeMap[type].count;
	}

	GLuint BufferObject::getTypeSize() const {
		return BufferObject::typeMap[type].size;
	}

	GLenum BufferObject::getGlType() const {
		return BufferObject::typeMap[type].glType;
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferObject>& _objects)
		: objects(_objects), stride(0) {

		GLuint offset = 0;
		for (BufferObject& object : objects) {
			GLuint objectSize = object.getTypeSize() * object.getCount();

			object.offset = offset;
			offset += objectSize;
			stride += objectSize;
		}
	}

	BufferLayout::BufferLayout(const std::vector<BufferObject>& _objects)
		: objects(_objects), stride(0) {

		GLuint offset = 0;
		for (BufferObject& object : objects) {
			GLuint objectSize = object.getTypeSize() * object.getCount();

			object.offset = offset;
			offset += objectSize;
			stride += objectSize;
		}
	}

}