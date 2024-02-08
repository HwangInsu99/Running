
// RunningDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Running.h"
#include "RunningDlg.h"
#include "afxdialogex.h"
#include "Man.h"
#include "Object.h"

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
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
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


// CRunningDlg 대화 상자



CRunningDlg::CRunningDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RUNNING_DIALOG, pParent)
	, m_strIP(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	man = new Man();
	object = new Object();
	TimeSet = 0;
	M = 0;
	S = 0;
	bestM = 0;
	bestS = 0;
	bestDe = 0;
	time = "0:0:0";
	bestTime = "0:00:0";
	LB = false;
	RB = false;
}

void CRunningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRunningDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(UM_RECEIVE_WM_USER, OnReceive)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

LPARAM CRunningDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	TCHAR pTmp[256];
	CString strTmp;
	int bestT;
	memset(pTmp, '\0', 256);

	//서버에서 받은 숫자를 시간기록에 맞게 분해해서 표기함
	m_socCom.Receive(pTmp, 256);
	strTmp.Format(L"%s", pTmp);
	bestT = _ttoi(strTmp);
	bestM = bestT / 1000;
	bestS = bestT % 1000 / 10;
	bestDe = bestT % 10;

	Invalidate();

	return LPARAM();
}

// CRunningDlg 메시지 처리기

BOOL CRunningDlg::OnInitDialog()
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

	UpdateData(TRUE);
	m_socCom.Create();
	// IP 주소와 포트 번호를 지정
	m_socCom.Connect(m_strIP, 5000);
	m_socCom.Init(this->m_hWnd);
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetTimer(1, 50, nullptr);
	SetTimer(2, 100, nullptr);
	image.Load(L"images\\ma.png");
	man->LoadImages();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CRunningDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_CLOSE) { // 클라이언트 종료시 서버로 메세지
		m_socCom.Send(L"DISCONNECT", 256);
		this->EndDialog(IDCANCEL);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CRunningDlg::OnPaint()
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
		CPaintDC dc(this);

		CRect rect;
		this->GetClientRect(&rect);

		CDC MemDC;
		CBitmap* pOldBitmap, bmp;

		MemDC.CreateCompatibleDC(&dc);
		bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
		pOldBitmap = MemDC.SelectObject(&bmp);
		MemDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);


		CString str;
		str.Format(L"<- 이동   SPACE 점프   이동 ->");
		MemDC.TextOutW(175, 10, str);
		CString str1;
		str1.Format(L"F5 리셋");
		MemDC.TextOutW(250, 40, str1);
		MemDC.TextOutW(400, 40, time);
		CString str2;
		str2.Format(L"최고 기록");
		MemDC.TextOutW(20, 10, str2);
		bestTime.Format(L"%d:%d:%d", bestM, bestS, bestDe);
		MemDC.TextOutW(20, 40, bestTime);

		man->Draw(MemDC);
		object->Draw(&MemDC);

		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);

		MemDC.SelectObject(pOldBitmap);

		MemDC.DeleteDC();
		bmp.DeleteObject();

		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CRunningDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRunningDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다..
	//타이머기록을 제외한 프로그램전체의 진행
	if (nIDEvent == 1) {
		man->Tick();
		object->Tick();
		if (man->Collide(object)) {
			KillTimer(1);
			KillTimer(2);
			SendTime();
			CString str;
			str.Format(L"내 기록 : %s", (LPCTSTR)time);
			CString str2;
			str2.Format(L"최고기록 : %s", (LPCTSTR)bestTime);
			MessageBox(str2 + "\n" + str);
		}
		Invalidate();
	}
	//타이머기록
	else if (nIDEvent == 2) {
		time.Format(L"%d:%d:%d", M, S, ++TimeSet);
		if (S == 60)
		{
			++M;
			S = 0;
		}
		if (TimeSet == 10)
		{
			++S;
			TimeSet = 0;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CRunningDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nChar)
	{
	case VK_LEFT:
		LB = true;
		man->direction = Man::DIR_LEFT;
		break;
	case VK_RIGHT:
		RB = true;
		man->direction = Man::DIR_RIGHT;
		break;
	case VK_SPACE:
		man->StartJUMP();
		break;
	case VK_F5:
		Reset();
	}

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CRunningDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nChar == VK_LEFT)
		LB = false;
	if (nChar == VK_RIGHT)
		RB = false;
	if (LB == true && RB == false)
		man->direction = Man::DIR_LEFT;
	if (LB == false && RB == true)
		man->direction = Man::DIR_RIGHT;
	if (LB == false && RB == false)
		man->direction = Man::DIR_STOP;

	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CRunningDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMinTrackSize = CPoint(600, 400);
	lpMMI->ptMaxTrackSize = CPoint(600, 400);

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


BOOL CRunningDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;
}

void CRunningDlg::Reset()
{
	//클라이언트 초기화
	LB = false;
	RB = false;
	SetTimer(1, 50, nullptr);
	TimeSet = 0;
	M = 0;
	S = 0;
	time = "NULL";
	SetTimer(2, 100, nullptr);
	object->Reset();
	man->Reset();
}

void CRunningDlg::SendTime()
{
	UpdateData(TRUE);
	TCHAR pTmp[256];
	CString strTmp;
	int sumT;

	//기록의 분 초 소수점을 하나의 숫자로 만들어서 서버에보냄
	sumT = M * 1000 + S * 10 + TimeSet;
	strTmp.Format(L"%d", sumT);
	memset(pTmp, '\0', 256);
	wcscat_s(pTmp, strTmp);
	m_socCom.Send(pTmp, 256);

	UpdateData(FALSE);
}

BOOL CRunningDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
