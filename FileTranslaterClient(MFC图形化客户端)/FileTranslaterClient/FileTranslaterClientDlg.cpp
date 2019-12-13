
// FileTranslaterClientDlg.cpp: 实现文件
//
/*
	@author:yuheng
	@time:20191101
*/
#include "stdafx.h"
#include "FileTranslaterClient.h"
#include "FileTranslaterClientDlg.h"
#include "afxdialogex.h"
#include <iostream>
using namespace std;

#define PORT 8087
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
#pragma comment(lib, "WS2_32")
#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CFileTranslaterClientDlg 对话框



CFileTranslaterClientDlg::CFileTranslaterClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILETRANSLATERCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileTranslaterClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_IPADDRESS1, serverIP);
	DDX_Control(pDX, IDC_LIST1, messageList);
	DDX_Control(pDX, IDC_IPADDRESS1, severIP);
	DDX_Control(pDX, IDC_EDIT1, UpLoadName);
	DDX_Control(pDX, IDC_EDIT2, download_path);
}

BEGIN_MESSAGE_MAP(CFileTranslaterClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CFileTranslaterClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CFileTranslaterClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFileTranslaterClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFileTranslaterClientDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CFileTranslaterClientDlg::OnBnClickedButton5)
	ON_EN_CHANGE(IDC_EDIT2, &CFileTranslaterClientDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT1, &CFileTranslaterClientDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON4, &CFileTranslaterClientDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CFileTranslaterClientDlg 消息处理程序

BOOL CFileTranslaterClientDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileTranslaterClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileTranslaterClientDlg::OnPaint()
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
HCURSOR CFileTranslaterClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileTranslaterClientDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void CFileTranslaterClientDlg::OnBnClickedButton1()
{
	//初始化 socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 2);
	assert(WSAStartup(socketVersion, &wsaData) == 0);

	//创建socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	assert(SOCKET_ERROR != client_socket);

	//指定服务端的地址
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	CString SERVER_IP;
	
	severIP.GetWindowTextA(SERVER_IP);

	server_addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	server_addr.sin_port = htons(PORT);
	if (SOCKET_ERROR != connect(client_socket, (LPSOCKADDR)&server_addr, sizeof(server_addr)))
	{
		messageList.AddString("连接成功");
	}
	else
	{
		messageList.AddString("连接失败");
	}

	
}


void CFileTranslaterClientDlg::OnBnClickedButton2()
{
	CFileDialog opendlg(true, NULL, NULL, 0, "All Files(*.*)|*.*||");
	if (opendlg.DoModal() == IDOK) { //显示对话框并允许用户进行选择。
		upload_file_location = opendlg.GetPathName(); //文件路径
		UpLoadName.SetWindowTextA(upload_file_location); //在编辑框显示文件路径
		messageList.AddString("打开文件成功");
	}
}

void CFileTranslaterClientDlg::OnBnClickedButton3()
{
	//标识上传 send(client_socket,"1", sizeof("1"), 0);
	char FileLocation[FILE_NAME_MAX_SIZE];
	strcpy(FileLocation, upload_file_location);
	int file_Location_size = strlen(FileLocation);
	while (FileLocation[file_Location_size] != '\\'&&file_Location_size > 0) { file_Location_size--; }
	char real_name[FILE_NAME_MAX_SIZE];
	memset(real_name, 0, sizeof(real_name));
	strcpy(real_name, FileLocation + file_Location_size + 1);                      //在这里存放真实名字
	cout << "发送到服务器的真实名字：\t" << real_name << endl;						//这个语句是用来测试的

	messageList.AddString(CString("要上传的文件是:")+CString(real_name));          //把名字显示在右边

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	
	if (send(client_socket, real_name, sizeof(real_name), 0) < 0)  //标记
	{
		messageList.AddString("在发送文件名字时出错" + WSAGetLastError());
		MessageBox("发送文件名时失败");
	}
	CString test;
	FILE *fp = fopen(FileLocation, "rb");  //以只读，二进制的方式打开一个文件


	if (NULL == fp)
	{
		messageList.AddString("这个文件不存在");
	}
	else
	{
		memset(buffer, 0, BUFFER_SIZE);
		int length = 0;

		while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
		{
			if (send(client_socket, buffer, length, 0) < 0)
			{
				messageList.AddString("文件发送失败"+WSAGetLastError());
				MessageBox("发送文件时失败");
				break;
			}
			memset(buffer, 0, BUFFER_SIZE);
		}

		fclose(fp);
	}

	messageList.AddString("文件发送成功");
	std::cout << "文件: " << real_name << std::endl << "发送成功 !" << std::endl;
	closesocket(client_socket);
	//释放 winsock 库
	WSACleanup();
}


void CFileTranslaterClientDlg::OnBnClickedButton5()
{
	//打开路径
	BROWSEINFO bi;
	char Buffer[MAX_PATH] = { 0 };
	//初始化入口参数bi开始
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = "打开路径";
	bi.ulFlags = BIF_EDITBOX;//包括文件
	bi.lpfn = NULL;
	bi.iImage = IDR_MAINFRAME;

	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if (pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);//取得文件夹路径到Buffer里
	}
	store_file_location = CString(Buffer);
	messageList.AddString("选择存放路径成功:"+store_file_location);
}


void CFileTranslaterClientDlg::OnEnChangeEdit2()
{
	download_path.GetWindowTextA(download_file_location);
}


void CFileTranslaterClientDlg::OnEnChangeEdit1()
{
}


void CFileTranslaterClientDlg::OnBnClickedButton4()
{
	//send(client_socket, "2", sizeof("2"), 0);标识下载
	//这是从服务器上下载东西的代码
	//输入文件名
	char file_name[FILE_NAME_MAX_SIZE + 1];
	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
	std::cout << "输入你想要在服务器上获取的名字：" << std::endl;  //测试
	strcpy(file_name, download_file_location);
	messageList.AddString(CString("要下载的路径：") + download_file_location);
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	strncpy(buffer, file_name, strlen(file_name) > BUFFER_SIZE ? BUFFER_SIZE : strlen(file_name));  //取较短的

	//向服务器发送文件名
	assert(send(client_socket, buffer, BUFFER_SIZE, 0) >= 0);

	//打开文件，准备写入
	//这里可以用filename代替
	char receive[FILE_NAME_MAX_SIZE];
	cout << "你想放在哪里？" << endl;
	strcpy(receive, store_file_location);
	int file_name_size = strlen(file_name);

	while (file_name[file_name_size] != '\\'&&file_name_size > 0) { file_name_size--; }
	char real_name[FILE_NAME_MAX_SIZE];
	memset(real_name, 0, sizeof(real_name));
	strcpy(real_name, file_name + file_name_size + 1);

	strcat(receive, "\\");
	cout << "接收到的真实名字" << real_name << endl;
	strcat(receive, real_name);
	FILE *fp = fopen(receive, "wb");  //以只写，二进制的方式打开一个文件
	assert(NULL != fp);
	memset(buffer, 0, BUFFER_SIZE);
	int length = 0;
	while ((length = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0)
	{
		assert(fwrite(buffer, sizeof(char), length, fp) >= (size_t)length);
		memset(buffer, 0, BUFFER_SIZE);
	}
	messageList.AddString("下载成功");
	std::cout << "接受文件 : " << file_name << endl << " 接收成功 !" << std::endl;
	fclose(fp);
	closesocket(client_socket);
}
