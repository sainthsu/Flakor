/****************************************************************************
Copyright (c) 2013-2014 saint

http://www.flakor.org for english
http://www.feike.org for chinese
****************************************************************************/
#ifndef _FK_VBO_H_
#define _FK_VBO_H_

#include "base/lang/Object.h"
#include "core/opengl/GL.h"

FLAKOR_NS_BEGIN

class Array;

/**
 *void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,const GLvoid * pointer);
 * 当绑定ARRAY_BUFFER时，pointer 为Vertex在ARRAY_BUFFER中的offset
*/
struct VBOAttribute
{
	const char* _name;
	int _location;//location in Shader
	int _offset;
	int _size;
	int _type;
	bool _normalized;

	VBOAttribute()
	:_name((const char *)"unnamed"),_location(0),_offset(0),_size(0),_type(0),_normalized(false)
	{
	}

	VBOAttribute(const char* name,int location,int offset,int size,int type)
	:_name(name),_location(location),_offset(offset),_size(size),_type(type),_normalized(false)
	{
	}

	VBOAttribute(const char* name,int location,int offset,int size,int type,bool normalized)
	:_name(name),_location(location),_offset(offset),_size(size),_type(type),_normalized(normalized)
	{
	}

	inline void vertexAttribPointer(int stride)
    {
        glVertexAttribPointer(_location, _size, _type, _normalized, stride, (GLvoid*)_offset);
    }
};

/**
 * VBO =  VertexBufferObject
 */
class VBO : Object
{
	public:
		VBO();
		~VBO();
		const static int HARDWARE_BUFFER_ID_INVALID = -1;

		static VBO* create(int sizePerVertex,int vertexNumber);
		/**
		 * 是否使用后自动销毁
		 * @return true if auto
		 */
		bool isAutoDispose();

		/**
		 * 获取该VBO对应的显卡bufferID
		 * @return int bufferID
		 */
		int getBufferID();
		/**
		 * 是否已载入显卡	
		 * @return true if loaded
		 */
		bool isLoaded();
		/**
		 * 标记VBO对象没有载入显卡硬件。如果要再次使用此VBO，将重载
		 * Mark this {@link VertexBufferObject} as not not loaded to hardware.
		 * It will reload itself to hardware when it gets used again.
		 */
		void setNotLoaded();
		/**
		 * 从显卡上卸载下来
		 * @param glState Opengle 状态
		 */
		void unload();

		/**
		 * 是否VBO数据已过期
		 * @return true if its dirty
		 */
		bool isDirty();

		/**
		 * 标记数据已过期
		 * Mark this {@link VertexBufferObject} dirty so it gets updated on the hardware.
		 */
		void setDirty();

		int getSizePerVertex() const;
		int getVertexNumber() const;

		void setUsage(GLenum usage);

		/**
		 * 浮点数的容量数
		 * @return the number of <code>float</code>s that fit into this {@link VBOInterface}.
		 */
		int getCapacity();

		/**
		 * Byte数的容量数
		 * @return the number of <code>byte</code>s that fit into this {@link VBOInterface}.
		 */
		int getByteCapacity();

		/**
		 * 获取显卡CPU中的Byte大小
		 * @return the number of <code>byte</code>s that are allocated on the GPU.
		 */
		int getGPUMemoryByteSize();

		void setAttributes(struct VBOAttribute **attributes,int count);
		/**
		 * 与显卡bufferID绑定
		 * @param glState
		 */
		/*void bind(GLState glState);
		void bind(GLState glState, ShaderProgram shaderProgram);
		void unbind(GLState glState, ShaderProgram shaderProgram);

		VBOManager* getVBOManager();*/

		/**
		 * 画VBO存储的信息
		 * @param primitiveType int
		 * @param count int
		 */
		void draw(int primitiveType, int count);
		void draw(int primitiveType, int offset, int count);

		void updateData(int index,int size,float data[]);

		void bind();
		virtual void onBufferData();
		void enableAndPointer();

	protected:
		int sizePerVertex;
		int vertexNumber;
    
		GLuint bufferID;
		GLenum usage;
        //是否画完自动废弃。
		bool autoDispose;
		bool dirty;
        //
        bool dispose;

        //实际的bufferdata。存到gpu就清空
		float *bufferData;

		int count;
		VBOAttribute** VBOAttributes;

};

FLAKOR_NS_END

#endif
