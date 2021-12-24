#ifndef JPEG_H
#define JPEG_H



/* Includes */



/* Defines */
typedef struct{
	int		nWidth;
	int		nHeight;
	int		nBPP;
	int		nColorSpace;
	int		nImageSize;
	unsigned char*	pImageBuf;
}ImageData;



/* Functions */



/*
 * Function name	: GetImageFromJpeg
 * Attr1		: Destination image pointer to make a raw image data.
 * Attr2		: Source image pointer from JPEG memory.
 * Return		: If failed, it returns -1.
 * Remarks		: It makes a raw image format from JPEG memory.
 */
int	GetImageFromJpeg(ImageData* destData, ImageData* srcData);



/*
 * Function name	: SetImageToJpeg
 * Attr1		: Destination image pointer to make a JPEG memory.
 * Attr2		: Source image pointer from raw data.
 * Return		: If failed, it returns -1.
 * Remarks		: It makes a JPEG format memory from raw image data.
 */
int	SetImageToJpeg(ImageData* destData, ImageData* srcData);



#endif
