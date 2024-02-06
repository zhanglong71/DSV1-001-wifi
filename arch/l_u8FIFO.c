#include "ch32v20x.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "arch.h"

#include "l_u8FIFO.h"
/*******************************************************************************
 * check is the FIFO empty
 * note: no change argument, so no need disable interrupt
 *******************************************************************************/
int u8FIFOisEmpty(const u8FIFO_t *q)
{
	if(q->in == q->out)
	{
		return TRUE;
	}
	return	FALSE;
}

/*******************************************************************************
 * get the number of data()
 * note: 
 *******************************************************************************/
//int u8FIFO_bufLen(const u8FIFO_t *q)
//{
//    return Mu8FIFO_bufLen(q);
//}

/*******************************************************************************
 * get the number of data()
 * note: 
 *******************************************************************************/
int u8FIFOlength(const u8FIFO_t *q)
{
#if 1
	if(q->in < q->out)
	{
		return (Mu8FIFO_bufLen(q) + q->in - q->out);
	}
	return	(q->in - q->out);
#else
    return ((U8FIFOSIZE + q->in - q->out) % U8FIFOSIZE);
#endif
}

/*******************************************************************************
 * check is the FIFO full
 *******************************************************************************/
int u8FIFOisFull(const u8FIFO_t *q)
{
#if 1
	if((q->in + 1 == q->out) || (((q->in + 1) % Mu8FIFO_bufLen(q)) == q->out))
	{
		return TRUE;
	}
	return	FALSE;
#else
    return (((q->in + 1) % U8FIFOSIZE) == q->out);
#endif
}

/*******************************************************************************
 * initial the FIFO
 *******************************************************************************/
void u8FIFOinit(u8FIFO_t *q)
{
	q->in = 0;
	q->out = 0;
}

void u8FIFOinit_irq(u8FIFO_t *q)
{
    IRQ_disable();
    u8FIFOinit(q);
    IRQ_enable();
}
/*******************************************************************************
 * put the data into the (loop)queue
 *******************************************************************************/
int u8FIFOin(u8FIFO_t *q, u8Data_t *u8Data)
{
    if(u8FIFOisFull(q))
    {
        return	FALSE;
    }

    q->buf[q->in] = u8Data->u8Val;
	q->in = (q->in + 1) % Mu8FIFO_bufLen(q);

    return TRUE;
}

int u8FIFOin_irq(u8FIFO_t *q, u8Data_t *u8Data)
{
	int iRet;

    IRQ_disable();
    iRet = u8FIFOin(q, u8Data);
    IRQ_enable();

	return	iRet;
}

int u8FIFOout(u8FIFO_t *q, u8Data_t *u8Data)
{
    if(u8FIFOisEmpty(q))
    {
        return	FALSE;
    }
    
    u8Data->u8Val = q->buf[q->out];
	q->out = ((q->out + 1) % Mu8FIFO_bufLen(q));

    return TRUE;
}

int u8FIFOout_irq(u8FIFO_t *q, u8Data_t *u8Data)
{
    int iRet;

    IRQ_disable();
    iRet = u8FIFOout(q, u8Data);
	IRQ_enable();
	    
	return	iRet;
}

/*******************************************************************************
 * get data from FIFO
 * no change the FIFO
 *******************************************************************************/
int u8FIFO_get(const u8FIFO_t *q, u8 offset, u8 *dst)
{
    if(u8FIFOisEmpty(q))
    {
        return	FALSE;
    }

    u8 start = (q->out + offset) % (Mu8FIFO_bufLen(q));
    u8 end = q->in;
    u8 curr = 0;

    for (; start != end; start = ((start + 1) % (Mu8FIFO_bufLen(q))), curr++) {
        dst[curr] = q->buf[start];
    }
    dst[curr] = '\0';

    return TRUE;
}

/*******************************************************************************
 * get data the last data from FIFO
 *******************************************************************************/
int u8FIFO_last(const u8FIFO_t *q, u8* ch)
{
    if(u8FIFOisEmpty(q))
    {
        return	FALSE;
    }

    if (q->in == 0) {
        *ch = q->buf[Mu8FIFO_bufLen(q) - 1];
    } else {
        *ch = q->buf[q->in - 1];
    }
    return TRUE;
}

/////////////////////////////////////////////////////
