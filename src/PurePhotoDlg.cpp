
// PurePhotoDlg.cpp : 实现文件
#include "stdafx.h"
#include "PurePhoto.h"
#include "PurePhotoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

// 对话框数据
    enum { IDD = IDD_ABOUTBOX };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// CPurePhotoDlg 对话框
CPurePhotoDlg::CPurePhotoDlg(CWnd* pParent /*=NULL*/): CDialogEx(CPurePhotoDlg::IDD, pParent), debugInfo(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    //一些变量初始化
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

// CPurePhotoDlg 消息处理程序
BOOL CPurePhotoDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // IDM_ABOUTBOX 必须在系统命令范围内。
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

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标


    //初始化对话框
    m_choose_style.SetCurSel(0);

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CPurePhotoDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CPurePhotoDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

//利用DC在图像显示区域绘图（解决失真问题）(解决图像完全显示问题)（解决图像居中显示问题）
void CPurePhotoDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
    //原始参数
    CRect rect;
    int rect_width = 805;
    int rect_height = 476;

    // 复制图片
    CvvImage cimg;
    cimg.CopyOf(img);

    //图片参数
    int width = cimg.Width();
    int height = cimg.Height();
    double per = width*1.0 / height;//横纵比例
    int new_height = height;
    int	new_width = width;
    //需要进行缩放
    if (rect_width < new_width || rect_height < new_height)
    {
        //计算合适的缩放
        new_width = rect_width;
        new_height = new_width / per;

        if (new_height > rect_height)
        {
            double scale = new_height * 1.0 / rect_height;
            new_height = rect_height / scale;
            new_width = new_height * per;
        }

        CPoint leftTop(0, 0);
        rect = CRect(leftTop, CSize(new_width, new_height));
    }

    //图片居中显示
    int offset_x = (rect_width / 2 - new_width / 2);
    int offset_y = (rect_height / 2 - new_height / 2);
    CPoint leftTop(offset_x, offset_y);
    rect = CRect(leftTop, CSize(new_width, new_height));

    //开始作图
    CDC *pDC = GetDlgItem(ID)->GetDC();
    HDC hDC = pDC->GetSafeHdc();
    cimg.DrawToHDC(hDC, &rect); // 将图片绘制到显示控件的指定区域内
    ReleaseDC(pDC);
}

//打开图像按钮事件处理
void CPurePhotoDlg::OnBnClickedBtnOpen()
{
    CString strFilePath;
    string charPath;

    //选择图片
    CFileDialog fileDlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
        NULL,
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("JPG Files (*.jpg)|*.jpg|BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"),
        NULL);

    //判断路径不为空
    if (fileDlg.DoModal() == IDOK) {
        //获取图片路径和图片名称
        strFilePath = fileDlg.GetPathName();
        //CString strFileName = fileDlg.GetFileName();
        if (strFilePath == _T("")) return;
        charPath = CT2A(strFilePath);
    }
    else return;

    //更新调试信息
    UpdateData();
    debugInfo.SetString(strFilePath);
    UpdateData(FALSE);

    IplImage *image = NULL; //原始图像
    if (image) cvReleaseImage(&image);
    image = cvLoadImage(charPath.c_str(), CV_LOAD_IMAGE_COLOR); //显示图片

    Mat temp = imread(charPath, CV_LOAD_IMAGE_COLOR);
    temp.copyTo(*currentSrc);
    currentPicPath = charPath;

    //清空画面
    InvalidateRect(NULL);
    UpdateWindow();
    DrawPicToHDC(image, IDC_CV_PIC);
}

//关闭图像按钮事件处理
void CPurePhotoDlg::OnBnClickedBtnClose()
{
    //重新绘制画面
    InvalidateRect(NULL);
    UpdateWindow();
    //释放图像
    if (currentSrc)
        currentSrc->release();
}

//设定结构元素按钮事件处理
void CPurePhotoDlg::OnBnClickedBtnElmSet(){ pDlg->DoModal(); }

//重置图像按钮事件处理
void CPurePhotoDlg::OnBnClickedBtnResetPic()
{
    //判断是否载入
    if (!currentSrc->data)
    {
        MessageBox(_T("请先载入图像！"), _T("错误！"), MB_OK);
        return;
    }

    //重新绘制画面
    InvalidateRect(NULL);
    UpdateWindow();

    //载入画面显示
    IplImage *image = NULL; //原始图像
    if (image) cvReleaseImage(&image);
    image = cvLoadImage(currentPicPath.c_str(), CV_LOAD_IMAGE_COLOR); //显示图片
    DrawPicToHDC(image, IDC_CV_PIC);

    //载入栈区
    Mat temp = imread(currentPicPath, CV_LOAD_IMAGE_COLOR);
    temp.copyTo(*currentSrc);
}

//腐蚀按钮事件处理 OK
void CPurePhotoDlg::OnBnClickedBtnErode()
{
    if (!currentSrc->data)
    {
        MessageBox(_T("请先载入图像！"), _T("错误！"), MB_OK);
        return;
    }
    Mat dst;
    currentSrc->copyTo(dst);
    if (m_choose_style.GetCurSel() == 1)
        erode(*currentSrc, dst, Mat());
    else
        MultiFilter(ERODE, *currentSrc, dst);
    dst.copyTo(*currentSrc);
    //图像显示
    IplImage *img = new IplImage(*currentSrc);
    DrawPicToHDC(img, IDC_CV_PIC);
}

//膨胀按钮事件处理 OK
void CPurePhotoDlg::OnBnClickedBtnDilate()
{
    if (!currentSrc->data)
    {
        MessageBox(_T("请先载入图像！"), _T("错误！"), MB_OK);
        return;
    }
    Mat dst;
    currentSrc->copyTo(dst);
    if (m_choose_style.GetCurSel() == 1)
        dilate(*currentSrc, dst, Mat());
    else
        MultiFilter(DILATE, *currentSrc, dst);
    dst.copyTo(*currentSrc);
    //图像显示
    IplImage *img = new IplImage(*currentSrc);
    DrawPicToHDC(img, IDC_CV_PIC);
}

//二值化按钮事件处理
void CPurePhotoDlg::OnBnClickedBtnBin()
{
    if (!currentSrc->data)
    {
        MessageBox(_T("请先载入图像！"), _T("错误！"), MB_OK);
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

//开运算按钮事件处理 OK
void CPurePhotoDlg::OnBnClickedBtnClaOpen()
{
    if (!currentSrc->data)
    {
        MessageBox(_T("请先载入图像！"), _T("错误！"), MB_OK);
        return;
    }
    Mat dst;
    currentSrc->copyTo(dst);
    if (m_choose_style.GetCurSel() == 1)
        morphologyEx(*currentSrc, dst, MORPH_OPEN, Mat());
    else
        MultiFilter(OPEN, *currentSrc, dst);
    dst.copyTo(*currentSrc);
    //图像显示
    IplImage *img = new IplImage(*currentSrc);
    DrawPicToHDC(img, IDC_CV_PIC);
}

//闭运算按钮事件处理 OK
void CPurePhotoDlg::OnBnClickedBtnClaClose()
{
    if (!currentSrc->data)
    {
        MessageBox(_T("请先载入图像！"), _T("错误！"), MB_OK);
        return;
    }
    Mat dst;
    currentSrc->copyTo(dst);
    if (m_choose_style.GetCurSel() == 1)
        morphologyEx(*currentSrc, dst, MORPH_CLOSE, Mat());
    else
        MultiFilter(CLOSE, *currentSrc, dst);
    dst.copyTo(*currentSrc);
    //图像显示
    IplImage *img = new IplImage(*currentSrc);
    DrawPicToHDC(img, IDC_CV_PIC);
}

//多通道多类型算法 OK
void CPurePhotoDlg::MultiFilter(FilterType type, const Mat src, Mat &dst)
{
    Mat temp;
    vector<Mat> channels;//存储处理前三通道
    vector<Mat> dsts;//存储处理后三通道

    split(src, channels);
    split(src, dsts);
    for (int i = 0; i < 3; i++)
    {
        switch (type){
            case ERODE:
                ErodeFilter(channels[i].data, dsts[i].data, src.cols, src.rows);
                break;
            case DILATE:
                DilateFilter(channels[i].data, dsts[i].data, src.cols, src.rows);
                break;
            case OPEN:
                OpenFilter(channels[i].data, dsts[i].data, src.cols, src.rows);
                break;
            case CLOSE:
                CloseFilter(channels[i].data, dsts[i].data, src.cols, src.rows);
                break;
        }
    }
    merge(dsts, temp);
    temp.copyTo(dst);
}

//单通道膨胀算法 OK 再试试3*3或动态调整的
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
                    int pos = (m + IMG_OFFSET) * ST_WIDTH + n + IMG_OFFSET;
                    tmp[pos] = src_data[(i + m)*cols + j + n] + pDlg->elm[m + 2][n + 2];
                    if (tmp[pos] > 255) tmp[pos] = 255;
                }
            }
            int max = 0;
            for (int k = 0; k < ST_WIDTH*ST_WIDTH; k++)
                if (tmp[k] > max) max = tmp[k];
            dst_data[i*cols + j] = max;
        }
    }
}

//单通道腐蚀算法 OK
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
                    int pos = (m + IMG_OFFSET) * ST_WIDTH + n + IMG_OFFSET;
                    tmp[pos] = src_data[(i + m)*cols + j + n] - pDlg->elm[m + 2][n + 2];
                    if (tmp[pos] < 0) tmp[pos] = 0;
                }
            }
            int min = 0xffffff;
            for (int k = 0; k < ST_WIDTH*ST_WIDTH; k++)
                if (tmp[k] < min) min = tmp[k];
            dst_data[i*cols + j] = min;
        }
    }
}

//单通道开运算（腐蚀+膨胀）算法 OK
void CPurePhotoDlg::OpenFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows)
{
    uint8_t *proc = new uint8_t[cols*rows];
    memcpy(proc, src_data, sizeof(uint8_t)*cols*rows);
    ErodeFilter(src_data, proc, cols, rows);
    DilateFilter(proc, dst_data, cols, rows);
    delete proc;
}

//单通道闭运算（膨胀+腐蚀）算法 OK
void CPurePhotoDlg::CloseFilter(const uint8_t src_data[], uint8_t dst_data[], int cols, int rows)
{
    uint8_t *proc = new uint8_t[cols*rows];
    memcpy(proc, src_data, sizeof(uint8_t)*cols*rows);
    DilateFilter(src_data, proc, cols, rows);
    ErodeFilter(proc, dst_data, cols, rows);
    delete proc;
}

//二值化算法 OK
void CPurePhotoDlg::BinFilter(const Mat src, Mat &dst)
{
    Mat temp1, temp2;
    cvtColor(src, temp1, CV_RGB2GRAY);
    threshold(temp1, temp2, 100, 255, THRESH_BINARY);
    cvtColor(temp2, dst, CV_GRAY2RGB);
}

//保存图像按钮事件处理
void CPurePhotoDlg::OnBnClickedBtnSave()
{
    if (!currentSrc->data)
    {
        MessageBox(_T("请先载入图像！"), _T("错误！"), MB_OK);
        return;
    }
    CFileDialog fileDlg(FALSE, _T("png"), _T("MyProcImg.png"), 0, _T("image Files(*.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*||"), this);

    if (IDOK == fileDlg.DoModal())
    {
        CString strFilePath = fileDlg.GetPathName();
        if (strFilePath)
        {
            string str = CStringA(strFilePath);
            imwrite(str, *currentSrc);//将image图像保存
            MessageBox(_T("图像已成功保存!"), _T("操作成功！"), MB_OK);
        }
    }
}
