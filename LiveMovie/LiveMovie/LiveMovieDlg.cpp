
// LiveMovieDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LiveMovie.h"
#include "LiveMovieDlg.h"
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


// CLiveMovieDlg 대화 상자



CLiveMovieDlg::CLiveMovieDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIVEMOVIE_DIALOG, pParent) 
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CLiveMovieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YUV_PATH, m_YuvPath);
	DDX_Control(pDX, IDC_TEMP1_PATH, m_TPath1);
	DDX_Control(pDX, IDC_TEMP2_PATH, m_TPath2);
	DDX_Control(pDX, IDC_WIDTH_EDIT, m_Width);
	DDX_Control(pDX, IDC_HEIGHT_EDIT, m_Height);
	SetDlgItemText(IDC_YUV_PATH, _T("Enter yuv file path"));
	SetDlgItemText(IDC_TEMP1_PATH, _T("Enter temp1 file path"));
	SetDlgItemText(IDC_TEMP2_PATH, _T("Enter temp2 file path"));
	SetDlgItemInt(IDC_WIDTH_EDIT, 1920);
	SetDlgItemInt(IDC_HEIGHT_EDIT, 1080);
}

BEGIN_MESSAGE_MAP(CLiveMovieDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OUTPUT_BUTTON, &CLiveMovieDlg::OnBnClickedOutputButton)
	ON_EN_CHANGE(IDC_YUV_PATH, &CLiveMovieDlg::OnEnChangeYuvPath)
	ON_EN_CHANGE(IDC_TEMP1_PATH, &CLiveMovieDlg::OnEnChangeTemp1Path)
	ON_EN_CHANGE(IDC_TEMP2_PATH, &CLiveMovieDlg::OnEnChangeTemp2Path)
	ON_BN_CLICKED(IDC_YUV_BUTTON, &CLiveMovieDlg::OnBnClickedYuvButton)
	ON_BN_CLICKED(IDC_TEMP1_BUTTON, &CLiveMovieDlg::OnBnClickedTemp1Button)
	ON_BN_CLICKED(IDC_TEMP2_BUTTON, &CLiveMovieDlg::OnBnClickedTemp2Button)
	ON_EN_CHANGE(IDC_WIDTH_EDIT, &CLiveMovieDlg::OnEnChangeWidthEdit)
	ON_EN_CHANGE(IDC_HEIGHT_EDIT, &CLiveMovieDlg::OnEnChangeHeightEdit)
END_MESSAGE_MAP()


// CLiveMovieDlg 메시지 처리기

BOOL CLiveMovieDlg::OnInitDialog()
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
	SetWindowText(_T("Find Contents In Live Encoding"));;
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CLiveMovieDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLiveMovieDlg::OnPaint()
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
HCURSOR CLiveMovieDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLiveMovieDlg::OnBnClickedOutputButton()
{

	CString x_size = _T("");
	m_Width.GetWindowTextW(x_size);
	int x = _ttoi(x_size);
	
	CString y_size = _T("");
	m_Height.GetWindowTextW(y_size);
	int y = _ttoi(y_size);

	int Y_SIZE = y;
	int X_SIZE = x;
	int YUV_SIZE = Y_SIZE * X_SIZE * 3 / 2;

	unsigned char* start_frame = new unsigned char[YUV_SIZE];
	FILE* startfile;
	CString startText = _T("");
	m_TPath1.GetWindowText(startText);
	fopen_s(&startfile, (CStringA)startText, "rb");
	fread(start_frame, sizeof(unsigned char), YUV_SIZE, startfile);
	
	unsigned char* end_frame = new unsigned char[YUV_SIZE];
	FILE* endfile;
	CString endText = _T("");
	m_TPath2.GetWindowText(endText);
	fopen_s(&endfile, (CStringA)endText, "rb");
	fread(end_frame, sizeof(unsigned char), YUV_SIZE, endfile);

	unsigned char* yuv_frame = new unsigned char[YUV_SIZE];
	FILE* yuvfile;
	CString yuvText = _T("");
	m_YuvPath.GetWindowText(yuvText);
	fopen_s(&yuvfile, (CStringA)yuvText, "rb");

	long long* startyframe_diff = new long long[MAX_FRAME];
	long long* endyframe_diff = new long long[MAX_FRAME];
	long long* startuvframe_diff = new long long[MAX_FRAME];
	long long* enduvframe_diff = new long long[MAX_FRAME];

	int NUMBER_OF_FRAME = 0;
	for (int i = 0; i < MAX_FRAME; i++) {
		bool fsize = fread(yuv_frame, sizeof(unsigned char), YUV_SIZE, yuvfile);
		if (feof(yuvfile)) {
			break;
		}
		NUMBER_OF_FRAME++;
		long long start_ysum = 0;
		long long end_ysum = 0;
		long long start_uvsum = 0;
		long long end_uvsum = 0;

		for (int j = 0; j < Y_SIZE * X_SIZE; j++) {
			start_ysum += abs(yuv_frame[j] - start_frame[j]);
			end_ysum += abs(yuv_frame[j] - end_frame[j]);
		}
		for (int j = Y_SIZE * X_SIZE; j < YUV_SIZE; j++) {
			start_uvsum += abs(yuv_frame[j] - start_frame[j]);
			end_uvsum += abs(yuv_frame[j] - end_frame[j]);

		}
		startyframe_diff[i] = start_ysum;
		endyframe_diff[i] = end_ysum;
		startuvframe_diff[i] = start_uvsum;
		enduvframe_diff[i] = end_uvsum;
	}

	int start = 0;
	int end = NUMBER_OF_FRAME;
	long long start_uvsum = 0;
	long long end_uvsum = 0;
	long long start_ysum = MAX_SUM;
	long long end_ysum = MAX_SUM;

	for (int i = 0; i < NUMBER_OF_FRAME - 1; i++) {
		long long start_uvdiff = abs(startuvframe_diff[i] - startuvframe_diff[i + 1]);
		long long end_uvdiff = abs(enduvframe_diff[i] - enduvframe_diff[i + 1]);
		if (start_uvdiff > end_uvdiff) {
			if (start_uvsum <= start_uvdiff) {
				if (start_ysum >= startyframe_diff[i] && startyframe_diff[i] <= DEFAULT_STARTFRAME_DIFF) {
					start_uvsum = start_uvdiff;
					start_ysum = startyframe_diff[i];
					start = i + 1;
					if (start >= end) {
						end_ysum = MAX_SUM;
					}
				}
			}
		}
		else {
			if (end_uvsum <= end_uvdiff) {
				if (end_ysum >= endyframe_diff[i + 1] && endyframe_diff[i + 1] <= DEFAULT_ENDFRAME_DIFF) {
					end_uvsum = end_uvdiff;
					end_ysum = endyframe_diff[i + 1];
					end = i + 1;
				}
			}
		}
	}
	
	CString msg;
	msg.Format(_T("%s\n start : %d, end : %d, lenght : %d"),yuvText,start,end,end-start);
	MessageBox(msg);

	delete[] yuv_frame;
	delete[] startyframe_diff;
	delete[] endyframe_diff;
	delete[] startuvframe_diff;
	delete[] enduvframe_diff;

}


void CLiveMovieDlg::OnEnChangeYuvPath()
{
	
}


void CLiveMovieDlg::OnEnChangeTemp1Path()
{
}


void CLiveMovieDlg::OnEnChangeTemp2Path()
{
}


void CLiveMovieDlg::OnBnClickedYuvButton()
{
	CString str = _T("ALL files(*.*)|*.*|");
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK) {
		CString strPathName = dlg.GetPathName();
		SetDlgItemText(IDC_YUV_PATH, strPathName);
	}
}

void CLiveMovieDlg::OnBnClickedTemp1Button()
{
	CString str = _T("ALL files(*.*)|*.*|");
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK) {
		CString strPathName = dlg.GetPathName();
		SetDlgItemText(IDC_TEMP1_PATH, strPathName);
	}
}

void CLiveMovieDlg::OnBnClickedTemp2Button()
{
	CString str = _T("ALL files(*.*)|*.*|");
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK) {
		CString strPathName = dlg.GetPathName();
		SetDlgItemText(IDC_TEMP2_PATH, strPathName);
	}
}


void CLiveMovieDlg::OnEnChangeWidthEdit()
{
}


void CLiveMovieDlg::OnEnChangeHeightEdit()
{
}






