#include "pch.h"
#include "OPenGLRenderer.h"

BEGIN_MESSAGE_MAP(OPenGLRenderer, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()


void OPenGLRenderer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 1:
		wglMakeCurrent(m_hdc, m_hrc);
		DrawGLScene();
		SwapBuffers(m_hdc);
		wglMakeCurrent(m_hdc, NULL);
		break;
	default:
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

OPenGLRenderer::OPenGLRenderer()
{
	m_hdc = NULL;
}

OPenGLRenderer::~OPenGLRenderer()
{

}

bool OPenGLRenderer::CreateGLContext(CRect rect, CWnd* parent)
{
	if (!InitContext(parent))
	{
		MessageBox(_T("ERROR Creating InitContext"));
		return false;
	};
	return true;
}

// 컨텍스트 초기화
bool OPenGLRenderer::InitContext(CWnd* parent)
{
	int bits = 16;
	static	PIXELFORMATDESCRIPTOR pfd =					// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,												// Version Number
		PFD_DRAW_TO_WINDOW |							// Format Must Support Window
		PFD_SUPPORT_OPENGL |							// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,								// Must Support Double Buffering
		PFD_TYPE_RGBA,									// Request An RGBA Format
		BYTE(bits),											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,								// Color Bits Ignored
		0,												// No Alpha Buffer
		0,												// Shift Bit Ignored
		0,												// No Accumulation Buffer
		0, 0, 0, 0,										// Accumulation Bits Ignored
		16,												// 16Bit Z-Buffer (Depth Buffer)
		0,												// No Stencil Buffer
		0,												// No Auxiliary Buffer
		PFD_MAIN_PLANE,									// Main Drawing Layer
		0,												// Reserved
		0, 0, 0											// Layer Masks Ignored
	};

	m_hdc = ::GetDC(m_hWnd);
	int nPixelFormat = ChoosePixelFormat(m_hdc, &pfd);
	if (nPixelFormat == 0) return false;
	BOOL bResult = SetPixelFormat(m_hdc, nPixelFormat, &pfd);
	if (!bResult) return false;

	m_hrc = wglCreateContext(m_hdc);
	wglMakeCurrent(m_hdc, m_hrc);

	if (!m_hrc)
		return false;

	CString str;
	str.Format(_T("OpenGL version: %s\n"), (CString)glGetString(GL_VERSION));
	TRACE(str);
	return true;
}

// 초기 씬 준비
void OPenGLRenderer::PrepareScene(int sx, int sy, int cx, int cy)
{
	glClearColor(0.0, 0.0, 1.0, 0.0); //background to clear with.
	wglMakeCurrent(m_hdc, m_hrc);

	GLfloat fAspect;

	if (cy == 0) cy = 1;

	glViewport(sx, sy, cx, cy);

	fAspect = (GLfloat)cx / (GLfloat)cy;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0f, fAspect, 1.0f, 10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, 0.1, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LINE_STIPPLE);//enable or disable server-side GL capabilities. 
	//glLineStipple(1, 0xF0F0);//specify the line stipple pattern 

	wglMakeCurrent(m_hdc, NULL);
}

// 초기 설정 작업
bool OPenGLRenderer::initAi()
{
	wglMakeCurrent(m_hdc, m_hrc);

	/*createAILogger();
	logInfo("App fired!");

	if (!Import3DFromFile())
		return 0;
	if (!LoadGLTextures(scene))
		return FALSE;

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
									 //glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClearDepth(1.0f);				// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);*/
	glEnable(GL_LIGHT1);

	wglMakeCurrent(m_hdc, NULL);

	xrot = 0.0;
	yrot = 0.0;
	zrot = 0.0;

	return true;
}

// 그리기 메소드
int OPenGLRenderer::DrawGLScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //배경 클리어
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 버퍼 비트 클리어
	glLoadIdentity();
	
	///
	glPushMatrix();
	glTranslatef(0, 0, -2);//정점의 이동
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);//회전 변환
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glRotatef(zrot, 0.0f, 0.0f, 1.0f);
	glScalef(0.25, 0.30, 0.25);

	// red 
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, 1, -1);
	glVertex3f(-1, 1, -1);
	glEnd();

	// green
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, 1);
	glEnd();

	// blue
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(-1, -1, -1);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, -1, 1);
	glVertex3f(1, 1, -1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, -1);
	glVertex3f(-1, 1, 1);
	glEnd();

	// Red coordinate (붉은색 좌표)
	glColor3f(1, 0, 0);
	glLineWidth(1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(8.0, 0.0, 0.0);
	glVertex3f(-8.0, 0.0, 0.0);
	glEnd();

	// Green coordinate (초록색 좌표)
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 8.0, 0.0);
	glVertex3f(0.0, -8.0, 0.0);
	glEnd();

	// Blue coordinate (파란색 좌표)
	glColor3f(0, 0, 1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 0.0, 8.0);
	glVertex3f(0.0, 0.0, -8.0);
	glEnd();

	// 낙하산 이음부
	glColor3f(1, 1, 0);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(1.0, 2.0, 0.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 2.0, -1.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 2.0, 0.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 2.0, 1.0);
	glEnd(); 

	// 낙하산 반구
	createHalfSphere(0.0, 100.0, 0.0, 1.0);

	glPopMatrix();
	glFlush();

	return TRUE;
}

//반구 그리기
void OPenGLRenderer::createHalfSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius) { 

	/*function createHalfSphere()     
		구의 중심 x, y, z 좌표를 받아 반구를 만드는 함수     
		x : 반구의 중심 x 좌표
		y : 반구의 중심 y 좌표
		z : 반구의 중심 z 좌표
		raidus : 반구의 반지름     
	*/  
	GLfloat angley; //y축 값을 구하기 위한 각도    
	GLfloat nexty; //다음 y축 값을 구하기 위한 각도     
	GLfloat anglex; //x, y축 값을 구하기 위한 각도     
	glColor3ub(255, 255, 0); //반구의 색 지정     
	glBegin(GL_QUAD_STRIP);     
	for(angley = 0.0f; angley <= (0.5f*GL_PI); angley += ((0.5f*GL_PI)/8.0f)) //반구만 그려야 하므로 0.5곱함
	{         
		y = radius*sin(angley); //y축 값 계산
		nexty = angley+((0.5f*GL_PI)/8.0f); //다음 angley값 저장
		for(anglex = 0.0f; anglex < (2.0f*GL_PI); anglex += (GL_PI/8.0f))
		{             
			x = radius*cos(angley)*sin(anglex);             
			z = radius*cos(angley)*cos(anglex);            
			glNormal3f(-cos(angley)*sin(anglex), -sin(angley), -cos(angley)*cos(anglex)); //반구의 안쪽으로 normal 벡터 생성             
			glVertex3f(x, y+2, z);             
			x = radius*cos(nexty)*sin(anglex);             
			z = radius*cos(nexty)*cos(anglex);             
			glNormal3f(-cos(nexty)*sin(anglex), -sin(nexty), -cos(nexty)*cos(anglex));            
			glVertex3f(x, radius*sin(nexty)+2, z);         
		}     
	}     
	glEnd(); 
}

// 씬 삭제
void OPenGLRenderer::DestroyScene()
{
	DestroyWindow();

	wglMakeCurrent(m_hdc, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
