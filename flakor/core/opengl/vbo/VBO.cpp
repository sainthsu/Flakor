#include "targetMacros.h"
#include "core/opengl/vbo/VBO.h"

FLAKOR_NS_BEGIN

/**
 * VBO =  VertexBufferObject
 */

VBO::VBO():
sizePerVertex(0),
vertexNumber(0),
bufferID(HARDWARE_BUFFER_ID_INVALID),
usage(0),
autoDispose(true),
dirty(true),
dispose(false),
VBOAttibutes(NULL)
{
}

VBO::~VBO()
{
	
}

VBO VBO::create(int sizePerVertex,int vertexNumber)
{
	VBO* v = new VBO();
	if(v != NULL)
	{
		v->sizePerVertex = sizePerVertex;
		v->vertexNumber = vertexNumber;
	}
	return *v;
}

/**
* 是否使用后自动销毁
* @return true if auto
*/
bool VBO::isAutoDispose()
{
	return autoDispose;
}

/**
* 获取该VBO对应的显卡bufferID
* @return int bufferID
		 */
int VBO::getBufferID()
{
	return bufferID;
}
/**
* 是否已载入显卡	
* @return true if loaded
*/
bool VBO::isLoaded()
{
	return bufferID != -1;
}

void VBO::setNotLoaded()
{

}

void VBO::unload()
{

}

bool VBO::isDirty()
{
	return dirty;
}

void VBO::setDirty()
{
	dirty = true;
}

int VBO::getSizePerVertex() const
{
	return sizePerVertex;
}
		
int VBO::getVertexNumber() const
{
	return vertexNumber;
}

int VBO::getCapacity()
{
	return 0;
}

int VBO::getByteCapacity()
{
	return 0;
}

int VBO::getGPUMemoryByteSize()
{
	return 0;
}

/*
void VBO::bind(GLState glStatae)
{
}

void VBO::bind(GLState glState, ShaderProgram shaderProgram)
{

}

void VBO::unbind(GLState glState, ShaderProgram shaderProgram)
{

}

VBOManager* VBO::getVBOManager()
{

}
*/
		
void VBO::draw(int primitiveType, int count)
{

}
void VBO::draw(int primitiveType, int offset, int count)
{

}

FLAKOR_NS_END
