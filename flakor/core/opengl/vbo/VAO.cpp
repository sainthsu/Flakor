#include "core/opengl/VBO/VAO.h"

FLAKOR_NS_BEGIN

VAO* VAO::create(int sizePerVertex,int vertexNumber)
{
	
}

void VAO::bind()
{
	glBindArrayBuffer(arrayID);
}

FLAKOR_NS_END
