
// testMFCDlg.h: 헤더 파일
//

#pragma once
#include "Cmycomm.h"
#include "OScopeCtrl.h"

typedef struct {
	float LDR = 0;
	float LED = 0;
}data;

// CtestMFCDlg 대화 상자
class CtestMFCDlg : public CDialogEx
{
// 생성입니다.
	Cmycomm* SP; // 통신 시리얼 객체
	COScopeCtrl* _rtGraph; // 오실로스코프 그래프 객체
	bool state = false;
	CString str;
	char incomming[13]="";
	int readResult = 0;
	data input;
public:
	CtestMFCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	data pharsing(char msg[]);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButtonSeon();
	afx_msg void OnBnClickedButtonFold();
};
