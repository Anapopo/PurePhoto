#pragma once
#include <string>
#include <stdint.h>
#include "afxwin.h"
// ElementSetDlg �Ի���

class ElementSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ElementSetDlg)

public:
	ElementSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	int elm[5][5];
	int m_elm_size_save = 0;
	void initMatBox();
	virtual ~ElementSetDlg();

// �Ի�������
	enum { IDD = IDD_ELM_SET };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
	//virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:

	afx_msg void OnBnClickedBtnResetElm();
	CComboBox m_elm_size;
	
	afx_msg void OnCbnSelchangeElmSize();
};
