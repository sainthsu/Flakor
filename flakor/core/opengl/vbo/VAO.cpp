#include "targetMacros.h"
#include "core/opengl/VBO/VBO.h"
#include "core/opengl/VBO/VAO.h"
#include "core/opengl/GLProgram.h"

#include <stdlib.h>

FLAKOR_NS_BEGIN

VAO::VAO():
sizePerVertex(0),
vertexNumber(0),
indiceNumber(0),
arrayID(VBO::HARDWARE_BUFFER_ID_INVALID),
vertexs(NULL),
indices(NULL),
autoDispose(true),
dirty(true),
dispose(false),
attrCount(0),
VBOAttributes(NULL)
{}

VAO* VAO::create(int sizePerVertex,unsigned int vertexNumber,unsigned int indiceNumber)
{
    VAO* vao = new VAO();
    if (vao != NULL) {
        vao->sizePerVertex = sizePerVertex;
        vao->vertexNumber = vertexNumber;
        vao->indiceNumber = indiceNumber;
    }
    return vao;
}

void VAO::onBufferData()
{
    
}

void VAO::bind()
{
	glBindVertexArray(arrayID);
}

FLAKOR_NS_END
