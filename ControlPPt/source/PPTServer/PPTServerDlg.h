
// PPTServerDlg.h : header file
//

#pragma once
#include <memory>
class ControlCenter;
// CPPTServerDlg dialog
class CPPTServerDlg : public CDialogEx
{
// Construction
public:
	CPPTServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PPTSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	void DrawQR(const CHAR* lpsSource);
	void SetBitMap();
	bool StartControlServer();
	void StopControlServer();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	HBITMAP hBmp_;
	int nSymbleSize;
	std::shared_ptr<ControlCenter> control_center_;
public:
	afx_msg void OnBnClickedOk();
};
