
// PurePhotoDlg.h : 头文件
//

#pragma once
#include <iostream>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "CvvImage.h"
#include "ElementSetDlg.h"
#include "afxwin.h"
using namespace cv;
using namespace std;

// CPurePhotoDlg 对话框
class CPurePhotoDlg : public CDialogEx
{
// 构造
public:
	CPurePhotoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PUREPHOTO_DIALOG };

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
	char* CPurePhotoDlg::CString2Char(CString str);
	
	void DrawPicToHDC(IplImage *img, UINT ID);
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnStnClickedCvPic();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedBtnElmSet();
	afx_msg void OnBnClickedBtnErode();
public:
	CString debugInfo;
	ElementSetDlg* pDlg;
	char *currentPicPath;
	Mat *currentSrc;
	CComboBox m_choose_style;
	enum FilterType{
		ERODE = 0,
		DILATE = 1,
		OPEN = 2,
		CLOSE = 3
	};
	//单通道腐蚀
	void ErodeFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows);

	//单通道膨胀
	void DilateFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows);

	//单通道开运算
	void OpenFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows);

	//单通道闭运算
	void CloseFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows);

	//三通道通道二值化运算
	void BinFilter(const Mat src, Mat &dst);

	//多通道多类型运算
	void MultiFilter(FilterType type, const Mat src, Mat &dst);

	//一些事件处理
	afx_msg void OnBnClickedBtnResetPic();
	afx_msg void OnBnClickedBtnDilate();
	afx_msg void OnBnClickedBtnClaOpen();
	afx_msg void OnBnClickedBtnClaClose();
	afx_msg void OnBnClickedBtnBin();
	afx_msg void OnCbnSelchangeCombo1();
	
	afx_msg void OnBnClickedBtnSave();
};
