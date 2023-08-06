/*******************************************************************************

*******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctosapi.h>

#include "../Includes/myEZLib.h"
#include "../Includes/msg.h"
#include "../Includes/wub_lib.h"


#include "..\Includes\CTOSInput.h"
#include "../Includes/POSTypedef.h"
#include "../FileModule/myFileFunc.h"

#include "../print/Print.h"
#include "../FileModule/myFileFunc.h"
#include "../Comm/V5Comm.h"


#include "..\Includes\CTOSInput.h"


#include "../Accum/accum.h"
#include "../DataBase/DataBaseFunc.h"
#include "..\Includes\POSSetting.h"
#include "..\ui\Display.h"
#include "..\Database\DatabaseFunc.h"
#include "..\debug\debug.h"
#include "..\Includes\POSTrans.h"
#include "..\Accum\Accum.h"
#include "..\filemodule\myFileFunc.h"
#include "..\Includes\MultiApLib.h"
#include "..\Aptrans\MultiAptrans.h"
#include "..\Aptrans\MultiShareEMV.h"
#include "..\PCI100\PCI100.h"


#define DISPLAY_POSITION_LEFT 0
#define DISPLAY_POSITION_CENTER 1
#define DISPLAY_POSITION_RIGHT 2
#define DISPLAY_LINE_SIZE 16


BOOL BolDetachGPRSChangeSetting=FALSE, BolDetachDIALChangeSetting=FALSE;

extern int inEthernet_GetTerminalIP(char *szTerminalIP);

void vdCTOS_uiPowerOff(void)
{
    BYTE block[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    USHORT ya,yb,xa,xb;
    unsigned char c;
        
    CTOS_LCDTClearDisplay();

    vduiDisplayStringCenter(3,"ARE YOU SURE");
    vduiDisplayStringCenter(4,"WANT TO POWER");
    vduiDisplayStringCenter(5,"OFF TERMINAL");
    CTOS_LCDTPrintXY(1,7,"NO[X]   YES[OK] ");
    c=WaitKey(60);
    
    if(c!=d_KBD_ENTER)
    {
        return;
    }
    
    for(ya =1; ya<5; ya++)
    {
        CTOS_Delay(100);
        CTOS_LCDTGotoXY(1,ya);
        CTOS_LCDTClear2EOL();
    }
    for(yb=8; yb>4; yb--)
    {
        CTOS_Delay(100);
        CTOS_LCDTGotoXY(1,yb);
        CTOS_LCDTClear2EOL();
    }
    CTOS_LCDTPrintXY(1,4,"----------------");
    for(xa=1; xa<8; xa++)
    {
        CTOS_Delay(25);
        CTOS_LCDTPrintXY(xa,4," ");
    }
    for(xb=16; xb>7; xb--)
    {
        CTOS_Delay(25);
        CTOS_LCDTPrintXY(xb,4," ");
    }
            
    CTOS_LCDGShowPic(58, 6, block, 0, 6);
    CTOS_Delay(250);
    CTOS_LCDTGotoXY(7,4);
    CTOS_LCDTClear2EOL();
    CTOS_Delay(250);

    //CTOS_PowerOff();
    CTOS_SystemReset();
}

void vdCTOS_uiRestart(void)
{
    BYTE block[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    USHORT ya,yb,xa,xb;
    unsigned char c;
    int inResult;
	      
    inResult=vduiAskConfirmation("RESTART?");
    if(inResult == d_OK)
    {
		for(ya =1; ya<5; ya++)
		{
			CTOS_Delay(100);
			CTOS_LCDTGotoXY(1,ya);
			CTOS_LCDTClear2EOL();
		}
		for(yb=8; yb>4; yb--)
		{
			CTOS_Delay(100);
			CTOS_LCDTGotoXY(1,yb);
			CTOS_LCDTClear2EOL();
		}
		CTOS_LCDTPrintXY(1,4,"----------------");
		for(xa=1; xa<8; xa++)
		{
			CTOS_Delay(25);
			CTOS_LCDTPrintXY(xa,4," ");
		}
		for(xb=16; xb>7; xb--)
		{
			CTOS_Delay(25);
			CTOS_LCDTPrintXY(xb,4," ");
		}
				
		CTOS_LCDGShowPic(58, 6, block, 0, 6);
		CTOS_Delay(250);
		CTOS_LCDTGotoXY(7,4);
		CTOS_LCDTClear2EOL();
		CTOS_Delay(250);
		
		CTOS_SystemReset();		
    }
}

int inCTOS_SelectHostSetting(void)
{
    BYTE bHeaderAttr = 0x01+0x04, iCol = 1;
    BYTE  x = 1;
    BYTE key;
    char szHeaderString[50] = "SELECT HOST";
    char szHostMenu[1024];
    char szHostName[50][100];
    int inCPTID[50];
    int inLoop = 0;
    
    if (inMultiAP_CheckSubAPStatus() == d_OK)
        return d_OK;
    

    memset(szHostMenu, 0x00, sizeof(szHostMenu));
    memset(szHostName, 0x00, sizeof(szHostName));
    memset((char*)inCPTID, 0x00, sizeof(inCPTID));

    inHDTReadHostName(szHostName, inCPTID);

    for (inLoop = 0; inLoop < 50; inLoop++)
    {
        if (szHostName[inLoop][0]!= 0)
        {
            strcat((char *)szHostMenu, szHostName[inLoop]);
            if (szHostName[inLoop+1][0]!= 0)
                strcat((char *)szHostMenu, (char *)" \n");      
        }
        else
            break;
    }
        
    key = MenuDisplay(szHeaderString, strlen(szHeaderString), bHeaderAttr, iCol, x, szHostMenu, TRUE);

    if (key == 0xFF) 
    {
        CTOS_LCDTClearDisplay();
        setLCDPrint(1, DISPLAY_POSITION_CENTER, "WRONG INPUT!!!");
        vduiWarningSound();
        return -1;  
    }

    if(key > 0)
    {
        if(d_KBD_CANCEL == key)
            return -1;
        
        vdDebug_LogPrintf("key[%d] HostID[%d]", key, inCPTID[key-1]);
        srTransRec.HDTid = inCPTID[key-1];
        strHDT.inHostIndex = inCPTID[key-1];
        inHDTRead(inCPTID[key-1]);
        inCPTRead(inCPTID[key-1]);
        strcpy(srTransRec.szHostLabel, strHDT.szHostLabel);
    }
    
    return inCPTID[key-1];
}

/*configuration functions*/
void vdCTOS_IPConfig(void)
{
    BYTE bRet, key;
    BYTE szInputBuf[5];
    int inResult;
    BYTE strOut[30],strtemp[17];
    USHORT ret;
    USHORT usLen;
    BOOL BolDetachLANChange=FALSE;
    int shHostIndex = 1;

    shHostIndex = inCTOS_SelectHostSetting();
    if (shHostIndex == -1)
        return;
                
    inResult = inCPTRead(shHostIndex);
    if(inResult != d_OK)
        return;

    CTOS_LCDTClearDisplay();
    vdDispTitleString("IP SETTING");
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "Connection Type");
        if((strCPT.inCommunicationMode)== ETHERNET_MODE)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");
        if((strCPT.inCommunicationMode)== DIAL_UP_MODE)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");        
        if((strCPT.inCommunicationMode)== GPRS_MODE)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "2");
        
        CTOS_LCDTPrintXY(1, 5, "0-DIAL-UP     1-LAN");
        CTOS_LCDTPrintXY(1, 6, "2-GPRS             ");
        
   
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31 || strOut[0]==0x32)
            {
                 if(strOut[0] == 0x31)
                 {
                        strCPT.inCommunicationMode = ETHERNET_MODE;
                        BolDetachLANChange = TRUE;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strCPT.inCommunicationMode = DIAL_UP_MODE;
                        BolDetachDIALChangeSetting = TRUE;
                 }
                 if(strOut[0] == 0x32)
                 {
                        strCPT.inCommunicationMode = GPRS_MODE;
                        BolDetachGPRSChangeSetting = TRUE;
                 }
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiClearBelow(6);
                vduiDisplayStringCenter(6,"PLEASE SELECT");
                vduiDisplayStringCenter(7,"A VALID");
                vduiDisplayStringCenter(8,"CONNECTION MODE");
                CTOS_Delay(2000);       
            }
        }
    }
    inResult = inCPTSave(shHostIndex);
    inResult = inCPTRead(shHostIndex);

    
    if(((strCPT.inCommunicationMode)== ETHERNET_MODE) || ((strCPT.inCommunicationMode)== GPRS_MODE))
    {
        inResult = inTCPRead(1);
        if(inResult != ST_SUCCESS)
            return;

        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "IP Config");
            if (strTCP.fDHCPEnable == IPCONFIG_DHCP)
                setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");
            if (strTCP.fDHCPEnable == IPCONFIG_STATIC)  
                setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
            
            CTOS_LCDTPrintXY(1, 5, "0-STATIC     1-DHCP");
            CTOS_LCDTPrintXY(1, 6, "                   ");
            
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret==1)
            {
                if (strOut[0]==0x30 || strOut[0]==0x31)
                {
                    BolDetachLANChange=TRUE;
                    
                    if(strOut[0] == 0x30)  
                            strTCP.fDHCPEnable = IPCONFIG_STATIC;
                    if(strOut[0] == 0x31)
                            strTCP.fDHCPEnable = IPCONFIG_DHCP;
                    break;
                }
                else
                {
                    vduiWarningSound();
                    vduiClearBelow(6);
                    vduiDisplayStringCenter(6,"PLEASE SELECT");
                    vduiDisplayStringCenter(7,"A VALID");
                    vduiDisplayStringCenter(8,"IP Config");
                    CTOS_Delay(2000);       
                }
            }
        }               
        inResult = inTCPSave(1);
        inResult = inTCPRead(1);

        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
		
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "PRI HOST IP");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szPriTxnHostIP);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strtemp, 0x00, sizeof(strtemp));
            memset(strOut,0x00, sizeof(strOut));
            ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
            if(ret==d_KBD_ENTER)
            {
                BolDetachLANChange=TRUE;
                memcpy(strCPT.szPriTxnHostIP,strOut,strlen(strOut));
                strCPT.szPriTxnHostIP[strlen(strOut)]=0;
                vdMyEZLib_LogPrintf("new host ip %s",strCPT.szPriTxnHostIP);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                        break;
            }
        inResult = inCPTSave(shHostIndex);
        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        
        while(1)
        {   
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "PRI HOST PORT");
            memset(szInputBuf, 0x00, sizeof(szInputBuf));
            sprintf(szInputBuf, "%d", strCPT.inPriTxnHostPortNum);
            setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 5, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>=1)
            {
                BolDetachLANChange=TRUE;
                strCPT.inPriTxnHostPortNum = atoi(strOut);      
                vdMyEZLib_LogPrintf("new host port %d",strCPT.inPriTxnHostPortNum);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }
        inResult = inCPTSave(shHostIndex);

        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SEC HOST IP");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szSecTxnHostIP);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strtemp, 0x00, sizeof(strtemp));
            memset(strOut,0x00, sizeof(strOut));
            ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
            if(ret==d_KBD_ENTER)
            {
                BolDetachLANChange=TRUE;
                memcpy(strCPT.szSecTxnHostIP,strOut,strlen(strOut));
                strCPT.szSecTxnHostIP[strlen(strOut)]=0;
                vdMyEZLib_LogPrintf("new host ip %s",strCPT.szSecTxnHostIP);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                        break;
            }
        inResult = inCPTSave(shHostIndex);
        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        
        while(1)
        {   
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SEC HOST PORT");
            memset(szInputBuf, 0x00, sizeof(szInputBuf));
            sprintf(szInputBuf, "%d", strCPT.inSecTxnHostPortNum);
            setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 5, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>=1)
            {
                BolDetachLANChange=TRUE;
                strCPT.inSecTxnHostPortNum = atoi(strOut);      
                vdMyEZLib_LogPrintf("new host port %d",strCPT.inSecTxnHostPortNum);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }
        inResult = inCPTSave(shHostIndex);

        inResult = inTCPRead(1);
        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SETTLE PRI HOST IP");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szPriSettlementHostIP);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strtemp, 0x00, sizeof(strtemp));
            memset(strOut,0x00, sizeof(strOut));
            ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
            if(ret==d_KBD_ENTER)
            {
                BolDetachLANChange=TRUE;
                memcpy(strCPT.szPriSettlementHostIP,strOut,strlen(strOut));
                strCPT.szPriSettlementHostIP[strlen(strOut)]=0;
                vdMyEZLib_LogPrintf("new host ip %s",strCPT.szPriSettlementHostIP);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                        break;
            }
        inResult = inCPTSave(shHostIndex);
        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        
        while(1)
        {   
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SETTLE PRI HOST PORT");
            memset(szInputBuf, 0x00, sizeof(szInputBuf));
            sprintf(szInputBuf, "%d", strCPT.inPriSettlementHostPort);
            setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 5, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>=1)
            {
                BolDetachLANChange=TRUE;
                strCPT.inPriSettlementHostPort = atoi(strOut);      
                vdMyEZLib_LogPrintf("new host port %d",strCPT.inPriSettlementHostPort);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }
        inResult = inCPTSave(shHostIndex);


        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SETTLE SEC HOST IP");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szSecSettlementHostIP);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strtemp, 0x00, sizeof(strtemp));
            memset(strOut,0x00, sizeof(strOut));
            ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
            if(ret==d_KBD_ENTER)
            {
                BolDetachLANChange=TRUE;
                memcpy(strCPT.szSecSettlementHostIP,strOut,strlen(strOut));
                strCPT.szSecSettlementHostIP[strlen(strOut)]=0;
                vdMyEZLib_LogPrintf("new host ip %s",strCPT.szSecSettlementHostIP);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                        break;
            }
        inResult = inCPTSave(shHostIndex);
        inResult = inCPTRead(shHostIndex);
        if(inResult != ST_SUCCESS)
            return;
        
        while(1)
        {   
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SETTLE SEC HOST PORT");
            memset(szInputBuf, 0x00, sizeof(szInputBuf));
            sprintf(szInputBuf, "%d", strCPT.inSecSettlementHostPort);
            setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 5, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>=1)
            {
                BolDetachLANChange=TRUE;
                strCPT.inSecSettlementHostPort = atoi(strOut);      
                vdMyEZLib_LogPrintf("new host port %d",strCPT.inSecSettlementHostPort);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }
        inResult = inCPTSave(shHostIndex);

        inResult = inCPTRead(shHostIndex); /*add ip header*/
        if(inResult != ST_SUCCESS)
            return;
        while(1)
        {   
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "IP HEADER");
            memset(szInputBuf, 0x00, sizeof(szInputBuf));
            sprintf(szInputBuf, "%d", strCPT.inIPHeader);
            setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>=1)
            {
                strCPT.inIPHeader= atoi(strOut);      
                vdMyEZLib_LogPrintf("new host port %d",strCPT.inSecTxnHostPortNum);
                break;
            }   
            if(ret == d_KBD_CANCEL)
                break;
        }
        inResult = inCPTSave(shHostIndex);
		
        inResult = inCPTRead(shHostIndex);      
        if(strTCP.fDHCPEnable == IPCONFIG_STATIC)
        {
            while(1)
            {
                vduiClearBelow(3);
                setLCDPrint(3, DISPLAY_POSITION_LEFT, "TERMINAL IP");
                setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szTerminalIP);
                
                strcpy(strtemp,"New:") ;     
                CTOS_LCDTPrintXY(1, 7, strtemp);
                memset(strtemp, 0x00, sizeof(strtemp));
                memset(strOut,0x00, sizeof(strOut));
                ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
                if(ret==d_KBD_ENTER)
                {
                    BolDetachLANChange=TRUE;
                    memcpy(strTCP.szTerminalIP, strOut, strlen(strOut));
                    strTCP.szTerminalIP[strlen(strOut)]=0;
                    vdMyEZLib_LogPrintf("new Client ip %s",strTCP.szTerminalIP);
                    break;
                }   
                if(ret == d_KBD_CANCEL)
                    break;
            }
            inResult = inTCPSave(1);
            inResult = inTCPRead(1);    
            
            while(1)
            {
                vduiClearBelow(3);
                setLCDPrint(3, DISPLAY_POSITION_LEFT, "Sub. Mask IP");
                setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szSubNetMask);
                
                strcpy(strtemp,"New:") ;   
                CTOS_LCDTPrintXY(1, 7, strtemp);
                memset(strtemp, 0x00, sizeof(strtemp));
                memset(strOut,0x00, sizeof(strOut));
                ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
                if(ret==d_KBD_ENTER)
                {
                    BolDetachLANChange=TRUE;
                    memcpy(strTCP.szSubNetMask, strOut,strlen(strOut));
                    strTCP.szSubNetMask[strlen(strOut)]=0;
                    vdMyEZLib_LogPrintf("new Sub. Mask ip %s", strTCP.szSubNetMask);
                    break;
                }   
                if(ret == d_KBD_CANCEL)
                    break;
            }               
                inResult = inTCPSave(1);
                inResult = inTCPRead(1);                
                while(1)
                {
                    vduiClearBelow(3);
                    setLCDPrint(3, DISPLAY_POSITION_LEFT, "Gateway IP");
                    setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szGetWay);
                    
                    strcpy(strtemp,"New:") ;  
                    CTOS_LCDTPrintXY(1, 7, strtemp);
                    memset(strtemp, 0x00, sizeof(strtemp));
                    memset(strOut,0x00, sizeof(strOut));
                    ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
                    if(ret==d_KBD_ENTER)
                    {
                        BolDetachLANChange=TRUE;
                        memcpy(strTCP.szGetWay, strOut,strlen(strOut));
                        strTCP.szGetWay[strlen(strOut)]=0;
                        vdMyEZLib_LogPrintf("new Gateway ip %s", strTCP.szGetWay);
                        break;
                    }   
                    if(ret == d_KBD_CANCEL)
                        break;
                }
                inResult = inTCPSave(1);
                inResult = inTCPRead(1);
                
                while(1)
                {
                    vduiClearBelow(3);
                    setLCDPrint(3, DISPLAY_POSITION_LEFT, "DNS1 IP");
                    if (wub_strlen(strTCP.szHostDNS1)<=0)
                        setLCDPrint(4, DISPLAY_POSITION_LEFT, "[NOT SET]");
                    else
                        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szHostDNS1);
                    
                    strcpy(strtemp,"New:");     
                    CTOS_LCDTPrintXY(1, 7, strtemp);
                    memset(strtemp, 0x00, sizeof(strtemp));
                    memset(strOut,0x00, sizeof(strOut));
                    ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 0, IP_LEN);
                    if(ret==d_KBD_ENTER)
                    {
                        if(strlen(strOut)>0)
                        {
                            BolDetachLANChange=TRUE;
                            memcpy(strTCP.szHostDNS1,strOut,strlen(strOut));
                            strTCP.szHostDNS1[strlen(strOut)]=0;
                            vdMyEZLib_LogPrintf("new DNS ip %s", strTCP.szHostDNS1);
                            break;
                        }
                        else
                        {
                            vduiClearBelow(3);
                            vduiDisplayStringCenter(4,"NO DNS IP");
                            vduiDisplayStringCenter(5,"ENTERED,DISABLE");
                            vduiDisplayStringCenter(6,"DNS IP?");
                            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                            key=struiGetchWithTimeOut();
                            if(key==d_KBD_ENTER)
                            {
                                memset(strTCP.szHostDNS1, 0, sizeof(strTCP.szHostDNS1));
                                break;
                            }
                        }
                    }                       
                    if(ret == d_KBD_CANCEL)
                        break;
                }
                inResult = inTCPSave(1);
                inResult = inTCPRead(1);

                while(1)
                {
                    vduiClearBelow(3);
                    setLCDPrint(3, DISPLAY_POSITION_LEFT, "DNS2 IP");
                    if (wub_strlen(strTCP.szHostDNS2)<=0)
                        setLCDPrint(4, DISPLAY_POSITION_LEFT, "[NOT SET]");
                    else
                        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szHostDNS2);
                    
                    strcpy(strtemp,"New:");     
                    CTOS_LCDTPrintXY(1, 7, strtemp);
                    memset(strtemp, 0x00, sizeof(strtemp));
                    memset(strOut,0x00, sizeof(strOut));
                    ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 0, IP_LEN);
                    if(ret==d_KBD_ENTER)
                    {
                        if(strlen(strOut)>0)
                        {
                            BolDetachLANChange=TRUE;
                            memcpy(strTCP.szHostDNS2,strOut,strlen(strOut));
                            strTCP.szHostDNS2[strlen(strOut)]=0;
                            vdMyEZLib_LogPrintf("new DNS ip %s", strTCP.szHostDNS2);
                            break;
                        }
                        else
                        {
                            vduiClearBelow(3);
                            vduiDisplayStringCenter(4,"NO DNS IP");
                            vduiDisplayStringCenter(5,"ENTERED,DISABLE");
                            vduiDisplayStringCenter(6,"DNS IP?");
                            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                            key=struiGetchWithTimeOut();
                            if(key==d_KBD_ENTER)
                            {
                                memset(strTCP.szHostDNS2, 0, sizeof(strTCP.szHostDNS2));
                                break;
                            }
                        }
                    }                       
                    if(ret == d_KBD_CANCEL)
                        break;
                }
                inResult = inTCPSave(1);
                inResult = inTCPRead(1);
            }
       
            if(BolDetachLANChange==TRUE)
            {
                srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;
                vduiClearBelow(3);
                CTOS_LCDTPrintXY (1,7, "Please Wait     ");       
                if(strCPT.inCommunicationMode == ETHERNET_MODE)
                    CTOS_LCDTPrintXY(1, 8, "Init LAN...     ");
                else if(strCPT.inCommunicationMode == GPRS_MODE)
                    CTOS_LCDTPrintXY(1, 8, "Init GPRS...     ");
                if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK) 
                {
                   
                    vdDisplayErrorMsg(1, 8, "COMM INIT ERR");
                    return;
                }
                
                srCommFuncPoint.inInitComm(&srTransRec,strCPT.inCommunicationMode);
                srCommFuncPoint.inGetCommConfig(&srTransRec);
                srCommFuncPoint.inSetCommConfig(&srTransRec);

				srCommFuncPoint.inDisconnect(&srTransRec);

            }          
    }
	
    if(strCPT.inCommunicationMode == ETHERNET_MODE)
    {
       if(strTCP.fDHCPEnable != IPCONFIG_STATIC)
       {
//           inEthernet_GetTerminalIP(strtemp);
//           if (strcmp(strtemp,"0.0.0.0") == 0)
           {
                srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;
                if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK) 
                {
                   
                    vdDisplayErrorMsg(1, 8, "COMM INIT ERR");
                    return;
                }
                srCommFuncPoint.inInitComm(&srTransRec,strCPT.inCommunicationMode);
                srCommFuncPoint.inGetCommConfig(&srTransRec);
                srCommFuncPoint.inSetCommConfig(&srTransRec); 
           }

			inEthernet_GetTerminalIP(strtemp);

			// set static IP			
			strTCP.fDHCPEnable = IPCONFIG_STATIC;
			strcpy(srEthernetRec.strLocalIP, strtemp);
			srCommFuncPoint.inSetCommConfig(&srTransRec); 

			// set DHCP
			strTCP.fDHCPEnable = IPCONFIG_DHCP;
		    srCommFuncPoint.inSetCommConfig(&srTransRec); 

			srCommFuncPoint.inDisconnect(&srTransRec);
           vduiClearBelow(3);
           setLCDPrint(3, DISPLAY_POSITION_LEFT, "DHCP TERMINAL IP");
           setLCDPrint(4, DISPLAY_POSITION_LEFT, strtemp);
           WaitKey(30);
       }
    }
    return ;
}


void vdCTOS_DialConfig(void)
{
    BYTE bRet,strOut[30],strtemp[17],key;
    BYTE szInputBuf[24+1];
    BYTE szIntComBuf[2];
    BYTE szPhNoBuf[9];
    BYTE szExtNoBuf[4];
    int inResult;
    USHORT ret;
    USHORT usLen;
    int shHostIndex = 1;
    
    shHostIndex = inCTOS_SelectHostSetting();
    if (shHostIndex == -1)
        return;
    
    inResult = inCPTRead(shHostIndex);  
    if (inResult != d_OK)
        return;
    
    inResult = inTCTRead(1);
    if (inResult != d_OK)
        return;
    
    CTOS_LCDTClearDisplay();
    vdDispTitleString("DIALUP SETTING");
    
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "PRIMARY PABX");
        if (wub_strlen(strTCT.szPabx)<=0)
            setLCDPrint(4,DISPLAY_POSITION_LEFT, "[DISABLED]");
        else
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szPabx);

        
        strcpy(strtemp,"New:") ; 
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        memset(strtemp, 0x00, sizeof(strtemp));
        usLen = 10;
        ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 0, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
                BolDetachDIALChangeSetting = TRUE;
                if(strlen(strOut)>0)
                {
                    memcpy(strTCT.szPabx,strOut,strlen(strOut));
                    strTCT.szPabx[strlen(strOut)]=',';
                    strTCT.szPabx[strlen(strOut)+1]=0;
                    break;
                }
                else
                {
                    vduiClearBelow(4);
                    vduiDisplayStringCenter(5,"NO PABX VALUE");
                    vduiDisplayStringCenter(6,"ENTERED,DISABLE");
                    vduiDisplayStringCenter(7,"PABX?");
                    vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                    key=struiGetchWithTimeOut();
                    if(key==d_KBD_ENTER)
                    {
                        memset(strTCT.szPabx,0,sizeof(strTCT.szPabx));
                                                break;
                    }
                }
        }
            if(ret == d_KBD_CANCEL)
                break;
        }

        inTCTSave(1);
        
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "PRIMARY LINE");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szPriTxnPhoneNumber);

            strcpy(strtemp,"New:") ; 
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            usLen = 18;
            ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>= 1)
            {
                BolDetachDIALChangeSetting = TRUE;
                memcpy(strCPT.szPriTxnPhoneNumber,strOut,strlen(strOut));
                strCPT.szPriTxnPhoneNumber[strlen(strOut)]=0;
                break;
            }
            if(ret == d_KBD_CANCEL)
                break;

        }
            
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SECONDARY LINE");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szSecTxnPhoneNumber);
            
            strcpy(strtemp,"New:") ; 
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            usLen = 18;
            ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>= 1)
            {
                BolDetachDIALChangeSetting = TRUE;
                memcpy(strCPT.szSecTxnPhoneNumber,strOut,strlen(strOut));
                strCPT.szSecTxnPhoneNumber[strlen(strOut)]=0;
                break;
            }
            if(ret == d_KBD_CANCEL)
                break;

        }
            
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SETTLEMENT PRI LINE");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szPriSettlePhoneNumber);
            
            strcpy(strtemp,"New:") ; 
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            usLen = 18;
            ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>= 1)
            {
                BolDetachDIALChangeSetting = TRUE;
                memcpy(strCPT.szPriSettlePhoneNumber,strOut,strlen(strOut));
                strCPT.szPriSettlePhoneNumber[strlen(strOut)]=0;
                break;
            }
            if(ret == d_KBD_CANCEL)
                break;

        }
            
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "SETTLEMENT SEC LINE");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strCPT.szSecSettlePhoneNumber);
            
            strcpy(strtemp,"New:") ; 
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            usLen = 18;
            ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                break;
            else if(0 == ret )
                break;
            else if(ret>= 1)
            {
                BolDetachDIALChangeSetting = TRUE;
                memcpy(strCPT.szSecSettlePhoneNumber,strOut,strlen(strOut));
                strCPT.szSecSettlePhoneNumber[strlen(strOut)]=0;
                break;
            }
            if(ret == d_KBD_CANCEL)
                break;

        }

    inResult = inCPTSave(shHostIndex);      
    inResult = inCPTRead(shHostIndex);      
            
    if(BolDetachDIALChangeSetting==TRUE)
    {
        srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;
        vduiClearBelow(3);
        CTOS_LCDTPrintXY (1,7, "Please Wait     ");       
        CTOS_LCDTPrintXY(1, 8, "Init Modem...     ");

        if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK) 
        {
           
            vdDisplayErrorMsg(1, 8, "COMM INIT ERR");
            return;
        }
        
        srCommFuncPoint.inInitComm(&srTransRec,strCPT.inCommunicationMode);
        srCommFuncPoint.inGetCommConfig(&srTransRec);
        srCommFuncPoint.inSetCommConfig(&srTransRec); 

    }
}

void vdCTOS_ModifyEdcSetting(void)
{
    BYTE bRet;
    BYTE szInputBuf[15+1];
    int inResult,inResult1;
    TRANS_TOTAL stBankTotal;
    BYTE strOut[30],strtemp[17],key;
    USHORT ret;
    USHORT usLen;
    BYTE szTempBuf[12+1];
    BOOL isKey;
    int shHostIndex = 1;
    int inNum = 0, inRet;
    
    shHostIndex = inCTOS_SelectHostSetting();
    if (shHostIndex == -1)
        return;

	CTOS_LCDTClearDisplay();

    if (inMultiAP_CheckMainAPStatus() == d_OK)
    {
        inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_EDC_SETTINGS);
        if(d_OK != inRet)
              return ;
    }
	else
	{
        if (inMultiAP_CheckSubAPStatus() == d_OK)
        {
            inRet = inCTOS_MultiAPGetData();
            if(d_OK != inRet)
                return ;
            
            inRet = inCTOS_MultiAPReloadHost();
            if(d_OK != inRet)
                return ;
        }
    }

    shHostIndex=strHDT.inHostIndex;

    inResult = inCTOS_CheckAndSelectMutipleMID();
    if(d_OK != inResult)
        return;
				
    CTOS_LCDTClearDisplay();
    vdDispTitleString("HOST SETTING");

    if(inBatchNumRecord()>0)
    {
        vdDisplayErrorMsg2(1, 8, "BATCH NOT", "EMPTY,SKIPPED.", MSG_TYPE_INFO);
        vdSetErrorMessage("");
		return;
    }
	
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TERMINAL ID");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strMMT[0].szTID);
    
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 8, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            inNum = inBatchNumRecord();
            if (inNum == 0)
            {
                memset(strMMT[0].szTID, 0x00, sizeof(strMMT[0].szTID));
                memcpy(strMMT[0].szTID, strOut, TERMINAL_ID_BYTES);
                strMMT[0].szTID[TERMINAL_ID_BYTES]=0;
                inMMTSave(strMMT[0].MMTid);
                break;
            }
            else
            {
                vduiWarningSound();
                vduiClearBelow(5);
                vduiDisplayStringCenter(6,"BATCH NOT");
                vduiDisplayStringCenter(7,"EMPTY,SKIPPED.");
                CTOS_Delay(2000);
                break;
            }
       }
   if (ret == d_KBD_CANCEL )
        break ;
    }
    
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "MERCHANT ID");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strMMT[0].szMID);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 15, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            inNum = inBatchNumRecord();
            if (inNum == 0)
            {
                memset(strMMT[0].szMID, 0x00, sizeof(strMMT[0].szMID));
                memcpy(strMMT[0].szMID, strOut, MERCHANT_ID_BYTES);
                strMMT[0].szMID[MERCHANT_ID_BYTES]=0;
                inMMTSave(strMMT[0].MMTid);
                break;
            }
            else
            {
                vduiWarningSound();
                vduiClearBelow(5);
                vduiDisplayStringCenter(6,"BATCH NOT");
                vduiDisplayStringCenter(7,"EMPTY,SKIPPED.");
                CTOS_Delay(2000);
                break;
            }
       }
       if (ret == d_KBD_CANCEL )
            break ;
    }

    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TPDU");
        wub_hex_2_str(strHDT.szTPDU,szTempBuf,5);
        setLCDPrint(4, DISPLAY_POSITION_LEFT, szTempBuf);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 10, 10, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==10)
        {
            inNum = inBatchNumRecord();
            if (inNum == 0)
            {
                memset(strHDT.szTPDU, 0x00, sizeof(strHDT.szTPDU)); 
                wub_str_2_hex(strOut,strtemp,TPDU_BYTES);
                memcpy(strHDT.szTPDU, strtemp, TPDU_BYTES/2);
                inHDTSave(shHostIndex);
                break;
            }
            else
            {
                vduiWarningSound();
                vduiClearBelow(5);
                vduiDisplayStringCenter(6,"BATCH NOT");
                vduiDisplayStringCenter(7,"EMPTY,SKIPPED.");
                CTOS_Delay(2000);
                break;
            }
       }
       if (ret == d_KBD_CANCEL )
            break ;
    }
    
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "NII");
        wub_hex_2_str(strHDT.szNII,szTempBuf,2);
        setLCDPrint(4, DISPLAY_POSITION_LEFT, szTempBuf);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 4, 4, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==4)
        {
            inNum = inBatchNumRecord();
            if (inNum == 0)
            {
                memset(strHDT.szNII, 0x00, sizeof(strHDT.szNII));
                wub_str_2_hex(strOut, strtemp, NII_BYTES);
                memcpy(strHDT.szNII, strtemp, NII_BYTES/2);
                inHDTSave(shHostIndex);
                break;
            }
            else
            {
                vduiWarningSound();
                vduiClearBelow(5);
                vduiDisplayStringCenter(6,"BATCH NOT");
                vduiDisplayStringCenter(7,"EMPTY,SKIPPED.");
                CTOS_Delay(2000);
                break;
            }
       }
       if (ret == d_KBD_CANCEL )
            break ;
        }

    inTCTRead(1);    
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "System PWD");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szSystemPW);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 6, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            if (strlen(strOut)>0)
            {
                memset(strTCT.szSystemPW,0x00,PASSWORD_LEN);
                memcpy(strTCT.szSystemPW,strOut, strlen(strOut));
                strTCT.szSystemPW[strlen(strOut)]=0;
                inTCTSave(1);
                break;
            }
            else
            {
                vduiClearBelow(3);
                vduiDisplayStringCenter(4,"NO PASSWORD");
                vduiDisplayStringCenter(5,"ENTERED,DISABLE");
                vduiDisplayStringCenter(6,"SYSTEM PWD?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                key=WaitKey(60);
                if(key==d_KBD_ENTER)
                {
                    memset(strTCT.szSystemPW,0x00,PASSWORD_LEN);
                    inTCTSave(1);
                    break;
                }
            }
    }
    if (ret == d_KBD_CANCEL )
        break ;
    }
  
    inTCTRead(1);
    while(1)
    {    
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "Engineer PWD");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szEngineerPW);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 6, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            if (strlen(strOut)>0)
            {
                memset(strTCT.szEngineerPW,0x00,PASSWORD_LEN);
                memcpy(strTCT.szEngineerPW,strOut, strlen(strOut));
                strTCT.szEngineerPW[strlen(strOut)]=0;
                inTCTSave(1);
                break;
            }
            else
            {
                vduiClearBelow(3);
                vduiDisplayStringCenter(4,"NO PASSWORD");
                vduiDisplayStringCenter(5,"ENTERED,DISABLE");
                vduiDisplayStringCenter(6,"ENGINEER PWD?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                key=WaitKey(60);
                if(key==d_KBD_ENTER)
                {
                    memset(strTCT.szEngineerPW,0x00,PASSWORD_LEN);
                    inTCTSave(1);
                    break;
                }
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
        }

	inTCTRead(1);
    while(1)
	{    
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "PM password");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szPMpassword);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 8, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            if (strlen(strOut)>0)
            {
                memset(strTCT.szPMpassword,0x00,8);
                memcpy(strTCT.szPMpassword,strOut, strlen(strOut));
                strTCT.szPMpassword[strlen(strOut)]=0;
                inTCTSave(1);
                break;
            }
            else
            {
                vduiClearBelow(3);
                vduiDisplayStringCenter(4,"NO PASSWORD");
                vduiDisplayStringCenter(5,"ENTERED,DISABLE");
                vduiDisplayStringCenter(6,"ENGINEER PWD?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                key=WaitKey(60);
                if(key==d_KBD_ENTER)
                {
                    memset(strTCT.szPMpassword,0x00,8);
                    inTCTSave(1);
                    break;
                }
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
	}

	inTCTRead(1);
    while(1)
	{    
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "FunKey PWD");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szFunKeyPW);
        
        strcpy(strtemp,"New:");
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 8, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            if (strlen(strOut)>0)
            {
                memset(strTCT.szFunKeyPW,0x00,8);
                memcpy(strTCT.szFunKeyPW,strOut, strlen(strOut));
                strTCT.szFunKeyPW[strlen(strOut)]=0;
                inTCTSave(1);
                break;
            }
            else
            {
                vduiClearBelow(3);
                vduiDisplayStringCenter(4,"NO PASSWORD");
                vduiDisplayStringCenter(5,"ENTERED,DISABLE");
                vduiDisplayStringCenter(6,"ENGINEER PWD?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                key=WaitKey(60);
                if(key==d_KBD_ENTER)
                {
                    memset(strTCT.szFunKeyPW,0x00,8);
                    inTCTSave(1);
                    break;
                }
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
	}

    inHDTRead(shHostIndex);
    while(1)
    {    
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "LAST 4 DIGIT");
        if(strHDT.fLast4Digit== 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        else if(strHDT.fLast4Digit == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strHDT.fLast4Digit = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strHDT.fLast4Digit = 0;
                 }

                 inHDTSave(shHostIndex);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(7,"PLEASE SELECT");
                vduiDisplayStringCenter(8,"A VALID INPUT");
                CTOS_Delay(2000);       
            }
        }
		
        if (ret == d_KBD_CANCEL )
            break ;
    }

	inHDTRead(shHostIndex);
    while(1)
    {    
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "Tip Allowed");
        if(strHDT.fTipAllowFlag== 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        else if(strHDT.fTipAllowFlag == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strHDT.fTipAllowFlag= 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strHDT.fTipAllowFlag = 0;
                 }

                 inHDTSave(shHostIndex);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(7,"PLEASE SELECT");
                vduiDisplayStringCenter(8,"A VALID INPUT");
                CTOS_Delay(2000);       
            }
        }
		
        if (ret == d_KBD_CANCEL )
            break ;
    }
	inHDTRead(shHostIndex);
            
	return ;
}

void vdCTOS_DeleteBatch(void)
{
    int         shHostIndex = 1;
    int         inResult,inRet;
    ACCUM_REC srAccumRec;
    STRUCT_FILE_SETTING strFile;
	
    vdDebug_LogPrintf("--vdCTOS_DeleteBatch--");
	vdDebug_LogPrintf("byERMMode[%d]", strTCT.byERMMode);
	
    vduiLightOn();                

    shHostIndex = inCTOS_SelectHostSetting();
    if (shHostIndex == -1)
        return;

    CTOS_LCDTClearDisplay();
	
    if (inMultiAP_CheckMainAPStatus() == d_OK)
    {
        inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_DELETE_BATCH);
        if(d_OK != inRet)
            return ;
    }
    else
    {
        if (inMultiAP_CheckSubAPStatus() == d_OK)
        {
            inRet = inCTOS_MultiAPGetData();
            if(d_OK != inRet)
                return ;

            inRet = inCTOS_MultiAPReloadHost();
            if(d_OK != inRet)
                return ;
        }
    }

    inResult = inCTOS_CheckAndSelectMutipleMID();
    if(d_OK != inResult)
        return;
          
    //inResult = vduiAskConfirmContinue();
    inResult=vduiAskConfirmation("DELETE BATCH?");
    if(inResult == d_OK)
    {
        char szBcd[INVOICE_BCD_SIZE+1];
		
        if(CN_TRUE == strMMT[0].fMustSettFlag)
        {
            strMMT[0].fMustSettFlag = CN_FALSE;
            inMMTSave(strMMT[0].MMTid);
        }

        memset(szBcd, 0x00, sizeof(szBcd));
        memcpy(szBcd, strMMT[0].szBatchNo, INVOICE_BCD_SIZE);    
        inBcdAddOne(szBcd, strMMT[0].szBatchNo, INVOICE_BCD_SIZE);
        strMMT[0].fMustSettFlag = CN_FALSE;
        inMMTSave(strMMT[0].MMTid);

		strMMT[0].fBatchNotEmpty = 0;	
		inMMTBatchNotEmptySave(strMMT[0].MMTid);	
		
		// Check for batch empty
		inRet = inCTOS_ChkBatchEmpty();
		if(d_OK != inRet)
		return;		
		
        // delete batch where hostid and mmtid is matcj
        inDatabase_BatchDelete();

        memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
        memset(&strFile,0,sizeof(strFile));
        vdCTOS_GetAccumName(&strFile, &srAccumRec);
    
        if((inResult = CTOS_FileDelete(strFile.szFileName)) != d_OK)
        {
            vdDebug_LogPrintf("[inMyFile_SettleRecordDelete]---Delete Record error[%04x]", inResult);
        }
        
        inCTOS_DeleteBKAccumTotal(&srAccumRec,strHDT.inHostIndex,srTransRec.MITid);

        inDatabase_InvoiceNumDelete(srTransRec.HDTid, srTransRec.MITid);
		
        inMyFile_ReversalDelete();

        inMyFile_AdviceDelete();
        
        inMyFile_TCUploadDelete();

        // For ERM
		if (strTCT.byERMMode != 0)
		{
			vdLinuxCommandClearERMBitmap();
			vdClearERMBatch(FALSE);
		}
			
        CTOS_LCDTClearDisplay();
		if (isCheckTerminalMP200() == d_OK || isCheckTerminalNonTouch() == d_OK)
		{
			//setLCDPrint27(5, d_LCD_ALIGNCENTER, "CLEAR BATCH");
			//setLCDPrint27(6, d_LCD_ALIGNCENTER, "RECORD DONE");			
			vdDisplayMessageBox(1, 8, "CLEAR BATCH", "RECORD DONE", "", MSG_TYPE_INFO);
		}
		else
		{
	        //setLCDPrint(5, DISPLAY_POSITION_CENTER, "CLEAR BATCH");
	        //setLCDPrint(6, DISPLAY_POSITION_CENTER, "RECORD DONE");
	        
			vdDisplayMessageBox(1, 8, "CLEAR BATCH", "RECORD DONE", "", MSG_TYPE_INFO);
		}
		
        CTOS_Delay(1000); 
    }                
}

void vdCTOS_DeleteReversal(void)
{
    CHAR szFileName[d_BUFF_SIZE];
    int         shHostIndex = 1;
    int         inResult,inRet;
    ACCUM_REC srAccumRec;
    STRUCT_FILE_SETTING strFile;
    
    vduiLightOn();                

    shHostIndex = inCTOS_SelectHostSetting();
    if (shHostIndex == -1)
        return;

    CTOS_LCDTClearDisplay();
		
    if (inMultiAP_CheckMainAPStatus() == d_OK)
    {
        inRet = inCTOS_MultiAPSaveData(d_IPC_CMD_DELETE_REVERSAL);
        if(d_OK != inRet)
            return ;
    }
    else
    {
        if (inMultiAP_CheckSubAPStatus() == d_OK)
        {
            inRet = inCTOS_MultiAPGetData();
            if(d_OK != inRet)
                return ;

            inRet = inCTOS_MultiAPReloadHost();
            if(d_OK != inRet)
                return ;
        }
    }

    inResult = inCTOS_CheckAndSelectMutipleMID();    
    if(d_OK != inResult)
        return;

    memset(szFileName,0,sizeof(szFileName));
    sprintf(szFileName, "%s%02d%02d.rev"
                    , strHDT.szHostLabel
                    , strHDT.inHostIndex
                    , srTransRec.MITid);

    vdDebug_LogPrintf("Rever Name %s",szFileName);
    
    if((inResult = inMyFile_CheckFileExist(szFileName)) < 0)
    {
        vdDebug_LogPrintf("inMyFile_CheckFileExist <0");
        vduiClearBelow(2);
        vdDisplayErrorMsg(1, 8, "NO REVERSAL EXISTS");
        return ;
    }
            
    //inResult = vduiAskConfirmContinue();    
    inResult=vduiAskConfirmation("CLEAR REVERSAL?");
    if(inResult == d_OK)
    {
        inMyFile_ReversalDelete();   
		vdDisplayMessageBox(1, 8, "CLEAR REVERSAL", "RECORD DONE", "", MSG_TYPE_INFO);
		CTOS_Beep();
		CTOS_Delay(1000);
    }                
}

void vdCTOS_PrintEMVTerminalConfig(void)
{
    CTOS_LCDTClearDisplay();
    vdPrintEMVTags();
    
    return;
}

void vdCTOS_Debugmode(void)
{
    BYTE bRet;
    BYTE szInputBuf[15+1];
    int inResult,inResult1;
    TRANS_TOTAL stBankTotal;
    BYTE strOut[30],strtemp[17],key;
    USHORT ret;
    USHORT usLen;
    BYTE szTempBuf[12+1];
    BOOL isKey;
    int shHostIndex = 1;
    int inNum = 0;
    int inRet = 0;

    inRet = inTCTRead(1);  
    vdDebug_LogPrintf(". inTCTRead(%d)",inRet);

    CTOS_LCDTClearDisplay();
    vdDispTitleString("DEBUG MODE");
    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "DEBUG MODE");
        if(strTCT.byRS232DebugPort == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        if(strTCT.byRS232DebugPort == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");        
        if(strTCT.byRS232DebugPort == 2)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "2");
        if(strTCT.byRS232DebugPort == 8)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "8");  
        
        CTOS_LCDTPrintXY(1, 5, "0-NO        1-COM1");
        CTOS_LCDTPrintXY(1, 6, "2-COM2    8-USB");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31 || strOut[0]==0x32 || strOut[0]==0x38)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.byRS232DebugPort = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.byRS232DebugPort = 0;
                 }
                 if(strOut[0] == 0x32)
                 {
                        strTCT.byRS232DebugPort = 2;
                 }
                 if(strOut[0] == 0x38)
                 {
                        strTCT.byRS232DebugPort = 8;
                 }
                
                 inRet = inTCTSave(1);
                 
                 vdDebug_LogPrintf(". inTCTSave(%d)",inRet);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(6,"PLEASE SELECT");
                vduiDisplayStringCenter(7,"A VALID");
                vduiDisplayStringCenter(8,"DEBUG MODE");
                CTOS_Delay(2000);       
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
    }
       
    return ;
}
void vdCTOSS_InjectMKKey(void)
{
	BYTE bInBuf[250];
	BYTE bOutBuf[250];
	BYTE *ptr = NULL;
	USHORT usInLen = 0;
	USHORT usOutLen = 0;
	USHORT usResult;


	CTOS_LCDTClearDisplay();
	CTOS_LCDTPrintXY(1, 1, "INJECT MK");

	memset(bOutBuf, 0x00, sizeof(bOutBuf));
	memset(bInBuf, 0x00, sizeof(bInBuf));
	
	vdDebug_LogPrintf("d_IPC_CMD_EMV_INJECTMK usInLen[%d] ",usInLen);
	
	usResult = inMultiAP_RunIPCCmdTypes("com.Source.SHARLS_EMV.SHARLS_EMV", d_IPC_CMD_EMV_INJECTMK, bInBuf, usInLen, bOutBuf, &usOutLen);

	if (usResult != d_OK)
		return ;

	CTOS_LCDTPrintXY(1, 1, "INJECT MK");
	vdDebug_LogPrintf("d_IPC_CMD_EMV_INJECTMK usOutLen[%d] ",usOutLen);
	DebugAddHEX("MK",bOutBuf,usOutLen);
	
	inTCTRead(1);
	vdDebug_LogPrintf("InjectMKKey..byPinPadType=[%d],byPinPadPort=[%d]",strTCT.byPinPadType,strTCT.byPinPadPort);
	if (1 == strTCT.byPinPadType)
	{
		if (usOutLen == 8)
			inPCI100_SaveMKProcess(MK_DES_KEY,usOutLen,bOutBuf);
		if (usOutLen == 16)
			inPCI100_SaveMKProcess(MK_3DES_DOUBLE_KEY,usOutLen,bOutBuf);
		if (usOutLen == 24)
			inPCI100_SaveMKProcess(MK_3DES_TRIPLE_KEY,usOutLen,bOutBuf);
	}

	return;
}

void vdCTOSS_SelectPinpadType(void)
{
    BYTE bRet;
    BYTE szInputBuf[15+1];
    int inResult,inResult1;
    TRANS_TOTAL stBankTotal;
    BYTE strOut[30],strtemp[17],key;
    USHORT ret;
    USHORT usLen;
    BYTE szTempBuf[12+1];
    BOOL isKey;
    int shHostIndex = 1;
    int inNum = 0;
    int inRet = 0;

    inRet = inTCTRead(1);  
    vdDebug_LogPrintf(". inTCTRead(%d)",inRet);

    CTOS_LCDTClearDisplay();
    vdDispTitleString("SETTING");
    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "PINPAD TYPE");
        if(strTCT.byPinPadType == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        if(strTCT.byPinPadType == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");        
        if(strTCT.byPinPadType == 2)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "2");
  
        
        CTOS_LCDTPrintXY(1, 5, "0-None	1-PCI100");
        CTOS_LCDTPrintXY(1, 6, "2-OTHER    ");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31 || strOut[0]==0x32)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.byPinPadType = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.byPinPadType = 0;
                 }
                 if(strOut[0] == 0x32)
                 {
                        strTCT.byPinPadType = 2;
                 }
 
                
                 inRet = inTCTSave(1);
                 
                 vdDebug_LogPrintf(". inTCTSave(%d)",inRet);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(6,"PLEASE SELECT");
                vduiDisplayStringCenter(7,"A VALID");
                vduiDisplayStringCenter(8,"DEBUG MODE");
                CTOS_Delay(2000);       
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
    }
       
    return ;
}

int inCTOSS_GetCtlsMode(void)
{
	inTCTRead(1);

	return strTCT.byCtlsMode;
}

void vdCTOSS_CtlsMode(void)
{
    BYTE bRet;
    BYTE szInputBuf[15+1];
    int inResult,inResult1;
    TRANS_TOTAL stBankTotal;
    BYTE strOut[30],strtemp[17],key;
    USHORT ret;
    USHORT usLen;
    BYTE szTempBuf[12+1];
    BOOL isKey;
    int shHostIndex = 1;
    int inNum = 0;
    int inRet = 0;

    inRet = inTCTRead(1);  
    vdDebug_LogPrintf(". inTCTRead(%d)",inRet);

    CTOS_LCDTClearDisplay();
    vdDispTitleString("SETTING");
    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "CTLS MODE");
        if(strTCT.byCtlsMode == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        if(strTCT.byCtlsMode == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");        
        if(strTCT.byCtlsMode == 2)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "2");
 
        
        CTOS_LCDTPrintXY(1, 5, "0-Disable 1-Internal");
        CTOS_LCDTPrintXY(1, 6, "        2-External");
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            if (strOut[0]==0x30 || strOut[0]==0x31 || strOut[0]==0x32)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.byCtlsMode = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.byCtlsMode = 0;
                 }
                 if(strOut[0] == 0x32)
                 {
                        strTCT.byCtlsMode = 2;
                 }                 
                
                 inRet = inTCTSave(1);
                 
                 vdDebug_LogPrintf(". inTCTSave(%d)",inRet);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(6,"PLEASE SELECT");
                vduiDisplayStringCenter(7,"A VALID");
                vduiDisplayStringCenter(8,"DEBUG MODE");
                CTOS_Delay(2000);       
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
    }
       
    return ;
}


void vdCTOS_DemoMode(void)
{
    BYTE bRet;
    BYTE szInputBuf[15+1];
    int inResult,inResult1;
    TRANS_TOTAL stBankTotal;
    BYTE strOut[30],strtemp[17],key;
    USHORT ret;
    USHORT usLen;
    BYTE szTempBuf[12+1];
    BOOL isKey;
    int shHostIndex = 1;
    int inNum = 0;
    int inNumOfHost = 0;
    int inNumOfMerchant = 0;
    int inLoop =0 ;
    //int inResult = 0;
    ACCUM_REC srAccumRec;
    STRUCT_FILE_SETTING strFile;

    inTCTRead(1);   
    
    CTOS_LCDTClearDisplay();
    vdDispTitleString("DEMO MODE");
    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "DEMO MODE");
        if(strTCT.fDemo == 0)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "0");
        if(strTCT.fDemo == 1)
            setLCDPrint(4, DISPLAY_POSITION_LEFT, "1");     
        
        CTOS_LCDTPrintXY(1, 5, "0-DISABLE      1-ENABLE");
        
   
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret==1)
        {
            //check host num
            inNumOfHost = inHDTNumRecord();
            
            vdDebug_LogPrintf("[inNumOfHost]-[%d]", inNumOfHost);
            for(inNum =1 ;inNum <= inNumOfHost; inNum++)
            {
                if(inHDTRead(inNum) == d_OK)
                {
                    inMMTReadNumofRecords(strHDT.inHostIndex,&inNumOfMerchant);
                
                    vdDebug_LogPrintf("[inNumOfMerchant]-[%d]strHDT.inHostIndex[%d]", inNumOfMerchant,strHDT.inHostIndex);
                    for(inLoop=1; inLoop <= inNumOfMerchant;inLoop++)
                    {
                        if((inResult = inMMTReadRecord(strHDT.inHostIndex,strMMT[inLoop-1].MITid)) !=d_OK)
                        {
                            vdDebug_LogPrintf("[read MMT fail]-Mitid[%d]strHDT.inHostIndex[%d]inResult[%d]", strMMT[inLoop-1].MITid,strHDT.inHostIndex,inResult);
                            continue;
                            //break;
                        }
                        else    // delete batch where hostid and mmtid is match  
                        {
                            strMMT[0].HDTid = strHDT.inHostIndex;
                            strMMT[0].MITid = strMMT[inLoop-1].MITid;
                            inDatabase_BatchDeleteHDTidMITid();
                            vdDebug_LogPrintf("[inDatabase_BatchDelete]-Mitid[%d]strHDT.inHostIndex[%d]", strMMT[inLoop-1].MITid,strHDT.inHostIndex);
                            
                            memset(&srAccumRec, 0x00, sizeof(ACCUM_REC));
                            memset(&strFile,0,sizeof(strFile));
                            srTransRec.MITid = strMMT[inLoop-1].MITid;
                            vdCTOS_GetAccumName(&strFile, &srAccumRec);
                            
                            if((inResult = CTOS_FileDelete(strFile.szFileName)) != d_OK)
                            {
                                vdDebug_LogPrintf("[inMyFile_SettleRecordDelete]---Delete Record error[%04x]", inResult);
                            }

                            inCTOS_DeleteBKAccumTotal(&srAccumRec,strHDT.inHostIndex,srTransRec.MITid);

                            inDatabase_InvoiceNumDelete(srTransRec.HDTid, srTransRec.MITid);
							
                            inMyFile_ReversalDelete();

                            inMyFile_AdviceDelete();
                            
                            inMyFile_TCUploadDelete();

                        }
                    }
                }
                else
                    continue;

            }
            
            if (strOut[0]==0x30 || strOut[0]==0x31)
            {
                 if(strOut[0] == 0x31)
                 {
                        strTCT.fDemo = 1;
                 }
                 if(strOut[0] == 0x30)
                 {
                        strTCT.fDemo = 0;
                 }
                 inTCTSave(1);
                 break;
             }
             else
             {
                vduiWarningSound();
                vduiDisplayStringCenter(6,"PLEASE SELECT");
                vduiDisplayStringCenter(7,"A VALID");
                vduiDisplayStringCenter(8,"DEBUG MODE");
                CTOS_Delay(2000);       
            }
        }
        if (ret == d_KBD_CANCEL )
            break ;
    }
       
    return ;
}


void vdCTOS_GPRSSetting(void)
{
    BYTE bRet,strOut[30],strtemp[17],key;
    BYTE szInputBuf[24+1];
    BYTE szIntComBuf[2];
    BYTE szPhNoBuf[9];
    BYTE szExtNoBuf[4];
    int inResult;
    USHORT ret;
    short inRtn;
    USHORT shMaxLen = 30;
    
    inResult = inTCPRead(1);
    if (inResult != d_OK)
        return;
        
    CTOS_LCDTClearDisplay();
    vdDispTitleString("GPRS SETUP");
    
    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "APN");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szAPN);
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &shMaxLen, 1, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            BolDetachGPRSChangeSetting=TRUE;
            memcpy(strTCP.szAPN, strOut,strlen(strOut));
            strTCP.szAPN[strlen(strOut)]=0;
            inResult = inTCPSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }               

    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "GPRS USER NAME");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCP.szUserName);

        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &shMaxLen, 1, d_INPUT_TIMEOUT);
        if(ret==d_KBD_ENTER)
        {
            BolDetachGPRSChangeSetting=TRUE;
            memcpy(strTCP.szUserName, strOut,strlen(strOut));
            strTCP.szUserName[strlen(strOut)]=0;
            inResult = inTCPSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }               

    while(1)
    {
        clearLine(3);
        clearLine(4);
        clearLine(5);
        clearLine(6);
        clearLine(7);
        clearLine(8);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "GPRS PASSWORD");
        setLCDPrint(4, DISPLAY_POSITION_CENTER, strTCP.szPassword);

        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &shMaxLen, 1, d_INPUT_TIMEOUT);
        
        if(ret==d_KBD_ENTER)
        {
            BolDetachGPRSChangeSetting=TRUE;
            memcpy(strTCP.szPassword, strOut,strlen(strOut));
            strTCP.szPassword[strlen(strOut)]=0;
            inResult = inTCPSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }   

    if(BolDetachGPRSChangeSetting==TRUE)
    {
            srTransRec.usTerminalCommunicationMode = strCPT.inCommunicationMode;
            clearLine(3);
            clearLine(4);
            clearLine(5);
            clearLine(6);
            clearLine(7);
            clearLine(8);
            CTOS_LCDTPrintXY (1,7, "Please Wait     ");       
            CTOS_LCDTPrintXY(1, 8, "Init GPRS...     ");
            if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK) 
            {
               
                vdDisplayErrorMsg(1, 8, "COMM INIT ERR");
                return;
            }
                          
            srCommFuncPoint.inInitComm(&srTransRec,GPRS_MODE);
            srCommFuncPoint.inGetCommConfig(&srTransRec);
            srCommFuncPoint.inSetCommConfig(&srTransRec);

    }

    return ;
}

/* delete a Char in string */
void DelCharInStr(char *str, char c, int flag)
{
    int i,l;

    l=strlen(str);

    if (flag == STR_HEAD)
    {
        for (i=0;i<l&&str[i]==c;i++);
        if (i>0) strcpy(str,str+i);
    }

    if (flag == STR_BOTTOM)
    {
        for (i=l-1;i>=0&&str[i]==c;i--);
        str[i+1]='\0';
    }

    if (flag == STR_ALL)
    {
        for (i=0;i<l;i++)
            if (str[i]==c)
            {
                strcpy(str+i,str+i+1);
                i--;
            }
    }
}



void vdConfigEditAddHeader(void)
{
    USHORT inRet, inSelIndex=7;
    BYTE strBuff[50];
    BYTE strActualValue[50];
    USHORT inResult;
    
    STR *keyboardLayoutEnglish[]={" 0", "qzQZ1", "abcABC2", "defDEF3", "ghiGHI4",
    "jklJKL5", "mnoMNO6", "prsPRS7", "tuvTUV8", "wxyWXY9", ":;/\\|?,.<>_",
    ".!@#$%^&*()-"};
    //numeric keyboard (0123456789) and radix point '.'
    STR *keyboardLayoutNumberWithRadixPoint[]={"0", "1", "2", "3", "4", "5", "6",
    "7", "8", "9", "", "."};
    //numeric keyboard (0123456789) without radix point
    STR *keyboardLayoutNumber[]={"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "", ""};
    
    //Parameter for CTOS_UIKeypad:
    //1. usX: 4 => Cursor horizontal position at 4
    //2. usY: 2 => Cursor vertical position at 2
    //3. *pcaKeyboardLayout[]: keyboardLayoutEnglish => use english keyboard
    //layout defined at the beginning of the program
    //4. ucCursorBlinkInterval: 40 => Cursor blink at 400ms interval
    //5. ucDelayToNextChar: 80 => Move Cursor to next character if user did not
    //press any key within 800ms
    //6. boEnableCursorMove: d_TRUE => Allow user to move cursor by F3 and F4 key
    //7. boOneRadixPointOnly: d_FALSE => Do not check radix point
    //8. ucDigitAfterRadixPoint: 0 => Do not limit digits after radix point
    //9. bPasswordMask: 0 => Do not mask user data.
    //10. caData[]: baBuff => Pointer to store the user data retrieved
    //11. ucDataLen: 9 => baBuff has 9 bytes. User may input 8 bytes max.
    
    inResult = inPCTRead(1);
     
//---------HEADER LINE 1---------------------//
    vduiClearBelow(2);  
    vduiDisplayStringCenter(1, "ADDRESS HEADER");
    CTOS_LCDTPrintXY(1, 2, "OLD HEADER LINE1:           ");
    CTOS_LCDTPrintXY(1, 3, strPCT.szRctHdr1);
    CTOS_LCDTPrintXY(1, 4, &strPCT.szRctHdr1[16]);
    CTOS_LCDTPrintXY(1, 5, &strPCT.szRctHdr1[32]);
    CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE1:");
    memset(strBuff,0x00, sizeof(strBuff));  
    memset(strActualValue,0x00, sizeof(strActualValue));
    
    inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);

    if(inRet==d_OK)
    {
        if (strlen(strBuff)>0)
        {
            memcpy(strActualValue,strBuff,strlen(strBuff));

            if (strlen(strBuff)>=15)
            {
                vduiClearBelow(2);  
                vduiDisplayStringCenter(4,"CONTINUE INPUT");
                vduiDisplayStringCenter(5,"MORE TEXT?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");

                while(1)
                {
                    inRet=struiGetchWithTimeOut();
                    if (inRet==d_KBD_ENTER)
                    {
                        vduiClearBelow(2);  
                        CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE1:");
                        memset(strBuff,0x00, sizeof(strBuff));
                        
                        vduiClearBelow(7);  
                        CTOS_LCDTPrintXY(1, 7, strActualValue);
                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                        if(inRet==d_OK)
                        {
                            if (strlen(strBuff)>0)
                            {
                                memcpy(&strActualValue[15],strBuff,strlen(strBuff));
                                if (strlen(strBuff)>=15)
                                {
                                    vduiClearBelow(2);  
                                    vduiDisplayStringCenter(4,"CONTINUE INPUT");
                                    vduiDisplayStringCenter(5,"MORE TEXT?");
                                    vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                                    inRet=struiGetchWithTimeOut();
                                    if (inRet==d_KBD_ENTER)
                                    {
                                        vduiClearBelow(2);
                                        CTOS_LCDTPrintXY(1, 5, "ENTR NEW LINE1:");
                                        memset(strBuff,0x00, sizeof(strBuff));                                                                  
                                        CTOS_LCDTPrintXY(1, 6, strActualValue);
                                        CTOS_LCDTPrintXY(1, 7, &strActualValue[16]);
                                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                                        if(inRet==d_OK)
                                        {    
                                           memcpy(&strActualValue[30],strBuff,strlen(strBuff));
                                        }
                                        break;
                                    }
                                    else if (inRet==d_KBD_CANCEL)
                                        break;
                                    else
                                            vduiWarningSound();
                                }
                            }
                            else
                                break;
                                                
                        }
                        break;
                    }
                    else if (inRet==d_KBD_CANCEL)
                        break;
                    else
                        vduiWarningSound();
                }
            }
            memset(strPCT.szRctHdr1,0x00, sizeof(strPCT.szRctHdr1));    
            memcpy(strPCT.szRctHdr1,strActualValue,strlen(strActualValue));
            
        }
        else
        {
            vduiClearBelow(3);  
            vduiDisplayStringCenter(4,"NO VALUE ENTERED");
            vduiDisplayStringCenter(5,"SAVE EMPTY?");
            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
            while(1)
            {
                inRet=struiGetchWithTimeOut();
                if (inRet==d_KBD_ENTER)
                {
                    memset(strPCT.szRctHdr1,0x00, sizeof(strPCT.szRctHdr1));    
                    break;
                }
                else if (inRet==d_KBD_CANCEL)
                    break;
                else
                    vduiWarningSound();
            }
        }
    }
        
   
    //---------HEADER LINE 2---------------------//    
    vduiClearBelow(2);  
    vduiDisplayStringCenter(1, "ADDRESS HEADER");
    CTOS_LCDTPrintXY(1, 2, "OLD HEADER LINE2:           ");
    CTOS_LCDTPrintXY(1, 3, strPCT.szRctHdr2);
    CTOS_LCDTPrintXY(1, 4, &strPCT.szRctHdr2[16]);
    CTOS_LCDTPrintXY(1, 5, &strPCT.szRctHdr2[32]);
    CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE2:");
    memset(strBuff,0x00, sizeof(strBuff));  
    memset(strActualValue,0x00, sizeof(strActualValue));
    
    inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);

    if(inRet==d_OK)
    {
        if (strlen(strBuff)>0)
        {
            memcpy(strActualValue,strBuff,strlen(strBuff));

            if (strlen(strBuff)>=15)
            {
                vduiClearBelow(2);  
                vduiDisplayStringCenter(4,"CONTINUE INPUT");
                vduiDisplayStringCenter(5,"MORE TEXT?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");

                while(1)
                {
                    inRet=struiGetchWithTimeOut();
                    if (inRet==d_KBD_ENTER)
                    {
                        vduiClearBelow(2);  
                        CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE2:");
                        memset(strBuff,0x00, sizeof(strBuff));
                        
                        vduiClearBelow(7);  
                        CTOS_LCDTPrintXY(1, 7, strActualValue);
                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                        if(inRet==d_OK)
                        {
                            if (strlen(strBuff)>0)
                            {
                                memcpy(&strActualValue[15],strBuff,strlen(strBuff));
                                if (strlen(strBuff)>=15)
                                {
                                    vduiClearBelow(2);  
                                    vduiDisplayStringCenter(4,"CONTINUE INPUT");
                                    vduiDisplayStringCenter(5,"MORE TEXT?");
                                    vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                                    inRet=struiGetchWithTimeOut();
                                    if (inRet==d_KBD_ENTER)
                                    {
                                        vduiClearBelow(2);
                                        CTOS_LCDTPrintXY(1, 5, "ENTR NEW LINE2:");
                                        memset(strBuff,0x00, sizeof(strBuff));                                                                  
                                        CTOS_LCDTPrintXY(1, 6, strActualValue);
                                        CTOS_LCDTPrintXY(1, 7, &strActualValue[16]);
                                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                                        if(inRet==d_OK)
                                        {    
                                           memcpy(&strActualValue[30],strBuff,strlen(strBuff));
                                        }
                                        break;
                                    }
                                    else if (inRet==d_KBD_CANCEL)
                                        break;
                                    else
                                        vduiWarningSound();
                                }
                            }
                            else
                                break;
                                                
                        }
                        break;
                    }
                    else if (inRet==d_KBD_CANCEL)
                        break;
                    else
                        vduiWarningSound();
                }
            }
            memset(strPCT.szRctHdr2,0x00, sizeof(strPCT.szRctHdr2));    
            memcpy(strPCT.szRctHdr2,strActualValue,strlen(strActualValue));
            
        }
        else
        {
            vduiClearBelow(3);  
            vduiDisplayStringCenter(4,"NO VALUE ENTERED");
            vduiDisplayStringCenter(5,"SAVE EMPTY?");
            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
            while(1)
            {
                inRet=struiGetchWithTimeOut();
                if (inRet==d_KBD_ENTER)
                {
                    memset(strPCT.szRctHdr2,0x00, sizeof(strPCT.szRctHdr2));    
                    break;
                }
                else if (inRet==d_KBD_CANCEL)
                    break;
                else
                    vduiWarningSound();
            }
        }
    }    
        
        
   //---------HEADER LINE 3---------------------//    
    vduiClearBelow(2);  
    vduiDisplayStringCenter(1, "ADDRESS HEADER");
    CTOS_LCDTPrintXY(1, 2, "OLD HEADER LINE3:           ");
    CTOS_LCDTPrintXY(1, 3, strPCT.szRctHdr3);
    CTOS_LCDTPrintXY(1, 4, &strPCT.szRctHdr3[16]);
    CTOS_LCDTPrintXY(1, 5, &strPCT.szRctHdr3[32]);
    CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE3:");
    memset(strBuff,0x00, sizeof(strBuff));  
    memset(strActualValue,0x00, sizeof(strActualValue));
    
    inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);

    if(inRet==d_OK)
    {
        if (strlen(strBuff)>0)
        {
            memcpy(strActualValue,strBuff,strlen(strBuff));

            if (strlen(strBuff)>=15)
            {
                vduiClearBelow(2);  
                vduiDisplayStringCenter(4,"CONTINUE INPUT");
                vduiDisplayStringCenter(5,"MORE TEXT?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");

                while(1)
                {
                    inRet=struiGetchWithTimeOut();
                    if (inRet==d_KBD_ENTER)
                    {
                        vduiClearBelow(2);  
                        CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE3:");
                        memset(strBuff,0x00, sizeof(strBuff));
                        
                        vduiClearBelow(7);  
                        CTOS_LCDTPrintXY(1, 7, strActualValue);
                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                        if(inRet==d_OK)
                        {
                            if (strlen(strBuff)>0)
                            {
                                memcpy(&strActualValue[15],strBuff,strlen(strBuff));
                                if (strlen(strBuff)>=15)
                                {
                                    vduiClearBelow(2);  
                                    vduiDisplayStringCenter(4,"CONTINUE INPUT");
                                    vduiDisplayStringCenter(5,"MORE TEXT?");
                                    vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                                    inRet=struiGetchWithTimeOut();
                                    if (inRet==d_KBD_ENTER)
                                    {
                                        vduiClearBelow(2);
                                        CTOS_LCDTPrintXY(1, 5, "ENTR NEW LINE3:");
                                        memset(strBuff,0x00, sizeof(strBuff));                                                                  
                                        CTOS_LCDTPrintXY(1, 6, strActualValue);
                                        CTOS_LCDTPrintXY(1, 7, &strActualValue[16]);
                                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                                        if(inRet==d_OK)
                                        {    
                                           memcpy(&strActualValue[30],strBuff,strlen(strBuff));
                                        }
                                        break;
                                    }
                                    else if (inRet==d_KBD_CANCEL)
                                        break;
                                    else
                                            vduiWarningSound();
                                }
                            }
                            else
                                break;
                                                
                        }
                        break;
                    }
                    else if (inRet==d_KBD_CANCEL)
                        break;
                    else
                        vduiWarningSound();
                }
            }
            memset(strPCT.szRctHdr3,0x00, sizeof(strPCT.szRctHdr3));    
            memcpy(strPCT.szRctHdr3,strActualValue,strlen(strActualValue));
            
        }
        else
        {
            vduiClearBelow(3);  
            vduiDisplayStringCenter(4,"NO VALUE ENTERED");
            vduiDisplayStringCenter(5,"SAVE EMPTY?");
            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
            while(1)
            {
                inRet=struiGetchWithTimeOut();
                if (inRet==d_KBD_ENTER)
                {
                    memset(strPCT.szRctHdr3,0x00, sizeof(strPCT.szRctHdr3));    
                    break;
                }
                else if (inRet==d_KBD_CANCEL)
                    break;
                else
                    vduiWarningSound();
            }
        }
    }         
        
      
        
    //---------HEADER LINE 4---------------------//    
    vduiClearBelow(2);  
    vduiDisplayStringCenter(1, "ADDRESS HEADER");
    CTOS_LCDTPrintXY(1, 2, "OLD HEADER LINE4:           ");
    CTOS_LCDTPrintXY(1, 3, strPCT.szRctHdr4);
    CTOS_LCDTPrintXY(1, 4, &strPCT.szRctHdr4[16]);
    CTOS_LCDTPrintXY(1, 5, &strPCT.szRctHdr4[32]);
    CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE4:");
    memset(strBuff,0x00, sizeof(strBuff));  
    memset(strActualValue,0x00, sizeof(strActualValue));
    
    inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);

    if(inRet==d_OK)
    {
        if (strlen(strBuff)>0)
        {
            memcpy(strActualValue,strBuff,strlen(strBuff));

            if (strlen(strBuff)>=15)
            {
                vduiClearBelow(2);  
                vduiDisplayStringCenter(4,"CONTINUE INPUT");
                vduiDisplayStringCenter(5,"MORE TEXT?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");

                while(1)
                {
                    inRet=struiGetchWithTimeOut();
                    if (inRet==d_KBD_ENTER)
                    {
                        vduiClearBelow(2);  
                        CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE4:");
                        memset(strBuff,0x00, sizeof(strBuff));
                        
                        vduiClearBelow(7);  
                        CTOS_LCDTPrintXY(1, 7, strActualValue);
                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                        if(inRet==d_OK)
                        {
                            if (strlen(strBuff)>0)
                            {
                                memcpy(&strActualValue[15],strBuff,strlen(strBuff));
                                if (strlen(strBuff)>=15)
                                {
                                    vduiClearBelow(2);  
                                    vduiDisplayStringCenter(4,"CONTINUE INPUT");
                                    vduiDisplayStringCenter(5,"MORE TEXT?");
                                    vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                                    inRet=struiGetchWithTimeOut();
                                    if (inRet==d_KBD_ENTER)
                                    {
                                        vduiClearBelow(2);
                                        CTOS_LCDTPrintXY(1, 5, "ENTR NEW LINE4:");
                                        memset(strBuff,0x00, sizeof(strBuff));                                                                  
                                        CTOS_LCDTPrintXY(1, 6, strActualValue);
                                        CTOS_LCDTPrintXY(1, 7, &strActualValue[16]);
                                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                                        if(inRet==d_OK)
                                        {    
                                           memcpy(&strActualValue[30],strBuff,strlen(strBuff));
                                        }
                                        break;
                                    }
                                    else if (inRet==d_KBD_CANCEL)
                                        break;
                                    else
                                            vduiWarningSound();
                                }
                            }
                            else
                                break;
                                                
                        }
                        break;
                    }
                    else if (inRet==d_KBD_CANCEL)
                        break;
                    else
                        vduiWarningSound();
                }
            }
            memset(strPCT.szRctHdr4,0x00, sizeof(strPCT.szRctHdr4));    
            memcpy(strPCT.szRctHdr4,strActualValue,strlen(strActualValue));
            
        }
        else
        {
            vduiClearBelow(3);  
            vduiDisplayStringCenter(4,"NO VALUE ENTERED");
            vduiDisplayStringCenter(5,"SAVE EMPTY?");
            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
            while(1)
            {
                inRet=struiGetchWithTimeOut();
                if (inRet==d_KBD_ENTER)
                {
                    memset(strPCT.szRctHdr4,0x00, sizeof(strPCT.szRctHdr4));    
                    break;
                }
                else if (inRet==d_KBD_CANCEL)
                    break;
                else
                    vduiWarningSound();
            }
        }
    }   
        
        
        
   //---------HEADER LINE 5---------------------//    
    vduiClearBelow(2);  
    vduiDisplayStringCenter(1, "ADDRESS HEADER");
    CTOS_LCDTPrintXY(1, 2, "OLD HEADER LINE5:           ");
    CTOS_LCDTPrintXY(1, 3, strPCT.szRctHdr5);
    CTOS_LCDTPrintXY(1, 4, &strPCT.szRctHdr5[16]);
    CTOS_LCDTPrintXY(1, 5, &strPCT.szRctHdr5[32]);
    CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE5:");
    memset(strBuff,0x00, sizeof(strBuff));  
    memset(strActualValue,0x00, sizeof(strActualValue));
    
    inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);

    if(inRet==d_OK)
    {
        if (strlen(strBuff)>0)
        {
            memcpy(strActualValue,strBuff,strlen(strBuff));

            if (strlen(strBuff)>=15)
            {
                vduiClearBelow(2);  
                vduiDisplayStringCenter(4,"CONTINUE INPUT");
                vduiDisplayStringCenter(5,"MORE TEXT?");
                vduiDisplayStringCenter(8,"NO[X] YES[OK]");

                while(1)
                {
                    inRet=struiGetchWithTimeOut();
                    if (inRet==d_KBD_ENTER)
                    {
                        vduiClearBelow(2);  
                        CTOS_LCDTPrintXY(1, 6, "ENTR NEW LINE5:");
                        memset(strBuff,0x00, sizeof(strBuff));
                        
                        vduiClearBelow(7);  
                        CTOS_LCDTPrintXY(1, 7, strActualValue);
                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                        if(inRet==d_OK)
                        {
                            if (strlen(strBuff)>0)
                            {
                                memcpy(&strActualValue[15],strBuff,strlen(strBuff));
                                if (strlen(strBuff)>=15)
                                {
                                    vduiClearBelow(2);  
                                    vduiDisplayStringCenter(4,"CONTINUE INPUT");
                                    vduiDisplayStringCenter(5,"MORE TEXT?");
                                    vduiDisplayStringCenter(8,"NO[X] YES[OK]");
                                    inRet=struiGetchWithTimeOut();
                                    if (inRet==d_KBD_ENTER)
                                    {
                                        vduiClearBelow(2);
                                        CTOS_LCDTPrintXY(1, 5, "ENTR NEW LINE5:");
                                        memset(strBuff,0x00, sizeof(strBuff));                                                                  
                                        CTOS_LCDTPrintXY(1, 6, strActualValue);
                                        CTOS_LCDTPrintXY(1, 7, &strActualValue[16]);
                                        inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,strBuff, 16);
                                        if(inRet==d_OK)
                                        {    
                                           memcpy(&strActualValue[30],strBuff,strlen(strBuff));
                                        }
                                        break;
                                    }
                                    else if (inRet==d_KBD_CANCEL)
                                        break;
                                    else
                                            vduiWarningSound();
                                }
                            }
                            else
                                break;
                                                
                        }
                        break;
                    }
                    else if (inRet==d_KBD_CANCEL)
                        break;
                    else
                        vduiWarningSound();
                }
            }
            memset(strPCT.szRctHdr5,0x00, sizeof(strPCT.szRctHdr5));    
            memcpy(strPCT.szRctHdr5,strActualValue,strlen(strActualValue));
            
        }
        else
        {
            vduiClearBelow(3);  
            vduiDisplayStringCenter(4,"NO VALUE ENTERED");
            vduiDisplayStringCenter(5,"SAVE EMPTY?");
            vduiDisplayStringCenter(8,"NO[X] YES[OK]");
            while(1)
            {
                inRet=struiGetchWithTimeOut();
                if (inRet==d_KBD_ENTER)
                {
                    memset(strPCT.szRctHdr5,0x00, sizeof(strPCT.szRctHdr5));    
                    break;
                }
                else if (inRet==d_KBD_CANCEL)
                    break;
                else
                    vduiWarningSound();
            }
        }
    }         
        
    inPCTSave(1);
}

void vdCTOSS_DownloadMode(void)
{
	CTOS_LCDTClearDisplay();
	CTOS_EnterDownloadMode();

	inCTOS_KillALLSubAP();
	
	exit(0);
	return ;
}

void vdCTOSS_CheckMemory(void)
{
	vdCTOSS_GetMemoryStatus("MEMORY");
	return ;
}

/* ==========================================================================
 *
 * FUNCTION NAME: SetRTC
 *
 * DESCRIPTION:   Use this function to set the real-time clock's data and time.
 *
 * RETURN:        none.
 *
 * NOTES:         none.
 *
 * ========================================================================== */
void CTOSS_SetRTC(void)
{
	//Declare Local Variable //
	CTOS_RTC SetRTC;
	USHORT i;
	BYTE isSet = FALSE;
	BYTE baYear[4+1],baMonth[2+1],baDay[2+1],baHour[2+1],baMinute[2+1],baSecond[2+1];
	BYTE key;
	BYTE babuff[d_BUFF_SIZE];

	CTOS_LCDTClearDisplay();
	CTOS_LCDTSetReverse(TRUE);
	CTOS_LCDTPrintXY(1, 1, "     Set RTC     ");
	CTOS_LCDTSetReverse(FALSE);

	//Read the date and the time //
	CTOS_RTCGet(&SetRTC);

	//Show on the LCD Dispaly //
	CTOS_LCDTPrintXY(1, 2, "   Get   Set");
	sprintf(babuff,"YY:%04d",SetRTC.bYear + 2000);
	CTOS_LCDTPrintXY(1, 3, babuff);
	sprintf(babuff,"MM:%02d",SetRTC.bMonth);
	CTOS_LCDTPrintXY(1, 4, babuff);
	sprintf(babuff,"DD:%02d",SetRTC.bDay);
	CTOS_LCDTPrintXY(1, 5, babuff);
	sprintf(babuff,"hh:%02d",SetRTC.bHour);
	CTOS_LCDTPrintXY(1, 6, babuff);
	sprintf(babuff,"mm:%02d",SetRTC.bMinute);
	CTOS_LCDTPrintXY(1, 7, babuff);
	sprintf(babuff,"ss:%02d",SetRTC.bSecond);
	CTOS_LCDTPrintXY(1, 8, babuff);

	sprintf(babuff,"%02d",SetRTC.bDoW);
	CTOS_LCDTPrintXY(15, 8, babuff);

	//Input data for the setting //
	i = sizeof(baYear);
  	if (InputString(10,3,FALSE, 0x02, baYear, &i, i, d_INPUT_TIMEOUT) == 1){    //Input Year //
    	SetRTC.bYear = (wub_str_2_long(baYear) - 2000);
    	isSet = TRUE;
  	}
  	i = sizeof(baMonth);
  	if (InputString(10,4,FALSE, 0x02, baMonth, &i, i, d_INPUT_TIMEOUT) == 1){   //Input Month //
    	SetRTC.bMonth = wub_str_2_long(baMonth);
    	isSet = TRUE;
  	}
  	i = sizeof(baDay);
  	if (InputString(10,5,FALSE, 0x02, baDay, &i, i, d_INPUT_TIMEOUT) == 1){	  //Input Day //
    	SetRTC.bDay = wub_str_2_long(baDay);
    	isSet = TRUE;
  	}
  	i = sizeof(baHour);
  	if (InputString(10,6,FALSE, 0x02, baHour, &i, i, d_INPUT_TIMEOUT) == 1){  //Input Hour //
    	SetRTC.bHour = wub_str_2_long(baHour);
    	isSet = TRUE;
  	}
  	i = sizeof(baMinute);
  	if (InputString(10,7,FALSE, 0x02, baMinute, &i, i, d_INPUT_TIMEOUT) == 1){  //Input Minute //
    	SetRTC.bMinute = wub_str_2_long(baMinute);
    	isSet = TRUE;
  	}
  	i = sizeof(baSecond);
  	if (InputString(10,8,FALSE, 0x02, baSecond, &i, i, d_INPUT_TIMEOUT) == 1){  //Input Second //
    	SetRTC.bSecond = wub_str_2_long(baSecond);
    	isSet = TRUE;
  	}
  
  	if (isSet){
    	//Set the date and time //
    	if (CTOS_RTCSet(&SetRTC) == d_OK) CTOS_LCDTPrintXY(15, 2, "OK");
   	 	else CTOS_LCDTPrintXY(13, 2, "Fail");
    	isSet = FALSE;
  	}
  
//  	CTOS_KBDGet ( &key );
  	return;
}
void vdCTOSS_FileUpload(void)
{
    int st, uintRet;
	unsigned char szDisplay[40];
    unsigned char temp[1024];
    unsigned char public_path[50] = "/data/data/pub/";
    unsigned char private_path[50] = "/data/data/pub/";
    FILE  *fPubKey;
    unsigned char szFileName1[50];
	char szFileName[50];
	ULONG ulSize;
	int iExp;
    STR *keyboardLayoutEnglish[]={" 0", "qzQZ1", "abcABC2", "defDEF3", "ghiGHI4",
    "jklJKL5", "mnoMNO6", "prsPRS7", "tuvTUV8", "wxyWXY9", ":;/\\|?,.<>_",
    ".!@#$%^&*()-"};
    int inRet;

	CTOS_LCDTClearDisplay ();                                                                                
                                                                                                           
                               
    CTOS_LCDTSetReverse(FALSE);    
    CTOS_USBOpen();
    
    CTOS_LCDTPrintXY(1,1,"ENTER FILENAME:");
    inRet=CTOS_UIKeypad(1, 8, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0,szFileName1, 50);
    if(inRet==d_OK)
    {
        if (strlen(szFileName1)>0)
		{
            memset(szFileName, 0x00, sizeof(szFileName));
            sprintf(szFileName,"%s%s", public_path,szFileName1);
        }
        else
        {
             CTOS_LCDTPrintXY(1,1,"TRANSFER CANCEL");
             WaitKey(3);
             return;
        }
    }
    else
    {
        CTOS_LCDTPrintXY(1,1,"TRANSFER CANCEL");
        WaitKey(3);
        return;
    }
    //strcpy(szFileName1,"V5S_VISAMASTER.prm");
    //sprintf(szFileName,"%s%s", public_path,szFileName1);
    
    ulSize = 0;
	fPubKey = fopen( (char*)szFileName1, "rb");
	if (fPubKey == NULL)
	{
        CTOS_LCDTPrintXY(1,1,"OPEN FILE ERROR");
        CTOS_LCDTPrintXY(1,2,szFileName1);
        //CTOS_LCDTPrintXY(1,3,&szFileName[19]);
        WaitKey(5);
		//memset(szFileName, 0x00, sizeof(szFileName));
        //sprintf(szFileName,"%s%s", private_path,szFileName1);
        fPubKey = fopen( (char*)szFileName, "rb");
        if (fPubKey == NULL)
        {
            CTOS_LCDTPrintXY(1,1,"OPEN FILE ERROR");
            CTOS_LCDTPrintXY(1,2,szFileName);
            CTOS_LCDTPrintXY(1,3,&szFileName[19]);
            
            memset(szFileName, 0x00, sizeof(szFileName));
            sprintf(szFileName,"%s%s", private_path,szFileName1);
            fPubKey = fopen( (char*)szFileName, "rb");
            if (fPubKey == NULL)
            {
                CTOS_LCDTPrintXY(1,1,"OPEN FILE ERROR");
                CTOS_LCDTPrintXY(1,2,szFileName);
                CTOS_LCDTPrintXY(1,3,&szFileName[19]);
                WaitKey(5);
                return;
            }
        }
		
	}
    
    CTOS_LCDTPrintXY(1,1,"                 ");
    CTOS_LCDTPrintXY(1,2,"                 ");
    CTOS_LCDTPrintXY(1,3,"                 ");
	CTOS_LCDTPrintXY(1,1,"Transfer Size");
	while(1)
	{
		uintRet = fread( temp, 1, sizeof(temp), fPubKey);
		ulSize += uintRet;
		sprintf(szDisplay,"%ld", ulSize);
		CTOS_LCDTPrintXY(1,2,szDisplay);
		if(uintRet > 0)
			CTOS_USBTxData(temp, uintRet);
		if(uintRet <=0)
			break;
		CTOS_Delay(5);
	}
	fclose(fPubKey); 
	CTOS_LCDTPrintXY(1,3,"File Transmitted");
	WaitKey(3); 
    /*if(remove(szFileName1) == -1)
    {
        CTOS_LCDTPrintXY(1,3,"File Delete Error");
   
    }
    else CTOS_LCDTPrintXY(1,3,"File Deleted");
    WaitKey(3);*/
}

int inCTOS_PromptPassword(void)
{
    int inRet = d_NO;
    
    CTOS_LCDTClearDisplay();
    vduiClearBelow(7);
    vdCTOS_SetTransType(SETUP);
    
    vdDispTransTitle(SETUP);
    
    inRet = inCTOSS_CheckMemoryStatus();
    if(d_OK != inRet)
        return inRet;
    
    inRet = inCTOS_GetTxnPassword();
    
    if(d_OK != inRet)
    {
        inCTOS_IdleEventProcess();
        return inRet;
    }
    
    return d_OK;
}

void vdCTOS_TMSSetting(void)
{
	BYTE strOut[30],strtemp[17];
	BYTE szInputBuf[5];
    int inResult;
    int ret;
	USHORT usLen;

    CTOS_LCDTClearDisplay();
    vdDispTitleString("TMS SETTING");

    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	
    while(1)
    {   
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS GAP");
        memset(szInputBuf, 0x00, sizeof(szInputBuf));
        sprintf(szInputBuf, "%d", strTCT.usTMSGap);
        setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 3, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            strTCT.usTMSGap=atoi(strOut);      
            vdMyEZLib_LogPrintf("strTCT.usTMSGap %d",strTCT.usTMSGap);
			inResult = inTCTSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }	

    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	
    while(1)
    {   
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS COMM MODE");
		setLCDPrint(4, DISPLAY_POSITION_LEFT, "0-DIALUP 1-ETH");
		setLCDPrint(5, DISPLAY_POSITION_LEFT, "2-GPRS");
        memset(szInputBuf, 0x00, sizeof(szInputBuf));
        sprintf(szInputBuf, "%d", strTCT.inTMSComMode);
        setLCDPrint(6, DISPLAY_POSITION_LEFT, szInputBuf);
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            strTCT.inTMSComMode=atoi(strOut);      
            vdMyEZLib_LogPrintf("strTCT.usTMSGap %d",strTCT.inTMSComMode);
			inResult = inTCTSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }	
    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	
    while(1)
    {
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE IP");
        setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemoteIP);
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strtemp, 0x00, sizeof(strtemp));
        memset(strOut,0x00, sizeof(strOut));
        ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
        if(ret==d_KBD_ENTER)
        {
            memcpy(strTCT.szTMSRemoteIP,strOut,strlen(strOut));
            strTCT.szTMSRemoteIP[strlen(strOut)]=0;
            vdMyEZLib_LogPrintf("strTCT.szTMSRemoteIP %s",strTCT.szTMSRemoteIP);
			inResult = inTCTSave(1);
            break;
        }	
        if(ret == d_KBD_CANCEL)
            break;
    }
	
    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;	
    while(1)
    {   
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE PORT");
        memset(szInputBuf, 0x00, sizeof(szInputBuf));
        sprintf(szInputBuf, "%d", strTCT.usTMSRemotePort);
        setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 5, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            break;
        else if(0 == ret )
            break;
        else if(ret>=1)
        {
            strTCT.usTMSRemotePort=atoi(strOut);      
            vdMyEZLib_LogPrintf("strTCT.usTMSRemotePort %d",strTCT.usTMSRemotePort);
			inResult = inTCTSave(1);
            break;
        }   
        if(ret == d_KBD_CANCEL)
            break;
    }	

    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;
	while(1)
	{
		vduiClearBelow(3);
		setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE PHONE");
		setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemotePhone);
	
		strcpy(strtemp,"New:") ; 
		CTOS_LCDTPrintXY(1, 7, strtemp);
		memset(strOut,0x00, sizeof(strOut));
		usLen = 32;
		ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
		if (ret == d_KBD_CANCEL )
			break;
		else if(0 == ret )
			break;
		else if(ret>= 1)
		{
			memcpy(strTCT.szTMSRemotePhone,strOut,strlen(strOut));
			strTCT.szTMSRemotePhone[strlen(strOut)]=0;
			inResult = inTCTSave(1);
			break;
		}
		if(ret == d_KBD_CANCEL)
			break;
	}

    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;
	while(1)
	{
		vduiClearBelow(3);
		setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE ID");
		setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemoteID);
	
		strcpy(strtemp,"New:") ; 
		CTOS_LCDTPrintXY(1, 7, strtemp);
		memset(strOut,0x00, sizeof(strOut));
		usLen = 32;
		//ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
		ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
		if (ret == d_KBD_CANCEL )
			break;
		else if(0 == ret )
			break;
		else if(ret>= 1)
		{
			memcpy(strTCT.szTMSRemoteID,strOut,strlen(strOut));
			strTCT.szTMSRemoteID[strlen(strOut)]=0;
			inResult = inTCTSave(1);
			break;
		}
		if(ret == d_KBD_CANCEL)
			break;
	}
	
    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return;
	while(1)
	{
		vduiClearBelow(3);
		setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE PASSWORD");
		setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemotePW);
	
		strcpy(strtemp,"New:") ; 
		CTOS_LCDTPrintXY(1, 7, strtemp);
		memset(strOut,0x00, sizeof(strOut));
		usLen = 32;
		//ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
		ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
		if (ret == d_KBD_CANCEL )
			break;
		else if(0 == ret )
			break;
		else if(ret>= 1)
		{
			memcpy(strTCT.szTMSRemotePW,strOut,strlen(strOut));
			strTCT.szTMSRemotePW[strlen(strOut)]=0;
			inResult = inTCTSave(1);
			break;
		}
		if(ret == d_KBD_CANCEL)
			break;
	
	}

	
}

int  inCTOS_TMSPreConfigSetting(void)
{
	BYTE strOut[30],strtemp[17];
	BYTE szInputBuf[5];
    int inResult;
    int ret;
	USHORT usLen;

    CTOS_LCDTClearDisplay();
    vdDispTitleString("TMS SETTING");

    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return d_NO;	
	
    while(1)
    {   
        vduiClearBelow(3);
        setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS COMM MODE");
		setLCDPrint(4, DISPLAY_POSITION_LEFT, "0-DIALUP 1-ETH");
		setLCDPrint(5, DISPLAY_POSITION_LEFT, "2-GPRS");
        memset(szInputBuf, 0x00, sizeof(szInputBuf));
        sprintf(szInputBuf, "%d", strTCT.inTMSComMode);
        setLCDPrint(6, DISPLAY_POSITION_LEFT, szInputBuf);
        
        strcpy(strtemp,"New:") ;
        CTOS_LCDTPrintXY(1, 7, strtemp);
        memset(strOut,0x00, sizeof(strOut));
        ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 1, 0, d_INPUT_TIMEOUT);
        if (ret == d_KBD_CANCEL )
            return d_NO;
        else if(0 == ret )
            return d_NO;
        else if(ret>=1)
        {
            strTCT.inTMSComMode=atoi(strOut);      
            vdMyEZLib_LogPrintf("strTCT.usTMSGap %d",strTCT.inTMSComMode);
			inResult = inTCTSave(1);
            break;
        }   
    }	
	
    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return  d_NO;	

    if(strTCT.inTMSComMode == DIAL_UP_MODE)
    {
		if(strlen(strTCT.szTMSRemotePhone) == 0)
		{
            while(1)
            {
                vduiClearBelow(3);
                setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE PHONE");
                setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemotePhone);
                
                strcpy(strtemp,"New:") ; 
                CTOS_LCDTPrintXY(1, 7, strtemp);
                memset(strOut,0x00, sizeof(strOut));
                usLen = 32;
                ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
                if (ret == d_KBD_CANCEL )
                    return d_NO;
                else if(0 == ret )
                    return d_NO;
                else if(ret>= 1)
                {
                    memcpy(strTCT.szTMSRemotePhone,strOut,strlen(strOut));
                    strTCT.szTMSRemotePhone[strlen(strOut)]=0;
                    inResult = inTCTSave(1);
                    break;
                }
            }
		}
    
	
        inResult = inTCTRead(1);
        if(inResult != d_OK)
            return d_NO;

		if(strlen(strTCT.szTMSRemoteID) == 0)
		{
        	while(1)
        	{
        		vduiClearBelow(3);
        		setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE ID");
        		setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemoteID);
        	
        		strcpy(strtemp,"New:") ; 
        		CTOS_LCDTPrintXY(1, 7, strtemp);
        		memset(strOut,0x00, sizeof(strOut));
        		usLen = 32;
        		//ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
        		ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
        		if (ret == d_KBD_CANCEL )
        			return d_NO;
        		else if(0 == ret )
        			return d_NO;
        		else if(ret>= 1)
        		{
        			memcpy(strTCT.szTMSRemoteID,strOut,strlen(strOut));
        			strTCT.szTMSRemoteID[strlen(strOut)]=0;
        			inResult = inTCTSave(1);
        			break;
        		}
        	}
		}
		
        inResult = inTCTRead(1);
        if(inResult != d_OK)
            return d_NO;

        if(strlen(strTCT.szTMSRemotePW) == 0)
        {
        	while(1)
        	{
        		vduiClearBelow(3);
        		setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE PASSWORD");
        		setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemotePW);
        	
        		strcpy(strtemp,"New:") ; 
        		CTOS_LCDTPrintXY(1, 7, strtemp);
        		memset(strOut,0x00, sizeof(strOut));
        		usLen = 32;
        		//ret = InputString(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
        		ret= InputStringAlpha(1, 8, 0x00, 0x02, strOut, &usLen, 1, d_INPUT_TIMEOUT);
        		if (ret == d_KBD_CANCEL )
        			return d_NO;
        		else if(0 == ret )
        			return d_NO;
        		else if(ret>= 1)
        		{
        			memcpy(strTCT.szTMSRemotePW,strOut,strlen(strOut));
        			strTCT.szTMSRemotePW[strlen(strOut)]=0;
        			inResult = inTCTSave(1);
        			break;
        		}
        	}
        }
		
        inResult = inTCTRead(1);
        if(inResult != d_OK)
            return d_NO;		
	}

    if(strlen(strTCT.szTMSRemoteIP) == 0)
    {
        while(1)
        {
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE IP");
            setLCDPrint(4, DISPLAY_POSITION_LEFT, strTCT.szTMSRemoteIP);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strtemp, 0x00, sizeof(strtemp));
            memset(strOut,0x00, sizeof(strOut));
            ret= struiApiGetStringSub(strtemp, 1,  8, strOut, MODE_IPADDRESS, 7, IP_LEN);
            if(ret==d_KBD_ENTER)
            {
                memcpy(strTCT.szTMSRemoteIP,strOut,strlen(strOut));
                strTCT.szTMSRemoteIP[strlen(strOut)]=0;
                vdMyEZLib_LogPrintf("strTCT.szTMSRemoteIP %s",strTCT.szTMSRemoteIP);
                inResult = inTCTSave(1);
                break;
            }	
            if(ret == d_KBD_CANCEL)
                return d_NO;
        }
    }
    
    inResult = inTCTRead(1);
    if(inResult != d_OK)
    return d_NO;	
    
    if(strTCT.usTMSRemotePort == 0)
    {
        while(1)
        {   
            vduiClearBelow(3);
            setLCDPrint(3, DISPLAY_POSITION_LEFT, "TMS REMOTE PORT");
            memset(szInputBuf, 0x00, sizeof(szInputBuf));
            sprintf(szInputBuf, "%d", strTCT.usTMSRemotePort);
            setLCDPrint(4, DISPLAY_POSITION_LEFT, szInputBuf);
            
            strcpy(strtemp,"New:") ;
            CTOS_LCDTPrintXY(1, 7, strtemp);
            memset(strOut,0x00, sizeof(strOut));
            ret= shCTOS_GetNum(8, 0x01,  strOut, &usLen, 1, 5, 0, d_INPUT_TIMEOUT);
            if (ret == d_KBD_CANCEL )
                return d_NO;
            else if(0 == ret )
                return d_NO;
            else if(ret>=1)
            {
                strTCT.usTMSRemotePort=atoi(strOut);      
                vdMyEZLib_LogPrintf("strTCT.usTMSRemotePort %d",strTCT.usTMSRemotePort);
                inResult = inTCTSave(1);
                break;
            }   
        }	
    }
	
    inResult = inTCTRead(1);
    if(inResult != d_OK)
        return d_NO;

    return d_OK;
}

int inCTOS_CommsFallback(int shHostIndex)
{
	int inResult = 0;

	vdDebug_LogPrintf("inCTOS_CommsFallback, Host Index [%d]", shHostIndex);
	vdDebug_LogPrintf("srTransRec.usTerminalCommunicationMode [%d]", srTransRec.usTerminalCommunicationMode);
	vdDebug_LogPrintf("strCPT.inCommunicationMode [%d]", strCPT.inCommunicationMode);

	vduiClearBelow(3);
	CTOS_LCDTPrintXY (1,6, "Comms Fallback");
	CTOS_LCDTPrintXY (1,7, "Please Wait 	");

	if ((srTransRec.usTerminalCommunicationMode == ETHERNET_MODE) || (srTransRec.usTerminalCommunicationMode == GPRS_MODE))
	{
		CTOS_LCDTPrintXY(1, 8, "Init Modem... 	");
		srTransRec.usTerminalCommunicationMode = DIAL_UP_MODE;
	}
	//else
	//{
		//CTOS_LCDTPrintXY(1, 8, "Init IP...	  ");
		//srTransRec.usTerminalCommunicationMode = ETHERNET_MODE;
	//}
	
	if (inCTOS_InitComm(srTransRec.usTerminalCommunicationMode) != d_OK) 
	{
		vdDebug_LogPrintf("inCTOS_CommsFallback, COMM INIT ERR");
		vdDisplayErrorMsg(1, 8, "COMM INIT ERR");
		return d_NO;
	}

	inCTOS_CheckInitComm(srTransRec.usTerminalCommunicationMode);

	vdDebug_LogPrintf("vdCTOS_DialBackupConfig, inCTOS_CheckInitComm");

	if (srTransRec.byOffline == CN_FALSE)
	{
		inResult = srCommFuncPoint.inCheckComm(&srTransRec);
	}

	vduiClearBelow(3);

	return d_OK;	
}


//hubing enhance ECR
int get_env_int (char *tag)
{
	int     ret = -1;
	char    buf[64];

    memset (buf, 0, sizeof (buf));
    if ( inCTOSS_GetEnvDB (tag, buf) == d_OK )
    {
        ret = atoi (buf);
    }

	vdDebug_LogPrintf("get_env_int [%s]=[%d]", tag, ret);

    return ret;
}
//hubing enhance ECR
void put_env_int(char *tag, int value)
{
	int     ret = -1;
	char    buf[64];

    memset (buf, 0, sizeof (buf));
    //int2str (buf, value);
    sprintf(buf, "%d", value);
    ret = inCTOSS_PutEnvDB (tag, buf);

	vdDebug_LogPrintf("put_env_int [%s]=[%d] ret[%d]", tag, value, ret);
}


void vdCTOSS_EditTable(void)
{
	BYTE strOut[100];
	BYTE szdatabase[100];
	BYTE sztable[100];
    USHORT usLen;
	USHORT ret;
	char szDispay[50];

	memset(szDispay,0x00,sizeof(szDispay));
	memset(szdatabase,0x00,sizeof(szdatabase));
	memset(sztable,0x00,sizeof(sztable));
	sprintf(szDispay,"EDIT DATABASE");
	CTOS_LCDTClearDisplay();
	
    vdDispTitleString(szDispay);            
	CTOS_LCDTPrintXY(1, 3, "DATABASE NAME:");
	
	usLen = 20;
	CTOS_LCDFontSelectMode(d_FONT_FNT_MODE);
    ret = InputStringAlphaEx(1, 7, 0x00, 0x02, szdatabase, &usLen, 1, d_INPUT_TIMEOUT);
//	if (strTCT.inThemesType == 1)
//	{
//		CTOS_LCDTTFSelect("tahoma.ttf", 0);
//		CTOS_LCDTTFSwichDisplayMode(d_TTF_MODE_VIETNAM);
//	}
//	else
	{
		CTOS_LCDTTFSelect(d_FONT_DEFAULT_TTF, 0);
		CTOS_LCDTTFSwichDisplayMode(d_TTF_MODE_DEFAULT);
	}

	if (ret == d_KBD_CANCEL || 0 == ret )
	{
		return ;
	}
	if(ret>= 1)
    {
    	vduiClearBelow(2);
        vdDebug_LogPrintf("szdatabase[%s].usLen=[%d].",szdatabase,usLen);
		CTOS_LCDTPrintXY(1, 3, "TABLE NAME:");
	
		usLen = 20;
		CTOS_LCDFontSelectMode(d_FONT_FNT_MODE);
	    ret = InputStringAlphaEx(1, 7, 0x00, 0x02, sztable, &usLen, 1, d_INPUT_TIMEOUT);
//		if (strTCT.inThemesType == 1)
//		{
//			CTOS_LCDTTFSelect("tahoma.ttf", 0);
///			CTOS_LCDTTFSwichDisplayMode(d_TTF_MODE_VIETNAM);
//		}
//		else
		{
			CTOS_LCDTTFSelect(d_FONT_DEFAULT_TTF, 0);
			CTOS_LCDTTFSwichDisplayMode(d_TTF_MODE_DEFAULT);
		}
		
		if (ret == d_KBD_CANCEL || 0 == ret )
		{
			return ;
		}
		if(ret>= 1)
	    {
	        vdDebug_LogPrintf("sztable[%s].usLen=[%d].szdatabase[%s]",sztable,usLen,szdatabase);
			inCTOSS_EditTalbe(sztable,szdatabase);
	    }
	
    }
	
}

void put_env_char(char *tag, char *value)
{
	int     ret = -1;
	char    buf[12];

    memset (buf, 0, sizeof (buf));
    //int2str (buf, value);
    sprintf(buf, "%s", value);
    ret = inCTOSS_PutEnvDB (tag, buf);

	vdDebug_LogPrintf("put_env_char [%s]=[%s] ret[%d]", tag, value, ret);
}

