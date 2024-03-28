#include "ch32v20x.h"
#include <string.h>
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "arch.h"
#include "f_idle.h"
#include "main.h"
#include "l_jsonTL.h"

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
        /** nothing  **/
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
		break;
        
    case CMSG_INIT:
        g_tick = 0;
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

/** get wifi netinfo **/
int f_getNetInfo(unsigned *pMsg)
{
    func_t func;
    msg_t msg;
    int_paction_void_t funcArr[] = {
        doNothing,
        reportgetSsid,
        reportgetIp,
        reportgetMac,
        reportgetRssi,
    };

    switch(((msg_t *)pMsg)->msgType)
    {
    case CMSG_INIT:
        g_tick = 0;
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
        // (void)memset(&g_netInfo, 0, sizeof(g_netInfo));
        g_netInfo.flag = 0;
		break;
    
    case CMSG_TMR:
        g_tick++;
        if (g_tick >= MTABSIZE(funcArr)) {
            fstack_init(&g_fstack);
            func.func = f_idle;
            fstack_push(&g_fstack, &func);
            msg.msgType = CMSG_INIT;
            msgq_in_irq(&g_msgq, &msg);
        } else {
            (void)funcArr[g_tick]();
        }
        break;
        
    default:
        break;
    }
}


