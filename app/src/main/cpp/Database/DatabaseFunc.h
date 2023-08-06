#ifndef _DATABASEFUNC__H
#define	_DATABASEFUNC__H

#ifdef	__cplusplus
extern "C" {
#endif
#include "../Database/dct.h"
#include "../Database/ins.h"
#include "../Database/prm.h"
#include "../Includes/POSTypedef.h"

#include "../Includes/Trans.h"
#include "../FileModule/myFileFunc.h"

//��ע�⣬��Ҫ����ļ���Ĵ�Сд
#define DB_TERMINAL "/data/data/pub/TERMINAL.S3DB"
//#define DB_BATCH	"/data/data/pub/S1_BANCNET.S3DB"
#define DB_BATCH	"/data/data/com.Source.S1_BANCNET.BANCNET/S1_BANCNET.S3DB"

#define DB_EMV	"/data/data/pub/EMV.S3DB"
#define DB_COM	"/data/data/pub/COM.S3DB"

#define DB_MULTIAP	"/data/data/pub/MULTIAP.S3DB"
#define DB_WAVE		"/data/data/pub/WAVE.S3DB"

#define DB_ERM	"/data/data/pub/ERM.S3DB"
#define DB_ERM_BATCH  "/data/data/pub/ERMBATCH.S3DB"

#define DB_MULTIAP_JOURNAL "/data/data/pub/MULTIAP.S3DB-journal"
#define DB_INVOICE "/data/data/pub/INVOICE.S3DB"

#define ISOLOG_DB "/data/data/com.Source.S1_MCC.MCC/ISOLOG.S3DB"

#define DB_POST	"/data/data/pub/POST.S3DB"


BOOL fGetDBOpen(void);
void vdSetDBOpen(BOOL fOpen);

int inAIDNumRecord(void);
int inHDTNumRecord(void);
int inTCTNumRecord(void);
int inCDTNumRecord(void);
int inRDTNumRecord(void);
int inEMVNumRecord(void);
int inAIDNumRecord(void);
int inMSGNumRecord(void);
int inTLENumRecord(void);
int inIITNumRecord(void);
int inMITRead(int inSeekCnt);

int inCPTRead(int inSeekCnt);
int inCPTSave(int inSeekCnt);

int inPCTRead(int inSeekCnt);
int inPCTSave(int inSeekCnt);

int inHDTRead(int inSeekCnt);
int inHDTSave(int inSeekCnt);
int inCDTRead(int inSeekCnt);
int inCDTReadMulti(char *szPAN, int *inFindRecordNum);
int inCDTMAX(void);

int inEMVRead(int inSeekCnt);
int inAIDRead(int inSeekCnt);
int inAIDReadbyRID(int inSeekCnt, char * PRid);
int inTCTRead(int inSeekCnt);
int inTCTSave(int inSeekCnt);
int inTCTMenuSave(int inSeekCnt);
int inTCPRead(int inSeekCnt);
int inTCPSave(int inSeekCnt);
int inMSGRead(int inSeekCnt);
int inTLERead(int inSeekCnt);
int inTLESave(int inSeekCnt);
int inIITRead(int inSeekCnt);
int inIITSave(int inSeekCnt);

int inCSTNumRecord(void);
int inCSTRead(int inSeekCnt);
int inCSTSave(int inSeekCnt);

int inMMTReadRecord(int inHDTid,int inMITid);

int inMMTNumRecord(void);

int inMMTReadNumofRecords(int inSeekCnt,int *inFindRecordNum);

int inMMTSave(int inSeekCnt);

int inPITNumRecord(void);
int inPITRead(int inSeekCnt);
int inPITSave(int inSeekCnt);

int inWaveAIDRead(int inSeekCnt);
int inWaveAIDNumRecord(void);
int inWaveEMVNumRecord(void);
int inWaveEMVRead(int inSeekCnt);



int inMSGResponseCodeRead(char* szMsg, char *szMsg2, int inMsgIndex, int inHostIndex);
int inMSGResponseCodeReadByHostName(char* szMsg, char *szMsg2, int inMsgIndex, char *szHostName);


int inDatabase_BatchDeleteHDTidMITid(void);
int inDatabase_BatchDelete(void);
int inDatabase_BatchInsert(TRANS_DATA_TABLE *transData);
int inDatabase_BatchSave(TRANS_DATA_TABLE *transData, int inStoredType);
int inDatabase_BatchRead(TRANS_DATA_TABLE *transData, int inSeekCnt);
int inDatabase_BatchSave(TRANS_DATA_TABLE *transData, int inStoredType);
int inDatabase_BatchSearch(TRANS_DATA_TABLE *transData, char *hexInvoiceNo);
int inDatabase_BatchCheckDuplicateInvoice(char *hexInvoiceNo);
int inBatchNumRecord(void);
int inDatabase_BatchReadByHostidAndMITid(TRANS_DATA_TABLE *transData,int inHDTid,int inMITid);
int inBatchByMerchandHost(int inNumber, int inHostIndex, int inMerchIndex, char *szBatchNo, int *inTranID);
int inDatabase_BatchReadByTransId(TRANS_DATA_TABLE *transData, int inTransDataid);


int inHDTReadHostName(char szHostName[][100], int inCPTID[]);
int inERMAP_Database_BatchDelete(void);
int inERMAP_Database_BatchInsert(ERM_TransData *strERMTransData);

int inMultiAP_Database_BatchRead(TRANS_DATA_TABLE *transData);
int inMultiAP_Database_BatchUpdate(TRANS_DATA_TABLE *transData);
int inMultiAP_Database_BatchDelete(void);
int inMultiAP_Database_BatchInsert(TRANS_DATA_TABLE *transData);

int inMultiAP_Database_EMVTransferDataInit(void);
int inMultiAP_Database_EMVTransferDataWrite(USHORT usDataLen, BYTE *bEMVData);
int inMultiAP_Database_EMVTransferDataRead(USHORT *usDataLen, BYTE *bEMVData);

int inMultiAP_Database_COM_Read(void);
int inMultiAP_Database_COM_Save(void);
int inMultiAP_Database_COM_Clear(void);

int inDatabase_BancnetBatchSearch(TRANS_DATA_TABLE *transData, char *szBancnetTraceNo);
int inDatabase_InvoiceNumInsert(TRANS_DATA_TABLE *transData);
int inDatabase_InvoiceNumSearch(TRANS_DATA_TABLE *transData, char *hexInvoiceNo);
int inDatabase_InvoiceNumDelete(int HDTid, int MITid);
int inEMVReadData(int inSeekCnt); // Force to read record of 7(BANCNET) -- sidumili

// ERM
int inERMAP_Database_ERMAdviceBatchDelete(void);
int inERMAP_Database_ERMTransDataBatchDelete(void);
int inERMAP_Database_ERMTransDataBackupBatchDelete(void);

int inDatabase_TerminalOpenDatabase(const char *szDBName);
int inDatabase_TerminalCloseDatabase(void);

int inMMTBatchNotEmptySave(int inSeekCnt);
int inCheckBatcheNotEmtpy(void);
int inTLESecRead(int inSeekCnt);

int inSSLRead(int inSeekCnt);
int inSSLSave(int inSeekCnt);


#endif	/* _DATABASEFUNC__H */

