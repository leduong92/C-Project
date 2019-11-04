#ifndef BCR_TYPES_H
#define BCR_TYPES_H
//========================================================================================================
//	FileName	:BcrTypes.h
//	Function: define structurs and macros for BCR communication.
//========================================================================================================

#define MAX_JOBORDER	40


// Status Stock
#define NO_STOCK	"0"
#define TAKE_IN		"1"

//Print Packing label


//thuy close this code, cause: definition at resource.h
//#define IDI_ICON1                       102

#define IDD_DLG_MAIN                    104
#define IDI_ICON_LOGIN                  104
#define IDB_S_T                         137
#define IDB_S_S                         138
#define IDB_N_R                         139
#define IDB_M_C                         140
#define IDB_E_C                         141
#define IDB_E_S                         142
#define IDB_L_C                         143
#define IDB_F_C                         144
#define IDB_A_C                         145
#define IDB_Q_C                         146
#define IDB_JIS                         147

//thuy close this code, cause: definition at resource.h
/*#define IDC_ST_TITLE                    1000
#define IDC_ST_VERSION                  1001
#define IDC_BT_ENDAPP                   1004
#define IDC_ST_MESSAGE                  1005
#define IDC_ST_COUNT                    1007
#define IDC_ST_TIMER                    1018
#define IDC_ST_ITEM0                    1021
#define IDC_ST_ITEM1                    1022
#define IDC_ST_ITEM2                    1023
#define IDC_ST_ITEM3                    1024
#define IDC_ST_ITEM4                    1025
#define IDC_ST_ITEM5                    1026
#define IDC_ST_ITEM6                    1027
#define IDC_ST_ITEM7                    1028*/
#define IDC_ST_DATA0                    1042

//thuy close this code, cause: definition at resource.h
//#define IDC_JOB_TAG                     1042
#define IDC_ST_DATA1                    1043

//thuy close this code, cause: definition at resource.h
//#define IDC_ITEM                        1043
#define IDC_ST_DATA2                    1044

//thuy close this code, cause: definition at resource.h
//#define IDC_QTY_PACK                    1044
#define IDC_ST_DATA3                    1045

//thuy close this code, cause: definition at resource.h
//#define IDC_QTY_BOX                     1045
#define IDC_ST_DATA4                    1046

//thuy close this code, cause: definition at resource.h
//#define IDC_ST_PACK_TYPE                1046
#define IDC_ST_DATA5                    1047

//thuy close this code, cause: definition at resource.h
//#define IDC_BOX_TYPE                    1047
#define IDC_ST_DATA6                    1048

//thuy close this code, cause: definition at resource.h
//#define IDC_CUST_NAME                   1048
//#define IDC_BT_PRINT                    1056
#define IDC_ST_HANDY                    1062
#define IDC_BT_TEST                     1063
#define IDC_ST_HANDY_STATUS             1064
#define IDC_BT_TEST2                    1065
#define IDC_ST_HANDY_DATA               1066
#define IDC_ST_MONITOR_ITEM1            1067

//thuy close this code, cause: definition at resource.h
//#define IDC_BT_START                    1068
#define IDC_ST_HANDY_TCP                1069
#define IDC_ST_MONITOR_ITEM2            1070

//thuy close this code, cause: definition at resource.h
//#define IDC_BT_END                      1071
#define IDC_ST_MONITOR_ITEM3            1073
////////////////////////////////////////////////////////
		// From BHT
		typedef	struct	{
			char	ElementCode[3];
			char	Data[15];
		}	DATA;
	
		typedef	struct	{
			//char	ID;
			//char	SubID[2];
			char	LocationCd[3];
			char	UserId[3];
			char	JobTag[11];
			char	dollar;
			char	Wc[6];
			char	JobClassCode[3];
			char	JobPatternNo;
			char	MachineNo[3];			
			char	Filler[14];
			DATA	Data[200];
			//char	qtyJob[6];
		}RCV;   
////////////////////////////////////////////////////////
		//User Master
		typedef struct{
			char UserLoc[3];	// 
			char UserId[3];		// 
			char FirstName[12];	// 
			char LastName[12];	// 
			char AdminSign;		// 
			char null;
		} BCR_USER;

///////////////////////////////////////////////////////
		//Element Master
		typedef struct{
			char ElementCd[3];			// 
			char ElementName[20];		//
			char null;
		} BCR_ELEMENT;
		
////////////////////////////////////////////////////////
		//Transaction order master
		typedef struct{
			char FunctionNo[2];		// 
			char ElementCode[3];		// 
			char Sign;
		} TRN_ORDER;

////////////////////////////////////////////////////////
		//Curing Job to BHT::::: Record length should be 128 bytes + TRN_ORDER * 40
		typedef struct{
			char DataNo[3];		// 
			char JobNo[7];		// 
			char Suffix[3];
			char Item[20];
			char ObjSign[4];
			char QtyJob[8];
			char QtyPack[7];
			char QtyBox[7];
			char Status;
			char Material[14];
			char Filler[54];	// 128 byte 68
		} BCR_JOB_CURING;


////////////////////////////////////////////////////////
///* Need to arrage *////
		//Rubber Job to BHT ::::: Record length should be 128 bytes + TRN_ORDER * 40
		typedef struct{
			char DataNo[3];		// 
			char JobNo[7];		// 
			char Suffix[3];
			char Item[20];
			char ObjSign[4];
			char QtyJob[8];
			char QtyPack[7];
			char QtyBox[7];
			char Status;
			char Filler[68];	// 128 byte
		} BCR_JOB_RUBBER;


////////////////////////////////////////////////////////
///* Need to arrage *////
		//Metal Job to BHT ::::: Record length should be 128 bytes + TRN_ORDER * 40
		typedef struct{
			char DataNo[3];		// 
			char JobNo[7];		// 
			char Suffix[3];
			char Item[20];
			char ObjSign[4];
			char QtyJob[8];
			char QtyPack[7];
			char QtyBox[7];
			char Status;
			char Filler[68];	// 128 byte
		} BCR_JOB_METAL;

////////////////////////////////////////////////////////
///* Need to arrage *////
		//Job to BHT ::::: Record length should be 128 bytes + TRN_ORDER * 40
		typedef struct{
			char 	order[128];		// for setting BCR_JOB_CURING or BCR_JOB_MATAL or BCR_JOB_RUBBER
			TRN_ORDER	TrnOrder[MAX_JOBORDER];
		} BCR_JOB_ORDER;

////////////////////////////////////////////////////////
///* Data for PostCuring *////
// 2006.07.11 matsuda 
		typedef struct{
			char Id[3];
			char Material[4];
			char Time[6];
			char Temp[6];
			char Wc[6];
			char MachineNo[6];
			char Weight[6];
			char Filler[91];		// 128 byte
		} BCR_POSTCURE_DATA;


#endif
