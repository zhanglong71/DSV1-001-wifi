#include "ch32v20x.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "arch.h"

/*******************************************************************************
 * IRQ_disable()/IRQ_enable()
 *******************************************************************************/
void IRQ_disable(void)
{
    __disable_irq();
}

void IRQ_enable(void)
{
    __enable_irq();
}

/*******************************************************************************
 *
 * function stack operation
 *
 * note: top-1 Ö¸ÏòµÄ²ÅÊÇÓÐÐ§Êý¾Ý
 *
 *******************************************************************************/
void fstack_init(fstack_t *s)
{
    s->top = 0;
}

int fstack_pop(fstack_t *s, func_t *f)
{
    if((s->top <= 0) || (s->top > STACKSIZE)) {    /** make sure ... top [1..STACKSIZE - 1]  **/
        return    FALSE;
    }
    f->func = s->func[s->top - 1].func;
    f->arg = s->func[s->top - 1].arg;
    s->top--;
    return TRUE;
}

int fstack_push(fstack_t *s, func_t *f)   /** make sure ... top [0..STACKSIZE-2]  **/
{
    if(s->top >= STACKSIZE) {
        return    FALSE;
    }
    s->func[s->top].func = f->func;
    s->func[s->top].arg = f->arg;
    s->top++;
    
    return TRUE;
}

int fstack_update(fstack_t *s, func_t *f)
{
    /** Same As: stackpop() then stackpush() **/
    s->func[s->top - 1].func = f->func;
    s->func[s->top - 1].arg = f->arg;
    
    return TRUE;
}
/**
 * get function from the top of the stack
 *
 **/
int fstack_top(const fstack_t *s, func_t *f)   /** make sure ... top [1..STACKSIZE - 1]  **/
{
    if((s->top < 1) || (s->top > STACKSIZE)) {
        return    FALSE;
    }
    f->func = s->func[s->top - 1].func;
    f->arg = s->func[s->top - 1].arg;

    return TRUE;
}
/*******************************************************************************
 *
 * msg queue operation
 *
 *******************************************************************************/
int msg_init(msg_t *pMsg)
{
    if((pMsg != 0)) {
        pMsg->msgType = CMSG_NONE;
        return    TRUE;
    } else {
        return FALSE;
    }
}

void msgq_init(msgq_t *q)
{
    q->tail = q->head = 0;
}

static int isempty(msgq_t *q)
{
    if(q->tail == q->head) {
        return TRUE;
    }
    return FALSE;
}

static int isfull(msgq_t *q)
{
    if((q->tail + 1 == q->head) || (((q->tail + 1) % QUEUESIZE) == (q->head))) {
        return TRUE;
    }
    return FALSE;
}

int msgq_in(msgq_t *q, msg_t *val)            //Note: check queue full is necessary before invoke this routine
{
    if(isfull(q)) {
        return FALSE;
    }
    q->msg[q->tail].msgType = val->msgType;    
    q->msg[q->tail].msgValue = val->msgValue;
    
    q->tail = (q->tail + 1) % QUEUESIZE;

    return TRUE;
}

int msgq_in_irq(msgq_t *q, msg_t *val)        //Note: check queue full is necessary before invoke this routine
{
    int iRet;
    
    IRQ_disable();
    iRet = msgq_in(q, val);
    IRQ_enable();

    return iRet;
}

int msgq_out(msgq_t *q, msg_t *val)        //Note: check queue empty is necessary before invoke this routine
{
    if(isempty(q)) {
        return  FALSE;
    }
    
    val->msgType = q->msg[q->head].msgType;
    val->msgValue = q->msg[q->head].msgValue;
    q->head = (q->head + 1) % QUEUESIZE;
        
    return    TRUE;
}

int msgq_out_irq(msgq_t *q, msg_t *val)        //Note: check queue empty is necessary before invoke this routine
{
    int iRet;
    
    IRQ_disable();
    iRet = msgq_out(q, val);
    IRQ_enable();
        
    return    iRet;
}

/*******************************************************************************
 * Timer operation
 *******************************************************************************/
void SetTimer(Timer_t * timer, int tick, msgType_t msgType)
{
    timer->tick = tick;
    timer->tick_bak = tick;
    timer->msgType = msgType;
}
void SetTimer_irq(Timer_t * timer, int tick, msgType_t msgType)
{
    IRQ_disable();
    SetTimer(timer, tick, msgType);
    IRQ_enable();
}

void ClrTimer(Timer_t * timer)
{
    timer->tick = 0;
    timer->tick_bak = 0;
}
void ClrTimer_irq(Timer_t * timer)
{
    IRQ_disable();
    ClrTimer(timer);
    IRQ_enable();
}

#if 0
/*******************************************************************************
 * delay() operation
 * 
 * wait until timer out
 *******************************************************************************/
int g_tmr_delay;


void delay(int TickCount)
{
    g_tmr_delay = TickCount;
    
    while(g_tmr_delay);
}

/***************************************
 * ä»¥å½¢å¦?
 * while(1) {
 *    LEDall_Toggle();
    delaySpin(x);    //nop()
 * }
 * çš„æ–¹å¼æµ‹å¾?
 * nop(); - 988/83 = 11.903us
 * 1 - 984/46 = 21.391us
 * 2 - 968/37 = 26.162us
 * 6 - 972/24 = 40.5us
 * 12 - 892/14us = 63.714us
 * 
 * 24 - 100us
 * 116(118) - 500us
 * 237 - 1ms
 * 474 - 2ms
 * 1185 - 5ms
 * 2350 - 10ms
 * ...
 * 4740 - 20ms(ä»?60ms/14æµ‹å¾—)
 ***************************************/
void delaySpin(u32 _loop)
{
    while(_loop-- > 0);
}

/** 1us(æ²¡å…³ä¸­æ–­ï¼Œè‡³å°?us) **/
void delaySpin_us(u32 _us)
{
    for(; _us > 0; _us--)delaySpin(24);
}

/** 1ms(æ²¡å…³ä¸­æ–­ï¼Œè‡³å°?ms) **/
void delaySpin_ms(u32 _ms)
{
    for(; _ms > 0; _ms--)delaySpin(237);
}
/*******************************************************************************/
void atomic_set(short *_var, short _value)
{
    IRQ_disable();
    *_var = _value;
    IRQ_enable();
}

short atomic_get(short *_var)
{
    short tmp;
    
    IRQ_disable();
    tmp = *_var;
    IRQ_enable();
    
    return    tmp;
}
#endif

#if    0
/*******************************************************************************
 * check is the queue empty
 *******************************************************************************/
static int isActionQueEmpty(actionQueue_t *q)
{
    if(q->tail == q->head)
    {
        return TRUE;
    }
    return    FALSE;
}

/*******************************************************************************
 * check is the queue full
 *******************************************************************************/
static int isActionQueFull(actionQueue_t *q)
{
    if((q->tail + 1 == q->head) || (((q->tail + 1) % ACTIONQUEUESIZE) == q->head))
    {
        return TRUE;
    }
    return    FALSE;
}
/*******************************************************************************/
void adcSample_Init(adcData_t *adcData, Timer_t *timer, msgType_t stepMsgType, int tick)
{
    adcData->u16count = 0;
    adcData->u16currValue = 0;
    adcData->u16average = 0;
    
    adcData->stepMsgType = stepMsgType;
    adcData->tick = tick;
    adcData->timer = timer;
    ClrTimer(timer);
    
    g_flag &= (~(1<<8));
    g_flag &= (~(1<<9));
}

/*******************
 * è®¡ç®—/èŽ·å–å‡å€?
 *******************/
int adcSample_In(adcData_t *adcData, u16 __value)
{
    adcData->u16count += 1;
    adcData->u16currValue = __value;
    
    adcData->u16average = (((adcData->u16count - 1) * (adcData->u16average)) + __value)/(adcData->u16count);    //é‡æ–°è®¡ç®—å‡å€?
    
    return TRUE;
}

int adcSample_Out(adcData_t *adcData, u16 *__average)
{
    if(adcData->u16count >= 16) {
        if(__average != 0) {
            *__average = adcData->u16average;    //å–å‡å€?
        }
        return TRUE;
    }
    return    FALSE;
}

/*******************************************************************************
 * put the data into the (loop)queue
 *******************************************************************************/
void actionQueueInit(actionQueue_t *q, Timer_t *timer, msgType_t stepMsgType, msgType_t overMsgType)
{
    q->tail = q->head = 0;
    
    q->stepMsgType = stepMsgType;  
    q->overMsgType = overMsgType;
    q->timer = timer;
    ClrTimer(q->timer);
}

/*******************************************************************************
 * put the data into the (loop)queue
 *******************************************************************************/
int actionQueueIn(actionQueue_t *q, action_t *action)
{
    if(isActionQueFull(q)) {
        return FALSE;
    }

    q->buf[q->tail].actionType = action->actionType;
    q->buf[q->tail].actionTime = action->actionTime;
    q->buf[q->tail].actionPara = action->actionPara;
    q->buf[q->tail].func       = action->func;
    q->tail = (q->tail + 1) % ACTIONQUEUESIZE;
    
    return TRUE;
}

int actionQueueOut(actionQueue_t *q, action_t *action)
{
    if(isActionQueEmpty(q)) {
        return FALSE;
    }
    
    action->actionType = q->buf[q->head].actionType;
    action->actionTime = q->buf[q->head].actionTime;
    action->actionPara = q->buf[q->head].actionPara;
    action->func       = q->buf[q->head].func;
    
    q->head = (q->head + 1) % ACTIONQUEUESIZE;
       
    return TRUE;
}
#endif

/////////////////////////////////////////////////////