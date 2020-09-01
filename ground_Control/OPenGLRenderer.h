#pragma once
#include <afxwin.h>
#include <glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <math.h>

class OPenGLRenderer : public CWnd
{
public:
	OPenGLRenderer();
	virtual ~OPenGLRenderer();
	bool CreateGLContext(CRect rect, CWnd* parent);
	void PrepareScene(int sx, int sy, int cx, int cy);				// Scene preparation
	void DestroyScene();
	int DrawGLScene();				//Here's where we do all the drawing
	void createHalfSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius); // draw half shpere
	bool initAi();

public:
	GLfloat		xrot = 0;
	GLfloat		yrot = 0;
	GLfloat		zrot = 0;
	GLfloat		GL_PI = 3.141592;
protected:
	bool InitContext(CWnd* parent);					 // Creates OpenGL Rendering Context

	HGLRC m_hrc;                        // OpenGL Rendering Context 
	HDC m_hdc;
	CRect m_rect;


public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

