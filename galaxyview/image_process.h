#ifndef	IMAGE_PROCESS_H
#define	IMAGE_PROCESS_H



/* Defines */
typedef struct {
	// Have to be setted by user.
	int	nCenterX;
	int	nCenterY;
	int	nInRadius;
	int	nOutRadius;
	int	nSrcWidth;
	int	nSrcHeight;
	// 
	int	nWidth;
	int	nHeight;
	int	nBPP;
	int	nScaleImageWidth;
	// User can modify this parameter.
	int	nStartPixel;

	unsigned char*	pImageBuf;
	unsigned int*	pTable;
}DonutData;



/*
 * Function name	: InitializeDonut
 * Return		:
 * Remarks		: Allocate image memory and circle table memory
 */
int InitializeDonut(DonutData* data);



/*
 * Function name	: DonutToStick
 * Return		:
 * Remarks		: Convert from donut image to stick image
 */
int	DonutToStick(DonutData* data, unsigned char* pSrcBuf);



/*
 * Function name	: DonutToPiece2X
 * Return		:
 * Remarks		: Convert from donut image to piece of cake image
 */
int	DonutToPiece2X(DonutData* data, unsigned char* pSrcBuf);



/*
 * Function name	: ReleaseDonut
 * Return		:
 * Remarks		: Release all resource
 */
int	ReleaseDonut(DonutData* data);



int	YUYVToRGB(unsigned char* pDest, unsigned char* pSrc, int nWidth, int nHeight);



#endif
