
// wclientDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "wclient.h"
#include "wclientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
////debug start
//#include <stdio.h>
//#include <stdlib.h>
 
//typedef unsigned short WORD;
//typedef unsigned long  DWORD;
//typedef unsigned long  LONG;
//typedef unsigned char  BYTE;
 
#ifndef BI_RGB
#define BI_RGB         0
#endif
 
#pragma pack(1)
 
//typedef struct tagBITMAPFILEHEADER
//{
//    WORD   bfType;
//    DWORD  bfSize;
//    WORD   bfReserved1;
//    WORD   bfReserved2;
//    DWORD  bfOffBits;
//}BITMAPFILEHEADER;
 
//typedef struct tagBITMAPINFOHEADER
//{
//    DWORD   biSize;
//    LONG    biWidth;
//    LONG    biHeight;         
//    WORD    biPlanes;         
//    WORD    biBitCount;       
//    DWORD   biCompression;   
//    DWORD   biSizeImage;      
//    LONG    biXPelsPerMeter;   
//    LONG    biYPelsPerMeter;  
//    DWORD   biClrUsed;        
//    DWORD   biClrImportant;    
//}BITMAPINFOHEADER;
// 
//typedef struct tagRGBQUAD
//{
//       BYTE rgbBlue;
//       BYTE rgbGreen;
//       BYTE rgbRed;
//       BYTE rgbReserved;
//}RGBQUAD;
 
#pragma pack()
 
// �׷��� RAW ������ �׷��� ��Ʈ�� ���Ϸ� ��ȯ
int Color_Raw2Bmp(char *pRawName, DWORD nWidth, DWORD nHeight, char *pBmpName)
{
       BITMAPFILEHEADER  file_h;
       BITMAPINFOHEADER  info_h;
       DWORD             dwBmpSize=0;
       DWORD             dwRawSize=0;
       DWORD             dwLine=0;
       long              lCount, i;
       FILE             *in, *out;
       char             *pData=NULL;
       RGBQUAD           rgbPal[256];
 
       in=fopen(pRawName, "rb");
       if (in==NULL)
       {
         printf("File Open Error!\n");
              return 0;
       }
      
       out=fopen(pBmpName, "wb");
      
       file_h.bfType      = 0x4D42;
       file_h.bfReserved1 = 0;
       file_h.bfReserved2 = 0;
       file_h.bfOffBits   =  sizeof(rgbPal) + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
       info_h.biSize          = sizeof(BITMAPINFOHEADER);
       info_h.biWidth         = (DWORD)nWidth;
       info_h.biHeight        = (DWORD)nHeight;
       info_h.biPlanes        = 1;
       info_h.biBitCount      = 24;// ����̸� 8
       info_h.biCompression   = BI_RGB;
       info_h.biXPelsPerMeter = 0;
       info_h.biYPelsPerMeter = 0;
       info_h.biClrUsed       = 0;
       info_h.biClrImportant  = 0;
      
       dwLine=((((info_h.biWidth * info_h.biBitCount) + 31) &~ 31) >> 3) ;
       dwBmpSize=dwLine * info_h.biHeight;
       info_h.biSizeImage     = dwBmpSize;
       file_h.bfSize          = dwBmpSize + file_h.bfOffBits + 2;
 
	   dwRawSize=info_h.biWidth*info_h.biHeight*(info_h.biBitCount/8);
       pData=(char *)malloc(sizeof(char)*dwRawSize+16);
      
       if (pData)
       {
             fread(pData, 1, dwRawSize, in);
           
             for(i=0; i < 256; i++)
             {
                    rgbPal[i].rgbRed=(BYTE)(i%256);
                    rgbPal[i].rgbGreen=rgbPal[i].rgbRed;
                    rgbPal[i].rgbBlue=rgbPal[i].rgbRed;
                    rgbPal[i].rgbReserved=0;
             }
            
             fwrite((char *)&file_h, 1, sizeof(BITMAPFILEHEADER), out);
             fwrite((char *)&info_h, 1, sizeof(BITMAPINFOHEADER), out);
             fwrite((char *)rgbPal, 1, sizeof(rgbPal), out);
             lCount=dwRawSize;
            
			 for(lCount-=(long)(info_h.biWidth*(info_h.biBitCount/8)); lCount >= 0; lCount-=(long)(info_h.biWidth*(info_h.biBitCount/8)))
             {
                    fwrite((pData+lCount), 1, (long)dwLine, out);
             }
            
             free(pData);
       }
 
       fclose(in);
       fclose(out);
 
       return 1;
}
 
int convert(int argc, char **argv)
{
    char szRawName[80] = "0002.bin";
    char szBmpName[80] = "bintobmp.bmp";
    DWORD	nWidth = 640;
	DWORD	nHeight = 480;
 
    printf("RAW File Name ? ");
    //gets(szRawName);
    //fflush(stdin);
    printf("BMP File Name ? ");
    //gets(szBmpName);
    //fflush(stdin);
    printf("Raw Width ? ");
    //scanf("%lu",&nWidth);
    printf("Raw Height ? ");
    //scanf("%lu",&nHeight);
 
    Color_Raw2Bmp(szRawName, nWidth, nHeight, szBmpName);
 
    return 0;
}
////debuf end

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CwclientDlg ��ȭ ����




CwclientDlg::CwclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CwclientDlg::IDD, pParent),
	m_socket(INVALID_SOCKET),
	m_nPresentWidth(0),
	m_nPresentHeight(0),
	m_nVerticalFlip(0),
	m_nMode(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SCREEN, m_staticScreen);
	DDX_Control(pDX, IDC_IPADDRESS_GV, m_ipAddressCtrl);
	DDX_Control(pDX, IDC_EDIT_PORT, m_ceditPort);
}

BEGIN_MESSAGE_MAP(CwclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_IMAGE, &CwclientDlg::OnBnClickedButtonImage)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CwclientDlg::OnBnClickedButtonConnect)
	ON_MESSAGE(CLIENT_MESSAGE, &CwclientDlg::OnClientMessage)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CwclientDlg::OnBnClickedButtonDisconnect)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MESSAGE_SEND_MOVE, &CwclientDlg::OnMessageSendMove)
//	ON_WM_MBUTTONDBLCLK()
	ON_MESSAGE(WM_MESSAGE_SEND_MODE, &CwclientDlg::OnMessageSendMode)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CwclientDlg �޽��� ó����

BOOL CwclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//convert(0,0);return FALSE;// bin to BMP
	int	nTT = -555;
	int	nAA;

	nAA = nTT % 500;
	nTT = (unsigned int)nAA;

	m_hDC = m_staticScreen.GetDC()->GetSafeHdc();	// HDC �ޱ�

	m_ipAddressCtrl.SetWindowTextA("127.0.0.1");
	m_ceditPort.SetWindowTextA("12777");

#define W	640
#define H	480

	ReLocationItems(W, H);

	m_frameCount = 0;								// Frame count per second on 1 sec timer

	SetTimer(1, 1000, 0);							// Timer ����

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CwclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		// ������ ��
		if(nID == 0xf060){
			if(m_socket != INVALID_SOCKET){
				int	nData = 99;
				send(m_socket, (const char*)&nData, 4, 0);			// Program quit command
				Sleep(1000);
				closesocket(m_socket);
			}

			KillTimer(1);
		}
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CwclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CwclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// **deprecated
//http://www.devpia.com/MAEUL/Contents/Detail.aspx?BoardID=50&MAEULNo=20&no=886642&ref=886633
//http://stackoverflow.com/questions/10461324/create-hbitmap-from-jpeg-memory-buffer
void CwclientDlg::OnBnClickedButtonImage()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	byte*	pImageBuf;
	CFile	file;
	int		nSize;
	CImage	img;
	IStream*	pStream;

	if(file.Open(".\\test_mjpeg_org.jpg", CFile::modeRead) == false){
		AfxMessageBox("File load fail!");
		return;
	}

	if(SUCCEEDED(CreateStreamOnHGlobal(NULL, TRUE, &pStream)) < 0){
		AfxMessageBox("CreateStreamOnHGlobal fail!");
		return;
	}

	file.SeekToBegin();					// Set begin
	nSize = file.GetLength();			// File size

	pImageBuf = new byte[nSize];		// Buffer allocate

	file.Read(pImageBuf, nSize);		// Read file

	ULONG	ulWirteByte = 0;
	ULARGE_INTEGER	sz;

	sz.HighPart = 0;
	sz.LowPart = nSize;

	pStream->SetSize(sz);

	pStream->Write(pImageBuf, nSize, &ulWirteByte);	// memory copy to IStream

	delete pImageBuf;

	img.Load(pStream);

	int	nWidth, nHeight;

	nWidth = img.GetWidth();
	nHeight = img.GetHeight();

	// â�� ũ�⸦ �����մϴ�.
	ReLocationItems(nWidth, nHeight);

	img.Draw(m_hDC, 0, 0);
}



// Ŭ���̾�Ʈ ���� �ʱ�ȭ
int CwclientDlg::InitClientSocket(){
	WORD	wVersionRequested;
	WSADATA	wsaData;
	int		nError = 0;
	char	chIp[16];
	byte	ipA, ipB, ipC, ipD;
	USHORT	nPort;
	CString	str;

	if(m_socket != INVALID_SOCKET){
		return -1;
	}

	wVersionRequested = MAKEWORD(2, 2);

	nError = WSAStartup(wVersionRequested, &wsaData);
	if(nError != 0){
		AfxMessageBox("Error] WSAStartup error");
		return -1;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, 0);			// ���� ����

	if(m_socket == INVALID_SOCKET){
		return -1;
	}

	memset(&m_srv_addr, 0, sizeof(struct sockaddr_in));	// �ʱ�ȭ

	// �����쿡 ���� IP�� PORT�� �޾ƿɴϴ�.
	UpdateData(TRUE);
	m_ipAddressCtrl.GetAddress(ipA, ipB, ipC, ipD);
	m_ceditPort.GetWindowTextA(str);
	nPort = (USHORT)_ttoi(str);

	sprintf_s(chIp, 16, "%d.%d.%d.%d", ipA, ipB, ipC, ipD);

	// IP�� PORT�� �ְ� ������ �����մϴ�.
	m_srv_addr.sin_family = AF_INET;
	m_srv_addr.sin_addr.s_addr = inet_addr(chIp);
	m_srv_addr.sin_port = htons(nPort);

	WSAAsyncSelect(m_socket, m_hWnd, CLIENT_MESSAGE, FD_CONNECT);

	/*if(connect(m_socket, (struct sockaddr *)&m_srv_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR){
		m_socket == INVALID_SOCKET;
		return -1;
	}*/

	connect(m_socket, (struct sockaddr *)&m_srv_addr, sizeof(struct sockaddr_in));
	// ���� ���δ� �޼��� �ݹ� �Լ����� Ȯ�� ����

	return 1;
}



// �����ư
//http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=663
//http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=667
//http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=668
void CwclientDlg::OnBnClickedButtonConnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int	nResult = 0;

	if(InitClientSocket() == -1){								// Client socket ����
		int	nError = WSAGetLastError();

		switch(nError){
		case WSAEINVAL:
			AfxMessageBox("WSAEINVAL");
			break;
		default:
			AfxMessageBox("���� ���� ����");
			break;
		}

		AfxMessageBox("������ ������ �ȉ���ϴ�.");

		return;
	}


}



// Client message
afx_msg LRESULT CwclientDlg::OnClientMessage(WPARAM wParam, LPARAM lParam)
{
	int		nSize = 0;
	int		nReceiveSize = 0;
	int		nReceiveMaxSize = 0;

	switch(WSAGETSELECTEVENT(lParam)){
	case FD_CONNECT:
		WSAAsyncSelect(m_socket, m_hWnd, CLIENT_MESSAGE, FD_READ);
		//AfxMessageBox("connected");
		break;
	case FD_READ:
		byte	bufSize[16];

		// ���ۼӵ� ���̿� ���� FD_READ�� �ߺ��Ǿ� ���� �� �����Ƿ�
		// FD_READ�� ����ǰ� �ִ� ����, FD_READ�� �񵿱⸦ Ǯ���ش�.
		WSAAsyncSelect(m_socket, m_hWnd, CLIENT_MESSAGE, FD_CLOSE);

		// recv(������ ����ϴ� ����, ���� ������ ũ�⸦ ������ ����, ���� �������� ũ��, flag);
		// Size
		nReceiveSize = 4;
		while(nReceiveSize){
			nSize = recv(m_socket, (char*)&bufSize[4-nReceiveSize], nReceiveSize, 0);
			if(nSize < 0){
				int	nError = WSAGetLastError();
				// do - error
				switch(nError){
				case WSAEINVAL:
				case WSAEWOULDBLOCK:
					//AfxMessageBox("WSAEINVAL");
					Sleep(1);
					break;
				case WSAECONNABORTED:
					Sleep(1);
					break;
				default:
					// do - error
					//AfxMessageBox("nReceiveDataError");
					return 0;
				}
			}else{
				nReceiveSize = nReceiveSize - nSize;
			}
		}
		
		// Data
		nReceiveSize = *(int*)bufSize;
		nReceiveMaxSize = nReceiveSize;

		byte*	buf;

		buf = new byte[1280*720*4];

		while(nReceiveSize > 0){
			nSize = recv(m_socket, (char*)&buf[nReceiveMaxSize-nReceiveSize], nReceiveSize, 0);
			if(nSize < 0){
				int	nError = WSAGetLastError();

				switch(nError){
				case WSAEINVAL:
				case WSAEWOULDBLOCK:
					//AfxMessageBox("WSAEINVAL");
					Sleep(1);
					break;
				case WSAECONNABORTED:
					Sleep(1);
					break;
				default:
					// do - error
					delete buf;
					//AfxMessageBox("nReceiveDataError");
					return 0;
				}
			}else{
				nReceiveSize = nReceiveSize - nSize;
			}
		}

		// Draw JPEG Image
		DrawJPEG(buf, nReceiveMaxSize);
		Sleep(1);
#if 0
		{
		CImage		cimg;
		
		cimg.Create(320, 240, 24);

		//memcpy(pCimBuf, pIplCamera->imageData, 640*480*3);
		byte	*pHBuf, *pVBuf;
		int		nDestPixelWidth;
		int		nPitch;
		int		nDestHeight;

		nDestHeight = cimg.GetHeight();
		nPitch = cimg.GetPitch();
		nDestPixelWidth = cimg.GetWidth() * (cimg.GetBPP()/8);
		pVBuf = (byte*)cimg.GetBits();

		for(int i=0;i<nDestHeight;i++){
			pHBuf = pVBuf + (i * nPitch);
			for(int j=0;j<nDestPixelWidth;j++){
				//pHBuf[j] = pIplCamera->imageData[j+(i*nDestPixelWidth)];
				pHBuf[j] = buf[0];
			}
		}
		
		cimg.Draw(m_hDC, 0, 0);
		}
#endif

		m_frameCount++;

		delete buf;

		// �ٽ� ���ο� ������ �ޱ� ���Ͽ�, �񵿱� ������ FD_READ�� �߰��Ѵ�.
		WSAAsyncSelect(m_socket, m_hWnd, CLIENT_MESSAGE, FD_READ | FD_CLOSE);

		//AfxMessageBox("read");
		break;
	case FD_CLOSE:
		DestroySocket(m_socket);
		m_socket = INVALID_SOCKET;
		WSACleanup();
		AfxMessageBox("close");
		break;
	}
	return 0;
}



// 
void CwclientDlg::DestroySocket(SOCKET parm_socket){
	// �Ѿ�� ������ ��ȿ���� ���� ������ ��� �������� �ʰ� ������.
	if(parm_socket == INVALID_SOCKET){
		return;
	}

	// LINGER ����ü�� ���� l_linger�� LINGER�ɼ��� TRUE�� ���� �������ش�.
	LINGER l_linger = {TRUE, 0};

	// ���� �ɼ��� �����Ѵ�.
	// setsockopt(�ɼ��� �����ų ����, level, �ɼ��̸�, �ɼ��� ������ ���, �ɼ��� ũ��);
	setsockopt(parm_socket, SOL_SOCKET, SO_LINGER, (char FAR *)&l_linger, sizeof(l_linger));


	// �Ѿ�� ���� ����
	closesocket(parm_socket);
}



// **deprecated
void CwclientDlg::OnBnClickedButtonDisconnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	byte	buf[8];
	int		nSize = 0;

	if(m_socket == INVALID_SOCKET){
		return;
	}

	DestroySocket(m_socket);
	//closesocket(m_socket);
	//m_socket == INVALID_SOCKET;

	return;

	*(int*)buf = 99;		// quit command

	nSize = send(m_socket, (const char*)buf, 4, 0);

	if(nSize == 4){
		closesocket(m_socket);
		AfxMessageBox("Disconnected");
	}else{
		AfxMessageBox("Disconnected fail!");
	}

	m_socket == INVALID_SOCKET;
}


void CwclientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CFile	file;
	char	buf[1024];

	sprintf_s(buf, 1024, "%d\r\n", m_frameCount);

	file.Open(".\\log.txt", CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
	file.SeekToEnd();

	file.Write(buf, strlen(buf));

	file.Close();

	m_frameCount = 0;

	CDialogEx::OnTimer(nIDEvent);
}



void CwclientDlg::DrawJPEG(byte* pBuf, int nSize){
	CImage	img;
	IStream*	pStream;

	if(SUCCEEDED(CreateStreamOnHGlobal(NULL, TRUE, &pStream)) < 0){
		AfxMessageBox("CreateStreamOnHGlobal fail!");
		return;
	}

	ULONG	ulWirteByte = 0;
	ULARGE_INTEGER	sz;

	sz.HighPart = 0;
	sz.LowPart = nSize;

	pStream->SetSize(sz);

	pStream->Write(pBuf, nSize, &ulWirteByte);	// memory copy to IStream

	img.Load(pStream);

	int	nWidth, nHeight;

	nWidth = img.GetWidth();
	nHeight = img.GetHeight();

	// ȭ�� ũ�� ����
	ReLocationItems(nWidth, nHeight);

	if(m_nVerticalFlip & 0x01){
		// do - flip
	}

	img.Draw(m_hDC, 0, 0);
}



// nX : �̹��� ���� ����
// nY : �̹��� ���� ����
// ������ ���� ũ�Ⱑ �޶��� ��츸 ����
#define	ITEM_COUNT		5
#define	BUTTUN_WIDTH	70
#define	IP_BOX_WIDTH	190
#define	STATUS_WIDTH	100
#define	BUTTUN_HEIGHT	20
#define	IMAGE_ITEM_GAP	15
void CwclientDlg::ReLocationItems(int nWidth, int nHeight){
	struct ITEMS{
		int	nID;
		int	nWidth;
	};
	
	int	nX, nY;
	CString	str;

	ITEMS items[ITEM_COUNT] = {	{IDC_BUTTON_CONNECT, BUTTUN_WIDTH}, 
								{IDC_IPADDRESS_GV, IP_BOX_WIDTH}, 
								{NULL, IP_BOX_WIDTH}, 
								{IDC_EDIT_PORT, BUTTUN_WIDTH},
								{IDC_STATIC_STATUS, STATUS_WIDTH}
	};

	if(m_nPresentWidth == nWidth && m_nPresentHeight == nHeight){
		return;
	}

	this->MoveWindow(0, 0, nWidth+40, 15+nHeight+BUTTUN_HEIGHT+BUTTUN_HEIGHT+BUTTUN_HEIGHT);
	m_staticScreen.MoveWindow(10, 10, nWidth, nHeight);

	// ���� ���� ����
	m_nPresentWidth = nWidth;
	m_nPresentHeight = nHeight;

	nX = nWidth/ITEM_COUNT;
	nY = nHeight + IMAGE_ITEM_GAP;

	str.Format("W=%d, H=%d", nWidth, nHeight);

	GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextA(str);

	for(int i=0;i<ITEM_COUNT;i++){
		if(items[i].nID != NULL){
			GetDlgItem(items[i].nID)->MoveWindow(nX*i+10, nY, items[i].nWidth, BUTTUN_HEIGHT);
		}
	}
}



// Ű���� ���콺 �̺�Ʈ �Լ��Դϴ�.
//BOOL CwclientDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//	HWND hwnd;
//
//	hwnd = GetDlgItem(IDC_STATIC_SCREEN)->m_hWnd;
//
//	char	temp[1024];
//
//	//sprintf_s(temp, 1024, "%08x=%08x\r\m", pMsg->hwnd, hwnd);
//
//	//OutputDebugString(temp);
//
//	//if(pMsg->hwnd != hwnd){
//		if(pMsg->message == WM_LBUTTONDOWN){
//			m_nClick == 1;
//			OutputDebugString("D");
//		}else if(pMsg->message == WM_LBUTTONUP){
//			m_nClick == 0;
//			OutputDebugString("U");
//		}else if(pMsg->message == WM_MOUSEMOVE){
//			if(m_nClick == 1){										// ���콺�� ������ ��츸 ������
//				OutputDebugString("M");
//				CPoint	pos;
//				GetCursorPos(&pos);
//				GetDlgItem(IDC_STATIC_SCREEN)->ScreenToClient(&pos);
//				//if(pos.x < 0 || pos.y < 0){
//
//				//}else if(pos.x > m_nPresentWidth || pos.y > m_nPresentHeight){
//
//				//}else{
//					sprintf_s(temp, 1024, "X=%d,Y=%d\r\m", pos.x, pos.y);
//					OutputDebugString(temp);
//				//}
//			}
//		}
//	//}
//
//	return CDialogEx::PreTranslateMessage(pMsg);
//}


void CwclientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//OutputDebugString("D");
	SetCapture();

	// ȭ�� �ȿ����� �����մϴ�.
	CPoint	pos;
	GetCursorPos(&pos);
	GetDlgItem(IDC_STATIC_SCREEN)->ScreenToClient(&pos);

	if(pos.x < 0 || pos.y < 0){

	}else if(pos.x > m_nPresentWidth || pos.y > m_nPresentHeight){

	}else{
		m_nDragOn = 1;
		m_nDragX = -1;
		m_nDragY = -1;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CwclientDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//OutputDebugString("U");
	ReleaseCapture();

	// ȭ�� �ȿ����� �����մϴ�.
	CPoint	pos;
	GetCursorPos(&pos);
	GetDlgItem(IDC_STATIC_SCREEN)->ScreenToClient(&pos);

	if(pos.x < 0 || pos.y < 0){

	}else if(pos.x > m_nPresentWidth || pos.y > m_nPresentHeight){

	}else{
		m_nDragOn = 0;
	}
	
	CDialogEx::OnLButtonUp(nFlags, point);
}



void CwclientDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(m_nDragOn == 1){
		char temp[1024];
		//OutputDebugString("M");

		CPoint	pos;
		GetCursorPos(&pos);
		GetDlgItem(IDC_STATIC_SCREEN)->ScreenToClient(&pos);

		if(pos.x < 0 || pos.y < 0){
			m_nDragOn = 0;
		}else if(pos.x > m_nPresentWidth || pos.y > m_nPresentHeight){
			m_nDragOn = 0;
		}else{
			if(m_nDragX < 0){
				m_nDragX = pos.x;
			}else{
				int	nMovedX = 0;
				nMovedX = m_nDragX - pos.x;

				// ���� ���� �̻� ������ ���
				if(abs(nMovedX) > 24){
					// send(
					m_nDragX = pos.x;
					sprintf_s(temp, 1024, " X=%d ", nMovedX);
					//OutputDebugString(temp);
					PostMessage(WM_MESSAGE_SEND_MOVE, m_socket, nMovedX);
				}
			}
			//sprintf_s(temp, 1024, "X=%d,Y=%d\r\m", pos.x, pos.y);
			//OutputDebugString(temp);
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}



afx_msg LRESULT CwclientDlg::OnMessageSendMove(WPARAM wParam, LPARAM lParam)
{
	SOCKET	socket = wParam;
	int		nMovedX = lParam;
	short	nTemp = 0;
	int		nData = 0;
	
	nTemp = (short)nMovedX;

	nData = 0x80000000;

	nData |= (nTemp&0xFFFF);

	char	temp[1024];
	sprintf_s(temp, 1024, " %d_%d_%08x ", nMovedX, socket, nData);
	OutputDebugString(temp);

	if(m_socket != INVALID_SOCKET){
		nMovedX = send(m_socket, (const char*)&nData, 4, 0);

		if(nMovedX == 0){
			// ���� ������
		}else if(nMovedX == SOCKET_ERROR){
			nMovedX = WSAGetLastError();
			sprintf_s(temp, 1024, "Error[%d]\r\n", nMovedX);
			OutputDebugString(temp);
		}
	}

	return 0;
}



// ����Ŭ���� ��尡 ����˴ϴ�.
//void CwclientDlg::OnMButtonDblClk(UINT nFlags, CPoint point)
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
//#define	MODE_COUNT	3
//	m_nMode++;
//	m_nMode = m_nMode % MODE_COUNT;
//
//	PostMessage(WM_MESSAGE_SEND_MODE, m_nMode, NULL);
//
//	CDialogEx::OnMButtonDblClk(nFlags, point);
//}


afx_msg LRESULT CwclientDlg::OnMessageSendMode(WPARAM wParam, LPARAM lParam)
{
	int		nMode = wParam;
	short	nTemp = 0;
	int		nData = 0;
	
	nTemp = (short)nMode;

	nData = 0x40000000;

	nData |= (nTemp&0xFFFF);

	if(m_socket != INVALID_SOCKET){
		nMode = send(m_socket, (const char*)&nData, 4, 0);

		if(nMode == 0){
			// ���� ������
		}else if(nMode == SOCKET_ERROR){
			nMode = WSAGetLastError();
			char	temp[1024];
			sprintf_s(temp, 1024, "Error[%d]\r\n", nMode);
			OutputDebugString(temp);
		}
	}

	return 0;
}



void CwclientDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
#define	MODE_COUNT	3
	m_nMode++;
	m_nMode = m_nMode % MODE_COUNT;

	PostMessage(WM_MESSAGE_SEND_MODE, m_nMode, NULL);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}



void CwclientDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_nVerticalFlip++;

	CDialogEx::OnRButtonUp(nFlags, point);
}
