
// PurePhoto.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPurePhotoApp: 
// �йش����ʵ�֣������ PurePhoto.cpp
//

class CPurePhotoApp : public CWinApp
{
public:
	CPurePhotoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPurePhotoApp theApp;