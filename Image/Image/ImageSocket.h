#pragma once
#include <winsock.h>
#include <vector>
#include <string>

using namespace  std;
class Callback
{
public:

	virtual void OnDataCallback(char* pData, DWORD dwLength) = 0;
};
namespace IPC_SOCKET
{
	static const int	nPort = 8869;
	static const char* pIp = "127.0.0.1";
};


//待处理的TCP包
typedef struct _TCP_PACKET_
{
	char* pData;		//缓冲区指针
	DWORD	dwDataLength;	//长度
}TCP_PACKET, * PTCP_PACKET;
class ImageSocket
	{
	public:
		static ImageSocket* GetInstance();
		~ImageSocket();
	public:
		bool Start(string strIp, int nPort);

		void Stop();

		void SetPacketReceiver(Callback* pReceiver);

		void ClientListenThread();//服务端包接收线程,监听client端发的消息

		bool OnTcpPacket();

		bool SendPacket(char* pData, DWORD nDataLen);
	private:
		ImageSocket();
		void AddTcpPakcet(char* pData, DWORD dwLength); //在m_vecTcpPacket中添加一个待处理包

	private:
		Callback* m_pPacketpReceiver = nullptr; //接收接收处理器

		SOCKET m_clientSocket = INVALID_SOCKET;		//客户端socket

		std::vector<TCP_PACKET>	m_vecTcpPacket;	//待处理的TCP包队列
		CRITICAL_SECTION m_CriticalSection;
		bool m_bExit = false;

		string m_strIp = IPC_SOCKET::pIp;
		int m_nPort = IPC_SOCKET::nPort;
	};


