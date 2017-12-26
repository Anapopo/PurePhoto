
// PurePhotoDlg.h : ͷ�ļ�
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

// CPurePhotoDlg �Ի���
class CPurePhotoDlg : public CDialogEx
{
// ����
public:
	CPurePhotoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PUREPHOTO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	//��ͨ����ʴ
	void ErodeFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows);

	//��ͨ������
	void DilateFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows);

	//��ͨ��������
	void OpenFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows);

	//��ͨ��������
	void CloseFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows);

	//��ͨ��ͨ����ֵ������
	void BinFilter(const Mat src, Mat &dst);

	//��ͨ������������
	void MultiFilter(FilterType type, const Mat src, Mat &dst);

	//һЩ�¼�����
	afx_msg void OnBnClickedBtnResetPic();
	afx_msg void OnBnClickedBtnDilate();
	afx_msg void OnBnClickedBtnClaOpen();
	afx_msg void OnBnClickedBtnClaClose();
	afx_msg void OnBnClickedBtnBin();
	afx_msg void OnCbnSelchangeCombo1();
	
	afx_msg void OnBnClickedBtnSave();
};
