/* Includes */
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#include <linux/videodev2.h>

#include "Cam.h"



/* Functions */
static int xioctl(int fd, int request, void *arg){
	int r;

	do r = ioctl(fd, request, arg);
	while ( -1 == r && EINTR == errno);

	return r;
}



int	InitializeCam(CamInfo* info){
	int	fd;
	struct	v4l2_format		fmt = {0};
	struct	v4l2_requestbuffers	req = {0};
	struct	v4l2_buffer		buf = {0};
	int	i;

	// 1> Open device file.
	fd = open(info->chDev, O_RDWR);
	if(fd < 0){
		printf("*Cam] %s open fail!\r\n", info->chDev);
		return -1;
	}

	// 2> Get video capability
	// do - will be made

	// 3> Get and set input format
	if(info->nSizeForce){
		printf("Cam] forced ");
		fmt.type		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		fmt.fmt.pix.width	= info->nWidth;
		fmt.fmt.pix.height	= info->nHeight;
		if(info->nColorSpace == 3){
			fmt.fmt.pix.pixelformat	= V4L2_PIX_FMT_YUYV;
		}else if(info->nColorSpace == 2){
			fmt.fmt.pix.pixelformat	= V4L2_PIX_FMT_RGB24;
		}else{
			fmt.fmt.pix.pixelformat	= V4L2_PIX_FMT_MJPEG;
		}
		fmt.fmt.pix.field	= V4L2_FIELD_NONE;
		if(ioctl(fd, VIDIOC_S_FMT, &fmt) < 0){
			printf("*Cam] VIDIO_S_FMT error!\r\n");
			close(fd);
			return -1;
		}
	}else{
		fmt.type		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if(ioctl(fd, VIDIOC_G_FMT, &fmt) < 0){
			printf("*Cam] VIDIO_G_FMT error!\r\n");
			close(fd);
			return -1;
		}
		printf("Cam] default ");
	}
	printf("size : width %d, height %d, pixel format : ", fmt.fmt.pix.width, fmt.fmt.pix.height);
	if(fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG){
		printf("MJPEG\r\n");
	}else{
		printf("ETC\r\n");
	}

	// 4> Request buffers
	req.count = CAM_BUF_COUNT;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	
	if(xioctl(fd, VIDIOC_REQBUFS, &req) < 0){
		printf("*Cam] VIDIOC_REQBUFS error!\r\n");
		close(fd);
		return -1;
	}

	/* 5> Query buffer */
	for(i=0;i<CAM_BUF_COUNT;i++){
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		if(xioctl(fd, VIDIOC_QUERYBUF, &buf) < 0){
			printf("*Cam] VIDIOC_QUERYBUF error!\r\n");
			close(fd);
			return -1;
		}

		info->pImageBuf[i] = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
		printf("Cam] cam%d =  Buffer Length : %d\tAddress : %p\n", buf.index, buf.length, info->pImageBuf[i]);
	}

	/* 6> QBUF */
	for(i=0;i<CAM_BUF_COUNT;i++){
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		if(xioctl(fd, VIDIOC_QBUF, &buf) < 0){
			printf("*Cam] VIDIOC_QBUF error!\r\n");
			//fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
			close(fd);
			return -1;
		}
	}

	printf("Cam] VIDIOC_QBUF\r\n");

	if(xioctl(fd, VIDIOC_STREAMON, &buf.type) < 0){
		printf("*Cam] VIDIOC_STREAMON error!\r\n");
		//fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
		close(fd);
		return -1;
	}
	
	printf("Cam] VIDIOC_STREAMON \r\n");

	fd_set	fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	struct timeval	tv = {0};
	tv.tv_sec = 2;
	int r= select(fd+1, &fds, NULL, NULL, &tv);
	if(-1 == r){
		printf("*Cam] select error\r\n");
		close(fd);
		return -1;
	}

	// copy a file discriptor
	info->fd = fd;

	return 1;
}



int	GetCamImage(CamInfo* camInfo){
	struct	v4l2_buffer		buf = {0};

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	if(-1 == xioctl(camInfo->fd, VIDIOC_DQBUF, &buf)){
		printf("*Cam] VIDIOC_DQBUF\r\n");
		//close(fd);
		return -1;
	}

	// do - check index
	camInfo->pJpeg = camInfo->pImageBuf[buf.index];

	//printf("Cam] VIDIOC_DQBUF\r\n");
	
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	if(xioctl(camInfo->fd, VIDIOC_QBUF, &buf) < 0){
		printf("*Cam] VIDIOC_QBUF error!\r\n");
		//fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
		close(camInfo->fd);
		return -1;
	}

	return buf.bytesused;
}



int	ReleaseCam(CamInfo* camInfo){

	if(camInfo->fd){
		close(camInfo->fd);
	}

	printf("Cam] Cam resource free~\r\n");

	return 1;
}
