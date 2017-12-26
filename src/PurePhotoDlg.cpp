
// PurePhotoDlg.cpp : ʵ���ļ�
#include "stdafx.h"
#include "PurePhoto.h"
#include "PurePhotoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CPurePhotoDlg �Ի���
CPurePhotoDlg::CPurePhotoDlg(CWnd* pParent /*=NULL*/): CDialogEx(CPurePhotoDlg::IDD, pParent), debugInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//һЩ������ʼ��
	currentSrc = new Mat;
	pDlg = new ElementSetDlg();
}

void CPurePhotoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEST, debugInfo);
	DDX_Control(pDX, IDC_CHOOSE_STYLE, m_choose_style);
}

BEGIN_MESSAGE_MAP(CPurePhotoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CPurePhotoDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CPurePhotoDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_ELM_SET, &CPurePhotoDlg::OnBnClickedBtnElmSet)
	ON_BN_CLICKED(IDC_BTN_ERODE, &CPurePhotoDlg::OnBnClickedBtnErode)
	ON_BN_CLICKED(IDC_BTN_RESET_PIC, &CPurePhotoDlg::OnBnClickedBtnResetPic)
	ON_BN_CLICKED(IDC_BTN_DILATE, &CPurePhotoDlg::OnBnClickedBtnDilate)
	ON_BN_CLICKED(IDC_BTN_CLA_OPEN, &CPurePhotoDlg::OnBnClickedBtnClaOpen)
	ON_BN_CLICKED(IDC_BTN_CLA_CLOSE, &CPurePhotoDlg::OnBnClickedBtnClaClose)
	ON_BN_CLICKED(IDC_BTN_BIN, &CPurePhotoDlg::OnBnClickedBtnBin)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CPurePhotoDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()

// CPurePhotoDlg ��Ϣ�������
BOOL CPurePhotoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
	
	//��ʼ���Ի���
	m_choose_style.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPurePhotoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == SC_SIZE) return;
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CPurePhotoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CPurePhotoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//CString ת��Ϊchar*
char* CPurePhotoDlg::CString2Char(CString str)
{
	char *ch = new char[32];
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, str, -1, ch, 32, NULL, NULL);
	return ch;
}

//����DC��ͼ����ʾ�����ͼ�����ʧ�����⣩
void CPurePhotoDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);

	//������
	CPoint p1 = rect.TopLeft();//��x����y
	CPoint p2 = rect.BottomRight();

	CvvImage cimg;
	cimg.CopyOf(img); // ����ͼƬ
	//ͼƬ����
	int width = cimg.Width();
	int height = cimg.Height();
	double per = width*1.0 / height;//���ݱ���

	//�ӿ�ԭʼ����
	int offset_x = 0;
	int rect_width = p2.x - p1.x;
	int rect_height = p2.y - p1.y;

	double per2 = rect_width / width;
	double per3 = rect_height / height;

	int new_height = rect_height;
	int new_width =  new_height * per;
	if (new_width >= rect_width) new_width = rect_width;
	else {
		offset_x = (rect_width/2 - new_width/2);
	}
	//todo ������ʾ
	rect.SetRect(p1.x+offset_x, p1.y, p1.x+offset_x+new_width, p1.y+new_height);
	

	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC(pDC);
}

//��ͼ��ť�¼�����
void CPurePhotoDlg::OnBnClickedBtnOpen()
{

	//ѡ��ͼƬ
	CFileDialog fileDlg(TRUE, _T("png"), NULL, 0, _T("image Files(*.bmp; *.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*||"), this);
	fileDlg.DoModal();
	//��ȡͼƬ·����ͼƬ����
	CString strFilePath = fileDlg.GetPathName();
	CString strFileName = fileDlg.GetFileName();
	//�ж�·����Ϊ��
	if (strFilePath == _T("")) return;
	char *charPath = CString2Char(strFilePath);

	UpdateData();
	debugInfo.SetString(strFilePath);
	UpdateData(FALSE);

	IplImage *image = NULL; //ԭʼͼ��
	if (image) cvReleaseImage(&image);
	image = cvLoadImage(charPath, CV_LOAD_IMAGE_COLOR); //��ʾͼƬ

	Mat temp = imread(charPath, CV_LOAD_IMAGE_COLOR);
	temp.copyTo(*currentSrc);
	
	int len = strlen(charPath);    //�����ַ�������  
	currentPicPath = new char[len + 1];    //����洢�ռ�  
	strcpy_s(currentPicPath, len + 1, charPath); //��s���ַ������Ƶ�str�����һ���ռ�Ϊ'\0'������  

	//��ջ���
	InvalidateRect(NULL);
	UpdateWindow();
	DrawPicToHDC(image, IDC_CV_PIC);
	delete charPath;
}

//�ر�ͼ��ť�¼�����
void CPurePhotoDlg::OnBnClickedBtnClose()
{
	IplImage *image = cvLoadImage("D:\\default.png", 1); //��ʾͼƬ 
	DrawPicToHDC(image, IDC_CV_PIC);
	if (image) cvReleaseImage(&image);
	//������ջ��
	Mat temp = imread("D:\\default.png", CV_LOAD_IMAGE_COLOR);
	temp.copyTo(*currentSrc);
}

//�趨�ṹԪ�ذ�ť�¼�����
void CPurePhotoDlg::OnBnClickedBtnElmSet(){ pDlg->DoModal(); }

//����ͼ��ť�¼�����
void CPurePhotoDlg::OnBnClickedBtnResetPic()
{
	//�ж��Ƿ�����
	if (!currentPicPath)
	{
		MessageBox(_T("��������ͼ��"), _T("����"), MB_OK);
		return;
	}
	//���뻭����ʾTODO���ϲ�
	IplImage *image = cvLoadImage(currentPicPath, 1);
	DrawPicToHDC(image, IDC_CV_PIC);
	if (image) cvReleaseImage(&image);
	
	//����ջ��
	Mat temp = imread(currentPicPath, CV_LOAD_IMAGE_COLOR);
	temp.copyTo(*currentSrc);
}

//��ʴ��ť�¼����� OK
void CPurePhotoDlg::OnBnClickedBtnErode()
{	
	if (!currentSrc->data)
	{
		MessageBox(_T("��������ͼ��"), _T("����"), MB_OK);
		return;
	}
	Mat dst;
	currentSrc->copyTo(dst);
	if (m_choose_style.GetCurSel() == 1)
		erode(*currentSrc, dst, Mat());
	else
		MultiFilter(ERODE, *currentSrc, dst);
	dst.copyTo(*currentSrc);
	//ͼ����ʾ
	IplImage *img = new IplImage(*currentSrc);
	DrawPicToHDC(img, IDC_CV_PIC);
}

//���Ͱ�ť�¼����� OK
void CPurePhotoDlg::OnBnClickedBtnDilate()
{
	if (!currentSrc->data)
	{
		MessageBox(_T("��������ͼ��"), _T("����"), MB_OK);
		return;
	}
	Mat dst;
	currentSrc->copyTo(dst);
	if (m_choose_style.GetCurSel() == 1)
		dilate(*currentSrc, dst, Mat());
	else
		MultiFilter(DILATE, *currentSrc, dst);
	dst.copyTo(*currentSrc);
	//ͼ����ʾ
	IplImage *img = new IplImage(*currentSrc);
	DrawPicToHDC(img, IDC_CV_PIC);
}

//��ֵ����ť�¼�����
void CPurePhotoDlg::OnBnClickedBtnBin()
{
	if (!currentSrc->data)
	{
		MessageBox(_T("��������ͼ��"), _T("����"), MB_OK);
		return;
	}

	if (currentSrc->isContinuous())
	{
		Mat dst;
		currentSrc->copyTo(dst);
		BinFilter(*currentSrc, dst);
		dst.copyTo(*currentSrc);

		IplImage *img = new IplImage(*currentSrc);
		DrawPicToHDC(img, IDC_CV_PIC);
	}
}

//�����㰴ť�¼����� OK
void CPurePhotoDlg::OnBnClickedBtnClaOpen()
{
	if (!currentSrc->data)
	{
		MessageBox(_T("��������ͼ��"), _T("����"), MB_OK);
		return;
	}
	Mat dst;
	currentSrc->copyTo(dst);
	if (m_choose_style.GetCurSel() == 1) 
		morphologyEx(*currentSrc, dst, MORPH_OPEN, Mat());
	else
		MultiFilter(OPEN, *currentSrc, dst);
	dst.copyTo(*currentSrc);
	//ͼ����ʾ
	IplImage *img = new IplImage(*currentSrc);
	DrawPicToHDC(img, IDC_CV_PIC);
}

//�����㰴ť�¼����� OK
void CPurePhotoDlg::OnBnClickedBtnClaClose()
{
	if (!currentSrc->data)
	{
		MessageBox(_T("��������ͼ��"), _T("����"), MB_OK);
		return;
	}
	Mat dst;
	currentSrc->copyTo(dst);
	if (m_choose_style.GetCurSel() == 1)
		morphologyEx(*currentSrc, dst, MORPH_CLOSE, Mat());
	else
		MultiFilter(CLOSE, *currentSrc, dst);
	dst.copyTo(*currentSrc);
	//ͼ����ʾ
	IplImage *img = new IplImage(*currentSrc);
	DrawPicToHDC(img, IDC_CV_PIC);
}

//��ͨ���������㷨 OK
void CPurePhotoDlg::MultiFilter(FilterType type, const Mat src, Mat &dst)
{	
	Mat temp;
	vector<Mat> channels;//�洢����ǰ��ͨ��
	vector<Mat> dsts;//�洢�������ͨ��

	split(src, channels);
	split(src, dsts);
	for (int i = 0; i < 3; i++)
	{
		switch (type){
		case ERODE:
			ErodeFilter(channels[i].data, dsts[i].data, channels[i].cols, channels[i].rows);
			break;
		case DILATE:
			DilateFilter(channels[i].data, dsts[i].data, channels[i].cols, channels[i].rows);
			break;
		case OPEN:
			OpenFilter(channels[i].data, dsts[i].data, channels[i].cols, channels[i].rows);
			break;
		case CLOSE:
			CloseFilter(channels[i].data, dsts[i].data, channels[i].cols, channels[i].rows);
			break;
		}
	}
	merge(dsts, temp);
	temp.copyTo(dst);
}

//��ͨ�������㷨 OK ������3*3��̬������
void CPurePhotoDlg::DilateFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows)
{

	short IMG_OFFSET = (pDlg->m_elm_size_save != 0) ? pDlg->m_elm_size_save : 1;
	short ST_WIDTH = IMG_OFFSET * 2 + 1;

	int i, j, m, n;
	int *tmp = new int[ST_WIDTH*ST_WIDTH];

	for (i = IMG_OFFSET; i < rows - IMG_OFFSET; i++)
	{
		for (j = IMG_OFFSET; j < cols - IMG_OFFSET; j++)
		{
			for (m = -IMG_OFFSET; m <= IMG_OFFSET; m++)
			{
				for (n = -IMG_OFFSET; n <= IMG_OFFSET; n++)
				{
					tmp[(m + IMG_OFFSET) * ST_WIDTH + n + IMG_OFFSET] = src_data[(i + m)*cols + j + n];
					//
					tmp[(m + IMG_OFFSET) * ST_WIDTH + n + IMG_OFFSET] += pDlg->elm[m + 2][n + 2];
					if (tmp[(m + IMG_OFFSET) * ST_WIDTH + n + IMG_OFFSET] > 255)
						tmp[(m + IMG_OFFSET) * ST_WIDTH + n + IMG_OFFSET] = 255;
				}
			}
			int max = 0;
			for (int k = 0; k < ST_WIDTH*ST_WIDTH; k++)
			if (tmp[k] > max) max = tmp[k];
			dst_data[i*cols + j] = max;
		}
	}
}

//��ͨ����ʴ�㷨 OK
void CPurePhotoDlg::ErodeFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows)
{
	short IMG_OFFSET = (pDlg->m_elm_size_save != 0) ? pDlg->m_elm_size_save : 1;
	short ST_WIDTH = IMG_OFFSET * 2 + 1;

	int i, j, m, n;
	int *tmp = new int[ST_WIDTH*ST_WIDTH];

	for (i = IMG_OFFSET; i < rows - IMG_OFFSET; i++)
	{
		for (j = IMG_OFFSET; j < cols - IMG_OFFSET; j++)
		{
			for (m = -IMG_OFFSET; m <= IMG_OFFSET; m++)
			{
				for (n = -IMG_OFFSET; n <= IMG_OFFSET; n++)
				{
					tmp[(m + IMG_OFFSET) * ST_WIDTH + n + IMG_OFFSET] = src_data[(i + m)*cols + j + n];
					//
					tmp[(m + IMG_OFFSET) * ST_WIDTH + n + IMG_OFFSET] -= pDlg->elm[m + 2][n + 2];
					if (tmp[(m + IMG_OFFSET) * ST_WIDTH + n + IMG_OFFSET] < 0) 
						tmp[(m + IMG_OFFSET) * ST_WIDTH + n + IMG_OFFSET] = 0;
				}
			}
			int min = 0xffffff;
			for (int k = 0; k < ST_WIDTH*ST_WIDTH; k++)
			if (tmp[k] < min) min = tmp[k];
			dst_data[i*cols + j] = min;
		}
	}

}

//��ͨ�������㣨��ʴ+���ͣ��㷨 OK
void CPurePhotoDlg::OpenFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows)
{
	uint8_t *proc = new uint8_t[cols*rows];
	memcpy(proc, src_data, sizeof(uint8_t)*cols*rows);
	ErodeFilter(src_data, proc, cols, rows);
	DilateFilter(proc, dst_data, cols, rows);
	delete proc;
}

//��ͨ�������㣨����+��ʴ���㷨 OK
void CPurePhotoDlg::CloseFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows)
{
	uint8_t *proc = new uint8_t[cols*rows];
	memcpy(proc, src_data, sizeof(uint8_t)*cols*rows);
	DilateFilter(src_data, proc, cols, rows);
	ErodeFilter(proc, dst_data, cols, rows);
	delete proc;
}

//��ֵ���㷨 OK
void CPurePhotoDlg::BinFilter(const Mat src, Mat &dst)
{
	Mat temp1, temp2;
	cvtColor(src, temp1, CV_RGB2GRAY);
	threshold(temp1, temp2, 100, 255, THRESH_BINARY);
	cvtColor(temp2, dst, CV_GRAY2RGB);
}

//����ͼ��ť�¼�����
void CPurePhotoDlg::OnBnClickedBtnSave()
{
	if (!currentSrc->data)
	{
		MessageBox(_T("��������ͼ��"), _T("����"), MB_OK);
		return;
	}
	CFileDialog fileDlg(FALSE, _T("png"), _T("MyProcImg.png"), 0, _T("image Files(*.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*||"), this);
	
	
	if (IDOK == fileDlg.DoModal())
	{
		CString strFilePath = fileDlg.GetPathName();
		if (strFilePath)
		{
			string str = CStringA(strFilePath);
			imwrite(str, *currentSrc);//��imageͼ�񱣴�
			MessageBox(_T("ͼ���ѳɹ�����!"), _T("�����ɹ���"), MB_OK);
		}
	}
	
	
}

