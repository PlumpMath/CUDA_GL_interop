#include "CudaHelper.h"
#include <iostream>

__global__ void fill_buffer(uchar4 * d_dst, int w, int h) {
	int blockId = blockIdx.x + blockIdx.y * gridDim.x;
	int threadId = blockId * (blockDim.x * blockDim.y)
	 			 + (threadIdx.y * blockDim.x) + threadIdx.x;

	 if (threadId < w*h) {

	 	d_dst[threadId].x = 255; //magRed[i*w + j];
		d_dst[threadId].y = 255;
		d_dst[threadId].z = 255;  //magBlue[i*w + j];
		d_dst[threadId].w = 255;
	 }
}

void CUDAHelper::GeneratePBO(uchar4* dptr, int h, int w) {
	const int BLOCKDIM_X = 32;
	const int BLOCKDIM_Y = 32;

	dim3 grid(iDivUp(w, BLOCKDIM_X), iDivUp(h, BLOCKDIM_Y));
	dim3 block(BLOCKDIM_X, BLOCKDIM_Y);
	// std::cout << "before launching kernel" << std::endl;
	fill_buffer<<< grid, block >>> (dptr, h, w);
}