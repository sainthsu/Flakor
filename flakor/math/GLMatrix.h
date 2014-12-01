#ifndef _FK_GLMATRIX_H_
#define _FK_GLMATRIX_H_

#include "math/MatrixStack.h"

#define GL_MODELVIEW 0x1700
#define GL_PROJECTION 0x1701
#define GL_TEXTURE 0x1702

typedef unsigned int StackMode;

FLAKOR_NS_BEGIN

#ifdef __cplusplus
extern "C" {
#endif

void lazyInitialize();
void GLMode(StackMode mode);
void GLPush(void);
void GLPop(void);
void GLLoadIdentity();

void GLFreeAll();
void GLMultiply(const Matrix4* in);
void GLLoad(Matrix4* in);
void GLGet(StackMode mode, Matrix4* out);
void GLTranslatef(float x, float y, float z);

void GLRotatef(float angle, float x, float y, float z);
void GLScalef(float x, float y, float z);

#ifdef __cplusplus
}
#endif

FLAKOR_NS_END

#endif
