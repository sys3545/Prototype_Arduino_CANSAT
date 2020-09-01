
// testMFCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "testMFC.h"
#include "testMFCDlg.h"
#include <math.h>
#include <stdlib.h>
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestMFCDlg 대화 상자



CtestMFCDlg::CtestMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_test = NULL;
}

void CtestMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPENGL, m_pLeft);
	DDX_Control(pDX, IDC_BUTTON1, m_wing);
	DDX_Control(pDX, IDC_BUTTON2, m_wing2);
}

BEGIN_MESSAGE_MAP(CtestMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CtestMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CtestMFCDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CtestMFCDlg 메시지 처리기

BOOL CtestMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetWindowText(_T("Ground Control - SHIM EUN SONG")); // Window title 설정
	SetBackgroundColor(RGB(255, 255, 255)); // Window 배경색 설정
	SetDlgItemText(IDC_EDIT_ONOFF, _T("OFF")); // edit control 초기값 설정
	
	m_wing.LoadBitmaps(IDB_BITMAP2, NULL, NULL, NULL); // 연결 버튼 그림설정
	m_wing.SizeToContent();// 크기 맞추기
	m_wing2.LoadBitmaps(IDB_BITMAP3, NULL, NULL, NULL); // 연결해제 버튼 그림설정
	m_wing2.SizeToContent();// 크기 맞추기
	
	// 오실로스코프 컨트롤이 위치할 영역 가져오기 - 초기화
	CRect rtGraph;
	GetDlgItem(IDC_STATIC_RT_GRAPH)->GetWindowRect(rtGraph);

	ScreenToClient(rtGraph);

	_rtGraph = new COScopeCtrl(3);		//cos,sin,tan 3개의 그래프 예약
	_rtGraph->Create(WS_VISIBLE | WS_CHILD, rtGraph, this, IDC_STATIC_RT_GRAPH);
	_rtGraph->SetRanges(400., 1000.);
	_rtGraph->autofitYscale = true;
	_rtGraph->SetYUnits("");
	_rtGraph->SetXUnits("Time");
	_rtGraph->SetLegendLabel("LDR", 0);
	_rtGraph->SetLegendLabel("LED", 1);
	_rtGraph->SetLegendLabel("test", 2);
	_rtGraph->SetPlotColor(RGB(255, 0, 0), 0);
	_rtGraph->SetPlotColor(RGB(0, 255, 0), 1);
	_rtGraph->SetPlotColor(RGB(0, 0, 255), 2);
	_rtGraph->InvalidateCtrl();

	SetTimer(1000, 10, NULL); // 타이머 셋팅

	// OpenGL 생성 및 초기화 작업
	CRect rectLeft;
	int iWidth, iHeight;

	m_pLeft.GetWindowRect(rectLeft);
	iWidth = rectLeft.Width();
	iHeight = rectLeft.Height();

	m_test = new OPenGLRenderer;
	CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, (HBRUSH)GetStockObject(WHITE_BRUSH), NULL);
	m_test->CreateEx(0, className, _T("OpenGLCube"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rectLeft, this, 0);
	m_test->ShowWindow(SW_SHOW);

	m_test->CreateGLContext(rectLeft, this);
	m_test->PrepareScene(0, 0, iWidth, iHeight); //will show without this but as black & white.
	m_test->SetTimer(1, 10, 0);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CtestMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CtestMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CtestMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 오실로스코프 관련 함수
void CtestMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (state) { // 수신 값 받아오기
		readResult = SP->ReadData(incomming, 13);
		if (readResult && incomming[0] == 'M') {
			incomming[readResult] = 0;
			str = incomming;
			
			if (incomming[1] - '0' > 0) {
				input = pharsing(incomming);
				SetDlgItemText(IDC_EDIT_MSG, str);
			}
		}
	}
	// 수신값 그래프 출력, OpenGL 변수 설정
	if (nIDEvent == 1000 && state) {
		double t = (double)GetTickCount() / 1000.;

		double value[3] = { input.LDR, input.LED, 400 };

		_rtGraph->AppendPoints(value);
		m_test->xrot += input.LDR / 1000;
		m_test->yrot += input.LDR / 1000;
		m_test->zrot += input.LDR / 1000;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CtestMFCDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 오실로스코프 삭제
	delete _rtGraph;
}

HBRUSH CtestMFCDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.

	return hbr;
}

data CtestMFCDlg::pharsing(char msg[]) 
{
	data temp;

	temp.LDR = (float)(msg[1] - '0') * 100;
	temp.LDR += (float)(msg[2] - '0') * 10;
	temp.LDR += (float)(msg[3] - '0');
	temp.LED = (float)(msg[4] - '0') * 600;
	
	return temp;
}


////////////////////////////////////////// Button ////////////////////////////////////////////////

void CtestMFCDlg::OnBnClickedButton1()
{
	SP = new Cmycomm("\\\\.\\COM5");

	if (SP->IsConnected()) {
		state = true;
		MessageBox(_T("Successful connected!"), _T("Caption"), MB_ICONINFORMATION);
		SetDlgItemText(IDC_EDIT_ONOFF, _T("ON"));
	}

}

void CtestMFCDlg::OnBnClickedButton2()
{	
	if (SP->IsConnected()) {
		SP->~Cmycomm();
		state = false;
		MessageBox(_T("Port Closed!"), _T("Caption"), MB_ICONINFORMATION);
		SetDlgItemText(IDC_EDIT_ONOFF, _T("OFF"));
	}
}
