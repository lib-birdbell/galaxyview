#ifndef LAN_H
#define LAN_H



/* Includes */
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>



/* Type defines */
typedef struct{
	// Socket
	int			serverSocket;
	int			connectedSocket;
	struct sockaddr_in	serverAddress;
	struct sockaddr_in	connectedAddress;
	// Thread
	pthread_t		pServerThread;
	int			nServerThreadID;
	pthread_t		pSendThread;
	int			nSendThreadID;
	pthread_t		pReceiveThread;
	int			nReceiveThreadID;
	// Image
	int			nImageSize;
	unsigned char*		pImageData;
	// Command
	int			nReceiveData;
	// Lan server port
	int			nPort;
}LanData;



/* Functions */



/*
 * Function name	: ThreadLanServer
 * Attr1		: LanData to accept for Client.
 * Remarks		: Wait client connection.
 */
void *ThreadLanServer(void* data);



/*
 * Function name	: ThreadLanSend
 * Attr1		: LanData to send data.
 * Remarks		: Send image data by Thread.
 */
void *ThreadLanSend(void* data);



/*
 * Function name	: ThreadLanReceive
 * Attr1		: LanData to receive data.
 * Remarks		: Receive command data by Thread.
 */
void *ThreadLanReceive(void* data);



/*
 * Function name	: InitializeLan
 * Attr1		: LanData pointer to initialize Server.
 * Return		: If failed. It returns -1.
 * Remarks		: It implements socket create, bind, listen, and create thread for accept and receive.
 */
int InitializeLan(LanData* data);



/*
 * Function name	: SendDataOnLan
 * Attr1		: LanData pointer to send data to client.
 * Return		: If not ready to send, It returns 0. But if have a problem, It returns -1. Not ready is not problem.
 * Remarks		: If client connected, it send image data.
 */
int SendDataOnLan(LanData* data);



/*
 * Function name	: ReceiveHandler
 * Attr1		: LanData pointer to receive data from client
 * Return		: Command. If command not exist, it returns 0.
 * Remarks		: If client conneceted, and receive thread exist, and receive data exist, return command.
 */
int ReceiveHandler(LanData* data);



/*
 * Funtion name		: ReleaseLan
 * Attr1		: LanData pointer to release all resource.
 * Remarks		: All resource released. Call the program when you want quit.
 */
void ReleaseLan(LanData* data);



/*
 * Function name	: GetLanSendState
 * Attr1		: LanData pointer to get Send Thread status.
 * Return		: If send thread not ready, it returns -1.
 * Remarks		: To checking a Send thread status. 
 */
int GetLanSendState(LanData* data);



/*
 * Function name	: ClientClose
 * Attr1		: LanData to close connected client.
 * Remarks		: It close connected client.
 */
void ClientClose(LanData* data);



#endif
