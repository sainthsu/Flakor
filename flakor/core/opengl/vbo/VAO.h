/*
 VAO Vertex Array Object-VAO，VAO是一个对象，其中包含一个或者更多的Vertex Buffer Objects
 glGenVertexArrays 创建一个Vertex Array Object,
 然后使用glBindVertexArray绑定VAO，一旦VAO绑定后，
 使glGenBuffers 创建一个Vertex Buffer Object, 
 当然仍然需要使用glBindBuffer绑定VBO；
    顺序如下：
    1. Generate Vertex Array Object
    2. Bind Vertex Array Object
    3. Generate Vertex Buffer Object
    4. Bind Vertex Buffer Object
*/
#ifndef _FK_VAO_H_
#define _FK_VAO_H_

#include "base/lang/Object.h"
#include "core/opengl/GL.h"


FLAKOR_NS_BEGIN

struct VBOAttribute;

/**
 *VAO = Vertex Array Object
 */
class VAO : public Object
{
protected:
	unsigned int sizePerVertex;
	unsigned int vertexNumber;
    unsigned int indiceNumber;
    
	GLuint arrayID;
	GLuint bufferID[2];//0: vertex  1: indices
	GLenum usage[2];//0: vertex  1: indices

	float *vertexs;
	GLushort *indices;

    int attrCount;
    VBOAttribute** VBOAttributes;

	//是否画完自动废弃。
	bool autoDispose;
	bool dirty;
    //
	bool dispose;

public:
	static VAO* create(int sizePerVertex,unsigned int vertexNumber,unsigned int indiceNumber);

    VAO();
    
        /**
         * 是否使用后自动销毁
         * @return true if auto
         */
        bool isAutoDispose();

        /**
         * 获取该VAO对应的显卡arrayID
         * @return int arrayID
         */
        int getArrayID();
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

		void bind();
        virtual void onBufferData();
		void setUsage(GLenum usage);
        void setVertexData(int index,int size,float data[]);
		void setIndexData(GLushort *data,int length);

        //draw VAO
        void draw(GLenum mode, int count);
        void draw(GLenum mode, int count, int offset);
};

FLAKOR_NS_END

#endif

