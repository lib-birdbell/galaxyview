/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>

#include "Jpeg.h"



/* Functions */

int	GetImageFromJpeg(ImageData* destData, ImageData* srcData){
	struct jpeg_decompress_struct	cinfo;
	struct jpeg_error_mgr		err;

	JSAMPARRAY	buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW)*1);
	int		row_stride;
	int		nSavedCount = 0;
	int		nFourByteAlign = 0;
	unsigned char*	pBuf;

	// 2> Set jpeg error
	cinfo.err = jpeg_std_error(&err);

	// 3> Create decompress info
	jpeg_create_decompress(&cinfo);

	// 4> Set jpeg from memory
	jpeg_mem_src(&cinfo, srcData->pImageBuf, srcData->nImageSize);

	// 5> Read jpeg header
	jpeg_read_header(&cinfo, TRUE);

	// 6> Start decompress
	jpeg_start_decompress(&cinfo);

	// 7> Set image features
	destData->nWidth = cinfo.output_width;
	destData->nHeight = cinfo.output_height;
	destData->nBPP = cinfo.output_components;

	row_stride = cinfo.output_width * cinfo.output_components;

	//buffer[0] = (JSAMPROW)malloc(sizeof(JSAMPLE) *row_stride);	

	//buffer = (*cinfo.mem->alloc_sallay)
	//		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	// Do not allocate but input pointer
	pBuf = (unsigned char*)malloc(sizeof(unsigned char) * row_stride);

	nSavedCount = 0;
	// This have to be 4 byte align
	nFourByteAlign = (row_stride % 4);
	if(nFourByteAlign != 0){
		nFourByteAlign -= 4;
	}

	while(cinfo.output_scanline < cinfo.output_height){
		//jpeg_read_scanlines(&cinfo, buffer, 1);
		jpeg_read_scanlines(&cinfo, &pBuf, 1);

		// Have to be modified from memcpy to pointer add.
		memcpy(&destData->pImageBuf[nSavedCount], pBuf, row_stride);

		nSavedCount += row_stride + nFourByteAlign;
	}

	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	free(pBuf);

	return 1;
}



int	SetImageToJpeg(ImageData* destData, ImageData* srcData){
	struct jpeg_compress_struct	cinfo;
	struct jpeg_error_mgr		jerr;

	unsigned long			lJpegSize;

	int				nLine = 0;
	int				nRow_stride = 0;

	JSAMPROW			row_pointer[1];

	int				i, j;
	int				nWidth, nHeight;
	unsigned char*			pBuf;
	JSAMPROW			y[16], cb[16], cr[16];
	JSAMPARRAY			data[3];

	nRow_stride = srcData->nWidth * srcData->nBPP;

	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_compress(&cinfo);

	jpeg_mem_dest(&cinfo, &destData->pImageBuf, &lJpegSize);

	cinfo.image_width = srcData->nWidth;
	cinfo.image_height = srcData->nHeight;
	cinfo.input_components = srcData->nBPP;
	cinfo.in_color_space = srcData->nColorSpace;

	jpeg_set_defaults(&cinfo);

	if(srcData->nColorSpace == JCS_YCbCr){
		//jpeg_set_colorspace(&cinfo, JCS_YCbCr);
		//cinfo.raw_data_in = TRUE;
		cinfo.comp_info[0].h_samp_factor = 2;
		cinfo.comp_info[0].v_samp_factor = 2;
		cinfo.comp_info[1].h_samp_factor = 1;
		cinfo.comp_info[1].v_samp_factor = 1;
		cinfo.comp_info[2].h_samp_factor = 1;
		cinfo.comp_info[2].v_samp_factor = 1;
	}
	cinfo.num_components = 3;

	jpeg_set_quality(&cinfo, 15, TRUE);
	cinfo.dct_method = JDCT_FLOAT;
	//cinfo.dct_method = JDCT_FASTEST;

	jpeg_start_compress(&cinfo, TRUE);

	if(srcData->nColorSpace == JCS_YCbCr){
		pBuf = srcData->pImageBuf;
		nWidth = srcData->nWidth;
		nHeight = srcData->nHeight;
		data[0] = y;
		data[1] = cb;
		data[2] = cr;

		for(j=0;j<nHeight;j+=16){
			for(i=0;i<16;i++){
				y[i] = pBuf + nWidth*(i+j);
				if(i%2 == 0){
					cb[i] = pBuf + nWidth*nHeight + nWidth/2*((i+j));
					cr[i] = pBuf + nWidth*nHeight + nWidth/2*nHeight + nWidth/2*((i+j));
					cb[i+1] = pBuf + nWidth*nHeight + nWidth/2*((i+j));
					cr[i+1] = pBuf + nWidth*nHeight + nWidth/2*nHeight + nWidth/2*((i+j));
				}
			}
			jpeg_write_raw_data(&cinfo, data, 16);
		}

	}else{
		while(cinfo.next_scanline < cinfo.image_height){
			row_pointer[0] = &srcData->pImageBuf[cinfo.next_scanline * nRow_stride];
			jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}
	}

	jpeg_finish_compress(&cinfo);

	destData->nImageSize = (int)lJpegSize;
	//destData->nImageSize = jpeg_mem_size(&cinfo);

	jpeg_destroy_compress(&cinfo);


	return 1;	
}
