
// PPTServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PPTServer.h"
#include "PPTServerDlg.h"
#include "afxdialogex.h"
#include "QR_Encode.h"
#include "utils.h"
#include "control_center.h"
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CPPTServerDlg dialog



CPPTServerDlg::CPPTServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PPTSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPPTServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPPTServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CPPTServerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPPTServerDlg message handlers

BOOL CPPTServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	if (!StartControlServer()) {
		MessageBox(L"start control server failed");
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPPTServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

bool CPPTServerDlg::StartControlServer() {
	if (!control_center_) {
		control_center_.reset(new ControlCenter());
	}
	if (!control_center_) {
		printf("crate control_center failed!\n");
		return false;
	}
	if (!control_center_->Init()) {
		printf("control_center init failed!\n");
		return false;
	}
	if (!control_center_->Start()) {
		printf("control_center start failed!\n");
		return false;
	}
	return true;
}
void CPPTServerDlg::StopControlServer() {
	if (control_center_) {
		control_center_->Stop();
		control_center_->UnInit();
	}
}
void CPPTServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		SetBitMap();
	}
}
void CPPTServerDlg::DrawQR(const CHAR* lpsSource){

	std::shared_ptr<CQR_Encode> pQR_Encode(new CQR_Encode());
	if (pQR_Encode)
	{
		if (pQR_Encode->EncodeData(1, 0, TRUE, -1, lpsSource))
		{
			HDC hMemWork = CreateCompatibleDC(NULL);
			if (hMemWork)
			{
				if (hBmp_ == NULL || DeleteObject(hBmp_))
				{
					hBmp_ = CreateBitmap(pQR_Encode->m_nSymbleSize, pQR_Encode->m_nSymbleSize, 1, 1, NULL);
					if (hBmp_)
					{
						HBITMAP	hOldBitmap = (HBITMAP)SelectObject(hMemWork, hBmp_);
						nSymbleSize = pQR_Encode->m_nSymbleSize;
						PatBlt(hMemWork, 0, 0, nSymbleSize, nSymbleSize, WHITENESS);
						int	i;
						for (i = 0; i<nSymbleSize; ++i)
						{
							int j;
							for (j = 0; j<nSymbleSize; ++j)
							{
								if (pQR_Encode->m_byModuleData[i][j])
								{
									SetPixel(hMemWork, i + QR_MARGIN, j + QR_MARGIN, RGB(0, 0, 0));
								}
							}
						}
						SelectObject(hMemWork, hOldBitmap);
						DeleteObject(hOldBitmap);
						DeleteDC(hMemWork);
					}
				}
			}
		}
	}
}
void CPPTServerDlg::SetBitMap() {
	std::string ip_addr;
	utils::GetLocalIP(ip_addr);
	DrawQR("192.168.1.13");
	HWND  hwnd = this->GetSafeHwnd();
	const HDC hdc = ::GetWindowDC(hwnd);
	const HDC hdc_mem = CreateCompatibleDC(hdc);
	SelectObject(hdc_mem, hBmp_);
	RECT rect;
	::GetClientRect(hwnd, &rect);
	StretchBlt(hdc, 120, 50, rect.bottom - 100, rect.bottom - 100, hdc_mem, 0, 0, nSymbleSize, nSymbleSize, SRCCOPY);
	DeleteDC(hdc_mem);
} 
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPPTServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CPPTServerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	StopControlServer();
}
