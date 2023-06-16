
// ImageDlg.cpp: 实现文件
//
#include "pch.h"
#include "framework.h"
#include "Image.h"
#include "ImageDlg.h"
#include "afxdialogex.h"
#include <string>
#include <afxwin.h>
#include <winsock2.h>  
#include <ws2tcpip.h>
#include <iostream>
#include "ImageSocket.h"
#include "include/json/json.h"
#include <string>
#include <iostream>

using namespace std;
#define BUFFSIZE	
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma warning(disable:4996)

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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


// CImageDlg 对话框



CImageDlg::CImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SINGLE, &CImageDlg::OnBnClickedButtonSingle)
	ON_BN_CLICKED(IDC_BUTTON_DIR, &CImageDlg::OnBnClickedButtonDir)
	ON_EN_CHANGE(IDC_EDIT_INPUT, &CImageDlg::OnEnChangeEditInput)
	ON_BN_CLICKED(IDC_BUTTON_START, &CImageDlg::OnBnClickedButtonStart)
	ON_EN_CHANGE(IDC_EDIT_RESULT, &CImageDlg::OnEnChangeEditResult)
END_MESSAGE_MAP()


// CImageDlg 消息处理程序

BOOL CImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	ImageSocket::GetInstance()->SetPacketReceiver(this);
	ImageSocket::GetInstance()->Start("127.0.0.1", 35000);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImageDlg::OnBnClickedButtonSingle()
{
	// TODO: 在此添加控件通知处理程序代码
	CString OpenFilter = _T("图片文件(*.*)|*.*|");

	CFileDialog FileOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, OpenFilter);
	if (IDOK == FileOpen.DoModal())
	{
		CString PathName = FileOpen.GetPathName();
		GetDlgItem(IDC_EDIT_INPUT)->SetWindowTextW(PathName);
		GetDlgItem(IDC_EDIT_INPUT)->SetFont(&m_oFont);
	}
}


void CImageDlg::OnBnClickedButtonDir()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("从下面选文件夹目录:");
	bi.ulFlags = BIF_RETURNFSANCESTORS; //BIF_BROWSEINCLUDEFILES
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return;
	}
	SHGetPathFromIDList(idl, szBuffer);
	if (szBuffer)
	{
		std::wstring strFilePath = szBuffer;

		//文本框写入
		CEdit* pEditSourceFileDir = (CEdit*)GetDlgItem(IDC_EDIT_INPUT);
		if (pEditSourceFileDir)
		{
			pEditSourceFileDir->SetWindowText(szBuffer);
			pEditSourceFileDir->SetFont(&m_oFont);
		}
	}
}

void CImageDlg::OnEnChangeEditInput()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	// TODO:  在此添加控件通知处理程序代码
}



void CImageDlg::OnBnClickedButtonStart()
{
	Json::Value vRoot;
	Json::Value vEducation;
	CString address;
	GetDlgItem(IDC_EDIT_INPUT)->GetWindowTextW(address);
	std::string str = CT2A(address.GetString());
	vEducation["path"] = str;
	//vEducation["path"] = "D:\\0_KCA25G_Line_3_20220413221335_ZL007_7_LKN.jpg";
	vEducation["index"] = "0";
	vRoot["type"] = "classify";
	vRoot["params"] = vEducation;
	string strRoot = vRoot.toStyledString();
	//SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	//SOCKADDR_IN addrSrv;
	//addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//案例服务器和客户端都在本地，固可以使用本地回路地址127.0.0.1
	//addrSrv.sin_family = AF_INET;
	//addrSrv.sin_port = htons(35000);
	//connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	/*SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);*/

	char* pData = new char[strRoot.length()];
	memset(pData, 0, strRoot.length());
	memcpy(pData, strRoot.c_str(), strRoot.length());
	ImageSocket::GetInstance()->SendPacket(pData, strRoot.length());	
	//char receiveBuf[BUFFSIZE + 1];
//memset(receiveBuf, 0, sizeof(receiveBuf));


}

void CImageDlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CImageDlg::OnEnChangeEditResult()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CImageDlg::OnDataCallback(char* pData, DWORD dwLength)
{
	string strData(pData, dwLength);
	Json::Reader reader;
	Json::Value vRoot;

	if (!reader.parse(strData, vRoot))
	{
		return;
	}

	string strClass = vRoot["class"].asString();

	CString res;
	//res = strRoot.c_str();
	res.Format(_T("%s"), strClass.c_str());
	GetDlgItem(IDC_EDIT_RESULT)->SetWindowTextW(res);
}