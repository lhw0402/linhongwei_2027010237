
// ImageDlg.h: 头文件
//

#pragma once


// CImageDlg 对话框
class CImageDlg : public CDialogEx
{
// 构造
public:
	CImageDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSingle();
	afx_msg void OnBnClickedButtonDir();
	afx_msg void OnEnChangeEditInput();
	afx_msg void OnBnClickedButton3();
private:
	CFont m_oFont;
	
public:
	afx_msg void OnEnChangeEdit3();
};
