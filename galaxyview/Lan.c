#include "Lan.h"
#include <stdio.h>	// printf
#include <stdlib.h>	// malloc
#include <string.h>	// memset
#include <errno.h>	// errno



/* Defines */
#define	NOT_READY	-1



void *ThreadLanServer(void* data){
	LanData*	pData;
	int*		nCount;
	int		addrlen = 0;

	// Copy LanData pointer
	pData = (LanData*)data;

	// Set address data length
	addrlen = sizeof(pData->connectedAddress);

	printf("Lan] accept loop start\r\n");

	// Loop, until program exit command ordered.
	while(pData->nServerThreadID != NOT_READY){
		// 1> Wait for client accept
		pData->connectedSocket = accept(pData->serverSocket, (struct sockaddr*)&pData->connectedAddress, &addrlen);

		// 2-A> If not connected, try again
		if(pData->connectedSocket == NOT_READY){
			printf("*Lan] Client not accepted\r\n");
			sleep(1);
		}
		// 2-B> If connected, wait until client close
		else{
			printf("Lan] client connected!\r\n");

			// Loop until client disconnected.
			while(pData->connectedSocket != NOT_READY){
				sleep(1);
			}
		}
	}//while
}



void *ThreadLanSend(void* data){
	LanData*	pData;
	int		nDataSize = 0;
	int		nSendSize = 0;
	int		nTotalSize = 0;
	unsigned char*	pBuf;
	int		nStep = 0;

	// Copy LanData pointer
	pData = (LanData*)data;

	// Header has 4 bytes.
	pBuf = (unsigned char*)&pData->nImageSize;
	nDataSize = 4;
	nTotalSize = nDataSize;

	while(nDataSize > 0){
		// 1> Data send to client.
		nSendSize = send(pData->connectedSocket, &pBuf[nTotalSize - nDataSize], nDataSize, MSG_DONTWAIT);

		// 2-A> Data transmitted.
		if(nSendSize > 0){
			nDataSize -= nSendSize;

			// Divide two step. First is size. Second is Image.
			if(nStep == 0 && nDataSize == 0){
				nDataSize = pData->nImageSize;
				nTotalSize = nDataSize;
				pBuf = pData->pImageData;
				nStep++;
			}
		}
		// 2-B> Send fail!
		else{
			// do - If send fail occur continuously, send cancle.
			//printf("*Lan] write error![%d]\r\n", errno);
			usleep(67000);
		}

		// 3> If flag set, thread quit.
		if(pData->nSendThreadID == NOT_READY){
			printf("Lan] thread quit nSendThreadID (close)\r\n");
			break;
		}
		// 4> If flag set, thread quit.
		if(pData->connectedSocket == NOT_READY){
			printf("Lan] thread quit connectedocket (close)\r\n");
			break;
		}
	}

	// 5> Reset flag(send image end)
	pData->nSendThreadID = NOT_READY;
}



void *ThreadLanReceive(void* data){
	LanData*	pData;
	unsigned char	buf[8];
	int		nSize = 0;
	struct linger	l_linger = {1, 0};

	printf("Lan] Receive Thread created\r\n");

	// Copy LanData pointer.
	pData = (LanData*)data;

	// Loop until client disconnected.
	while(pData->nReceiveThreadID != NOT_READY){

		// If connected, read data.
		if(pData->connectedSocket != NOT_READY){
			// 1> Receive data from client.
			nSize = recv(pData->connectedSocket, buf, 4, MSG_DONTWAIT);

			// 2-A> Receive data
			if(nSize > 0){
				// do - data combination
				if(pData->nReceiveData == NOT_READY){
					pData->nReceiveData = *(int*)buf;
				}
			}
			// 2-B> Disconnected
			else if(nSize == 0){
				printf("*Lan] receive 0(Disconnected)\r\n");
				// client quit - forced quit
				setsockopt(pData->connectedSocket, SOL_SOCKET, SO_LINGER, &l_linger, sizeof(struct linger));
				close(pData->connectedSocket);
				pData->connectedSocket = NOT_READY;
				pData->nSendThreadID = NOT_READY;
				sleep(1);
			}
			// 2-C> Receive data error
			else{
				// do - exception
				//printf("*Lan] receive data error![%d]\r\n", errno);
				usleep(67000);
			}
		}
	}//while

	printf("Lan] Receive Thread end\r\n");
}



int InitializeLan(LanData* data){
	int		nID = 0;

	// 1> Struct data initialization
	data->serverSocket = NOT_READY;
	data->connectedSocket = NOT_READY;
	data->pServerThread = 0;
	data->nServerThreadID = NOT_READY;
	data->pSendThread = 0;
	data->nSendThreadID = NOT_READY;
	data->pReceiveThread = 0;
	data->nReceiveThreadID = NOT_READY;

	data->nImageSize = 0;
	data->pImageData = NULL;

	data->nReceiveData = NOT_READY;

	// 2> Create server socket
	data->serverSocket = socket(PF_INET, SOCK_STREAM, 0/*IPPROTO_TCP*/);
	if(data->serverSocket == NOT_READY){
		printf("*Lan] create server socket fail!\r\n");
		return -1;
	}

	// 3> Set server
	memset(&data->serverAddress, 0, sizeof(data->serverAddress));
	data->serverAddress.sin_family = AF_INET;
	data->serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	data->serverAddress.sin_port = htons(data->nPort);

	// 4> Bind
	if(bind(data->serverSocket, (struct sockaddr*)&data->serverAddress, sizeof(data->serverAddress)) == -1){
		printf("*Lan] bind fail!\r\n");
		return -1;
	}

	// 5> Listen(One at a same time)
	if(listen(data->serverSocket, 1) == -1){
		printf("*Lan] listen fail!\r\n");
		return -1;
	}

	// 6> Create Server accept thread
	data->nServerThreadID = 1;
	
	nID = pthread_create(&data->pServerThread, NULL, ThreadLanServer, (void*)data);

	if(nID != 0){
		printf("*Lan] server thread create fail!\r\n");
		return -1;
	}

	data->nServerThreadID = nID;

	// 7> Create Receive thread
	data->nReceiveThreadID = 1;

	nID = pthread_create(&data->pReceiveThread, NULL, ThreadLanReceive, (void*)data);

	if(nID != 0){
		printf("*Lan] receive thread create fail!\r\n");
		return -1;
	}

	data->nReceiveThreadID = nID;

	printf("Lan] Initialization done!\r\n");

	return 1;
}



int SendDataOnLan(LanData* data){
	int	nID = 0;

	// 1> If client not connected, skip.
	if(data->connectedSocket == NOT_READY){
		// It's not error. Just skip.
		return 0;
	}

	// 2> If busy, skip.
	if(data->nSendThreadID != NOT_READY){
		// It's not error. Just skip.
		return 0;
	}

	// Set temp constant to avoid reading -1
	data->nSendThreadID = 1;

	// 3> Create Server accept thread
	nID = pthread_create(&data->pSendThread, NULL, ThreadLanSend, (void*)data);
	if(nID != 0){
		// do - exception
		data->nSendThreadID = NOT_READY;
		printf("Lan] Thread create error![%d]\r\n", nID);
		return -1;
	}

	// 4> Thread detach to eliminate memory leak when thread ended.
	pthread_detach(data->pSendThread);

	return 0;
}



int ReceiveHandler(LanData* data){
	int 	nProcess = data->nReceiveData;
	int 	nID;
	short	nPixel;

	// 1> Check client connect
	if(data->connectedSocket == NOT_READY){
		return 0;
	}

	// 2> Check receive thread exist
	if(data->nReceiveThreadID == NOT_READY){
		return 0;
	}

	// 3> Check recieved data exist.
	if(nProcess == NOT_READY){
		return 0;
	}

	// 4> Data reset to not ready
	data->nReceiveData = NOT_READY;

	return nProcess;
}



void ReleaseLan(LanData* data){
	struct linger	l_linger = {1, 0};

	// 1> Close the connected client, if exist.
	if(data->connectedSocket != -1){
		data->nSendThreadID = -1;
		data->nReceiveThreadID = -1;
		sleep(1);
		setsockopt(data->connectedSocket, SOL_SOCKET, SO_LINGER, &l_linger, sizeof(struct linger));
		close(data->connectedSocket);
		printf("Lan] Client resource free~\r\n");
	}

	// 2> Close the server, if exist.
	if(data->serverSocket != -1){
		data->nServerThreadID = -1;
		sleep(1);
		setsockopt(data->serverSocket, SOL_SOCKET, SO_LINGER, &l_linger, sizeof(struct linger));
		close(data->serverSocket);
		printf("Lan] Server resource free~\r\n");
	}
}



int GetLanSendState(LanData* data){
	// 1> If client not connected, not ready.
	if(data->connectedSocket == NOT_READY){
		return -1;
	}

	// 2> If busy, not ready.
	if(data->nSendThreadID != NOT_READY){
		return -1;
	}

	// 3> Send thread ready.
	return 0;
}



void ClientClose(LanData* data){
	struct linger	l_linger = {1, 0};

	data->nSendThreadID = NOT_READY;

	setsockopt(data->connectedSocket, SOL_SOCKET, SO_LINGER, &l_linger, sizeof(struct linger));
	if(data->connectedSocket != NOT_READY){
		close(data->connectedSocket);
	}
	data->connectedSocket = NOT_READY;
}
