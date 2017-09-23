
// expDlg.h : 头文件
//

#pragma once
#include "afxwin.h"



// CexpDlg 对话框
class CexpDlg : public CDialogEx
{
// 构造
public:
	CexpDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EXP_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonRecati();
	afx_msg void OnBnClickedButtonConati();
public:
	/* 力传感器，全局变量*/
	WORD wVersionRequested;
	WSADATA wsaData;
	CString strState;
	CListBox m_list_State;
	SOCKET socketHandle;
	struct sockaddr_in addrCin; 
	int COMMAND;
	int NUM_SAMPLES;
	char request[8];
	CString strDebug;
	void SensorData();

	/* UR机器人，全局变量*/
	//SOCKET sockClient;
	struct sockaddr_in addrSrv;
	void SendData();

	CStatic m_static_sensordata;
	afx_msg void OnBnClickedButtonConur();
	CStatic m_list_state1;
	afx_msg void OnBnClickedButtonRecvur();
	CStatic m_list_state2;
};

