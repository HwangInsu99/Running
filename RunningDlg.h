
// RunningDlg.h: 헤더 파일
//

#include "SocCom.h"

#pragma once
class Man;
class Object;

// CRunningDlg 대화 상자
class CRunningDlg : public CDialogEx
{
// 생성입니다.
public:
	CRunningDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	SocCom m_socCom;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RUNNING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LPARAM OnReceive(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	bool LB;
	bool RB;
	int TimeSet, M, S;
	int bestM;
	int bestS;
	int bestDe;
	CImage image;
	Man* man;
	Object* object;
	CString m_strIP;
	CString time;
	CString bestTime;


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void Reset();
	void SendTime();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
