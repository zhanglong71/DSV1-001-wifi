#include "ch32v20x.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "arch.h"
#include "l_timer.h"

void TimingDelay_Decrement(void)
{
	msg_t msg;
	/***************************************************/
    for(int i = 0; i < TIMER_NUM; i++) {
        if(g_timer[i].tick_bak > 0) {
            if(g_timer[i].tick > 0) {
                g_timer[i].tick--;
            } else {
                g_timer[i].tick = g_timer[i].tick_bak;
                
                msg.msgType = g_timer[i].msgType;		/** message type **/
                msg.msgValue = i;						/** message value(Timer index) **/
                msgq_in(&g_msgq, &msg);
            }
        }
    }
}
