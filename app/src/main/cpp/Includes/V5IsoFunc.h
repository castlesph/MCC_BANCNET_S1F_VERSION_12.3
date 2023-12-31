/* 
 * File:   V5IsoFunc.h
 * Author: Administrator
 *
 * Created on 2012年8月6日, 下午 9:23
 */

#ifndef V5ISOFUNC_H
#define    V5ISOFUNC_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../Includes/POSTypedef.h"
#include "../Includes/ISOEnginee.h"


#define BIT_MAP_MAX_ARRAY    	30

#define BIT_MAP_SIZE        	8

#define ISO_SEND_SIZE       	2048
#define ISO_REC_SIZE        	2048


void vdDispTextMsg(char *szTempMsg);
void vdDecideWhetherConnection(TRANS_DATA_TABLE *srTransPara);
int inBuildAndSendIsoData(void);
int inSnedReversalToHost(TRANS_DATA_TABLE *srTransPara, int inTransCode);
int inSaveReversalFile(TRANS_DATA_TABLE *srTransPara, int inTransCode);
int inProcessReversal(TRANS_DATA_TABLE *srTransPara);
int inProcessAdviceTrans(TRANS_DATA_TABLE *srTransPara, int inAdvCnt);
int inProcessEMVTCUpload(TRANS_DATA_TABLE *srTransPara, int inAdvCnt);
int inPackSendAndUnPackData(TRANS_DATA_TABLE *srTransPara, int inTransCode);
int inBuildOnlineMsg(TRANS_DATA_TABLE *srTransPara);
int inSetBitMapCode(TRANS_DATA_TABLE *srTransPara, int inTransCode);
int inPackMessageIdData(int inTransCode, unsigned char *uszPackData, char *szMTI);
int inPackPCodeData(int inTransCode, unsigned char *uszPackData, char *szPCode);
void vdModifyBitMapFunc(int inTransCode, int *inBitMap);
int inCheckIsoHeaderData(char *szSendISOHeader, char *szReceISOHeader);
int inSendAndReceiveFormComm(TRANS_DATA_TABLE* srTransPara,
                                unsigned char* uszSendData,
                                 int inSendLen,
                                unsigned char* uszReceData);
int inProcessOfflineTrans(TRANS_DATA_TABLE *srTransPara);
int inAnalyseIsoData(TRANS_DATA_TABLE *srTransPara);

int inCheckHostRespCode(TRANS_DATA_TABLE *srTransPara);
int inCheckTransAuthCode(TRANS_DATA_TABLE *srTransPara);

int inAnalyseChipData(TRANS_DATA_TABLE *srTransPara);
int inAnalyseNonChipData(TRANS_DATA_TABLE *srTransPara);

int inAnalyseReceiveData(void);
int inAnalyseAdviceData(int inPackType);
void vdDiapTransFormatDebug(unsigned char *bSendBuf, int inSendLen);
int inPorcessTransUpLoad(TRANS_DATA_TABLE *srTransPara);

int inPackIsoFunc02(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc03(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc04(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc05(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc07(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc11(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc12(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc13(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc14(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc22(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc23(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc24(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc25(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc28(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc35(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc37(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc38(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc39(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc41(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc42(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc43(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc45(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc48(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc50(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc52(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc54(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc55(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc56(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc57(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc60(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc61(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoInstallment61(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc62(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc63(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc64(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);
int inPackIsoFunc84(TRANS_DATA_TABLE *srTransPara, unsigned char* uszSendData);

int inUnPackIsoFunc02(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc04(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc05(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc11(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc07(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc12(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc13(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc28(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc37(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc38(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc39(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc41(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc42(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc43(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc50(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc53(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc54(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc55(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc57(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoFunc61(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoInstallment61(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inUnPackIsoUnknown(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inPackISOEMVData(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inPackISOPayWaveData(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inPackISOPayPassData(TRANS_DATA_TABLE *srTransPara, unsigned char *uszUnPackBuf);
int inCTOS_PackDemoResonse(TRANS_DATA_TABLE *srTransPara,unsigned char *uszRecData);

void vdInitialISOFunction(ISO_FUNC_TABLE *srPackFunc);
int inBaseRespValidation(TRANS_DATA_TABLE *srOrgTransPara,TRANS_DATA_TABLE *srTransPara);
int inProcessAdviceTCTrail(TRANS_DATA_TABLE *srTransPara);
int inProcessLogon(void);
int inCheckTCExist(void);
int inPacktTerminalCapability(TRANS_DATA_TABLE *srTransPara);
void vdIncSTAN(TRANS_DATA_TABLE *srTransPara);
void vdGetTimeDate(TRANS_DATA_TABLE *srTransPara);


#ifdef    __cplusplus
}
#endif

#endif    /* V5ISOFUNC_H */

