
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctosapi.h>
#include "FeliCa/Util.h"
#include "Debug/debug.h"
#include "patrick-lib.h"
#include "Includes/CTOSInput.h"

#include "Filemodule/myFileFunc.h"

int gblinGPRSFallback = 0;


USHORT CTOS_LCDTSetReverse(BOOL boReverse)
{
    return d_OK;
    BYTE szMultipleMsgStr[512];

    memset(szMultipleMsgStr, 0x00, sizeof(szMultipleMsgStr));
    strcpy(szMultipleMsgStr, "|");

    strcat(szMultipleMsgStr, "test11111");
    strcat(szMultipleMsgStr, "|");

    vdDebug_LogPrintf("CTOS_LCDTSetReverse.......");
    inCallJAVA_DisplayMultipleMessage(szMultipleMsgStr);
}

USHORT CTOS_LCDTPrintAligned1(USHORT usY, UCHAR* pbBuf, BYTE bMode)
{

    BYTE szMultipleMsgStr[512];

    memset(szMultipleMsgStr, 0x00, sizeof(szMultipleMsgStr));
    strcpy(szMultipleMsgStr, "|");

    strcat(szMultipleMsgStr, "test11111");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "test22222");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "test33333");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "test44444");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "test55555");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "test66666");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "test77777");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "test88888");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "test99999");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "test00000");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "testaaaaa");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "testbbbbb");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "testccccc");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "testddddd");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "testeeeee");
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, "testfffff");
    strcat(szMultipleMsgStr, "|");
    vdDebug_LogPrintf("CTOS_LCDTPrintAligned.....");
    inCallJAVA_DisplayMultipleMessage(szMultipleMsgStr);
}

/*
USHORT CTOS_LCDTPrintAligned(USHORT usY, UCHAR* pbBuf, BYTE bMode)
{

    BYTE szMultipleMsgStr[512];
    BYTE szLine[12];
    BYTE szMode[12];

    memset(szMultipleMsgStr, 0x00, sizeof(szMultipleMsgStr));
    strcpy(szMultipleMsgStr, "|");

    sprintf(szLine, "%d", usY);
    sprintf(szMode, "%d", bMode);


    strcat(szMultipleMsgStr, szLine);
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, pbBuf);
    strcat(szMultipleMsgStr, "|");
    strcat(szMultipleMsgStr, szMode);
    strcat(szMultipleMsgStr, "|");
    vdDebug_LogPrintf("CTOS_LCDTPrintAligned.....[%s]", szMultipleMsgStr);

    vdDebug_LogPrintf("CTOS_LCDTPrintAligned.....");
    inCallJAVA_LCDTPrintAligned(szMultipleMsgStr);
}
*/

USHORT CTOS_LCDSelectModeEx(BYTE bMode,BOOL fClear)
{
}

#if 0
USHORT CTOS_LCDGShowBMPPic(USHORT usX, USHORT usY, BYTE *baFilename)
{

	return 0;
	
	vdDebug_LogPrintf("CTOS_LCDGShowBMPPic Start");

	inCallJAVA_DisplayImage(usX, usY,baFilename);

	vdDebug_LogPrintf("CTOS_LCDGShowBMPPic End");

}
#endif

USHORT CTOS_LCDBackGndColor(ULONG ulColor){

}

USHORT CTOS_LCDForeGndColor(ULONG ulColor){

}

USHORT CTOS_LCDFontSelectMode(BYTE bMode){

}

USHORT CTOS_LCDGShowPic(USHORT usX, USHORT usY, BYTE* baPat, ULONG ulPatLen, USHORT usXSize){

}

USHORT CTOS_LCDTClear2EOL(void){

}

USHORT CTOS_LCDTGotoXY(USHORT usX,USHORT usY){

}

USHORT CTOS_LCDTPutchXY (USHORT usX, USHORT usY, UCHAR bChar){

}

USHORT CTOS_LCDTPrint(UCHAR* sBuf){

}

USHORT CTOS_LCDSetContrast(BYTE bValue){

}

USHORT CTOS_LCDTSelectFontSize(USHORT usFontSize){

}

USHORT CTOS_LCDTTFSwichDisplayMode(USHORT usMode){

}

USHORT CTOS_LCDTTFSelect(BYTE *baFilename, BYTE bIndex){

}

BYTE InputAmountEx(USHORT usX, USHORT usY, BYTE *szCurSymbol, BYTE exponent, BYTE first_key, BYTE *baAmount, ULONG *ulAmount, USHORT usTimeOutMS, BYTE bIgnoreEnter)
{
    vdDebug_LogPrintf("=====InputAmountEx=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[512];
    BYTE szOutBuf[512];

    vdDebug_LogPrintf("InputAmountEx");
    /*Still use the old way make it work first*/
    memset(szInBuf, 0x00, sizeof(szInBuf));
    memset(szOutBuf, 0x00, sizeof(szOutBuf));

    strcpy(szInBuf, szCurSymbol);

       inRet = inCallJAVA_GetAmountString(szInBuf, szOutBuf, &byLen);

    //inCallJAVA_EnterAnyNum(&byLen, szOutBuf);
    //Tine --19Mar2019, to fix app crashing upon CANCEL from GetAmountString
    if (0 == strcmp(szOutBuf, "TO"))
        return 0xFF;
    else if (0 == strcmp(szOutBuf, "CANCEL")) {
        vdDebug_LogPrintf("=====Cancel InputAmountEx=====");
        *ulAmount = 0;
        return d_USER_CANCEL;
    }
    else if (0 == strcmp(szOutBuf, "TIME OUT")) {
        vdDebug_LogPrintf("=====TIME OUT InputAmountEx=====");
        *ulAmount = 0;
        return 0xFF;
    }
    else {
        memset(baAmount, 0x00, sizeof(baAmount));
        strcpy(baAmount, szOutBuf);
        vdDebug_LogPrintf("baAmount[%s] byLen[%d]", baAmount, byLen);
    }

    if (strlen(baAmount)>0 && byLen>0) {
        *ulAmount = atol(baAmount);
        vdDebug_LogPrintf("ulAmount[%ul] byLen[%d]", ulAmount, byLen);
    }
    else
        *ulAmount = 0;


    vdDebug_LogPrintf("=====End InputAmountEx=====");
    return d_OK;

}



USHORT usCTOSS_Confirm(BYTE *szDispString)
{
    vdDebug_LogPrintf("=====usCTOSS_ConfirmMenu=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[512];
    BYTE szOutBuf[512];

	
	vdDebug_LogPrintf("saturn usCTOSS_Confirm");

    inRet = inCallJAVA_UserConfirmMenu(szDispString, szOutBuf, &byLen);
	vdDebug_LogPrintf("saturn =====End usCTOSS_ConfirmMenu  szOutBuf[%s]=====", szOutBuf);

    vdDebug_LogPrintf("saturn =====End usCTOSS_ConfirmMenu=====");

    if (0 == strcmp(szOutBuf, "TO"))
        return d_NO;
    else if (0 == strcmp(szOutBuf, "CANCEL"))
        return d_USER_CANCEL;
	else if (0 == strcmp(szOutBuf, "TIME OUT"))
        return d_TIMEOUT;
    else
        return d_OK;
}

USHORT usCTOSS_Confirm3(BYTE *szDispString)
{
    vdDebug_LogPrintf("=====usCTOSS_ConfirmMenu=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[512];
    BYTE szOutBuf[512];

	
	vdDebug_LogPrintf("saturn usCTOSS_Confirm");

    inRet = inCallJAVA_UserConfirmMenu3(szDispString, szOutBuf, &byLen);
	vdDebug_LogPrintf("saturn =====End usCTOSS_ConfirmMenu  szOutBuf[%s]=====", szOutBuf);

    vdDebug_LogPrintf("saturn =====End usCTOSS_ConfirmMenu=====");

    if (0 == strcmp(szOutBuf, "TO"))
        return d_NO;
    else if (0 == strcmp(szOutBuf, "CANCEL"))
        return d_USER_CANCEL;
	else if (0 == strcmp(szOutBuf, "TIME OUT"))
        return d_TIMEOUT;
    else
        return d_OK;
}


USHORT usCTOSS_UpDown(BYTE *szDispString)
{
    vdDebug_LogPrintf("=====usCTOSS_UpDown=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[512];
    BYTE szOutBuf[512];

	

    inRet = inCallJAVA_UserSelectUpDown(szDispString, szOutBuf, &byLen);
	vdDebug_LogPrintf("=====End inCallJAVA_UserSelectUpDown  szOutBuf[%s]=====", szOutBuf);

    vdDebug_LogPrintf("=====End inCallJAVA_UserSelectUpDown=====");

    if (0 == strcmp(szOutBuf, "EXIT"))
        return d_KBD_CANCEL;
    else if (0 == strcmp(szOutBuf, "UP"))
        return d_KBD_UP;
    else if(0 == strcmp(szOutBuf, "DOWN"))
        return d_KBD_DOWN;
}


BYTE InputString(USHORT usX, USHORT usY, BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS)
{
    vdDebug_LogPrintf("=====InputString=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[512];
    BYTE szOutBuf[512];

	memset(szInBuf, 0x00, sizeof(szInBuf));

    if (0x01 == bInputMode)
        strcpy(szInBuf, "1");
    else if (0x02 == bInputMode)
        strcpy(szInBuf, "2");
	else if(0x03 == bInputMode)
		strcpy(szInBuf, "3");
	else 
		strcpy(szInBuf, "0");

	
    vdDebug_LogPrintf("=====InputString=====szInBuf[%s]", szInBuf);

    /*Still use the old way make it work first*/
    inRet = inCallJAVA_UserInputString(szInBuf, szOutBuf, &byLen);
    strcpy(pbaStr, szOutBuf);
    *usStrLen = byLen;

    vdDebug_LogPrintf("pbaStr[%s] *usStrLen[%d]", pbaStr, *usStrLen);
    vdDebug_LogPrintf("=====End InputString=====");

    if (0 == strcmp(szOutBuf, "TO"))
        return 0xFF;
    else if (0 == strcmp(szOutBuf, "CANCEL"))
        return 'C';
    else
        return 'A';
}

BYTE InputStringAlpha(USHORT usX, USHORT usY, BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS)
{
    vdDebug_LogPrintf("=====InputStringAlpha=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[128];
    BYTE szOutBuf[128];

    strcpy(szInBuf, "1");

    /*Still use the old way make it work first*/
    inRet = inCallJAVA_UserInputString(szInBuf, szOutBuf, &byLen);
    strcpy(pbaStr, szOutBuf);
    *usStrLen = byLen;

    vdDebug_LogPrintf("pbaStr[%s] *usStrLen[%d]", pbaStr, *usStrLen);
    vdDebug_LogPrintf("=====End InputStringAlpha=====");

    if (0 == strcmp(szOutBuf, "TO"))
        return 0xFF;
    else if (0 == strcmp(szOutBuf, "CANCEL"))
        return 'C';
    else
        return 'A';

    return d_OK;
}

BYTE InputStringAlphaEx(USHORT usX, USHORT usY, BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS)
{
    vdDebug_LogPrintf("=====InputStringAlphaEx=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[128];
    BYTE szOutBuf[128];

    strcpy(szInBuf, "1");

    /*Still use the old way make it work first*/
    inRet = inCallJAVA_UserInputString(szInBuf, szOutBuf, &byLen);
    strcpy(pbaStr, szOutBuf);
    *usStrLen = byLen;

    vdDebug_LogPrintf("pbaStr[%s] *usStrLen[%d]", pbaStr, *usStrLen);
    vdDebug_LogPrintf("=====End InputStringAlphaEx=====");

    if (0 == strcmp(szOutBuf, "TO"))
        return 0xFF;
    else if (0 == strcmp(szOutBuf, "CANCEL"))
        return 'C';
    else
        return 'A';

    return d_OK;
}

BYTE InputStringAlphaEx2(USHORT usX, USHORT usY, BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS)
{
    vdDebug_LogPrintf("=====InputStringAlphaEx2=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[128];
    BYTE szOutBuf[128];

    strcpy(szInBuf, "1");

    /*Still use the old way make it work first*/
    inRet = inCallJAVA_UserInputString(szInBuf, szOutBuf, &byLen);
    strcpy(pbaStr, szOutBuf);
    *usStrLen = byLen;

    vdDebug_LogPrintf("pbaStr[%s] *usStrLen[%d]", pbaStr, *usStrLen);
    vdDebug_LogPrintf("=====End InputStringAlphaEx2=====");

    if (0 == strcmp(szOutBuf, "TO"))
        return 0xFF;
    else if (0 == strcmp(szOutBuf, "CANCEL"))
        return 'C';
    else
        return 'A';

    return d_OK;
}

USHORT shCTOS_GetNum(IN  USHORT usY, IN  USHORT usLeftRight, OUT BYTE *baBuf, OUT  USHORT *usStrLen, USHORT usMinLen, USHORT usMaxLen, USHORT usByPassAllow, USHORT usTimeOutMS)
{
    vdDebug_LogPrintf("=====shCTOS_GetNum=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[128];
    BYTE szOutBuf[128];

    strcpy(szInBuf, "1");

    /*Still use the old way make it work first*/
    inRet = inCallJAVA_UserInputString(szInBuf, szOutBuf, &byLen);
    strcpy(baBuf, szOutBuf);
    *usStrLen = byLen;

    vdDebug_LogPrintf("baBuf[%s] *usStrLen[%d]", baBuf, *usStrLen);
    vdDebug_LogPrintf("=====End shCTOS_GetNum=====");

    return d_OK;
}

BYTE MenuDisplay(BYTE *sHeaderString, BYTE iHeaderStrLen, BYTE bHeaderAttr, BYTE iCol, BYTE center_x, BYTE *sItemsString, BYTE isReCount)
{
    vdDebug_LogPrintf("saturn =====MenuDisplay=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE bySelect = 0;

    BYTE szInBuf[1024];
    BYTE szOutBuf[1024];


    if (strlen(sHeaderString)>0)
    {
        strcpy(szInBuf, sHeaderString);
    }
    else
    {
        strcpy(szInBuf, "");
    }

    strcat(szInBuf, "|");

    /*After title string are menu items*/
    strcat(szInBuf, sItemsString);

    /*Still use the old way make it work first*/
    //inRet = inCallJAVA_DOptionMenuDisplay(szInBuf, szOutBuf, &byLen);
    inRet = inCallJAVA_DPopupMenuDisplay(szInBuf, szOutBuf, &byLen);
    //strcpy(baBuf, szOutBuf);
    //*usStrLen = byLen;
    bySelect = atoi(szOutBuf);

    vdDebug_LogPrintf("=====End MenuDisplay=====");

    if (0 == strcmp(szOutBuf, "TO"))
        return 0xFF;
    else if (0 == strcmp(szOutBuf, "CANCEL"))
        return 'C';

    return bySelect;
}

USHORT usCTOSS_DisplayUI(BYTE *szDispString)
{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[512];

	vdDebug_LogPrintf("saturn =====usCTOSS_DisplayUI=====");

    inRet = inCallJAVA_DisplayUI(szDispString, szOutBuf, &byLen);
	
	vdDebug_LogPrintf("saturn =====End usCTOSS_DisplayUI  szOutBuf[%s]=====", szOutBuf);
   
    return d_OK;
}



USHORT usCTOSS_BackToProgress(BYTE *szDispString)
{
  
    BYTE byLen = 0;
    int inRet = 0;
	
    BYTE szOutBuf[512];

	vdDebug_LogPrintf("=====usCTOSS_BackToProgress=====");
    inRet = inCallJAVA_BackToProgress(szDispString, szOutBuf, &byLen);
	vdDebug_LogPrintf("=====End usCTOSS_BackToProgress  szOutBuf[%s]=====", szOutBuf);

    return d_OK;
}

BYTE InputStringUI(BYTE bInputMode,  BYTE bShowAttr, BYTE *pbaStr, USHORT *usStrLen, USHORT usMinLen, USHORT usTimeOutMS, BYTE *password, BYTE *display) {
    vdDebug_LogPrintf("=====S1InputString=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[512];
    BYTE szOutBuf[512];
    vdDebug_LogPrintf("saturn test =====S1InputString===== %s %s", password, display);

    memset(szInBuf, 0x00, sizeof(szInBuf));
    memset(szOutBuf, 0x00, sizeof(szOutBuf));
	
    if (0x01 == bInputMode){
        strcpy(szInBuf, "1");
        strcat(szInBuf, "|");
        strcat(szInBuf, "1"); /*min len*/
        strcat(szInBuf, "|");
        strcat(szInBuf, "6"); /*max len*/
        strcat(szInBuf, "|");
        strcat(szInBuf, display);

    }
    else if (0x02 == bInputMode) {
        strcpy(szInBuf, "2");
        strcat(szInBuf, "|");
		strcat(szInBuf, "4"); /*min len*/
		strcat(szInBuf, "|");
		strcat(szInBuf, "6"); /*max len*/
		strcat(szInBuf, "|");
        strcat(szInBuf, display);
		strcat(szInBuf, "|");
		strcat(szInBuf, password);
    }
    else if(0x03 == bInputMode)
        strcpy(szInBuf, "3");
    else
        strcpy(szInBuf, "0");


    vdDebug_LogPrintf("saturn =====InputStringUI=====szInBuf[%s]", szInBuf);


    /*Still use the old way make it work first*/
    inRet = inCallJAVA_S1InputString(szInBuf, szOutBuf, &byLen);
    strcpy(pbaStr, szOutBuf);
    *usStrLen = byLen;

    vdDebug_LogPrintf("saturn pbaStr[%s] *usStrLen[%d]", pbaStr, *usStrLen);
    vdDebug_LogPrintf("saturn =====End InputStringUI=====");

    if (0 == strcmp(szOutBuf, "TO"))
        return 0xFF;
    else if (0 == strcmp(szOutBuf, "CANCEL"))
    {
		vdDebug_LogPrintf("=====Cancel InputStringUI=====");
			return 'C';
    }
		else
		{
			vdDebug_LogPrintf("=====OK InputStringUI=====");
			return 'A';
		}
}

USHORT usCTOSS_ConfirmInvAndAmt(BYTE *szDispString)
{
    vdDebug_LogPrintf("=====usCTOSS_ConfirmInvAndAmt=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[512];
    BYTE szOutBuf[512];

	
	
    inRet = inCallJAVA_UserConfirmMenuInvandAmt(szDispString, szOutBuf, &byLen);
	vdDebug_LogPrintf("=====End usCTOSS_ConfirmInvAndAmt  szOutBuf[%s]=====", szOutBuf);

    vdDebug_LogPrintf("=====End usCTOSS_ConfirmInvAndAmt=====");

    if (0 == strcmp(szOutBuf, "TO"))
        return d_TIMER_INVALID_PARA;
    else if (0 == strcmp(szOutBuf, "CANCEL"))
        return d_NO;
    else
        return d_OK;
}


USHORT usCTOSS_GetPinUI(void)
{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[512];
    BYTE szDispString[5]="TEST";

    //vdDebug_LogPrintf("saturn ====usCTOSS_GetPinUI=====");
    inRet = inCallJAVA_GetPinUI(szDispString, szOutBuf, &byLen);
   //vdDebug_LogPrintf("=====End usCTOSS_GetPinUI  szOutBuf[%s]=====", szOutBuf);

    return d_OK;
}
USHORT usCTOSS_LCDDisplay(BYTE *szStringMsg)
{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[512];
    memset(szOutBuf, 0x00, sizeof(szOutBuf));

    vdDebug_LogPrintf("=====usCTOSS_LCDDisplay=====");

    inRet = inCallJAVA_LCDDisplay(szStringMsg, szOutBuf, &byLen);

    vdDebug_LogPrintf("=====End usCTOSS_LCDDisplay  szOutBuf[%s]=====", szOutBuf);

    return d_OK;
}

USHORT usCTOSS_BatchReviewUI(BYTE *szDispString)
{
    vdDebug_LogPrintf("=====usCTOSS_BatchReviewUI=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[512];
    BYTE szOutBuf[512];

	

    inRet = inCallJAVA_BatchReviewUI(szDispString, szOutBuf, &byLen);
	vdDebug_LogPrintf("=====End usCTOSS_BatchReviewUI  szOutBuf[%s]=====", szOutBuf);

    vdDebug_LogPrintf("=====End usCTOSS_BatchReviewUI=====");

    if (0 == strcmp(szOutBuf, "NEXT"))
        return d_KBD_DOWN;
    if (0 == strcmp(szOutBuf, "PREVIOUS"))
        return d_KBD_UP;
    else if (0 == strcmp(szOutBuf, "CANCEL"))
        return d_KBD_CANCEL;
	else if (0 == strcmp(szOutBuf, "TIME OUT"))
		return 0xFF;
    else
        return d_OK;
}

USHORT usCTOSS_ConfirmOK(BYTE *szDispString)
{
    vdDebug_LogPrintf("=====usCTOSS_ConfirmMenu=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[512];
    BYTE szOutBuf[512];

	

    inRet = inCallJAVA_UserConfirmOKMenu(szDispString, szOutBuf, &byLen);
	vdDebug_LogPrintf("=====End usCTOSS_ConfirmOKMenu  szOutBuf[%s]=====", szOutBuf);

    vdDebug_LogPrintf("=====End usCTOSS_ConfirmOKMenu=====");

    if (0 == strcmp(szOutBuf, "TO"))
        return d_NO;
	else if (0 == strcmp(szOutBuf, "TIME OUT"))
		return 0xFF;
    else
        return d_OK;
}


USHORT usGetRootFS(BYTE baRootFS[20])

{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[30];

    vdDebug_LogPrintf("saturn =====usGetSerialNumber=====");


	//inRet = inCallJAVA_usGetSerialNumber(szStringMsg, szOutBuf, &byLen);
	inRet = inCallJAVA_usGetRootFS(szOutBuf, &byLen);

    vdDebug_LogPrintf("saturn =====End usGetRootFS  szOutBuf[%s]=====", szOutBuf);

    strcpy(baRootFS, szOutBuf);
    return d_OK;
}

USHORT PrintReceiptUI(BYTE *szStringMsg)
{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[512];

    vdDebug_LogPrintf("=====PrintReceiptUI=====");
    memset(szOutBuf,0x00,sizeof(szOutBuf));

    inRet = inCallJAVA_PrintReceiptUI(szStringMsg, szOutBuf, &byLen);

    vdDebug_LogPrintf("=====End PrintReceiptUI  szOutBuf[%s]=====", szOutBuf);

    //return d_OK;
    if (0 == strcmp(szOutBuf, "TIME OUT"))
        return  0xFF;
    else if (0 == strcmp(szOutBuf, "CANCEL")) {
        vdDebug_LogPrintf("=====PrintReceiptUI - CANCEL =====");
        return d_USER_CANCEL;
    }
    else
    {
        vdDebug_LogPrintf("=====PrintReceiptUI - OK =====");
        return d_OK;
    }

}

USHORT EliteReceiptUI(BYTE *szStringMsg)
{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[512];

    vdDebug_LogPrintf("=====EliteReceiptUI=====");
    memset(szOutBuf,0x00,sizeof(szOutBuf));

    inRet = inCallJAVA_EliteReceiptUI(szStringMsg, szOutBuf, &byLen);

    vdDebug_LogPrintf("=====End EliteReceiptUI  szOutBuf[%s]=====", szOutBuf);

    //return d_OK;
    if (0 == strcmp(szOutBuf, "TIME OUT"))
        return  0xFF;
    else
    {
        vdDebug_LogPrintf("=====EliteReceiptUI - OK =====");
        return d_OK;
    }

}


USHORT ConfirmCardDetails(BYTE *szConfirmCardDetails) {
    vdDebug_LogPrintf("=====ConfirmCardDetails=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[1024];
    BYTE szOutBuf[512];

    memset(szInBuf, 0x00, sizeof(szInBuf));
    memset(szOutBuf, 0x00, sizeof(szOutBuf));
    strcpy(szInBuf, szConfirmCardDetails);

    vdDebug_LogPrintf("=====Start ConfirmCardDetails  szInBuf[%s]=====", szInBuf);
    inRet = inCallJAVA_UserConfirmCard(szInBuf, szOutBuf, &byLen);
    vdDebug_LogPrintf("=====End ConfirmCardDetails  szOutBuf[%s]=====", szOutBuf);

    vdDebug_LogPrintf("=====End ConfirmCardDetails=====");

    if (0 == strcmp(szOutBuf, "TIME OUT"))
        return  0xFF;
    else if (0 == strcmp(szOutBuf, "CANCEL")) {
        vdDebug_LogPrintf("=====ConfirmCardDetails =====");
        return d_USER_CANCEL;
    }
    else
        return d_OK;
}

USHORT getAppPackageInfo(BYTE *szAppName, BYTE *szStringMsg)
{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[512];
	BYTE szInBuf[512];

    vdDebug_LogPrintf("=====getAppPackageInfo=====");

	memset(szInBuf, 0x00, sizeof(szInBuf));
    memset(szOutBuf, 0x00, sizeof(szOutBuf));
    memset(szStringMsg, 0x00, sizeof(szStringMsg));

	strcpy(szInBuf, szAppName);

    inRet = inCallJAVA_getAppPackageInfo(szInBuf, szOutBuf, &byLen);
    vdDebug_LogPrintf("=====End getAppPackageInfo  szOutBuf[%s]=====", szOutBuf);
	strcpy(szStringMsg, szOutBuf);

    return d_OK;
}

USHORT usCTOSS_DisplayBox(BYTE *szStringMsg)
{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[512];

    vdDebug_LogPrintf("=====usCTOSS_DisplayBox=====");

    inRet = inCallJAVA_DisplayBox(szStringMsg, szOutBuf, &byLen);

    vdDebug_LogPrintf("=====End usCTOSS_DisplayBox  szOutBuf[%s]=====", szOutBuf);

    return d_OK;
}


USHORT usCTOSS_DisplayErrorMsg2(BYTE *szStringMsg)
{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[512];

    vdDebug_LogPrintf("=====usCTOSS_DisplayErrorMsg2=====");

    inRet = inCallJAVA_DisplayErrorMsg2(szStringMsg, szOutBuf, &byLen);

    vdDebug_LogPrintf("=====End usCTOSS_DisplayErrorMsg2  szOutBuf[%s]=====", szOutBuf);

    return d_OK;
}

#if 1
BOOL usGetConnectionStatus(int inCommType)
{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[50];
	BYTE szStringMsg[3];

	memset(szStringMsg, 0x00, sizeof(szStringMsg));

	if (inCommType == 4)
		strcpy(szStringMsg, "4");
	else if (inCommType == 2)
		strcpy(szStringMsg, "2");
		
    vdDebug_LogPrintf("saturn =====usGetConnectionStatus=====");

	inRet = inCallJAVA_usGetConnectionStatus(szStringMsg, szOutBuf, &byLen);
	

    vdDebug_LogPrintf("saturn =====End usGetConnectionStatus  szOutBuf[%s]=====", szOutBuf);

	if (strcmp(szOutBuf, "YES") == 0)
		return TRUE;
	else
         {
               if ((inCommType == 4) && (strcmp(szOutBuf, "FALLBACK") == 0) && (strTCT.inGPRSFallback==1))
               {
                    gblinGPRSFallback = 1;
                    return TRUE;
               }
			   
               return FALSE;
         }

}

#else
BOOL usGetConnectionStatus(int inCommType)
{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[50];
	BYTE szStringMsg[3];

	memset(szStringMsg, 0x00, sizeof(szStringMsg));

	if (inCommType == 4)
		strcpy(szStringMsg, "4");
	else if (inCommType == 2)
		strcpy(szStringMsg, "2");
		
    vdDebug_LogPrintf("saturn =====usGetConnectionStatus=====");

	inRet = inCallJAVA_usGetConnectionStatus(szStringMsg, szOutBuf, &byLen);
	

    vdDebug_LogPrintf("saturn =====End usGetConnectionStatus  szOutBuf[%s]=====", szOutBuf);

	if (strcmp(szOutBuf, "YES") == 0)
		return TRUE;
	else
		return FALSE;

}
#endif

USHORT usCTOSS_Confirm2(BYTE *szDispString)
{
    vdDebug_LogPrintf("=====usCTOSS_ConfirmMenu=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[512];
    BYTE szOutBuf[512];

	

    inRet = inCallJAVA_UserConfirmMenu2(szDispString, szOutBuf, &byLen);
	vdDebug_LogPrintf("saturn =====End usCTOSS_ConfirmMenu2  szOutBuf[%s]=====", szOutBuf);

    vdDebug_LogPrintf("saturn =====End usCTOSS_ConfirmMenu=====");

    if (0 == strcmp(szOutBuf, "TIME OUT"))
        //return d_NO;
        return d_TIMEOUT;
    else if (0 == strcmp(szOutBuf, "CANCEL"))
        return d_USER_CANCEL;
    else
        return d_OK;
}

USHORT usGetSerialNumber(BYTE baFactorySN[16])

{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[30];

    vdDebug_LogPrintf("saturn =====usGetSerialNumber=====");


	//inRet = inCallJAVA_usGetSerialNumber(szStringMsg, szOutBuf, &byLen);
	inRet = inCallJAVA_usGetSerialNumber(szOutBuf, &byLen);

    vdDebug_LogPrintf("saturn =====End usGetSerialNumber  szOutBuf[%s]=====", szOutBuf);

    strcpy(baFactorySN, szOutBuf);
    return d_OK;
}

USHORT usCTOSS_DisplayStatusBox(BYTE *szStringMsg)
{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf[512];

    vdDebug_LogPrintf("=====usCTOSS_DisplayStatusBox=====");

    inRet = inCallJAVA_DisplayStatusBox(szStringMsg, szOutBuf, &byLen);

    vdDebug_LogPrintf("=====End usCTOSS_DisplayStatusBox  szOutBuf[%s]=====", szOutBuf);

    return d_OK;
}

USHORT usSetDateTime(BYTE *szDateTime)
{

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szOutBuf_setDateTime[512];
	memset(szOutBuf_setDateTime, 0x00, sizeof(szOutBuf_setDateTime));

    vdDebug_LogPrintf("saturn =====usSetDateTime=====");

    inRet = inCallJAVA_usSetDateTime(szDateTime, szOutBuf_setDateTime, &byLen);

    //vdDebug_LogPrintf("saturn ====End usCARDENTRY  szOutBuf[%s]=====", szOutBuf);

    //return d_OK;
    return d_OK;
}

USHORT usCTOSS_ConfirmationUI(BYTE *szDispString)
{
    vdDebug_LogPrintf("=====usCTOSS_ConfirmationUI=====");

    BYTE byLen = 0;
    int inRet = 0;

    BYTE szInBuf[512];
    BYTE szOutBuf[512];

	
	vdDebug_LogPrintf("saturn usCTOSS_ConfirmationUI");

    inRet = inCallJAVA_UserConfirmationUI(szDispString, szOutBuf, &byLen);
	vdDebug_LogPrintf("saturn =====End usCTOSS_ConfirmationUI  szOutBuf[%s]=====", szOutBuf);

    vdDebug_LogPrintf("saturn =====End usCTOSS_ConfirmationUI=====");

    if (0 == strcmp(szOutBuf, "TO"))
        return d_NO;
    else if (0 == strcmp(szOutBuf, "CANCEL"))
        return d_USER_CANCEL;
	else if (0 == strcmp(szOutBuf, "TIME OUT"))
        return d_TIMEOUT;
    else
        return d_OK;
}

