
#ifndef _FK_MATRIX_STACK_H_
#define _FK_MATRIX_STACK_H_

#include "./Matrices.h"

static MatrixStack* mainStack = NULL;

class MatrixStack :: Object.h
{
	private:
		int capacity;
		int itemCount;
		Matrix* top;
		Matrix* stack;
	public:
		MatrixStack(void);
		~MatrixStack(void);
		initialize();
		push(const Matrix4* matrix);
		pop(Matrix4* outMatrix);
		release();
}

#endif // define  _FK_MATRIX_STACK_H_
