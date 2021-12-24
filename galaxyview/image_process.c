#include "image_process.h"
#include <stdlib.h>
#include <math.h>



int InitializeDonut(DonutData* data){
	double	dOR, dIR;
	double	dPI = 3.14159265358979323846L;
	double	dW;
	double	dH;
	double	dDeg, dDegBase;
	double	dPixel;
	double	dRadian;
	double	dRadius;

	int		nHeight, nWidth, nBPP;
	int		i, j, k;
	int		nCenterX, nCenterY;
	int		nRowStride;
	int		nX, nY;
	int		nSrcN;
	int		nSrcWidth;

	dIR = data->nInRadius;
	dOR = data->nOutRadius;
	nBPP = data->nBPP;
	nCenterX = data->nCenterX;
	nCenterY = data->nCenterY;
	nSrcWidth = data->nSrcWidth;

	// Destination Image Height.
	dH = dOR - dIR;
	// Destination Image Width.
	dW = dPI * (dOR - (dH / (double)2)) * (double)2;

	nWidth = (int)dW;
	nHeight = (int)dH;

	data->nHeight = nHeight;
	data->nWidth = nWidth;

	// Src image width and height check.
	if(data->nSrcWidth < (data->nCenterX + data->nOutRadius)){
		return -1;
	}else if(0 > (data->nCenterX - data->nOutRadius)){
		return -1;
	}else if(data->nSrcWidth < (data->nCenterY + data->nOutRadius)){
		return -1;
	}else if(0 > (data->nCenterY - data->nOutRadius)){
		return -1;
	}else if(data->nInRadius > data->nOutRadius){
		return -1;
	}else if(data->pImageBuf == NULL){
		return -1;
	}

	// Set normal image size.
	if( ((nHeight / 3) * 4) < nWidth){
		data->nScaleImageWidth = (nHeight / 3) * 4;
	}else{
		data->nScaleImageWidth = nWidth;
	}

	// Make a circle position data.
	data->pTable = (unsigned int*)malloc(sizeof(int) * nWidth * nHeight * nBPP);
	if(data->pTable == NULL){
		return -1;
	}

	nRowStride = nWidth * nBPP;

	dDegBase = ((double)360 / dW);

	for(j=0;j<nHeight;j++){
		dRadius = (dIR + (double)j);				// Radian + pixel
		for(i=0;i<nRowStride;i+=nBPP){
			dPixel = i / nBPP;
			dDeg = dDegBase * dPixel;

			dRadian = (dDeg * dPI / (double)180);
			
			nX = (int)(cos(dRadian) * dRadius) + nCenterX;	// X + center X
			nY = (int)(sin(dRadian) * dRadius) + nCenterY;	// Y + center Y

			nSrcN = (nSrcWidth*nBPP*nY) + (nX*nBPP);

			for(k=0;k<nBPP;k++){
				data->pTable[i + (nRowStride * j) + k] = nSrcN + k;
			}
		}
	}

	// In order to safely allocate a memory bigger than expect.
	data->pImageBuf = (unsigned char*)malloc(sizeof(unsigned char) * nWidth * 2 * nHeight * 2 * nBPP);
	if(data->pImageBuf == NULL){
		return -1;
	}

	return 1;
}



int	DonutToStick(DonutData* data, unsigned char* pSrcBuf){
	int		nHeight, nWidth, nBPP;
	int		nStartPixel;
	int		nRowStride;
	int		i, j;
	int		nX;
	int		nEndPixel;
	int		nTable, nDest;
	unsigned char*	pDestBuf;
	
	// do - Src image width and height check.
	if(data->nSrcWidth < (data->nCenterX + data->nOutRadius)){
		return -1;
	}else if(0 > (data->nCenterX - data->nOutRadius)){
		return -1;
	}else if(data->nSrcWidth < (data->nCenterY + data->nOutRadius)){
		return -1;
	}else if(0 > (data->nCenterY - data->nOutRadius)){
		return -1;
	}else if(data->nInRadius > data->nOutRadius){
		return -1;
	}else if(data->pImageBuf == NULL){
		return -1;
	}else if(data->pTable == NULL){
		return -1;
	}

	nWidth = data->nWidth;
	nHeight = data->nHeight;
	nBPP = data->nBPP;

	nStartPixel = data->nStartPixel;

	// The starting pixel value is less than the width.
	nStartPixel = (nStartPixel % nWidth);

	// The starting pixel value is better than the 0.
	if(nStartPixel < 0){
		nStartPixel = nWidth + nStartPixel;
	}
	data->nStartPixel = nStartPixel;
	nStartPixel *= nBPP;
if(nStartPixel < 0){
nStartPixel = 0;
data->nStartPixel = 0;
}else if(nStartPixel > (nWidth*nBPP-1)){
nStartPixel = 0;
data->nStartPixel = 0;
}
	nRowStride = nWidth * nBPP;

	pDestBuf = data->pImageBuf;

	// Normaly image has RGB pixel.
	if(nBPP == 3){
		nEndPixel = nRowStride - nStartPixel;

		for(j=0;j<nHeight;j++){
			nX = nRowStride * j;

			for(i=nStartPixel;i<nRowStride;i+=3){
				nTable = data->pTable[nX + i];
				nDest = nX + i - nStartPixel;

				pDestBuf[nDest++] = pSrcBuf[nTable++];
				pDestBuf[nDest++] = pSrcBuf[nTable++];
				pDestBuf[nDest] = pSrcBuf[nTable];
			}
			
			for(i=0;i<nStartPixel;i+=3){
				nTable = data->pTable[nX + i];
				nDest = nX + i + nEndPixel;

				pDestBuf[nDest++] = pSrcBuf[nTable++];
				pDestBuf[nDest++] = pSrcBuf[nTable++];
				pDestBuf[nDest] = pSrcBuf[nTable];
			}
		}
		return 3;
	}else{
		for(j=0;j<nHeight;j++){
			nX = nRowStride * j;

			for(i=nStartPixel;i<nRowStride;i++){
				pDestBuf[nX + i - nStartPixel] = pSrcBuf[data->pTable[nX + i]];
			}
			for(i=0;i<nStartPixel;i++){
				pDestBuf[nX + i + (nRowStride - nStartPixel)] = pSrcBuf[data->pTable[nX + i]];
			}
		}
		return 7;
	}

	return 1;
}



int	DonutToPiece2X(DonutData* data, unsigned char* pSrcBuf){
	int		nHeight, nWidth, nBPP;
	int		nStartPixel;
	int		i, j;
	int		nTable, nTablePoint, nDest, nDestWidth;
	int		C00, C10, C01, C11;
	unsigned char*	pDestBuf;
	
	nWidth = data->nWidth;
	nHeight = data->nHeight;
	nBPP = data->nBPP;
	nDestWidth = data->nScaleImageWidth;

	nStartPixel = data->nStartPixel;

	// The starting pixel value is less than the width.
	nStartPixel = (nStartPixel % nWidth);

	// The starting pixel value is better than the 0.
	if(nStartPixel < 0){
		nStartPixel = nWidth + nStartPixel;
	}
	data->nStartPixel = nStartPixel;
	if(nStartPixel < 0){
		nStartPixel = 0;
		data->nStartPixel = 0;
	}else if(nStartPixel > (nWidth-1)){
		nStartPixel = 0;
		data->nStartPixel = 0;
	}

	// Multiple by pixel count.
	nStartPixel *= nBPP;
	nWidth *= nBPP;
	nDestWidth *= nBPP;

	pDestBuf = data->pImageBuf;

	// Normaly image has RGB pixel.
	if(nBPP == 3){
		if( (nDestWidth + nStartPixel) < nWidth){
			// Once
			for(j=0;j<nHeight-1;j++){
				// Make two horizontal line at once.
				for(i=nStartPixel;i<(nStartPixel + nDestWidth);i+=3){
					nDest = (nDestWidth * j * 4) + i * 2 - nStartPixel * 2;

					// 1> Red
					nTablePoint = nWidth * j + i;
					nTable = data->pTable[nTablePoint];
					C00 = pSrcBuf[nTable];
					nTablePoint += 3;
					nTable = data->pTable[nTablePoint];
					C10 = pSrcBuf[nTable];
					nTablePoint += nWidth;
					nTable = data->pTable[nTablePoint];
					C11 = pSrcBuf[nTable];
					nTablePoint -= 3;
					nTable = data->pTable[nTablePoint];
					C01 = pSrcBuf[nTable];

					// 00
					pDestBuf[nDest] = C00;
					// 10
					nDest += 3;
					pDestBuf[nDest] = (C00 + C10) >> 1;
					// 11
					nDest += (nDestWidth<<1);
					pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
					// 01
					nDest -= 3;
					pDestBuf[nDest] = (C00 + C01) >> 1;

					// 2> Green
					nTablePoint++;
					nTable = data->pTable[nTablePoint];
					C01 = pSrcBuf[nTable];
					nTablePoint += 3;
					nTable = data->pTable[nTablePoint];
					C11 = pSrcBuf[nTable];
					nTablePoint -= nWidth;
					nTable = data->pTable[nTablePoint];
					C10 = pSrcBuf[nTable];
					nTablePoint -= 3;
					nTable = data->pTable[nTablePoint];
					C00 = pSrcBuf[nTable];

					// 01
					nDest++;
					pDestBuf[nDest] = (C00 + C01) >> 1;
					// 11
					nDest += 3;
					pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
					// 10
					nDest -= (nDestWidth<<1);
					pDestBuf[nDest] = (C00 + C10) >> 1;
					// 00
					nDest -= 3;
					pDestBuf[nDest] = C00;

					// 3> Blue
					nTablePoint++;
					nTable = data->pTable[nTablePoint];
					C00 = pSrcBuf[nTable];
					nTablePoint += 3;
					nTable = data->pTable[nTablePoint];
					C10 = pSrcBuf[nTable];
					nTablePoint += nWidth;
					nTable = data->pTable[nTablePoint];
					C11 = pSrcBuf[nTable];
					nTablePoint -= 3;
					nTable = data->pTable[nTablePoint];
					C01 = pSrcBuf[nTable];

					// 00
					nDest++;
					pDestBuf[nDest] = C00;
					// 10
					nDest += 3;
					pDestBuf[nDest] = (C00 + C10) >> 1;
					// 11
					nDest += (nDestWidth<<1);
					pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
					// 01
					nDest -= 3;
					pDestBuf[nDest] = (C00 + C01) >> 1;
				}
			}

			for(i=nStartPixel;i<(nStartPixel + nDestWidth);i+=3){
				nDest = (nDestWidth * j * 4) + i * 2 - nStartPixel * 2;

				// Last Y line not reference next Y pixel.
				// 1> Red
				nTablePoint = nWidth * j + i;
				nTable = data->pTable[nTablePoint];
				C00 = pSrcBuf[nTable];
				nTablePoint += 3;
				nTable = data->pTable[nTablePoint];
				C10 = pSrcBuf[nTable];
				C11 = C10;
				C01 = C00;

				// 00
				pDestBuf[nDest] = C00;
				// 10
				nDest += 3;
				pDestBuf[nDest] = (C00 + C10) >> 1;
				// 11
				nDest += (nDestWidth<<1);
				pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
				// 01
				nDest -= 3;
				pDestBuf[nDest] = (C00 + C01) >> 1;

				// 2> Green
				nTablePoint++;
				nTable = data->pTable[nTablePoint];
				C10 = pSrcBuf[nTable];
				nTablePoint -= 3;
				nTable = data->pTable[nTablePoint];
				C00 = pSrcBuf[nTable];
				C11 = C10;
				C01 = C00;

				// 01
				nDest++;
				pDestBuf[nDest] = (C00 + C01) >> 1;
				// 11
				nDest += 3;
				pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
				// 10
				nDest -= (nDestWidth<<1);
				pDestBuf[nDest] = (C00 + C10) >> 1;
				// 00
				nDest -= 3;
				pDestBuf[nDest] = C00;

				// 3> Blue
				nTablePoint++;
				nTable = data->pTable[nTablePoint];
				C00 = pSrcBuf[nTable];
				nTablePoint += 3;
				nTable = data->pTable[nTablePoint];
				C10 = pSrcBuf[nTable];
				C11 = C10;
				C01 = C00;

				// 00
				nDest++;
				pDestBuf[nDest] = C00;
				// 10
				nDest += 3;
				pDestBuf[nDest] = (C00 + C10) >> 1;
				// 11
				nDest += (nDestWidth<<1);
				pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
				// 01
				nDest -= 3;
				pDestBuf[nDest] = (C00 + C01) >> 1;
			}
		}else{
			// Draw twice
			for(j=0;j<nHeight-1;j++){
				// Make two horizontal line at once.
				for(i=nStartPixel;i<(nWidth-3);i+=3){
					nDest = (nDestWidth * j * 4) + i * 2 - nStartPixel * 2;

					// 1> Red
					nTablePoint = nWidth * j + i;
					nTable = data->pTable[nTablePoint];
					C00 = pSrcBuf[nTable];
					nTablePoint += 3;
					nTable = data->pTable[nTablePoint];
					C10 = pSrcBuf[nTable];
					nTablePoint += nWidth;
					nTable = data->pTable[nTablePoint];
					C11 = pSrcBuf[nTable];
					nTablePoint -= 3;
					nTable = data->pTable[nTablePoint];
					C01 = pSrcBuf[nTable];

					// 00
					pDestBuf[nDest] = C00;
					// 10
					nDest += 3;
					pDestBuf[nDest] = (C00 + C10) >> 1;
					// 11
					nDest += (nDestWidth<<1);
					pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
					// 01
					nDest -= 3;
					pDestBuf[nDest] = (C00 + C01) >> 1;

					// 2> Green
					nTablePoint++;
					nTable = data->pTable[nTablePoint];
					C01 = pSrcBuf[nTable];
					nTablePoint += 3;
					nTable = data->pTable[nTablePoint];
					C11 = pSrcBuf[nTable];
					nTablePoint -= nWidth;
					nTable = data->pTable[nTablePoint];
					C10 = pSrcBuf[nTable];
					nTablePoint -= 3;
					nTable = data->pTable[nTablePoint];
					C00 = pSrcBuf[nTable];

					// 01
					nDest++;
					pDestBuf[nDest] = (C00 + C01) >> 1;
					// 11
					nDest += 3;
					pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
					// 10
					nDest -= (nDestWidth<<1);
					pDestBuf[nDest] = (C00 + C10) >> 1;
					// 00
					nDest -= 3;
					pDestBuf[nDest] = C00;

					// 3> Blue
					nTablePoint++;
					nTable = data->pTable[nTablePoint];
					C00 = pSrcBuf[nTable];
					nTablePoint += 3;
					nTable = data->pTable[nTablePoint];
					C10 = pSrcBuf[nTable];
					nTablePoint += nWidth;
					nTable = data->pTable[nTablePoint];
					C11 = pSrcBuf[nTable];
					nTablePoint -= 3;
					nTable = data->pTable[nTablePoint];
					C01 = pSrcBuf[nTable];

					// 00
					nDest++;
					pDestBuf[nDest] = C00;
					// 10
					nDest += 3;
					pDestBuf[nDest] = (C00 + C10) >> 1;
					// 11
					nDest += (nDestWidth<<1);
					pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
					// 01
					nDest -= 3;
					pDestBuf[nDest] = (C00 + C01) >> 1;
				}

				// Last 2x2 pixel reffrence first horizontal pixel.
				nDest = (nDestWidth * j * 4) + i * 2 - nStartPixel * 2;

				// 1> Red
				nTablePoint = nWidth * j + i;
				nTable = data->pTable[nTablePoint];
				C00 = pSrcBuf[nTable];
				nTablePoint += nWidth;
				nTable = data->pTable[nTablePoint];
				C01 = pSrcBuf[nTable];
				nTablePoint -= i;
				nTable = data->pTable[nTablePoint];
				C11 = pSrcBuf[nTable];
				nTablePoint -= nWidth;
				nTable = data->pTable[nTablePoint];
				C10 = pSrcBuf[nTable];

				// 00
				pDestBuf[nDest] = C00;
				// 10
				nDest += 3;
				pDestBuf[nDest] = (C00 + C10) >> 1;
				// 11
				nDest += (nDestWidth<<1);
				pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
				// 01
				nDest -= 3;
				pDestBuf[nDest] = (C00 + C01) >> 1;

				// 2> Green
				nTablePoint++;
				nTable = data->pTable[nTablePoint];
				C10 = pSrcBuf[nTable];
				nTablePoint += nWidth;
				nTable = data->pTable[nTablePoint];
				C11 = pSrcBuf[nTable];
				nTablePoint += i;
				nTable = data->pTable[nTablePoint];
				C01 = pSrcBuf[nTable];
				nTablePoint -= nWidth;
				nTable = data->pTable[nTablePoint];
				C00 = pSrcBuf[nTable];

				// 01
				nDest++;
				pDestBuf[nDest] = (C00 + C01) >> 1;
				// 11
				nDest += 3;
				pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
				// 10
				nDest -= (nDestWidth<<1);
				pDestBuf[nDest] = (C00 + C10) >> 1;
				// 00
				nDest -= 3;
				pDestBuf[nDest] = C00;

				// 3> Blue
				nTablePoint++;
				nTable = data->pTable[nTablePoint];
				C00 = pSrcBuf[nTable];
				nTablePoint += nWidth;
				nTable = data->pTable[nTablePoint];
				C01 = pSrcBuf[nTable];
				nTablePoint -= i;
				nTable = data->pTable[nTablePoint];
				C11 = pSrcBuf[nTable++];
				nTablePoint -= nWidth;
				nTable = data->pTable[nTablePoint];
				C10 = pSrcBuf[nTable];

				// 00
				nDest++;
				pDestBuf[nDest] = C00;
				// 10
				nDest += 3;
				pDestBuf[nDest] = (C00 + C10) >> 1;
				// 11
				nDest += (nDestWidth<<1);
				pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
				// 01
				nDest -= 3;
				pDestBuf[nDest] = (C00 + C01) >> 1;
			}

			for(i=nStartPixel;i<(nWidth-3);i+=3){
				nDest = (nDestWidth * j * 4) + i * 2 - nStartPixel * 2;

				// Last Y line not reference next Y pixel.
				// 1> Red
				nTablePoint = nWidth * j + i;
				nTable = data->pTable[nTablePoint];
				C00 = pSrcBuf[nTable];
				nTablePoint += 3;
				nTable = data->pTable[nTablePoint];
				C10 = pSrcBuf[nTable];
				C11 = C10;
				C01 = C00;

				// 00
				pDestBuf[nDest] = C00;
				// 10
				nDest += 3;
				pDestBuf[nDest] = (C00 + C10) >> 1;
				// 11
				nDest += (nDestWidth<<1);
				pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
				// 01
				nDest -= 3;
				pDestBuf[nDest] = (C00 + C01) >> 1;

				// 2> Green
				nTablePoint++;
				nTable = data->pTable[nTablePoint];
				C10 = pSrcBuf[nTable];
				nTablePoint -= 3;
				nTable = data->pTable[nTablePoint];
				C00 = pSrcBuf[nTable];
				C11 = C10;
				C01 = C00;

				// 01
				nDest++;
				pDestBuf[nDest] = (C00 + C01) >> 1;
				// 11
				nDest += 3;
				pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
				// 10
				nDest -= (nDestWidth<<1);
				pDestBuf[nDest] = (C00 + C10) >> 1;
				// 00
				nDest -= 3;
				pDestBuf[nDest] = C00;

				// 3> Blue
				nTablePoint++;
				nTable = data->pTable[nTablePoint];
				C00 = pSrcBuf[nTable];
				nTablePoint += 3;
				nTable = data->pTable[nTablePoint];
				C10 = pSrcBuf[nTable];
				C11 = C10;
				C01 = C00;

				// 00
				nDest++;
				pDestBuf[nDest] = C00;
				// 10
				nDest += 3;
				pDestBuf[nDest] = (C00 + C10) >> 1;
				// 11
				nDest += (nDestWidth<<1);
				pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
				// 01
				nDest -= 3;
				pDestBuf[nDest] = (C00 + C01) >> 1;
			}

			// Last X pixel reference first horizontal pixel, last Y pixel not reference .
			nDest = (nDestWidth * j * 4) + i * 2 - nStartPixel * 2;

			// 1> Red
			nTablePoint = nWidth * j + i;
			nTable = data->pTable[nTablePoint];
			C00 = pSrcBuf[nTable];
			nTablePoint -= i;
			nTable = data->pTable[nTablePoint];
			C10 = pSrcBuf[nTable];
			C01 = C00;
			C11 = C10;

			// 00
			pDestBuf[nDest] = C00;
			// 10
			nDest += 3;
			pDestBuf[nDest] = (C00 + C10) >> 1;
			// 11
			nDest += (nDestWidth<<1);
			pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
			// 01
			nDest -= 3;
			pDestBuf[nDest] = (C00 + C01) >> 1;

			// 2> Green
			nTablePoint++;
			nTable = data->pTable[nTablePoint];
			C10 = pSrcBuf[nTable];
			nTablePoint += i;
			nTable = data->pTable[nTablePoint];
			C00 = pSrcBuf[nTable];
			C01 = C00;
			C11 = C10;

			// 01
			nDest++;
			pDestBuf[nDest] = (C00 + C01) >> 1;
			// 11
			nDest += 3;
			pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
			// 10
			nDest -= (nDestWidth<<1);
			pDestBuf[nDest] = (C00 + C10) >> 1;
			// 00
			nDest -= 3;
			pDestBuf[nDest] = C00;

			// 3> Blue
			nTablePoint++;
			nTable = data->pTable[nTablePoint];
			C00 = pSrcBuf[nTable];
			nTablePoint -= i;
			nTable = data->pTable[nTablePoint];
			C10 = pSrcBuf[nTable];
			C01 = C00;
			C11 = C10;

			// 00
			nDest++;
			pDestBuf[nDest] = C00;
			// 10
			nDest += 3;
			pDestBuf[nDest] = (C00 + C10) >> 1;
			// 11
			nDest += (nDestWidth<<1);
			pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
			// 01
			nDest -= 3;
			pDestBuf[nDest] = (C00 + C01) >> 1;

			// Here is left half pixels.
			for(j=0;j<nHeight-1;j++){
				// Make two horizontal line at once.
				for(i=0;i<(nDestWidth -nWidth +nStartPixel);i+=3){
					nDest = (nDestWidth * j * 4) + i * 2 +(nWidth -nStartPixel) * 2;

					// 1> Red
					nTablePoint = nWidth * j + i;
					nTable = data->pTable[nTablePoint];
					C00 = pSrcBuf[nTable];
					nTablePoint += 3;
					nTable = data->pTable[nTablePoint];
					C10 = pSrcBuf[nTable];
					nTablePoint += nWidth;
					nTable = data->pTable[nTablePoint];
					C11 = pSrcBuf[nTable];
					nTablePoint -= 3;
					nTable = data->pTable[nTablePoint];
					C01 = pSrcBuf[nTable];

					// 00
					pDestBuf[nDest] = C00;
					// 10
					nDest += 3;
					pDestBuf[nDest] = (C00 + C10) >> 1;
					// 11
					nDest += (nDestWidth<<1);
					pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
					// 01
					nDest -= 3;
					pDestBuf[nDest] = (C00 + C01) >> 1;

					// 2> Green
					nTablePoint++;
					nTable = data->pTable[nTablePoint];
					C01 = pSrcBuf[nTable];
					nTablePoint += 3;
					nTable = data->pTable[nTablePoint];
					C11 = pSrcBuf[nTable];
					nTablePoint -= nWidth;
					nTable = data->pTable[nTablePoint];
					C10 = pSrcBuf[nTable];
					nTablePoint -= 3;
					nTable = data->pTable[nTablePoint];
					C00 = pSrcBuf[nTable];

					// 01
					nDest++;
					pDestBuf[nDest] = (C00 + C01) >> 1;
					// 11
					nDest += 3;
					pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
					// 10
					nDest -= (nDestWidth<<1);
					pDestBuf[nDest] = (C00 + C10) >> 1;
					// 00
					nDest -= 3;
					pDestBuf[nDest] = C00;

					// 3> Blue
					nTablePoint++;
					nTable = data->pTable[nTablePoint];
					C00 = pSrcBuf[nTable];
					nTablePoint += 3;
					nTable = data->pTable[nTablePoint];
					C10 = pSrcBuf[nTable];
					nTablePoint += nWidth;
					nTable = data->pTable[nTablePoint];
					C11 = pSrcBuf[nTable];
					nTablePoint -= 3;
					nTable = data->pTable[nTablePoint];
					C01 = pSrcBuf[nTable];

					// 00
					nDest++;
					pDestBuf[nDest] = C00;
					// 10
					nDest += 3;
					pDestBuf[nDest] = (C00 + C10) >> 1;
					// 11
					nDest += (nDestWidth<<1);
					pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
					// 01
					nDest -= 3;
					pDestBuf[nDest] = (C00 + C01) >> 1;
				}
			}

			for(i=0;i<(nDestWidth -nWidth +nStartPixel);i+=3){
				nDest = (nDestWidth * j * 4) + i * 2 + (nWidth -nStartPixel) * 2;

				// Last Y line not reference next Y pixel.
				// 1> Red
				nTablePoint = nWidth * j + i;
				nTable = data->pTable[nTablePoint];
				C00 = pSrcBuf[nTable];
				nTablePoint += 3;
				nTable = data->pTable[nTablePoint];
				C10 = pSrcBuf[nTable];
				C11 = C10;
				C01 = C00;

				// 00
				pDestBuf[nDest] = C00;
				// 10
				nDest += 3;
				pDestBuf[nDest] = (C00 + C10) >> 1;
				// 11
				nDest += (nDestWidth<<1);
				pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
				// 01
				nDest -= 3;
				pDestBuf[nDest] = (C00 + C01) >> 1;

				// 2> Green
				nTablePoint++;
				nTable = data->pTable[nTablePoint];
				C10 = pSrcBuf[nTable];
				nTablePoint -= 3;
				nTable = data->pTable[nTablePoint];
				C00 = pSrcBuf[nTable];
				C11 = C10;
				C01 = C00;

				// 01
				nDest++;
				pDestBuf[nDest] = (C00 + C01) >> 1;
				// 11
				nDest += 3;
				pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
				// 10
				nDest -= (nDestWidth<<1);
				pDestBuf[nDest] = (C00 + C10) >> 1;
				// 00
				nDest -= 3;
				pDestBuf[nDest] = C00;

				// 3> Blue
				nTablePoint++;
				nTable = data->pTable[nTablePoint];
				C00 = pSrcBuf[nTable];
				nTablePoint += 3;
				nTable = data->pTable[nTablePoint];
				C10 = pSrcBuf[nTable];
				C11 = C10;
				C01 = C00;

				// 00
				nDest++;
				pDestBuf[nDest] = C00;
				// 10
				nDest += 3;
				pDestBuf[nDest] = (C00 + C10) >> 1;
				// 11
				nDest += (nDestWidth<<1);
				pDestBuf[nDest] = (C00 + C10 + C01 + C11) >> 2;
				// 01
				nDest -= 3;
				pDestBuf[nDest] = (C00 + C01) >> 1;
			}
		}
	}else{
		for(j=0;j<nHeight;j++){
			for(i=nStartPixel;i<nDestWidth;i++){
				pDestBuf[nDestWidth * j + i - nStartPixel] = pSrcBuf[data->pTable[nDestWidth * j + i]];
			}
			for(i=0;i<nStartPixel;i++){
				pDestBuf[nDestWidth * j + i + (nDestWidth - nStartPixel)] = pSrcBuf[data->pTable[nDestWidth * j + i]];
			}
		}
		return 1;
	}

	return 1;
}



int	ReleaseDonut(DonutData* data){

	free(data->pTable);
	free(data->pImageBuf);

	return 1;
}



#define CLIP(x) (((x) < 0) ? 0 : (((x) > 255) ? 255 : (x)))
int	YUYVToRGB(unsigned char* pDest, unsigned char* pSrc, int nWidth, int nHeight){
	int	nYuyvPixelCount = 0;
	int	i, j;
	int	Y0, U, Y1, V;
	int	nWidthPointer;

	// width must be even.
	if(nWidth%2){
		return -1;
	}

	nWidth *= 3;

	for(j=0;j<nHeight;j++){
		for(i=0;i<nWidth;i+=6){
			Y0 = 76284*(pSrc[nYuyvPixelCount++]-16);
			U = pSrc[nYuyvPixelCount++]-128;
			Y1 = 76284*(pSrc[nYuyvPixelCount++]-16);
			V = pSrc[nYuyvPixelCount++]-128;
			nWidthPointer = j*nWidth+i;
			pDest[nWidthPointer+0] = CLIP( (Y0+104595*V) >> 16 );//R
			pDest[nWidthPointer+1] = CLIP( (Y0-53281*V-25625*U) >> 16 );//G
			pDest[nWidthPointer+2] = CLIP( (Y0+132252*U) >> 16 );//B
			pDest[nWidthPointer+3] = CLIP( (Y1+104595*V) >> 16 );//R
			pDest[nWidthPointer+4] = CLIP( (Y1-53281*V-25625*U) >> 16 );//G
			pDest[nWidthPointer+5] = CLIP( (Y1+132252*U) >> 16 );//B
		}
	}

	return 1;
}
