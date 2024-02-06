#include "ch32v20x.h"
#include <string.h>
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "arch.h"
#include "f_idle.h"
#include "main.h"

int f_idle(unsigned *pMsg)
{
    u8Data_t u8Data;
    switch(((msg_t *)pMsg)->msgType) 
    {
    case CMSG_TMR:
        g_tick++;
        if ((g_tick % 30) == 5) {  /** updata battery level every 30sec **/
            (void)reportBatteryLevel(sysvar.BAT_soc);
        }
        // SetTimer_irq(&g_timer[2], TIMER_30SEC, CMSG_TMR);  
      //???????????????????????????????????????
      #if 0
      int len = 0;
      //jsonTL_t* p = getReportCmdbyMode(g_tick & 0x1f); //????????????
      jsonTL_t* p = getGetCharCmdbyMode(g_tick & 0x1f); //????????????
      if (p != NULL) {
          /** head **/
          len = strlen(p->jHead);
          for (int i = 0; i < len; i++) {
               u8Data.u8Val = p->jHead[i];
               u8FIFOin_irq(&g_uart3TxQue, &u8Data);  //?????????????
          }
          u8Data.u8Val = ',';
          u8FIFOin_irq(&g_uart3TxQue, &u8Data);  //?????????????
          
          /** length **/
          len = strlen(p->jBody);
          u8Data.u8Val = len;
          u8FIFOin_irq(&g_uart3TxQue, &u8Data);  //?????????????
          
          u8Data.u8Val = ',';
          u8FIFOin_irq(&g_uart3TxQue, &u8Data);  //?????????????
      
          /** body **/
          for (int i = 0; i < len; i++) {
              u8Data.u8Val = p->jBody[i];
              u8FIFOin_irq(&g_uart3TxQue, &u8Data);  //?????????????
          }
      }
      #else
      //(void)reportScanWifi(NULL);
      //(void)reportConnectWifi(NULL);
      #endif
	  //???????????????????????????????????????
        break;
    case CMSG_DKEY:
        if (Mget_bit(g_flag, 1)) {  /** double click...  ok **/
            Mreset_bit(g_flag, 1);
            ClrTimer_irq(&g_timer[2]);
            if (((sysvar.keyfang & KeyInstallState)&&(sysvar.keyfang & KeyLongFang))/** set mode  **/) {
                (void)reportResetNet(0);
            }
        } else  { /**  **/
            Mset_bit(g_flag, 1);
            SetTimer_irq(&g_timer[2], TIMER_500MS, CDOUBLE_CLICK);
        }
        break;
    
    case CDOUBLE_CLICK:
        Mreset_bit(g_flag, 1);
        ClrTimer_irq(&g_timer[2]);
        break;
        
    case CSYS_INIT:
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
		break;
        
    case CMSG_INIT:
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
		break;
    default:
		break;
	}  

    return  0;
}

int f_init(unsigned *pMsg)
{
}


