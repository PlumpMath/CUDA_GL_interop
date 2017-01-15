#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>		// used for Qt OpenGL functions
// #ifdef MACOSX
#include <OpenGL/GLU.h>
#include <OpenGL/GL.h>

#include "cudaGLInteropHelper.h"
// #elif WIN32
// #include <gl/GLU.h>
// #include <gl/GL.h>
// #endif
// haven't tested on linux yet

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    // handle pixel buffer object creation and delation
	void createPBO(GLuint* pbo, int width, int height);
	void deletePBO(GLuint* pbo);
	void createTexture(GLuint* textureID, int width, int height);


	void SetBuffer(GLuint* pbo, uint8_t* imageData, int width, int height);

    // Pointer to GPU image data
    uint8_t* m_imageData;

    // used by OpenGL for rendering image to display
	GLuint m_pbo;
	GLuint m_textureID;
private:
	CudaGLInteropHelper* cuGLHelper;

	float translate_x;
	float translate_y;
};

#endif // OGLWIDGET_H