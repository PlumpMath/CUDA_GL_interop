#ifndef _CUDA_HELPER_H__
#define _CUDA_HELPER_H__
#include <OpenGL/GLU.h>
#include <OpenGL/GL.h>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

class CUDAHelper {
public:
	CUDAHelper() {
	
	};
	~CUDAHelper() {
	};
	void GeneratePBO(uchar4* dptr, int h, int w);
	int iDivUp(int a, int b){ return ((a % b) != 0) ? (a / b + 1) : (a / b); }
private:
};

#endif