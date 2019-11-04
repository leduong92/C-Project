#pragma once
#ifdef CMELP3E_EXPORTS  
#define CMELP3E __declspec(dllexport)   
#else  
#define CMELP3E __declspec(dllimport)   
#endif  
#include <winsock2.h>
#include <windows.h>

#define UDP_MELPORT 5000

#define DEV_WORD 0
#define DEV_BIT  1


#define P3E_FIX_DATALEN 24

class CMelsecP3E
{
public:
	CMelsecP3E(void);
	~CMelsecP3E(void);

	BOOL Init();

	void SetMode(BOOL binary_mode);	
	
	int P3EWrite(char* ipaddr, unsigned short port, int devtype, char* devaddr
		, short counts, char* data);
	int P3EWrite(char* ipaddr, unsigned short port, int devtype, char* devaddr
		, short counts, char* data, BOOL binary);

	int P3ERead(char* ipaddr, unsigned short port, char* buf, int bufsize
		, int devtype, char* devaddr, short counts);
	int P3ERead(char* ipaddr, unsigned short port, char* buf, int bufsize
		, int devtype, char* devaddr, short counts, BOOL binary);

	int Str2Mel(char* buf, unsigned int bufsize, char* str);
	int Mel2Str(char* buf, unsigned int bufsize, char* melstr);


	BOOL ULChg(char* buf);

	unsigned short htoi(char *hexstr, short len);


	BOOL m_fOpen;					
	BOOL m_fBinMode;				


	int UdpSend(char* ipaddr, unsigned short port, char* data, int size, char* rcv, int rsize);


	int P3EWriteA(char* ipaddr, unsigned short port, int devtype, char* devaddr, short counts, char* data);
	int P3EReadA(char* ipaddr, unsigned short port, char* buf, int bufsize
		, int devtype, char* devaddr, short counts);
	int UdpSendA(char* ipaddr, unsigned short port, char* data, int size, char* rcv, int rsize);


	int P3EWriteB(char* ipaddr, unsigned short port, int devtype, char* devaddr, short counts, char* data);
	int P3EReadB(char* ipaddr, unsigned short port, char* buf, int bufsize
		, int devtype, char* devaddr, short counts);
	int UdpSendB(char* ipaddr, unsigned short port, char* data, int size, char* rcv, int rsize);
	int UdpSendBit(char* ipaddr, unsigned short port, char* data, int size, char* rcv, int rsize, short counts);

};

extern "C"
{
	CMELP3E BOOL Init()
	{
		CMelsecP3E *melsec = new CMelsecP3E();
		return melsec->Init();
	}

	CMELP3E void SetMode(BOOL binary_mode)
	{
		CMelsecP3E *melsec = new CMelsecP3E();
		return melsec->SetMode(binary_mode);
	}

	CMELP3E int P3EWrite(char* ipaddr, unsigned short port, int devtype, char* devaddr
		, short counts, char* data)
	{
		CMelsecP3E *melsec = new CMelsecP3E();
		return melsec->P3EWrite(ipaddr, port, devtype, devaddr, counts, data);
	}
	CMELP3E int P3EWriteB(char* ipaddr, unsigned short port, int devtype, char* devaddr
		, short counts, char* data, BOOL binary)
	{
		{
			CMelsecP3E *melsec = new CMelsecP3E();
			return melsec->P3EWrite(ipaddr, port, devtype, devaddr, counts, data, binary);
		}
	}

	CMELP3E int P3ERead(char* ipaddr, unsigned short port, char* buf, int bufsize
		, int devtype, char* devaddr, short counts)
	{
		CMelsecP3E *melsec = new CMelsecP3E();
		return melsec->P3ERead(ipaddr, port, buf, bufsize,  devtype, devaddr, counts);
	}
	CMELP3E int P3EReadB(char* ipaddr, unsigned short port, char* buf, int bufsize
		, int devtype, char* devaddr, short counts, BOOL binary)
	{
		CMelsecP3E *melsec = new CMelsecP3E();
		return melsec->P3ERead(ipaddr, port, buf, bufsize, devtype, devaddr, counts, binary);
	}
	CMELP3E int Str2Mel(char* buf, unsigned int bufsize, char* str)
	{
		CMelsecP3E *melsec = new CMelsecP3E();
		return melsec->Str2Mel(buf, bufsize, str);
	}
	CMELP3E int Mel2Str(char* buf, unsigned int bufsize, char* melstr)
	{
		CMelsecP3E *melsec = new CMelsecP3E();
		return melsec->Mel2Str(buf, bufsize, melstr);
	}


	CMELP3E BOOL ULChg(char* buf)
	{
		CMelsecP3E *melsec = new CMelsecP3E();
		return melsec->ULChg(buf);
	}

	CMELP3E unsigned short htoi(char *hexstr, short len)
	{
		CMelsecP3E *melsec = new CMelsecP3E();
		return melsec->htoi(hexstr, len);
	}
}
