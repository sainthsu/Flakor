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

bool VAO::isAutoDispose()
{
    return autoDispose;
}

int VAO::getArrayID()
{
    return arrayID;
}

bool VAO::isLoaded()
{
    return arrayID != VBO::HARDWARE_BUFFER_ID_INVALID;
}

void VAO::setNotLoaded()
{
    
}

void VAO::unload()
{
    
}

bool VAO::isDirty()
{
    return dirty;
}

void VAO::setDirty()
{
    
}

int VAO::getSizePerVertex() const
{
    return sizePerVertex;
}

int VAO::getVertexNumber() const
{
    return vertexNumber;
}

void VAO::onBufferData()
{
    
}

void VAO::bind()
{
	glBindVertexArray(arrayID);
}

void VAO::setUsage(GLenum usage)
{
    
}

void setVertexData(int index,int size,float data[])
{
    
}

void setIndexData(GLushort *data,int length)
{
    
}

//draw VAO
void draw(GLenum mode, int count);
void draw(GLenum mode, int count, int offset);

FLAKOR_NS_END
