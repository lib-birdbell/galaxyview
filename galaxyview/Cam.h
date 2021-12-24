#ifndef CAM_H
#define CAM_H



/* Defines */
#define CAM_BUF_COUNT	4
typedef struct{
	int		fd;
	char		chDev[256];
	int		nWidth;
	int		nHeight;
	int		nBPP;
	int		nColorSpace;
	int		nSizeForce;
	int		nBufCount;
	unsigned char* 	pImageBuf[CAM_BUF_COUNT];
	unsigned char*	pJpeg;
}CamInfo;



/*
 * Function name	: InitializeCam
 * Attr1		: Initialize USB CAM
 * Return		: It returns -1, if failed.
 * Remarks		: It initializes USB CAM.
 */
int	InitializeCam(CamInfo* camInfo);



/*
 * Function name	: GetImage
 * Attr1		: 
 * Return		: 
 * Remarks		: Get image from USB CAM.
 */
int	GetCamImage(CamInfo* camInfo);



/*
 * Function name	: ReleaseCam
 * Attr1		: 
 * Return		: 
 * Remarks		: Release cam resource.
 */
int	ReleaseCam(CamInfo* camInfo);



#endif
