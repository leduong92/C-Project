//===================================================================================================
//	TIO.h
//
//		IO Class
//		SuoerClass = None
//	
//		Copyright  1997 Nok Corporation.
//			All rights reserved.
//			Coded by T.Hiraguchi
//		Modify	:1999.12.23 coba IOポート指定コンストラクタの追加
//				:1999.12.24 coba ブロックリード関数を追加
//===================================================================================================
#ifndef HIO
#define HIO


#include "windows.h" 
//#pragma once

#define DEFAULT_PORT_ADDRESS 0x0300

// ブロックリード読み出し用構造体
typedef struct{
	short pinstat[16];
} IOSTATUS_16;


class TIO
{
public:
       	 	TIO();
       	 	TIO(int addr);
			~TIO();
//	long	GetSlotBase(short id);
	virtual void    IO_setup(void);
	short	IO_In0_Stat();
	short	IO_In1_Stat();
	short	IO_In2_Stat();
	short	IO_In3_Stat();
	short	IO_In4_Stat();
	short	IO_In5_Stat();
	short	IO_In6_Stat();
	short	IO_In7_Stat();
	short	IO_In8_Stat();
	short	IO_In9_Stat();
	short	IO_In10_Stat();
	short	IO_In11_Stat();
	short	IO_In12_Stat();
	short	IO_In13_Stat();
	short	IO_In14_Stat();
	short	IO_In15_Stat();
		
	void	IO_Out0_On();
	void	IO_Out1_On();
	void	IO_Out2_On();
	void	IO_Out3_On();
	void	IO_Out4_On();
	void	IO_Out5_On();
	void	IO_Out6_On();
	void	IO_Out7_On();
	void	IO_Out8_On();
	void	IO_Out9_On();
	void 	IO_Out10_On();
	void	IO_Out11_On();
	void	IO_Out12_On();
	void 	IO_Out13_On();
	void	IO_Out14_On();
	void 	IO_Out15_On();

	void	IO_Out0_Off();
	void	IO_Out1_Off();
	void	IO_Out2_Off();
	void	IO_Out3_Off();
	void	IO_Out4_Off();
	void	IO_Out5_Off();
	void	IO_Out6_Off();
	void	IO_Out7_Off();
	void	IO_Out8_Off();
	void	IO_Out9_Off();
	void 	IO_Out10_Off();
	void	IO_Out11_Off();
	void	IO_Out12_Off();
	void 	IO_Out13_Off();
	void	IO_Out14_Off();
	void 	IO_Out15_Off();
	void    IO_Out_Port2(short port2);
	short	IO_Out_Stat();
	short	BoardCheck();

	// 全ポート一括読み出し
	BOOL ScanAll(IOSTATUS_16* stat);


private:
	int	IOPORT;	//	I/Oボード port address
	short port1,port2;
	int IsIO;


};
#endif
