#ifndef CUDA_GL_INTEROP_H__
#define CUDA_GL_INTEROP_H__

#include <OpenGL/GLU.h>
#include <OpenGL/GL.h>
#include "CudaHelper.h"

/*
* This class is written purely for the purpose of compiling CUDA source code
* without having to include related header to the main project
*/
class CudaGLInteropHelper {
public:
	CudaGLInteropHelper() {
		cuHelper = new CUDAHelper();
	};
	~CudaGLInteropHelper() {
		delete cuHelper;
	};

	void SetDevice(int dev_num);
	void RegisterBufferObject( GLuint* pbo );
	void UnregisterBufferObject( GLuint* pbo );

	void MapBufferObject(GLuint* pbo, int h, int w);
	void UnmapBufferObject(GLuint* pbo);
	void GenerateGLPBO(uchar4* dptr, int h, int w);

private:
	CUDAHelper* cuHelper;

};

#endif