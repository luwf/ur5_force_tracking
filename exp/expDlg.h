
// expDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"



// CexpDlg �Ի���
class CexpDlg : public CDialogEx
{
// ����
public:
	CexpDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EXP_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonRecati();
	afx_msg void OnBnClickedButtonConati();
public:
	/* ����������ȫ�ֱ���*/
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

	/* UR�����ˣ�ȫ�ֱ���*/
	//SOCKET sockClient;
	struct sockaddr_in addrSrv;
	void SendData();

	CStatic m_static_sensordata;
	afx_msg void OnBnClickedButtonConur();
	CStatic m_list_state1;
	afx_msg void OnBnClickedButtonRecvur();
	CStatic m_list_state2;
};

