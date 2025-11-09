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
		Mesh(std::ifstream& fin);
		Mesh(VertexArray* _vertexArray)
			: vertexArray(_vertexArray), copy(false) {
		}
		~Mesh();

		Mesh(const Mesh&);
		Mesh& operator=(const Mesh&);

		VertexArray* getVertexArray() const { return vertexArray; }
		bool isACopy() const { return copy; }
	};

}