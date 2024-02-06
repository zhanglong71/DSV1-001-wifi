#include "main.h"

#if 1
void IIC_Init(){
	GPIO_InitTypeDef GPIO_InitStructure={0};
	I2C_InitTypeDef I2C_InitTSturcture={0};

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	I2C_InitTSturcture.I2C_ClockSpeed = 100000;
	I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
	I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitTSturcture.I2C_OwnAddress1 = CW_ADDR;
	I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
	I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init( I2C1, &I2C_InitTSturcture );

	I2C_Cmd( I2C1, ENABLE );

	I2C_AcknowledgeConfig( I2C1, ENABLE );//打开ACK
}

/*********************************************************************
 * @fn      cw_read
 * @brief   Read one data from EEPROM.
 * @param   ReadAddr - Read frist address.
 * @return  temp - Read data.
 */
char cw_read(u8 ReadAddr,u8 *data){
	u16 tims=0;
	while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET ){//总线是否空闲
		if(++tims>6000)return CW221X_ERROR_IIC;
	}tims=0;
	I2C_GenerateSTART( I2C1, ENABLE );//产生起始信号

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ));//起始信号是否已产生
	I2C_Send7bitAddress( I2C1, CW_ADDR, I2C_Direction_Transmitter );//写信号

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) ){//BUSY, MSL, ADDR, TXE and TRA flags
		if(++tims>6000)return CW221X_ERROR_IIC;
	}tims=0;
	I2C_SendData( I2C1, (ReadAddr&0xFF) );//写入要读的地址
	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) ){
		if(++tims>6000)return CW221X_ERROR_IIC;
	}tims=0;
	I2C_GenerateSTART( I2C1, ENABLE );//产生起始信号

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
	I2C_Send7bitAddress( I2C1, CW_ADDR, I2C_Direction_Receiver );//读信号

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) );
  while( I2C_GetFlagStatus( I2C1, I2C_FLAG_RXNE ) ==  RESET )
	I2C_AcknowledgeConfig( I2C1, DISABLE );

  *data = I2C_ReceiveData( I2C1 );
  I2C_GenerateSTOP( I2C1, ENABLE );//停止信号

	return 0;
}


/*********************************************************************
 * @fn      AT24CXX_WriteOneByte
 * @brief   Write one data to EEPROM.
 * @param   WriteAddr - Write frist address.
 * @return  DataToWrite - Write data.
 */
char cw_write(u8 WriteAddr, u8 *DataToWrite){
	u16 tims=0;
	while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET ){
		if(++tims>1000)return CW221X_ERROR_IIC;
	}
	I2C_GenerateSTART( I2C1, ENABLE );//开始信号

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
	I2C_Send7bitAddress( I2C1, CW_ADDR, I2C_Direction_Transmitter );//器件地址

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) ){
		if(++tims>6000)return CW221X_ERROR_IIC;
	}tims=0;
	I2C_SendData( I2C1, (u8)(WriteAddr&0x00FF) );//写入要写的地址
	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) ){
		if(++tims>6000)return CW221X_ERROR_IIC;
	}tims=0;
	if( I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) !=  RESET )
	{
		I2C_SendData( I2C1, *DataToWrite );//写数据
	}

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
	I2C_GenerateSTOP( I2C1, ENABLE );//返回ACK
	return 0;
}


#else

//****************************Micro*******************************/
#define	READ_CW				0xc9
#define	WRITE_CW			0xc8

#define I2C_DELAY		18



#define SCL_Set 	GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define SCL_Res 	GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define SCL 			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)

#define SDA_Set		GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define SDA_Res 	GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define SDA 			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)


void IIC_Init(){
	GPIO_InitTypeDef GPIO_InitStructure={0};

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure );
	SDA_Set;
	SCL_Set;
}


void I2C_Delay10us(unsigned int us){
	unsigned char a, b;
	unsigned int i;
	for(i = 0; i < us; i++){
		for(b=1; b>0; b--)		{
			for(a=2; a>0; a--);
		}
	}
}

void start(void){			//IIC总线产生起始信号函数
	SDA_Set;				//拉高数据线
	SCL_Set;				//拉高时钟线
	I2C_Delay10us(I2C_DELAY);
	SDA_Res;				//在时钟线为高电平时，拉低数据线，产生起始信号。
	I2C_Delay10us(I2C_DELAY);
	SCL_Res;				//拉低时钟线
}
//-------------------------------------------------------------------
void stop(void){				//IIC总线产生停止信号函数
	SDA_Res;				//拉低数据线
	I2C_Delay10us(I2C_DELAY);
	SCL_Set;				//拉高时钟线。
	I2C_Delay10us(I2C_DELAY);
	SDA_Set;				//时钟时线为高电平时，拉高数据线，产生停止信号。
	I2C_Delay10us(I2C_DELAY);
	//	SCL_Res;
	I2C_Delay10us(I2C_DELAY);
}
//-------------------------------------------------------------------
char r_ack(void)				//接收应答信号函数
{
    char ack;				//定义一个位变量，来暂存应答状态。

	SCL_Set;				//拉高时钟线。
	I2C_Delay10us(I2C_DELAY);
	ack = SDA;				//读取应答信号的状态。
	I2C_Delay10us(I2C_DELAY);
	SCL_Res;				//拉低时钟线。
	I2C_Delay10us(I2C_DELAY);
	return ack;				//返回应答信号的状态，0表示应答，1表示非应答。
}
//---------------------------------------`----------------------------
void s_ack(void)				//发送应答信号函数
{
	SDA_Res;				//设置应答信号。  低电平应答
	I2C_Delay10us(I2C_DELAY);
	SCL_Set;				//拉高时钟线。
	I2C_Delay10us(I2C_DELAY);
	SCL_Res;				//拉低时钟线。
	SDA_Set;                //释放SDA线
	I2C_Delay10us(I2C_DELAY);

}
//---------------------------------------`----------------------------
void s_nack(void)				//发送非应答信号函数
{
	SDA_Set;				//设置非应答信号。 高电平非应答
	I2C_Delay10us(I2C_DELAY);
	SCL_Set;				//拉高时钟线。
	I2C_Delay10us(I2C_DELAY);
	SCL_Res;				//拉低时钟线。
	I2C_Delay10us(I2C_DELAY);

}
//-------------------------------------------------------------------
void write_byte(unsigned char w_data)		//向IIC总线写入一个字节的数据函数
{
	unsigned char i;
	for(i = 0; i < 8; i++)				//有8位数据
	{
		if(w_data & 0x80)SDA_Set;//写最高位的数据
		else SDA_Res;
		I2C_Delay10us(I2C_DELAY);
		SCL_Set;						//拉高时钟线，将数写入到设备中。
		I2C_Delay10us(I2C_DELAY);
		SCL_Res;						//拉低时钟线，允许改变数据线的状态
		I2C_Delay10us(I2C_DELAY);
		w_data = w_data << 1;				//数据左移一位，把次高位放在最高位,为写入次高位做准备
    }
}
//-------------------------------------------------------------------
u8 read_byte(void)			//从IIC总线读取一个字节的数据函数
{
	u8 i;
	u8 r_data=0;					//定义一个缓冲寄存器。
	for(i = 0; i < 8; i++)				//有8位数据
	{
		SCL_Set;						//拉高时钟线，为读取下一位数据做准备。
		r_data = (r_data<<1);				//将缓冲字节的数据左移一位，准备读取数据。
		I2C_Delay10us(I2C_DELAY);
		if(SDA)							//如果数据线为高平电平。
		{
			r_data |= 0x01;			//则给缓冲字节的最低位写1。
		}
		SCL_Res;						//拉低时钟线，为读取下一位数据做准备。
		I2C_Delay10us(I2C_DELAY);
	}
	return r_data;						//返回读取的一个字节数据。
}

//------------------------IIC读写函数--------------------------------
char cw_read(u8 point_reg,u8 *r_pdata){
	char ack;
	unsigned char i;
	start();
	write_byte(WRITE_CW);
	ack = 1;
	for(i = 0; i < 100; i++)
	{
		ack = r_ack();
		if(!ack)
		{
			break;
		}
		if(i == 99)
		{
			stop();
			return 1;
		}
	}
	write_byte(point_reg);
	ack = 1;
	for(i = 0; i < 100; i++)
	{
		ack = r_ack();
		if(!ack)
		{
			break;
		}
		if(i == 99)
		{
			stop();
			return 1;
		}
	}
	start();
	write_byte(READ_CW);
	ack = 1;
	for(i = 0; i < 100; i++)
	{
		ack=r_ack();
		if(!ack)
		{
			break;
		}
		if(i == 99)
		{
			stop();
			return 1;
		}
	}

	*r_pdata = read_byte();
	s_nack();
	stop();
	return 0;
}

char cw_read_nbyte(unsigned char point_reg,unsigned char *r_pdata, unsigned char len)
{
	char ack;
	unsigned char i;
	start();
	write_byte(WRITE_CW);
	ack = 1;
	for(i = 0; i < 100; i++)
	{
		ack = r_ack();
		if(!ack)
		{
			break;
		}
		if(i == 99)
		{
			stop();
			return 1;
		}
	}
	write_byte(point_reg);
	ack = 1;
	for(i = 0; i < 100; i++)
	{
		ack = r_ack();
		if(!ack)
		{
			break;
		}
		if(i == 99)
		{
			stop();
			return 1;
		}
	}
	start();
	write_byte(READ_CW);
	ack = 1;
	for(i = 0; i < 100; i++)
	{
		ack=r_ack();
		if(!ack)
		{
			break;
		}
		if(i == 99)
		{
			stop();
			return 1;
		}
	}

	while(0 != len)
	{
		*r_pdata = read_byte();
		if(1 == len)
		{
			break;
		}

		s_ack();
		len--;
		r_pdata++;
	}

	s_nack();
	stop();
	return 0;
}

char cw_write(u8 point_reg,u8 *w_pdata)
{
	char ack;
	unsigned char i;
	start();
	write_byte(WRITE_CW);
	ack = 1;
	for(i = 0; i < 100; i++)
	{
		ack = r_ack();
		if(!ack)
		{
			break;
		}
		if(i == 99)
		{
			stop();
			return 1;
		}
	}
	write_byte(point_reg);
	ack = 1;
	for(i = 0; i < 100; i++)
	{
		ack = r_ack();
		if(!ack)
		{
			break;
		}
		if(i == 99)
		{
			stop();
			return 1;
		}
	}
	write_byte(*w_pdata);
	ack = 1;
	for(i = 0; i < 100; i++)
	{
		ack = r_ack();
		if(!ack)
		{
			break;
		}
		if(i == 99)
		{
			stop();
			return 1;
		}
	}
	stop();
	return 0;
}

char cw_write_nbyte(unsigned char point_reg,unsigned char *w_pdata, unsigned char len)
{
	char ack;
	unsigned char i;
	start();
	write_byte(WRITE_CW);
	ack = 1;
	for(i = 0; i < 100; i++)
	{
		ack = r_ack();
		if(!ack)
		{
			break;
		}
		if(i == 99)
		{
			stop();
			return 1;
		}
	}
	write_byte(point_reg);
	ack = 1;
	for(i = 0; i < 100; i++)
	{
		ack = r_ack();
		if(!ack)
		{
			break;
		}
		if(i == 99)
		{
			stop();
			return 1;
		}
	}

	while(0 != len)
	{
		write_byte(*w_pdata);
		ack = 1;
		for(i = 0; i < 100; i++)
		{
			ack = r_ack();
			if(!ack)
			{
				break;
			}
			if(i == 99)
			{
				stop();
				return 1;
			}
		}

		w_pdata++;
		len--;
	}

	stop();
	return 0;
}

#endif
/***代码仅供参考***/





