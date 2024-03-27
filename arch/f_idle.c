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
    u8 cycle = 10;
    switch(((msg_t *)pMsg)->msgType) 
    {
    case CMSG_TMR:
        g_tick++;
        if (sysvar.sysfang & OFF_ON) {
            //cycle = 20;
        } else {
            cycle = 10;
        }
        
        /******************************************************************************************
         * ³äµç×´Ì¬
         ******************************************************************************************/
        if ((g_tick % cycle) == 0) {  /** updata battery level every CYCLE sec **/
            checkAndReportChargeStatus();
        }
        #if 0
        if (1 || ((g_tick % 30) == 5)) {  /** updata battery level every 30sec **/
            u8 charge_status;
            if (checkChargeChange(&charge_status) == TRUE) {
                reportComponentStatus(charge_status);
            } else {
                if (charge_status == CINDEX_UNCHARGED) {
                    (void)reportBatteryLevel(sysvar.BAT_soc);
                } else {
                    reportComponentStatus(charge_status);
                }
            }
        }
        #endif
      /******************************************************************************************
       * ¹¤×÷×´Ì¬ÉÏ±¨
       ******************************************************************************************/
      if ((g_tick % cycle) == 1) {  /** updata every CYCLE sec **/
          checkAndReportWorkMode();
      }
      /******************************************************************************************
       * ¹öÍ²×´Ì¬
       ******************************************************************************************/
      if ((g_tick % cycle) == 2) {  /** updata every CYCLE sec **/
          checkAndReportRollerStatus();
      }
      /******************************************************************************************
       * Ë®±Ã×´Ì¬
       ******************************************************************************************/
      if ((g_tick % cycle) == 3) {  /** updata every CYCLE sec **/
          checkAndReportPumpStatus();
      }
      /******************************************************************************************
       * µç³Ø×´Ì¬
       ******************************************************************************************/
      if ((g_tick % cycle) == 4) {  /** updata every CYCLE sec **/
          checkAndReportBatteryStatus();
      }
      /******************************************************************************************
       * ÇåË®×´Ì¬
       ******************************************************************************************/
      if ((g_tick % cycle) == 5) {  /** updata every CYCLE sec **/
          checkAndReportClearWaterStatus();
      }

      if ((g_tick % cycle) == 6) {  /** updata every CYCLE sec **/
          checkAndReportConnectionStatus();
      }
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
        if (g_flag & (1 << 1)) {  /** double click...  ok **/
            //Mreset_bit(g_flag,1);
            g_flag &= (~(1 << 1));
            ClrTimer_irq(&g_timer[2]);
   
            switch (IDs_.Equipment){
            case f_DragLala:
            case f_Spray:
            case f_Aspiration_Brush:
            case f_Feather:
            case f_WipeWin:
                break;
                
            case f_Empty:
                //if (((sysvar.keyfang & KeyInstallState) && (sysvar.keyfang & KeyLongFang))/** set mode  **/) {
                if (sysvar.keyfang & KeyInstallState) {//ÉèÖÃÖÐ
                    (void)reportResetNet(0); // !!!!!!!!!!
                }
                break;
            default:
                break;
            }
        } else  { /**  **/
            //Mset_bit(g_flag,1);
            g_flag |= (1 << 1);
            SetTimer_irq(&g_timer[2], TIMER_500MS, CDOUBLE_CLICK);
        }
        break;
    
    case CDOUBLE_CLICK:
        //Mreset_bit(g_flag, 1);
        g_flag &= (~(1 << 1));
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


