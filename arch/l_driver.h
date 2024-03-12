#ifndef __L_DRIVER_H__
#define __L_DRIVER_H__

int sysProcess(unsigned *data);
void DAEMON_USART3_Send(u8FIFO_t* u8TxQue);
void key_scan4setwifi(void);
void checkAndReportComponentStatus(void);

void checkAndAckGetCharWorkMode(void);
void checkAndAckGetCharRollerStatus(void);
void checkAndAckGetCharPumpStatus(void);
void checkAndAckGetCharBatteryStatus(void);
void checkAndAckGetCharChargeStatus(void);
void checkAndAckGetCharClearWaterStatus(void);

#endif
