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

FLAKOR_NS_BEGIN

class VAO : public Object
{
protected:
	int sizePerVertex;
	int vertexNumber;
	GLuint arrayID;
	unsigned short count;
	GLuint bufferID[2];//0: vertex  1: indices

	float *vertexs;
	GLushort *indices;

	//是否画完自动废弃。
	bool autoDispose;
	bool dirty;
    //
	bool dispose;

public:
	static VAO* create(int sizePerVertex,int vertexNumber);
	void bind();
	
};

FLAKOR_NS_END

#endif

