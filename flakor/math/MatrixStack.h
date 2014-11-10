
#ifndef _FK_MATRIX_STACK_H_
#define _FK_MATRIX_STACK_H_

#include "math/Matrices.h"

FLAKOR_NS_BEGIN

class MatrixStack
{
	private:
		int capacity;
		int itemCount;
		Matrix4* top;
		Matrix4* stack;
	public:
		MatrixStack(void);
		~MatrixStack(void);
		void initialize();
		void push(const Matrix4* matrix);
		void pop(Matrix4* outMatrix);
		void release();
};

FLAKOR_NS_END

#endif // define  _FK_MATRIX_STACK_H_
