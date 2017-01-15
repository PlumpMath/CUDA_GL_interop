#include "GLWidget.h"
#include <iostream>
// for CUDA OpenGL interop
// #include <cuda.h>
// #include <cuda_runtime.h>
// #include <cuda_gl_interop.h>


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    cuGLHelper = new CudaGLInteropHelper();

    translate_x = 0.0f;
    translate_y = 0.0f;


    
}

GLWidget::~GLWidget()
{
    delete cuGLHelper;
}

void GLWidget::createTexture(GLuint* textureID, int width, int height) {
    std::cout << "Creating texture" << std::endl;
    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    // Generate a texture ID
    glGenTextures(1, textureID);

    // Make this the current texture (remember that GL is state-based)
    glBindTexture(GL_TEXTURE_2D, *textureID);

    // Allocate the texture memory. The last parameter is NULL since we only
    // want to allocate memory, not initialize it
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

    // Must set the filter mode, GL_LINEAR enables interpolation when scaling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Note: GL_TEXTURE_RECTANGLE_ARB may be used instead of
    // GL_TEXTURE_2D for improved performance if linear interpolation is
    // not desired. Replace GL_LINEAR with GL_NEAREST in the 
    // glTexParameteri() call
    std::cout << "after Creating texture" << std::endl;
}

void GLWidget::createPBO(GLuint* pbo, int width, int height) {

    if (pbo) {
        std::cout << "Creating PBO" << std::endl;
        // set up vertex data parameter
        int num_texels = width * height;
        int num_values = num_texels * 4;
        int size_tex_data = sizeof(GLubyte) * num_values;

        // Generate a buffer ID called a PBO (Pixel Buffer Object)
        glGenBuffers(1, pbo);
        // Make this the current UNPACK buffer (OpenGL is state-based)
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *pbo);
        // Allocate data for the buffer. 4-channel 8-bit image
        glBufferData(GL_PIXEL_UNPACK_BUFFER, size_tex_data, NULL, GL_DYNAMIC_COPY);
        

        //cudaGLRegisterBufferObject(*pbo);
        cuGLHelper->RegisterBufferObject( pbo );
    }
}

void GLWidget::initializeGL()
{
    cuGLHelper->SetDevice(0);
    initializeOpenGLFunctions();



    glViewport(0, 0, 256, 256);



    glClearColor(1.0f,1.0f,1.0f,1.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    // glDisable(GL_CULL_FACE);

    // set orthographic projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);

    // set view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    

    // init CUDA GL device
    // cudaGLSetGLDevice(0);
    

    // initialize memory used for rendering to display
    createPBO(&m_pbo, 256, 256);
    createTexture(&m_textureID, 256, 256);

    // glEnable(GL_LIGHT0);
    // glEnable(GL_LIGHTING);
    // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    // glEnable(GL_COLOR_MATERIAL);
}

void GLWidget::deletePBO(GLuint* pbo) {

    if (pbo) {
        // unregister this buffer object with CUDA
        
        //cudaGLUnregisterBufferObject(*pbo);
        cuGLHelper->UnregisterBufferObject( pbo );

        // bind pbo to currnt array buffer and delete it
        glBindBuffer(GL_ARRAY_BUFFER, *pbo);
        glDeleteBuffers(1, pbo);

        pbo = NULL;
    }
}

void GLWidget::SetBuffer(GLuint* pbo, uint8_t* imageData, int width, int height) {
    // uchar4* dptr = nullptr;

    // map OpenGL buffer object for writing from CUDA on a single GPU
    // no data is moved. When mapped to CUDA, OpenGL 
    // should not use this buffer
    //cudaGLMapBufferObject((void**)&dptr, *pbo);
    // cuGLHelper->MapBufferObject(&dptr, pbo);
    cuGLHelper->MapBufferObject(pbo, height, width);
    // execute the kernel
    cuGLHelper->UnmapBufferObject(pbo);

    // unmap buffer object
    // cudaGLUnmapBufferObject(*pbo);
    // cuGLHelper->UnmapBufferObject(pbo);
}

void GLWidget::paintGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    this->SetBuffer(&m_pbo, nullptr, 256, 256);

    // Create a texture from the buffer
    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);

    // bind texture from PBO
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Note: glTexSubImage2D will perform a format conversion if the
    // buffer is a different format from the texture. We created the
    // texture with format GL_RGBA8. In glTexSubImage2D we specified
    // GL_BGRA and GL_UNSIGNED_INT. This is a fast-path combination

    // Note: NULL indicates the data resides in device memory
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw a single Quad with texture coordinates for each vertex.
    // glBegin(GL_QUADS);
    // glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
    // glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
    // glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
    // glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 0.0f, 0.0f);
    // glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-0.5, -0.5, 0);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f( 0.5, -0.5, 0);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f( 0.0,  0.5, 0);
    glEnd();

    this->update();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,0,0,0,0,1,0);
}