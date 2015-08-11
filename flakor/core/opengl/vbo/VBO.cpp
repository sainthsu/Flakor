#include "targetMacros.h"
#include "core/opengl/vbo/VBO.h"
#include "core/opengl/GLProgram.h"

#include <stdlib.h>

FLAKOR_NS_BEGIN

/**
 * VBO =  VertexBufferObject
 */

VBO::VBO():
sizePerVertex(0),
vertexNumber(0),
bufferID(HARDWARE_BUFFER_ID_INVALID),
usage(GL_STATIC_DRAW),
autoDispose(true),
dirty(true),
dispose(false),
count(0),
VBOAttributes(NULL)
{
}

VBO::~VBO()
{
	
}

VBO* VBO::create(int sizePerVertex,int vertexNumber)
{
	VBO* v = new VBO();
	if(v != NULL)
	{
		v->sizePerVertex = sizePerVertex;
		v->vertexNumber = vertexNumber;
        v->bufferData = (float *)malloc(sizePerVertex*vertexNumber*sizeof(float));
	}
	return v;
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

void VBO::setAttributes(struct VBOAttribute **attributes,int count)
{
	this->count = count;

	VBOAttributes = (VBOAttribute **)malloc(count*sizeof(VBOAttribute *));
	int i;
	for(i=0;i<count;i++)
	{
		VBOAttributes[i] = attributes[i];
	}
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

void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER,bufferID);
}

void VBO::setUsage(GLenum usage)
{
	this->usage = usage;
}

void VBO::updateData(int index,int size,float data[])
{
	int i,j;
	for(i=0;i<vertexNumber;i++)
	{
		for(j=0;j<size;j++)
			bufferData[index+i*sizePerVertex+j] = data[i*size+j]; 
	}
    dirty = true;
}

void VBO::onBufferData()
{
	int size = sizePerVertex*vertexNumber*sizeof(float);
	if(!isLoaded())
	{
		glGenBuffers(1,&bufferID);
		glBindBuffer(GL_ARRAY_BUFFER,bufferID);

		glBufferData(GL_ARRAY_BUFFER,size,bufferData,usage);
        dirty = false;
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER,bufferID);
		if(dirty)
			glBufferSubData(GL_ARRAY_BUFFER,0,size,bufferData);
	}
}

void VBO::enableAndPointer()
{
	int i;
	for(i=0;i<count;i++)
	{
		VBOAttribute *attri = VBOAttributes[i];
        FKLOG("%s index: %d",attri->_name,attri->_location);
		glEnableVertexAttribArray(attri->_location);
		glVertexAttribPointer(attri->_location,attri->_size,GL_FLOAT,attri->_normalized,sizePerVertex*sizeof(float),reinterpret_cast<GLvoid*>(attri->_offset));
	}
}

void VBO::draw(int primitiveType, int count)
{
	glDrawArrays(primitiveType,0,count);
}

void VBO::draw(int primitiveType, int offset, int count)
{
	glDrawArrays(primitiveType,offset,count);
}

FLAKOR_NS_END
