#ifndef BBB
#define BBB
//=====================================================================================================
//	Command structure for BT1500
//	File Name : haisobcr.h
//  2011.04.08 by coba
//=====================================================================================================
// modify
//=====================================================================================================
#define VERSION_STR "       ver 0.00"
#define MAINWNDNAM     "VNN"

// target PC macros
#define IPNAME "server_addr\0"
#define PORTNAME "port_no\0"


#define CMD_GET_STAFF "M00"		// M00 staff master

//----------------------------------------------------------------------------
//	 staff info.(PC->BCR)
//----------------------------------------------------------------------------
typedef	struct{
		char	flg;					// admin sign
		char	sp;
		char 	staff_id[6];	// staff-id
		char	name[20];
		char	null;
}	BM_USER;

//----------------------------------------------------------------------------
//	 for error message(PC=>BCR)
//----------------------------------------------------------------------------
typedef	struct{
		char	msg1[20];	// ‚Ps–Ú
		char	msg2[20];	// ‚Qs–Ú
		char	msg3[20];	// ‚Rs–Ú
		char	msg4[20];	// ‚Ss–Ú
		char	msg5[20];	// ‚Ts–Ú
}	BCR_MSG;

#endif
