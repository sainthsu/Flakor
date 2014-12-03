
#include "macros.h"
#include "math/MatrixStack.h"

#define MATRIX_STACK_INITIAL_SIZE 30
#define MATRIX_STACK_INCREMENT 50

FLAKOR_NS_BEGIN

MatrixStack::MatrixStack(void)
{
	//allocate memory
	this->stack = (Matrix4 *)malloc(sizeof(Matrix4) * MATRIX_STACK_INITIAL_SIZE);
	this->capacity = MATRIX_STACK_INITIAL_SIZE;
	this->itemCount = 0;
	this->top = NULL;
}

MatrixStack::~MatrixStack()
{
	free(this->stack);
	this->capacity = 0;
	this->itemCount = 0;
	this->top = NULL;
}

void MatrixStack::push(const Matrix4 *matrix)
{
	this->top = &this->stack[this->itemCount];
	//FKAssert(this->top == matrix,"pointer not same type!");
	memcpy(this->top,matrix,sizeof(Matrix4));
	this->itemCount++;

	if(this->capacity <= this->itemCount)
	{
		Matrix4* tmp = NULL;
		this->capacity += MATRIX_STACK_INCREMENT;
		tmp = this->stack;
		this->stack = (Matrix4*)malloc(this->capacity*sizeof(Matrix4));
		memcpy(this->stack,tmp,sizeof(Matrix4)*(this->capacity - MATRIX_STACK_INCREMENT));
		this->top = &this->stack[this->itemCount - 1];
	}

	this->stack[itemCount] = *matrix;
}

void MatrixStack::pop(Matrix4 *outMatrix)
{
	FKAssert(this->itemCount > 0,"Cannot pop an empty stack");

	if(outMatrix != NULL)
	{
		outMatrix = &this->stack[--this->itemCount];
	}
	else
	{
		this->itemCount--;
	}
	this->top = &this->stack[this->itemCount - 1];
}

void MatrixStack::release(void)
{
	free(this->stack);
    this->top = NULL;
    this->itemCount = 0;
    this->capacity = 0;
}

FLAKOR_NS_END
