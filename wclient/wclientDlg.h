
// wclientDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"



#define	CLIENT_MESSAGE			(WM_USER + 77)
#define	WM_MESSAGE_SEND_MOVE	(WM_USER + 78)
#define	WM_MESSAGE_SEND_MODE		(WM_USER + 79)



// CwclientDlg 대화 상자
class CwclientDlg : public CDialogEx
{
// 생성입니다.
public:
	CwclientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonImage();

public:
	HDC		m_hDC;
	CStatic m_staticScreen;
	int		InitClientSocket(void);
	SOCKET	m_socket;
	struct sockaddr_in	m_srv_addr;
	afx_msg void OnBnClickedButtonConnect();
	void	DestroySocket(SOCKET parm_socket);
	int		m_frameCount;
	void	DrawJPEG(byte* pBuf, int nSize);
	int		m_nPresentWidth;
	int		m_nPresentHeight;
	void	ReLocationItems(int nX, int nY);
	int		m_nDragOn;
	int		m_nMoveOn;
	int		m_nDragX, m_nDragY;
	int		m_nMode;
	int		m_nVerticalFlip;
protected:
	afx_msg LRESULT OnClientMessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CIPAddressCtrl m_ipAddressCtrl;
	CEdit m_ceditPort;
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	afx_msg LRESULT OnMessageSendMove(WPARAM wParam, LPARAM lParam);
public:
//	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
protected:
	afx_msg LRESULT OnMessageSendMode(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
