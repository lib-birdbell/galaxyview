/*
 * Date		: 2014/02/xx
 * Author	: Saejong Lee
 * gcc version	: 4.7.3
 * Linux	: Ubuntu 13.04
 * arm gcc	:
 * arm linux	:
 * This program is made by Saejong Lee.
 * It works like a webcam.
 * Receive image from USB CAM.
 * And image is processed.
 * Then send image by TCP/IP.
 */

/***************************************************
		Includes
***************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "Lan.h"
#include "Jpeg.h"
#include "Cam.h"
#include "image_process.h"



/***************************************************
		Defines
***************************************************/
#define	MAX_IMAGE_WIDTH		1920
#define	MAX_IMAGE_HEIGHT	1080
#define	MAX_IMAGE_PITCH		3

#define FAIL			-1

enum{
	IMAGE_DONUT = 0,
	IMAGE_STICK,
	IMAGE_PIECE2X
}IMAGE_TYPE;



/*
 * Function name	: RarseWord
 * Attr1		: Char pointer to store parsed 'Information WORD'.
 * Attr2		: Char pointer to store parsed 'Data WORD'.
 * Attr3		: Source sentence.
 * Attr4		: Character to separate 'Information word' and 'Data word'.
 * Remarks		: It eliminates ' '(sapce). Also, if meet parse character, parsing stopped. It assume that sentence consist of "Information word + separate character + data word".
 			If Source sentence(Attr3) consist of "WIDTH = 320".
			And separate character is '='.
			Result of this function is..
			Attr1 = "WIDTH"
			Attr2 = "320"
 */
void ParseWord(char* destWord, char* destData, char* src, char parse);



/*
 * Function name	: ReadConfigFile
 * Attr1		: Lan data pointer.
 * Attr2		: Cam infomation pointer.
 * Attr3		: Donut data pointer.
 * Remakrs		: Read config data, and set each parameters.
 */
void ReadConfigFile(LanData* lanData, CamInfo* camData, DonutData* donutData);



/*
 * Function name	: HelpMessage
 * Attr1		: Arguments counts.
 * Attr2		: Arguments values.
 * Remarks		: It prints information of this program.
 */
void HelpMessage(int argc, char* argv[]);



/***************************************************
		Global variables
***************************************************/
CamInfo*	g_pCamInfo;
LanData*	g_pLanData;
DonutData*	g_pDonutData;



/***************************************************
		Functions
***************************************************/
/*
 * Function name	: sigint_handler
 * Remarks		: It called when user input ctrl+c.
 */
void sigint_handler(int signo){
	printf("GV] Ctrl + [%d/0x%02x] pushed\r\n", signo, signo);

	// 1> Release Lan resource
	ReleaseLan(g_pLanData);

	ReleaseCam(g_pCamInfo);

	ReleaseDonut(g_pDonutData);

	printf("GV] program exit!\r\n");

	exit(0);
}



/*
 * Function name	: GetClock
 * Remarks		: It returns time value.
 */
static inline long GetClock(){
	struct timeval	tv;

	gettimeofday(&tv, NULL);

	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}









//deprecated
void SaveFile(unsigned char* chData, int nSize, int nNumber){
	char	temp[512];

	FILE	*fp;

	sprintf(temp, "./%04d.bin", nNumber);
	fp = fopen(temp, "w");

	fwrite(chData, 1, nSize, fp);

	fclose(fp);
}







//======================================================================
//				M A I N
//======================================================================
int main(int argc, char* argv[] ){
	/***************************************************
			Local variables
	***************************************************/
	CamInfo			camInfo;
	LanData			lanData;
	DonutData		donutData;
	struct	sigaction	act;

	long			lBeginTime, lEndTime;

	ImageData		usbImage;
	ImageData		rawImage;
	ImageData		jpegImage;
	ImageData		destImage;

	int			nCamCount = 0;
	int			nImageProcessCount = 0;

	int			nCommand;
	int			nReceivePixel = 0;

	int			nImageProcessType = IMAGE_STICK;



	/***************************************************
			1 Initialization
	***************************************************/
	// A> Read user arguments
	// B> Read and set config file
	// C> Set variable initialization
	// D> Set Ctrl-C handler
	// E> Init USB CAM
	// F> Init Socket

	if(argc > 1){
		HelpMessage(argc, argv);
		return 0;
	}

	// For Ctrl-C handler
	g_pLanData = &lanData;
	g_pCamInfo = &camInfo;
	g_pDonutData = &donutData;

	ReadConfigFile(&lanData, &camInfo, &donutData);

	// do - made a init func
	lanData.pImageData = NULL;
	lanData.nImageSize = 0;

	usbImage.nWidth = camInfo.nWidth;
	usbImage.nHeight = camInfo.nHeight;
	usbImage.nBPP = camInfo.nBPP;

	rawImage.nWidth = camInfo.nWidth;
	rawImage.nHeight = camInfo.nHeight;
	rawImage.nBPP = camInfo.nBPP;
	rawImage.nColorSpace = camInfo.nColorSpace;
	rawImage.pImageBuf = (unsigned char*)malloc(rawImage.nWidth * rawImage.nHeight * rawImage.nBPP);
	if(rawImage.pImageBuf == NULL){
		// do - exception
		printf("*GV] rawImage memory allocation error!\r\n");
		exit(1);
	}

	donutData.nSrcWidth = camInfo.nWidth;
	donutData.nSrcHeight = camInfo.nHeight;
	donutData.nBPP = camInfo.nBPP;
	donutData.nStartPixel = 0;
	
	// Set the value of the largest.
	jpegImage.pImageBuf = (unsigned char*)malloc(1280 * 720 * 4);
	jpegImage.nImageSize = 0;
	if(jpegImage.pImageBuf == NULL){
		// do - exception
		printf("*GV] jpegImage memory allocation error!\r\n");
		exit(1);
	}

	act.sa_handler = sigint_handler;
	sigfillset(&act.sa_mask);

	sigaction(SIGINT, &act, NULL);

	if(InitializeCam(&camInfo) == -1){
		// do -exception
		printf("*GV error] Cam initialization error!!\r\n");
		return 0;
	}

	if(InitializeLan(&lanData) == -1){	// If failed, it returns -1
		// do - exception
		printf("*GV error] Lan initialization error!!\r\n");
		return 0;
	}

	if(InitializeDonut(&donutData) == -1){
		// do - exception
		printf("*GV]donut init error!!\r\n");
		return 0;
	}

	lBeginTime = GetClock();



	while(1){	// **loop start
	/***************************************************
			2 Image processing
	***************************************************/
	// A> Get image from USB CAM
	// B> Image decompress or format change from YUYV to RGB 
	// C> Image transform donut to stick
	// D> Extra image processing
	// E> Image compress

		usbImage.nImageSize = GetCamImage(&camInfo);
		usbImage.pImageBuf = camInfo.pJpeg;

		nCamCount++;

		//GetImageFromJpeg(&rawImage, &usbImage);
		//GetImageFromJpeg(&rawImage, &jpegImage);
		YUYVToRGB(rawImage.pImageBuf, usbImage.pImageBuf,
				usbImage.nWidth, usbImage.nHeight);
				
		// Donut to stick
		if(nImageProcessType == IMAGE_STICK){
			if(DonutToStick(&donutData, rawImage.pImageBuf) == -1){
				printf("*GV] Donut error!\r\n");
			}
			destImage.pImageBuf = donutData.pImageBuf;
			destImage.nWidth = donutData.nWidth;
			destImage.nHeight = donutData.nHeight;
			destImage.nBPP = donutData.nBPP;
			destImage.nColorSpace = 2;//JCS_RGB;
		}
		// Donut to stick + stretching
		else if(nImageProcessType == IMAGE_PIECE2X){
			if(DonutToPiece2X(&donutData, rawImage.pImageBuf) == -1){
				printf("*GV] Donut error!\r\n");
			}
			destImage.pImageBuf = donutData.pImageBuf;
			destImage.nWidth = donutData.nScaleImageWidth*2;
			destImage.nHeight = donutData.nHeight*2;
			destImage.nBPP = donutData.nBPP;
			destImage.nColorSpace = 2;//JCS_RGB;
		}
		// Original donut image
		else{
			destImage.pImageBuf = rawImage.pImageBuf;
			destImage.nWidth = rawImage.nWidth;
			destImage.nHeight = rawImage.nHeight;
			destImage.nBPP = rawImage.nBPP;
			destImage.nColorSpace = 2;//JCS_RGB;
		}

	/***************************************************
		3 Send image(If connected client exist)
	***************************************************/
	// A> Processing command from client
	// B> Create thread for send image

		nCommand = ReceiveHandler(&lanData);

		// Nothing
		if(nCommand == 0){
		}
		// Pixel move command in Donut image.
		else if(nCommand&0x80000000){
			nReceivePixel = (nCommand & 0xFFFF);
			donutData.nStartPixel += (signed short)nReceivePixel;
		}
		// Image process type change.
		else if(nCommand&0x40000000){
			nImageProcessType = nCommand & 0xF;
		}
		// Client close command
		else if(nCommand == 99){
			ClientClose(&lanData);
		}

		if(GetLanSendState(&lanData) != -1){
			nImageProcessCount++;
			
			SetImageToJpeg(&jpegImage, &destImage);

			// Set Jpeg image data to LanData
			lanData.nImageSize = jpegImage.nImageSize;
			lanData.pImageData = jpegImage.pImageBuf;

			if(SendDataOnLan(&lanData) == -1){
				// do -exception
				printf("GV error] LanSendData error!\r\n");
			}
		}

		/* Time check */
		lEndTime = GetClock() - lBeginTime;

		if(lEndTime > 999){
			printf("GV] Cam %d FPS\r\n", nCamCount);
			nCamCount = 0;
			lBeginTime = GetClock();
		}
	}// **loop end

	return 0;
}







void ParseWord(char* destWord, char* destData, char* src, char parse){

	// Parsing word.
	while(*src){
		if(*src == parse){
			src++;
			break;
		}else if(*src != ' '){
			*destWord = *src;
			destWord++;
		}
		src++;
	}
	*destWord = '\0';

	// Parsing data.
	while(*src){
		if(*src != ' '){
			*destData = *src;
			destData++;
		}
		src++;
	}
	*destData = '\0';
}



#include <string.h>
void ReadConfigFile(LanData* lanData, CamInfo* camInfo, DonutData* donutData){
	FILE	*fp;
	int	nFileSize;
	char*	chConfig;
	char*	ptr;

	fp = fopen("./config.txt", "r");
	if(fp == NULL){
		printf("*GV] <config.txt> file must exist!!\r\n");
		// do - explain to wrtie <config.txt>.
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	nFileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	chConfig = (char*)malloc(sizeof(char)*nFileSize);
	if(chConfig == NULL){
		// do - exception
		printf("*GV] Read config file error![memory allocation fail]\r\n");
		exit(1);
	}

	if(fread(chConfig, 1, nFileSize, fp) != nFileSize){
		// do - exception
		printf("*GV] Read config file error![file size fail]\r\n");
		exit(1);
	}

	ptr = chConfig;
	char*	curLine;
	int	nLineSize;
	char*	chWord;
	char*	chData;
	curLine = chConfig;

	while(curLine){
		char* nextLine = strchr(curLine, '\n');
		if(nextLine){
			*nextLine = '\0';	// temporarily terminate the current line

			nLineSize = strlen(curLine);

			// '#' is comment
			if(curLine[0] != '#' && nLineSize != 0){
				chWord = (char*)malloc(sizeof(char)*nLineSize);
				chData = (char*)malloc(sizeof(char)*nLineSize);

				ParseWord(chWord, chData, curLine, '=');
				// Parsing and input data.
				if(strcasecmp(chWord, "lan_port") == 0){
					lanData->nPort = atoi(chData);
				}else if(strcasecmp(chWord, "cam_device") == 0){
					strcpy(camInfo->chDev, chData);
				}else if(strcasecmp(chWord, "cam_width") == 0){
					camInfo->nWidth = atoi(chData);
				}else if(strcasecmp(chWord, "cam_height") == 0){
					camInfo->nHeight = atoi(chData);
				}else if(strcasecmp(chWord, "cam_bpp") == 0){
					camInfo->nBPP = atoi(chData);
				}else if(strcasecmp(chWord, "cam_color_space") == 0){
					camInfo->nColorSpace = atoi(chData);
				}else if(strcasecmp(chWord, "cam_size_force") == 0){
					camInfo->nSizeForce = atoi(chData);
				}else if(strcasecmp(chWord, "donut_center_x") == 0){
					donutData->nCenterX = atoi(chData);
				}else if(strcasecmp(chWord, "donut_center_y") == 0){
					donutData->nCenterY = atoi(chData);
				}else if(strcasecmp(chWord, "donut_in_radius") == 0){
					donutData->nInRadius = atoi(chData);
				}else if(strcasecmp(chWord, "donut_out_radius") == 0){
					donutData->nOutRadius = atoi(chData);
				}else{
					printf("*GV] %s is not config parameter!\r\n", chWord);
				}
				free(chWord);
				free(chData);
			}
		}
		if(nextLine){
			*nextLine = '\n';
		}
		curLine = nextLine ? (nextLine+1) : NULL;
	}

	fclose(fp);
}



void HelpMessage(int argc, char* argv[]){
	printf("Galaxy View 1.0 (2014 Feb 16)\r\n");
	printf("Made by SaeJong-Lee for 1st SAMSUNG EXYNOS CONTEST.\r\n\r\n");
	printf("This is a image processing program on ODROID EXYNOS4412.\r\n\r\n");
	printf("usage : execute <galaxyview> with <config.txt>\r\n");
	printf("\r\n");
}
