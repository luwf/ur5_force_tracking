
// expDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "exp.h"
#include "expDlg.h"
#include "afxdialogex.h"
#include "data.h"
#include <string>
#include "Function.h"
#include<fstream>
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int ThreadFlag_ATI;  //时间线程
int ThreadFlag_UR;
int ThreadFlag_UR1;
DWORD WINAPI TimeThread_ATI(LPVOID lpParameter);  //线程
DWORD WINAPI TimeThread_UR(LPVOID lpParameter);  //线程
DWORD WINAPI TimeThread_UR1(LPVOID lpParameter);  //线程

SOCKET sockClient;
SOCKET sockClient1;
float Sensor_F_T[6];
ofstream atidata("atidata.txt",ios::out);
ofstream urdata("urdata.txt",ios::out);
float filter[30];

UrRobotState ur_msg;

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


// CexpDlg 对话框




CexpDlg::CexpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CexpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CexpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_AtiData, m_static_sensordata);
	DDX_Control(pDX, IDC_STATIC_SendData, m_list_state1);
	DDX_Control(pDX, IDC_STATIC_UrData, m_list_state2);
}

BEGIN_MESSAGE_MAP(CexpDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ConAti, &CexpDlg::OnBnClickedButtonConati)
	ON_BN_CLICKED(IDC_BUTTON_ConUr, &CexpDlg::OnBnClickedButtonConur)
	ON_BN_CLICKED(IDC_BUTTON_RecvUr, &CexpDlg::OnBnClickedButtonRecvur)
END_MESSAGE_MAP()


// CexpDlg 消息处理程序

BOOL CexpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	wVersionRequested = MAKEWORD( 1, 1 );
	int err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		AfxMessageBox(_T("TCP/IP初始化错误"));
	}

	if ( LOBYTE( wsaData.wVersion ) != 1 ||HIBYTE( wsaData.wVersion ) != 1 ) 
	{
		WSACleanup( );		
	}

	for(int i=0;i<30;i++)
	{
		filter[i]=0;
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CexpDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CexpDlg::OnPaint()
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
HCURSOR CexpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CexpDlg::OnBnClickedButtonConati()
{
	// TODO: 在此添加控件通知处理程序代码

	socketHandle=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

	addrCin.sin_addr.S_un.S_addr=inet_addr("192.168.1.84");
	addrCin.sin_family=AF_INET;
	addrCin.sin_port=htons(49152);

	int co= connect(socketHandle, (struct sockaddr *)&addrCin, sizeof(addrCin) ) ;

	COMMAND=3; /*传感器发送模式，1：高速实时发送模式7000HZ，2：高速缓冲发送模式，3：多传感器同步发送，0：停止发送，*/

	NUM_SAMPLES = 0;/*采样个数，可为任意设定的个数,   设为0时则不停的连续发送*/                 

	*(uint16*)&request[0] = htons(0x1234); /* standard header. */
	*(uint16*)&request[2] = htons(COMMAND); /* per table 9.1 in Net F/T user manual. */
	*(uint32*)&request[4] = htonl(NUM_SAMPLES); /* see section 9.1 in Net F/T user manual. */

	send( socketHandle, request, 8, 0 );


	HANDLE TimeFlag;
	TimeFlag=CreateThread(NULL, 0, TimeThread_ATI, this,NULL, NULL); //开启数据接收线程
	ThreadFlag_ATI=1;
}


DWORD WINAPI TimeThread_ATI(LPVOID lpParameter)
{
	CexpDlg* pView = (CexpDlg*)lpParameter;
	LARGE_INTEGER  litmp; 
	double dfMinus, dfFreq, dfTim;
	LONGLONG QPart1,QPart2;
	QueryPerformanceFrequency(&litmp); 
	dfFreq = (double)litmp.QuadPart; // 获得计数器的时钟频率

	while(ThreadFlag_ATI)
	{
		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;// 获得初始值
		do{
			QueryPerformanceCounter(&litmp); 
			QPart2 = litmp.QuadPart;// 获得中止值 
			dfMinus = (double)(QPart2-QPart1); 
			dfTim = dfMinus / dfFreq; // 获得对应的时间值，单位为秒
		}while(dfTim<0.001);  // 1ms	

		pView->SensorData();//采集力传感器数据
	}
	return 0;
}

void CexpDlg::SensorData()
{
	RESPONSE resp;			
	byte response[36];
	int i;
	float sum=0;

	//********* 传感实测量值*****//
	recv( socketHandle, (char *)response, 36, 0 );
	resp.rdt_sequence = ntohl(*(uint32*)&response[0]);
	resp.ft_sequence = ntohl(*(uint32*)&response[4]);
	resp.status = ntohl(*(uint32*)&response[8]);

	for( i = 0; i < 6; i++ )
	{
		resp.FTData[i] =ntohl(*(int32*)&response[12 + i * 4]);
		Sensor_F_T[i]=resp.FTData[i]*0.000001;
	}

	for(int j=29;j>0;j--)
	{
		filter[j]=filter[j-1];
		sum=sum+filter[j];
	}
	filter[0]=Sensor_F_T[2];

	Sensor_F_T[2]=(sum+filter[0])/30;

	strDebug.Format(_T("%.3f %.3f %.3f %.3f %.3f %.3f"),Sensor_F_T[0],Sensor_F_T[1],Sensor_F_T[2],Sensor_F_T[3],Sensor_F_T[4],Sensor_F_T[5]);
	m_static_sensordata.SetWindowText(strDebug);
}

void CexpDlg::OnBnClickedButtonConur()
{
	// TODO: 在此添加控件通知处理程序代码

	sockClient=socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr("192.168.1.102");
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(30002);
	
	int flag=connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	if(flag==0)
	{
		strState.Format(_T("连接UR机器人成功"));
		
		m_list_state1.SetWindowText(strState);

		HANDLE tempflag;
		tempflag=CreateThread(NULL, 0, TimeThread_UR, this,NULL, NULL); //开启数据发送线程
		ThreadFlag_UR=1;
	}
	else
	{
		strState.Format(_T("连接UR机器人失败"));
		m_list_state1.SetWindowText(strState);
	}
}

DWORD WINAPI TimeThread_UR(LPVOID lpParameter)
{
	CexpDlg* pView = (CexpDlg*)lpParameter;
	LARGE_INTEGER  litmp; 
	double dfMinus, dfFreq, dfTim;
	LONGLONG QPart1,QPart2;
	QueryPerformanceFrequency(&litmp); 
	dfFreq = (double)litmp.QuadPart; // 获得计数器的时钟频率
	
	double x0,y0,h,xr;
	h=0.005;
	x0=-0.5872;
	y0=0;
	xr=-0.7;

	CString temp1=_T("movel(p[");
	CString temp2=_T(",-0.092,0.696,2.55,2.40,-2.34],a=0.01,v=0.01,t=0.008)\n\r");
	CString temp3;
	CString buf;
	char *pBuff;

	while(ThreadFlag_UR)
	{
		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;// 获得初始值
		do{
			QueryPerformanceCounter(&litmp); 
			QPart2 = litmp.QuadPart;// 获得中止值 
			dfMinus = (double)(QPart2-QPart1); 
			dfTim = dfMinus / dfFreq; // 获得对应的时间值，单位为秒
		}while(dfTim<0.015);  // 20ms	

		/*atidata<<Sensor_F_T[2]<<endl;*/

		if(Sensor_F_T[2]>-6.5)
		{
			Result r_pro=hk(h,x0,y0,xr);
			x0=r_pro.xi;
			y0=r_pro.yi;
		}
		else
		{
			res r_con=iteration(h,x0,y0,xr,Sensor_F_T[2]);
			x0=r_con.xi;
			y0=r_con.yi;
			xr=r_con.xri;
		}
			temp3.Format(_T("%f"),x0);
			buf=temp1+temp3+temp2;

			USES_CONVERSION;
			pBuff=T2A(buf.GetBuffer(0));

			send(sockClient,pBuff,strlen(pBuff),0);

			pView->m_list_state1.SetWindowText(buf);;
	}

	return 0;
}

void CexpDlg::OnBnClickedButtonRecvur()
{
	// TODO: 在此添加控件通知处理程序代码

	sockClient1=socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv1;
	addrSrv1.sin_addr.S_un.S_addr=inet_addr("192.168.1.102");
	addrSrv1.sin_family=AF_INET;
	addrSrv1.sin_port=htons(30003);

	int flag=connect(sockClient1,(SOCKADDR*)&addrSrv1,sizeof(SOCKADDR));

	if(flag==0)
	{
		strState.Format(_T("连接UR机器人成功"));

		m_list_state1.SetWindowText(strState);

		HANDLE tempflag;
		tempflag=CreateThread(NULL, 0, TimeThread_UR1, this,NULL, NULL); //开启数据发送线程
		ThreadFlag_UR1=1;
	}
	else
	{
		strState.Format(_T("连接UR机器人失败"));
		m_list_state1.SetWindowText(strState);
	}
}

DWORD WINAPI TimeThread_UR1(LPVOID lpParameter)
{
	CString temp;

	while(ThreadFlag_UR1)
	{
		CexpDlg* pView = (CexpDlg*)lpParameter;
		LARGE_INTEGER  litmp; 
		double dfMinus, dfFreq, dfTim;
		LONGLONG QPart1,QPart2;
		QueryPerformanceFrequency(&litmp); 
		dfFreq = (double)litmp.QuadPart; // 获得计数器的时钟频率

		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;// 获得初始值
		do{
			QueryPerformanceCounter(&litmp); 
			QPart2 = litmp.QuadPart;// 获得中止值 
			dfMinus = (double)(QPart2-QPart1); 
			dfTim = dfMinus / dfFreq; // 获得对应的时间值，单位为秒
		}while(dfTim<0.008);  // 8ms

		urdata<<ur_msg.Tool_vector_actual[0]<<endl;
		atidata<<Sensor_F_T[2]<<endl;

		char recvBuf[1060];
		ZeroMemory(recvBuf,1060);
		recv(sockClient1,recvBuf,1060,0);
		memcpy((char*)(&ur_msg),recvBuf,sizeof(ur_msg));

		ur_msg.Tool_vector_actual[0]= SwapDoubleEndian(&ur_msg.Tool_vector_actual[0]);

		temp.Format(_T("%f"),ur_msg.Tool_vector_actual[0]);
		/*urdata<<ur_msg.Tool_vector_actual[0]<<endl;*/
		
		pView->m_list_state2.SetWindowText(temp);
	}
	
	return 0;
}