
// wclientDlg.cpp : 구현 파일
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
 
// 그레이 RAW 파일을 그레이 비트맵 파일로 변환
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
       info_h.biBitCount      = 24;// 흑백이면 8
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

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CwclientDlg 대화 상자




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


// CwclientDlg 메시지 처리기

BOOL CwclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//convert(0,0);return FALSE;// bin to BMP
	int	nTT = -555;
	int	nAA;

	nAA = nTT % 500;
	nTT = (unsigned int)nAA;

	m_hDC = m_staticScreen.GetDC()->GetSafeHdc();	// HDC 받기

	m_ipAddressCtrl.SetWindowTextA("127.0.0.1");
	m_ceditPort.SetWindowTextA("12777");

#define W	640
#define H	480

	ReLocationItems(W, H);

	m_frameCount = 0;								// Frame count per second on 1 sec timer

	SetTimer(1, 1000, 0);							// Timer 생성

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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
		// 종료할 때
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CwclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CwclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// **deprecated
//http://www.devpia.com/MAEUL/Contents/Detail.aspx?BoardID=50&MAEULNo=20&no=886642&ref=886633
//http://stackoverflow.com/questions/10461324/create-hbitmap-from-jpeg-memory-buffer
void CwclientDlg::OnBnClickedButtonImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	// 창의 크기를 조정합니다.
	ReLocationItems(nWidth, nHeight);

	img.Draw(m_hDC, 0, 0);
}



// 클라이언트 소켓 초기화
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

	m_socket = socket(AF_INET, SOCK_STREAM, 0);			// 소켓 생성

	if(m_socket == INVALID_SOCKET){
		return -1;
	}

	memset(&m_srv_addr, 0, sizeof(struct sockaddr_in));	// 초기화

	// 윈도우에 적힌 IP와 PORT를 받아옵니다.
	UpdateData(TRUE);
	m_ipAddressCtrl.GetAddress(ipA, ipB, ipC, ipD);
	m_ceditPort.GetWindowTextA(str);
	nPort = (USHORT)_ttoi(str);

	sprintf_s(chIp, 16, "%d.%d.%d.%d", ipA, ipB, ipC, ipD);

	// IP와 PORT를 넣고 소켓을 연결합니다.
	m_srv_addr.sin_family = AF_INET;
	m_srv_addr.sin_addr.s_addr = inet_addr(chIp);
	m_srv_addr.sin_port = htons(nPort);

	WSAAsyncSelect(m_socket, m_hWnd, CLIENT_MESSAGE, FD_CONNECT);

	/*if(connect(m_socket, (struct sockaddr *)&m_srv_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR){
		m_socket == INVALID_SOCKET;
		return -1;
	}*/

	connect(m_socket, (struct sockaddr *)&m_srv_addr, sizeof(struct sockaddr_in));
	// 성공 여부는 메세지 콜백 함수에서 확인 가능

	return 1;
}



// 연결버튼
//http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=663
//http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=667
//http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=668
void CwclientDlg::OnBnClickedButtonConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int	nResult = 0;

	if(InitClientSocket() == -1){								// Client socket 생성
		int	nError = WSAGetLastError();

		switch(nError){
		case WSAEINVAL:
			AfxMessageBox("WSAEINVAL");
			break;
		default:
			AfxMessageBox("소켓 생성 실패");
			break;
		}

		AfxMessageBox("아이쿠 연결이 안됬습니다.");

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

		// 전송속도 차이에 의해 FD_READ가 중복되어 들어올 수 있으므로
		// FD_READ가 실행되고 있는 동안, FD_READ의 비동기를 풀어준다.
		WSAAsyncSelect(m_socket, m_hWnd, CLIENT_MESSAGE, FD_CLOSE);

		// recv(서버와 통신하는 소켓, 실제 데이터 크기를 저장할 변수, 받을 데이터의 크기, flag);
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

		// 다시 새로운 정보를 받기 위하여, 비동기 설정에 FD_READ를 추가한다.
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
	// 넘어온 소켓이 유효하지 않은 소켓일 경우 실행하지 않고 나간다.
	if(parm_socket == INVALID_SOCKET){
		return;
	}

	// LINGER 구조체의 변수 l_linger에 LINGER옵션이 TRUE일 경우로 저장해준다.
	LINGER l_linger = {TRUE, 0};

	// 소켓 옵션을 설정한다.
	// setsockopt(옵션을 적용시킬 소켓, level, 옵션이름, 옵션을 저장할 장소, 옵션의 크기);
	setsockopt(parm_socket, SOL_SOCKET, SO_LINGER, (char FAR *)&l_linger, sizeof(l_linger));


	// 넘어온 소켓 제거
	closesocket(parm_socket);
}



// **deprecated
void CwclientDlg::OnBnClickedButtonDisconnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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

	// 화면 크기 변경
	ReLocationItems(nWidth, nHeight);

	if(m_nVerticalFlip & 0x01){
		// do - flip
	}

	img.Draw(m_hDC, 0, 0);
}



// nX : 이미지 가로 길이
// nY : 이미지 세로 길이
// 이전과 영상 크기가 달라진 경우만 동작
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

	// 현재 값을 저장
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



// 키보드 마우스 이벤트 함수입니다.
//BOOL CwclientDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
//			if(m_nClick == 1){										// 마우스가 눌러진 경우만 동작함
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//OutputDebugString("D");
	SetCapture();

	// 화면 안에서만 동작합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//OutputDebugString("U");
	ReleaseCapture();

	// 화면 안에서만 동작합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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

				// 일정 범위 이상 움직인 경우
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
			// 연결 끊어짐
		}else if(nMovedX == SOCKET_ERROR){
			nMovedX = WSAGetLastError();
			sprintf_s(temp, 1024, "Error[%d]\r\n", nMovedX);
			OutputDebugString(temp);
		}
	}

	return 0;
}



// 더블클릭시 모드가 변경됩니다.
//void CwclientDlg::OnMButtonDblClk(UINT nFlags, CPoint point)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
			// 연결 끊어짐
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
#define	MODE_COUNT	3
	m_nMode++;
	m_nMode = m_nMode % MODE_COUNT;

	PostMessage(WM_MESSAGE_SEND_MODE, m_nMode, NULL);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}



void CwclientDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_nVerticalFlip++;

	CDialogEx::OnRButtonUp(nFlags, point);
}
