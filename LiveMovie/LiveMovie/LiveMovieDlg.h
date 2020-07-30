
// LiveMovieDlg.h: 헤더 파일
//

#pragma once
#define MAX_FRAME 3000000
#define MAX_SUM 0xFFFFFFFF
#define DEFAULT_STARTFRAME_DIFF 3000000
#define DEFAULT_ENDFRAME_DIFF 3000000

// CLiveMovieDlg 대화 상자
class CLiveMovieDlg : public CDialogEx
{
// 생성입니다.
public:
	CLiveMovieDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIVEMOVIE_DIALOG };
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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOutputButton();
	afx_msg void OnEnChangeYuvPath();
	afx_msg void OnEnChangeTemp1Path();
	afx_msg void OnEnChangeTemp2Path();
	afx_msg void OnBnClickedYuvButton();
	afx_msg void OnBnClickedTemp1Button();
	afx_msg void OnBnClickedTemp2Button();
	afx_msg void OnEnChangeWidthEdit();
	afx_msg void OnEnChangeHeightEdit();
	CEdit m_YuvPath;
	CEdit m_TPath1;
	CEdit m_TPath2;
	CEdit m_Height;
	CEdit m_Width;
};
