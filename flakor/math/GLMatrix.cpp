#include "macros.h"
#include <assert.h>
#include "math/GLMatrix.h"

FLAKOR_NS_BEGIN

MatrixStack* modelviewStack;
MatrixStack* projectionStack;
MatrixStack* textureStack;

MatrixStack* currentStack = NULL;

static unsigned char initialized = 0;

#ifdef __cplusplus
extern "C" {
#endif

void lazyInitialize()
{

    if (!initialized) 
	{
        Matrix4* identity = new Matrix4(); //Temporary identity matrix

        //Initialize all 3 stacks
        modelviewStack = new MatrixStack();
        projectionStack = new MatrixStack();
        textureStack = new MatrixStack();

        currentStack = modelviewStack;
        initialized = 1;

        //Make sure that each stack has the identity matrix
        modelviewStack->push(identity);
        projectionStack->push(identity);
        textureStack->push(identity);
    }
}

void GLMode(StackMode mode)
{
    lazyInitialize();

    switch(mode)
    {
        case GL_MODELVIEW:
            currentStack = modelviewStack;
        break;
        case GL_PROJECTION:
            currentStack = projectionStack;
        break;
        case GL_TEXTURE:
            currentStack = textureStack;
        break;
        default:
            assert(0 && "Invalid matrix mode specified"); //TODO: Proper error handling
        break;
    }
}

void GLPush(void)
{
    Matrix4* top = new Matrix4();;

    lazyInitialize(); //Initialize the stacks if they haven't been already

    //Duplicate the top of the stack (i.e the current matrix)
    top->set(currentStack->top->get(),COLUMN_MAJOR);
    currentStack->push(top);
}

void GLPop(void)
{
    assert(initialized && "Cannot Pop empty matrix stack");
    //No need to lazy initialize, you shouldn't be popping first anyway!
    currentStack->pop(NULL);
}

void GLLoadIdentity()
{
    lazyInitialize();

    currentStack->top->identity(); //Replace the top matrix with the identity matrix
}

void GLFreeAll()
{
    //Clear the matrix stacks
    modelviewStack->release();
    projectionStack->release();
    textureStack->release();

    //Delete the matrices
    initialized = 0; //Set to uninitialized

    currentStack = NULL; //Set the current stack to point nowhere
}

void GLMultiply(const Matrix4* in)
{
    lazyInitialize();
    currentStack->top = &((*currentStack->top)*(*in));
}

void GLLoad(const Matrix4* in)
{
    lazyInitialize();
	
    in->set(currentStack->top->get(),COLUMN_MAJOR);
}

void GLGet(StackMode mode, Matrix4* out)
{
    lazyInitialize();

    switch(mode)
    {
        case GL_MODELVIEW:
            out->set(modelviewStack->top->get(),COLUMN_MAJOR);
        break;
        case GL_PROJECTION:
            out->set(projectStack->top->get(),COLUMN_MAJOR);
        break;
        case GL_TEXTURE:
            out->set(textureStack->top->get(),COLUMN_MAJOR);
        break;
        default:
            assert(1 && "Invalid matrix mode specified"); //TODO: Proper error handling
        break;
    }
}

void GLTranslatef(float x, float y, float z)
{
    Matrix4 *translation = new Matrix4();

    //Create a rotation matrix using the axis and the angle
    translation->translate(x,y,z);

    //Multiply the rotation matrix by the current matrix
    currentStack->top=(*currentStack->top)*(*translation);
}

void GLRotatef(float angle, float x, float y, float z)
{
	//Create an axis vector
    Vector3* axis = new Vector3(x, y, z);
    Matrix4* rotation = new Matrix4();

    //Create a rotation matrix using the axis and the angle
    rotation->rotate(angle, &axis);

    //Multiply the rotation matrix by the current matrix
    currentStack->top= (*currentStack->top)*(*rotation);
}

void GLScalef(float x, float y, float z)
{
    Matrix4* scaling = new Matrix4();
    scaling->scale(x, y, z);

    currentStack->top= (*currentStack->top)*(*scaling);
}

#ifdef __cplusplus
}
#endif

FLAKOR_NS_END
