#ifndef _Cellwise_CW2217_Driver_h_
#define _Cellwise_CW2217_Driver_h_


#define REG_CHIP_ID             0x00
#define REG_VCELL_H             0x02
#define REG_VCELL_L             0x03
#define REG_SOC_INT             0x04
#define REG_SOC_DECIMAL         0x05
#define REG_TEMP                0x06
#define REG_MODE_CONFIG         0x08
#define REG_GPIO_CONFIG         0x0A
#define REG_SOC_ALERT           0x0B
#define REG_TEMP_MAX            0x0C
#define REG_TEMP_MIN            0x0D
#define REG_CURRENT_H           0x0E
#define REG_CURRENT_L           0x0F
#define REG_T_HOST_H            0xA0
#define REG_T_HOST_L            0xA1
#define REG_USER_CONF           0xA2
#define REG_CYCLE_H             0xA4
#define REG_CYCLE_L             0xA5
#define REG_SOH                 0xA6
#define REG_IC_STATE            0xA7
#define REG_STB_CUR_H           0xA8
#define REG_STB_CUR_L           0xA9
#define REG_FW_VERSION          0xAB
#define REG_BAT_PROFILE         0x10

#define CONFIG_MODE_RESTART     0x30
#define CONFIG_MODE_ACTIVE      0x00
#define CONFIG_MODE_SLEEP       0xF0
#define CONFIG_UPDATE_FLG       0x80
#define IC_VCHIP_ID             0xA0
#define IC_READY_MARK           0x0C
#define IC_TEMP_READY           0x08
#define IC_VOL_CUR_READY        0x04

#define SIZE_OF_PROFILE         80

#define RSENSE  5//取样电阻值

#define INIT_TEST_TIME      50 /*must >= 50 , can not modify */

#define CW221X_ERROR_IIC      -1	//通信错误
#define CW221X_ERROR_CHIP_ID  -2
#define CW221X_ERROR_TIME_OUT -3
#define CW221X_NOT_ACTIVE          1//睡眠-未激活
#define CW221X_PROFILE_NOT_READY   2//未配置-没有准备好
#define CW221X_PROFILE_NEED_UPDATE 3//读取的配置信息不符合

#define GPIO_SOC_IRQ_VALUE      0x7f    /* 0x7F */
#define CW_SLEEP_COUNTS         80
#define CW_CONFIG_INT         	0x00 //中断设置

#define CW_ADDR  0xc8	//器件地址

typedef struct tagSTRUCT_CW_BATTERY {
	/*IC value*/
	unsigned int voltage;	
	unsigned int capacity;

	int temp;

	unsigned int cycle_count;
	unsigned int ErrNum;
	unsigned char SOH;
	unsigned char SOC;

	long current;

	/*Get from charger power supply*/
	unsigned char ErrF;
}STRUCT_CW_BATTERY;

extern STRUCT_CW_BATTERY   cw_bat;

void IIC_Init(void);
void dump_register(void);
void Delay_ms(unsigned int c) ;
char cw221x_get_REG(unsigned char reg,unsigned char *chip_id);
char cw_read(u8 ,u8 *);
char cw_read_nbyte(unsigned char point_reg,unsigned char *r_pdata, unsigned char len);
char cw_write(unsigned char point_reg,unsigned char *w_pdata);
char cw221x_get_chip_id(u8 *chip_id);
char cw221x_sleep(void);
char cw221x_get_vol(unsigned int *lp_vol);
char cw221x_get_capacity(unsigned int *lp_uisoc);
char cw221x_get_temp(int *lp_temp);
char cw221x_get_current(long *lp_current);
char cw221x_get_cycle_count(unsigned int *lp_count);
char cw221x_get_soh(unsigned char *lp_soh);
char cw221x_dump_register(void);
char cw_update_data(void);
char cw221x_bat_init(void);
void cw_delay10us(unsigned int us);
char cw221x_active(void);
#endif



