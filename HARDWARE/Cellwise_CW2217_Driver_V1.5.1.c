#include	"MAIN.H"





STRUCT_CW_BATTERY   cw_bat;

/**************************global**********************************/
static unsigned char config_profile_info[SIZE_OF_PROFILE] = {
	0x32,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xC6,0xD2,0xC3,0xB1,0xA5,0x9B,0xED,0xE7,
	0xE7,0x9F,0x8A,0x6B,0x54,0x48,0x3C,0x33,
	0x2C,0x26,0x20,0x44,0x46,0xD1,0x16,0xD4,
	0xA4,0x88,0x92,0xC0,0xCD,0xCA,0xC5,0xC1,
	0xC5,0xBD,0xC0,0xC5,0xC6,0xC3,0xBF,0xC2,
	0xCA,0xB7,0xDB,0xDE,0xF1,0xFF,0xFF,0x85,
	0x20,0x00,0xAB,0x10,0x00,0x90,0x87,0x00,
	0x00,0x00,0x64,0x1C,0xB3,0x99,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x36
};



void cw_delay10us(unsigned int us){
	unsigned char a,b;
	unsigned int i;
	for(i = 0; i < us; i++){
		for(b=1; b>0; b--){
			for(a=18; a>0; a--);
		}
	}
}

/*读取一个字*/
char cw221x_read_word(unsigned char point_reg, unsigned int *r_pdata){
	u8 reg_val[2] = {0 , 0};
	char ret;
	ret=cw_read(point_reg,&reg_val[0]);
	if(ret)return ret;
	ret=cw_read(point_reg+1,&reg_val[1]);
	if(ret)return ret;

	*r_pdata = (reg_val[0] << 8) + reg_val[1];
	return 0;
}


/*获得状态*/
int cw221x_get_state(void){
	char ret;
	unsigned char reg_val;
	unsigned char i;
	
	ret = cw_read(REG_MODE_CONFIG, &reg_val);
	if(ret)		return CW221X_ERROR_IIC;
	if (reg_val != CONFIG_MODE_ACTIVE)//检查是否睡眠
		return CW221X_NOT_ACTIVE;
	
	ret = cw_read(REG_SOC_ALERT, &reg_val);
	if (ret)		return CW221X_ERROR_IIC;
	if (0x00 == (reg_val & CONFIG_UPDATE_FLG))//检查是否已经配置
		return CW221X_PROFILE_NOT_READY;
	
	for (i = 0; i < SIZE_OF_PROFILE; i++) {//0x10+128
		ret = cw_read((REG_BAT_PROFILE + i), &reg_val);//检查配置信息
		if (ret)	return CW221X_ERROR_IIC;
		if (config_profile_info[i] != reg_val)
			break;
	}
	if ( i != SIZE_OF_PROFILE)
		return CW221X_PROFILE_NEED_UPDATE;
	
	return 0;
}

//更新配置信息
static int cw221x_write_profile(unsigned char buf[]){
	char ret;
	u8 i;
	for (i = 0; i < SIZE_OF_PROFILE; i++) {
		ret = cw_write(REG_BAT_PROFILE + i, &buf[i]);
		if (ret)return CW221X_ERROR_IIC;
	}

	return 0;
}
/*
CONFIG寄存器用于主机MCU配置燃油表lC。默认值为0xF0，设置SLEEP和RESTART位。要启动C，主机MCU需
要写入0x30以退出关机模式。然后写入0x00以重新启动仪表以进入激活模式。要重置lC，主机MCU需要按顺
序将0xF0、0x30和0x00写入该寄存器以 重新启动过程。CW2217将重新加载相关参数和设置，并重新启动SOC计算。*/
//睡眠
char cw221x_sleep(void){
	char ret;
	unsigned char reg_val = CONFIG_MODE_RESTART;

	ret = cw_write(REG_MODE_CONFIG, &reg_val);//写入0x30
	if(ret)return CW221X_ERROR_IIC;
	cw_delay10us(2 * 1000);

	reg_val = CONFIG_MODE_SLEEP;
	ret = cw_write(REG_MODE_CONFIG, &reg_val);//写入0xf0
	if(ret)return CW221X_ERROR_IIC;

	cw_delay10us(1 * 1000); 
	return 0;
}
//唤醒
char cw221x_active(){
	char ret;
	unsigned char reg_val = CONFIG_MODE_RESTART;

	ret = cw_write(REG_MODE_CONFIG, &reg_val);//写入0x30
	if (ret)return CW221X_ERROR_IIC;
//	cw_delay10us(2 * 1000);

	reg_val = CONFIG_MODE_ACTIVE;
	ret = cw_write(REG_MODE_CONFIG, &reg_val);//写入0x00
	if (ret < 0)return CW221X_ERROR_IIC;

//	cw_delay10us(1 * 1000);
	return 0;
}
//重新启动
char cw221x_restart(){
	char ret;
	unsigned char reg_val = CONFIG_MODE_SLEEP;

	ret = cw_write(REG_MODE_CONFIG, &reg_val);//写入0xf0
	if (ret < 0)return CW221X_ERROR_IIC;

	reg_val = CONFIG_MODE_RESTART;
	ret = cw_write(REG_MODE_CONFIG, &reg_val);//写入0x30
	if (ret)return CW221X_ERROR_IIC;

	reg_val = CONFIG_MODE_ACTIVE;
	ret = cw_write(REG_MODE_CONFIG, &reg_val);//写入0x00
	if (ret)return CW221X_ERROR_IIC;



	return 0;
}
/*CW221X更新配置文件函数，通常在初始化时调用*/
char cw221x_config_start_ic(){
	char ret = 0;
	unsigned char reg_val = 0;
	unsigned int count = 0;

	/*进入睡眠*/
	ret = cw221x_sleep();
	if (ret < 0)return ret;

	/* 更新新配置信息*/
	ret = cw221x_write_profile(config_profile_info);
	if (ret < 0)return ret;

	/* 设置UPDATE_FLAG位和SOC中断值*/
	reg_val = CONFIG_UPDATE_FLG | GPIO_SOC_IRQ_VALUE;   
	ret = cw_write(REG_SOC_ALERT, &reg_val);
	if (ret)return CW221X_ERROR_IIC;

	/*设置中断*/
	reg_val = CW_CONFIG_INT;
	ret = cw_write(REG_GPIO_CONFIG, &reg_val); 
	if (ret)return CW221X_ERROR_IIC;
	cw_delay10us(10 * 1000); /*sleep 100 ms must*/

	while (1) {
		/*唤醒*/
		ret = cw221x_active();
		if (ret < 0)return ret;
		cw_delay10us(10 * 6000); /*sleep 100 ms must*/
		cw_read(REG_IC_STATE, &reg_val);
		if (IC_READY_MARK == (reg_val & IC_READY_MARK))	break;

		if (++count > 0xffff) {
			cw221x_sleep();//进入睡眠
			return CW221X_ERROR_TIME_OUT;
		}
		cw_delay10us(10 * 6000); /*sleep 100 ms must*/
	}

	return 0;
}

/*器件初始化*/
char cw221x_init(void){
	char ret;
	u8 chip_id;

	ret = cw_read(REG_CHIP_ID, &chip_id);//获得ID
	if (ret < 0)	return ret;
	if(chip_id != IC_VCHIP_ID)	return CW221X_ERROR_CHIP_ID;//ID不匹配

	ret = cw221x_get_state();//获得状态
	if (ret < 0)	return ret;

	if (ret) {
		ret = cw221x_config_start_ic();//配置信息
		if (ret < 0)	return ret;
	}

	return 0;
}

/*获得电压*/
char cw221x_get_vol(unsigned int *lp_vol){
	unsigned int ad_value = 0;
	u8 ValL=0,ValH=0;

	if(cw_read(REG_VCELL_H,&ValH))	return CW221X_ERROR_IIC;
	if(cw_read(REG_VCELL_H+1,&ValL))	return CW221X_ERROR_IIC;
	
	ad_value  = ValH<<8;
	ad_value |= ValL;
	ad_value = ad_value * 5 / 16*4;
	*lp_vol = ad_value;
	
	return 0; 
}

/*获得SOC*/
char cw221x_get_capacity(unsigned int *lp_uisoc){
	unsigned int UI_SOC = 0;
	u8 SOCL=0,SOCH=0;
	if(cw_read(REG_SOC_INT,&SOCH))	return CW221X_ERROR_IIC;
	if(cw_read(REG_SOC_INT+1,&SOCL))	return CW221X_ERROR_IIC;

	SOCL/=25.6;
	UI_SOC=SOCH*10+SOCL;

	if(UI_SOC >= 1000)
		UI_SOC = 1000;

	*lp_uisoc = UI_SOC;
	
	return 0;
}

/*获得温度*/
char cw221x_get_temp(int *lp_temp){
	char ret = 0;
	unsigned char reg_val = 0;
		
	ret = cw_read(REG_TEMP, &reg_val);
	if(ret)	return CW221X_ERROR_IIC;

	*lp_temp = (int)reg_val * 10 / 2 - 400;
	
	return 0;
}

char get_complement_code(unsigned short raw_code){
	long complement_code = 0;
	int dir = 0;

	if (0 != (raw_code & 0x8000)){
		dir = -1;
		raw_code =  (~raw_code) + 1;
	}
	else{
		dir = 1;
	}

	complement_code = (long)raw_code * dir;

	return complement_code;
}

/*获得电流*/
char cw221x_get_current(long *lp_current){
	long current = 0;
	u8 CL=0,CH=0;

	if(cw_read(REG_SOC_INT,&CH))	return CW221X_ERROR_IIC;
	if(cw_read(REG_SOC_INT+1,&CL))	return CW221X_ERROR_IIC;

	current =	CH<<8;
	current |= CL;

	current = get_complement_code(current);
	current = current * 160 / RSENSE / 100;
	*lp_current = current;
	return 0; 
}

/*获得循环寿命*/
char cw221x_get_cycle_count(unsigned int *lp_count){
	char ret = 0;
	unsigned int temp_val_buff = 0;

	ret = cw221x_read_word(REG_CYCLE_H, &temp_val_buff);
	if(ret)return CW221X_ERROR_IIC;

	*lp_count = temp_val_buff/16;

	return 0;	
}

/*获得电池健康状态*/
char cw221x_get_soh(unsigned char *lp_soh){
	char ret = 0;
	unsigned char reg_val = 0;
	
	ret = cw_read(REG_SOH, &reg_val);
	if(ret)return CW221X_ERROR_IIC;

	*lp_soh = reg_val;
	return 0;
}


char cw_update_data(void){
	char ret = 0;
	
	/*IC value*/
	ret += cw221x_get_vol(&cw_bat.voltage);	//获得电压
	ret += cw221x_get_capacity(&cw_bat.capacity);//获得soc
	ret += cw221x_get_temp(&cw_bat.temp);//获得温度
	ret += cw221x_get_current(&cw_bat.current);//获得电流
	ret += cw221x_get_cycle_count(&cw_bat.cycle_count);//获得电池的循环寿命
	ret += cw221x_get_soh(&cw_bat.SOH);//获得电池的健康状态
  
	return ret;
}

char cw221x_bat_init(void){
	char ret=0;
#ifdef  CW2217
	char loop = 0;
	
	IIC_Init();
	
	ret = cw221x_init();
	while((++loop < 3) && (ret != 0)){
		ret = cw221x_init();
	}
	cw_update_data();
#endif
	return ret;

}






