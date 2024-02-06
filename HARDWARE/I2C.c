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

	I2C_AcknowledgeConfig( I2C1, ENABLE );//��ACK
}

/*********************************************************************
 * @fn      cw_read
 * @brief   Read one data from EEPROM.
 * @param   ReadAddr - Read frist address.
 * @return  temp - Read data.
 */
char cw_read(u8 ReadAddr,u8 *data){
	u16 tims=0;
	while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET ){//�����Ƿ����
		if(++tims>6000)return CW221X_ERROR_IIC;
	}tims=0;
	I2C_GenerateSTART( I2C1, ENABLE );//������ʼ�ź�

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ));//��ʼ�ź��Ƿ��Ѳ���
	I2C_Send7bitAddress( I2C1, CW_ADDR, I2C_Direction_Transmitter );//д�ź�

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) ){//BUSY, MSL, ADDR, TXE and TRA flags
		if(++tims>6000)return CW221X_ERROR_IIC;
	}tims=0;
	I2C_SendData( I2C1, (ReadAddr&0xFF) );//д��Ҫ���ĵ�ַ
	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) ){
		if(++tims>6000)return CW221X_ERROR_IIC;
	}tims=0;
	I2C_GenerateSTART( I2C1, ENABLE );//������ʼ�ź�

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
	I2C_Send7bitAddress( I2C1, CW_ADDR, I2C_Direction_Receiver );//���ź�

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) );
  while( I2C_GetFlagStatus( I2C1, I2C_FLAG_RXNE ) ==  RESET )
	I2C_AcknowledgeConfig( I2C1, DISABLE );

  *data = I2C_ReceiveData( I2C1 );
  I2C_GenerateSTOP( I2C1, ENABLE );//ֹͣ�ź�

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
	I2C_GenerateSTART( I2C1, ENABLE );//��ʼ�ź�

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
	I2C_Send7bitAddress( I2C1, CW_ADDR, I2C_Direction_Transmitter );//������ַ

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) ){
		if(++tims>6000)return CW221X_ERROR_IIC;
	}tims=0;
	I2C_SendData( I2C1, (u8)(WriteAddr&0x00FF) );//д��Ҫд�ĵ�ַ
	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) ){
		if(++tims>6000)return CW221X_ERROR_IIC;
	}tims=0;
	if( I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) !=  RESET )
	{
		I2C_SendData( I2C1, *DataToWrite );//д����
	}

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
	I2C_GenerateSTOP( I2C1, ENABLE );//����ACK
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

void start(void){			//IIC���߲�����ʼ�źź���
	SDA_Set;				//����������
	SCL_Set;				//����ʱ����
	I2C_Delay10us(I2C_DELAY);
	SDA_Res;				//��ʱ����Ϊ�ߵ�ƽʱ�����������ߣ�������ʼ�źš�
	I2C_Delay10us(I2C_DELAY);
	SCL_Res;				//����ʱ����
}
//-------------------------------------------------------------------
void stop(void){				//IIC���߲���ֹͣ�źź���
	SDA_Res;				//����������
	I2C_Delay10us(I2C_DELAY);
	SCL_Set;				//����ʱ���ߡ�
	I2C_Delay10us(I2C_DELAY);
	SDA_Set;				//ʱ��ʱ��Ϊ�ߵ�ƽʱ�����������ߣ�����ֹͣ�źš�
	I2C_Delay10us(I2C_DELAY);
	//	SCL_Res;
	I2C_Delay10us(I2C_DELAY);
}
//-------------------------------------------------------------------
char r_ack(void)				//����Ӧ���źź���
{
    char ack;				//����һ��λ���������ݴ�Ӧ��״̬��

	SCL_Set;				//����ʱ���ߡ�
	I2C_Delay10us(I2C_DELAY);
	ack = SDA;				//��ȡӦ���źŵ�״̬��
	I2C_Delay10us(I2C_DELAY);
	SCL_Res;				//����ʱ���ߡ�
	I2C_Delay10us(I2C_DELAY);
	return ack;				//����Ӧ���źŵ�״̬��0��ʾӦ��1��ʾ��Ӧ��
}
//---------------------------------------`----------------------------
void s_ack(void)				//����Ӧ���źź���
{
	SDA_Res;				//����Ӧ���źš�  �͵�ƽӦ��
	I2C_Delay10us(I2C_DELAY);
	SCL_Set;				//����ʱ���ߡ�
	I2C_Delay10us(I2C_DELAY);
	SCL_Res;				//����ʱ���ߡ�
	SDA_Set;                //�ͷ�SDA��
	I2C_Delay10us(I2C_DELAY);

}
//---------------------------------------`----------------------------
void s_nack(void)				//���ͷ�Ӧ���źź���
{
	SDA_Set;				//���÷�Ӧ���źš� �ߵ�ƽ��Ӧ��
	I2C_Delay10us(I2C_DELAY);
	SCL_Set;				//����ʱ���ߡ�
	I2C_Delay10us(I2C_DELAY);
	SCL_Res;				//����ʱ���ߡ�
	I2C_Delay10us(I2C_DELAY);

}
//-------------------------------------------------------------------
void write_byte(unsigned char w_data)		//��IIC����д��һ���ֽڵ����ݺ���
{
	unsigned char i;
	for(i = 0; i < 8; i++)				//��8λ����
	{
		if(w_data & 0x80)SDA_Set;//д���λ������
		else SDA_Res;
		I2C_Delay10us(I2C_DELAY);
		SCL_Set;						//����ʱ���ߣ�����д�뵽�豸�С�
		I2C_Delay10us(I2C_DELAY);
		SCL_Res;						//����ʱ���ߣ�����ı������ߵ�״̬
		I2C_Delay10us(I2C_DELAY);
		w_data = w_data << 1;				//��������һλ���Ѵθ�λ�������λ,Ϊд��θ�λ��׼��
    }
}
//-------------------------------------------------------------------
u8 read_byte(void)			//��IIC���߶�ȡһ���ֽڵ����ݺ���
{
	u8 i;
	u8 r_data=0;					//����һ������Ĵ�����
	for(i = 0; i < 8; i++)				//��8λ����
	{
		SCL_Set;						//����ʱ���ߣ�Ϊ��ȡ��һλ������׼����
		r_data = (r_data<<1);				//�������ֽڵ���������һλ��׼����ȡ���ݡ�
		I2C_Delay10us(I2C_DELAY);
		if(SDA)							//���������Ϊ��ƽ��ƽ��
		{
			r_data |= 0x01;			//��������ֽڵ����λд1��
		}
		SCL_Res;						//����ʱ���ߣ�Ϊ��ȡ��һλ������׼����
		I2C_Delay10us(I2C_DELAY);
	}
	return r_data;						//���ض�ȡ��һ���ֽ����ݡ�
}

//------------------------IIC��д����--------------------------------
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
/***��������ο�***/





