#include "cudaGLInteropHelper.h"
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

void CudaGLInteropHelper::RegisterBufferObject( GLuint* pbo ) {
	cudaGLRegisterBufferObject(*pbo);
}

void CudaGLInteropHelper::UnregisterBufferObject( GLuint* pbo ) {
	cudaGLUnregisterBufferObject(*pbo);
}

void CudaGLInteropHelper::SetDevice(int dev_num) {
	cudaGLSetGLDevice(dev_num);
}

void CudaGLInteropHelper::GenerateGLPBO(uchar4* dptr, int h, int w) {
	cuHelper->GeneratePBO(dptr, h, w);
}

void CudaGLInteropHelper::MapBufferObject(GLuint* pbo, int h, int w) {
	uchar4* dptr = nullptr;
	cudaGLMapBufferObject((void**)&dptr, *pbo);
	this->GenerateGLPBO(dptr, h, w);
}

void CudaGLInteropHelper::UnmapBufferObject(GLuint* pbo) {
	cudaGLUnmapBufferObject(*pbo);
}