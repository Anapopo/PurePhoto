// ElementSetDlg.cpp : 实现文件
//

#include "stdafx.h"

#include "PurePhoto.h"
#include "ElementSetDlg.h"
#include "afxdialogex.h"
using namespace std;

// ElementSetDlg 对话框

IMPLEMENT_DYNAMIC(ElementSetDlg, CDialogEx)

ElementSetDlg::ElementSetDlg(CWnd* pParent /*=NULL*/) : CDialogEx(ElementSetDlg::IDD, pParent)
{
	for (int i = 0; i < 5; i++)
	for (int j = 0; j < 5; j++)
		elm[i][j] = 0;
}

ElementSetDlg::~ElementSetDlg()
{
}

void ElementSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//1012-1036 ID连续
	for (int i = IDC_EDIT_00; i <= IDC_EDIT_44; i++)
	{
		int row = (i - IDC_EDIT_00) / 5;
		int col = (i - IDC_EDIT_00) % 5;

		DDX_Text(pDX, i, elm[row][col]);
		DDV_MinMaxInt(pDX, elm[row][col], 0, 7);
	}
	DDX_Control(pDX, IDC_ELM_SIZE, m_elm_size);
}


BEGIN_MESSAGE_MAP(ElementSetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_RESET_ELM, &ElementSetDlg::OnBnClickedBtnResetElm)
	ON_CBN_SELCHANGE(IDC_ELM_SIZE, &ElementSetDlg::OnCbnSelchangeElmSize)
END_MESSAGE_MAP()


// ElementSetDlg 消息处理程序

BOOL ElementSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_elm_size.SetCurSel(m_elm_size_save-1);
	//初始化矩阵盒子
	initMatBox();
	return TRUE;
}

void ElementSetDlg::OnBnClickedBtnResetElm()
{
	UpdateData();
	for (int i = 0; i < 5; i++)
	for (int j = 0; j < 5; j++)
		elm[i][j] = 0;
	UpdateData(FALSE);
}

void ElementSetDlg::initMatBox()
{
	for (int i = IDC_EDIT_00; i <= IDC_EDIT_44; i++)
		GetDlgItem(i)->ShowWindow(SW_HIDE);

	if (m_elm_size_save == 1)// 3*3
	{
		for (int i = 1; i < 4; i++)
		for (int j = 1; j < 4; j++)
			GetDlgItem(IDC_EDIT_00 + i * 5 + j)->ShowWindow(SW_SHOW);
	}
	else if (m_elm_size_save == 2)// 5*5
	{
		for (int i = IDC_EDIT_00; i <= IDC_EDIT_44; i++)
			GetDlgItem(i)->ShowWindow(SW_SHOW);
	}
}

void ElementSetDlg::OnCbnSelchangeElmSize() {
	m_elm_size_save = m_elm_size.GetCurSel()+1;
	initMatBox(); 
}
