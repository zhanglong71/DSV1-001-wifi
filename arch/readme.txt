2023/11/27 10:42:03 �����Ϣ���С�����ջ����ʱ��
2023/12/6 16:14:43 ͨ���յ���������������֤UART3�շ���������(�ж���)
2023/12/7 8:46:57 ͨ���յ���������������֤UART3�շ���������(��ȷ��)
          14:01:19 ����systick��ʱ��������10ms��ʱ����
          16:33:32 ����json��������(STC_ISP�������յ���Ӧ��ʽ���ַ���)
2023/12/8 19:14:15 ���� sm_sendData ��������
2023/12/11 8:50:58 sm_sendData��������������һ���򻺳������͵�������
    g_uart3TxQue�����������ˣ�һ�β��ܷ��������е����ݣ�sm_sendData(..)���������ݷ������ͣ���һ�ν�����ͷ�����ݳ��ȣ��������ĵ�ǰ(U8FIFOSIZE - 32)���ͳ�ȥ�������ٷ������������ķ���ֱ��ȫ��������ɣ�
    ���ǵ����ݽ�����δ���ʱ���������е����ݲ����������ݴ�����ɺ󣬽�FIFO��ͷβָ�����㣬�������ݼ�飡
    �ַ������ݼ�飺KEYʶ���ٳ��ȣ��Խ��յ���ָ������ȷ��������ܳ���
    �ַ���ʶ��
    �������ʶ��
2023/12/13 17:42:31 �ϱ��豸��Ϣ�ı��Ľ��ա�ʶ����Ӧ���� ����(����body�е����һ��"��\n����β������û����)
2023/12/14 10:21:39 ���ݳ��ȸĳ�ASCII��
2023/12/14 13:22:33 ȷ��getDevInfo, heartbeat����Ӧ������
2023/12/14 15:00:46 ȷ��reportService����Ӧ������
2023/12/15 8:51:35 Ŀǰ����Ϊgetchar���������Ӧ����(���ܴ���ע�͵����¼�鵽ģʽ��Чû�����ݷ���������ģʽ�����ݷ���ok)
2023/12/15 10:29:05 �������ݲ���������������
2023/12/15 17:03:51 �ӵ�ع������������շ�����
2023/12/18 9:10:12 ȷ��PutChar�Ľ��ռ���ӦӦ����������Ӧ�����ݴ����ƣ��ſ�����ԭ���ܴ�����շ���������������WIFIģ������
2023/12/19 11:26:28 UART3�����ʸ�Ϊ9600(ԭΪ115200)
2023/12/28 9:53:23 ��֤���յ���wifiģ��:1.���յ�getDevInfo���ݣ�������Ӧ��(����һ�����⹹�����Ϣ������error)��2.ģ�����յ�ScanWifi���ݣ�������Ӧ��(����scanWifi,4,fail��scanWifi,2,72)
2023/12/28 18:42:00 ����connectWifi�����쳣���Ӵ��ڴ�ӡ�����ļ����ֽ���ͬ����Ӵ�ӡ������������뷢���ǽ��ջ����������£�
2023/12/29 17:20:04 ��ɰ���˫��ɨ�輰������Ϣ����; ���uart3���ճ�ʱ����; ��ɵ��ÿ30������ϱ�����; ��һ������ϴ�ػ�����������(��wifi)���е���
2024/1/29 11:36:18 ׷�Ӽ�¼��֤����¼��������(wifi����û����ź�)
2024/1/29 15:12:37 ��֤ȷ�ϴ�������DM6����uart3��PC���ڶԽ��շ�����
2024/3/7 17:27:58 �޸ı��ķ��Ͷԡ�\n���Ĵ���(�����Զ����ͻس����У�ԭʼ�����в��ӻس�����)
2024/3/18 13:31:42 �������й���(ˮ�ò���λ״̬ץ����)��
    IDs_Judge()�в�ˮ��״̬; checkAndReportPumpStatus()����ָ�����(��ԭ����״̬����һ��)
2024/3/19 11:34:55 ����״̬��˫��������λwifiģ��
2024/3/26 11:04:07 �޸Ķ�getChar����Ĵ���������û�б仯��Ӧ���ϱ�������reportChar���б仯���ϱ�!
2024/3/27 9:48:51 ���getChar/reportChar status ״̬�ϱ�������������
2024/3/28 16:44:36 �޸�������Ϣ�߼�������·������(��һ�λ�Ͽ��������)����Ҫ��ѯ������Ϣ! ��ӦgetChar,7,netInfo/getChar,6,update

/*********************************************************************************************************************/
JSON����
1.���ݸ�ʽ: JSON�ɼ�ֵ����ɣ��ô�����{}��ʾ��ÿ����ֵ��֮���ö��ŷָ�
2.�����������������ַ������ͣ���˫���š���������
3.ֵ���������ַ���������
4.�ַ������ַ���������˫���š���������
5.���֣����ֿ����������򸡵������������ÿ�ѧ������
6.����ֵ����ȡtrue/false.
7.���飺�÷�����[]��ʾ������Ԫ��֮���ö��Ÿ���
8.���󣺶����ã�����ʾ����������֮���ö��Ÿ�����ÿ�������ɼ�����ֵ��ɣ�������ֵ֮����":"����
����json���ģ�

MCU��Ҫ���ղ������ı���body��
/****
 * 4.1 ����ָ���·��������·�1�����ݶ����û�����APPʱ�·���ָ���
 * WiFi req: putChar,37,{"switch":{"on":40,"target":65}}\n
 */
{
    "switch": {
        "on": 40,
        "target": 65
    }
}

/** 
 * 4.1 ����ָ���·���MCU �˿���ͨ��JSON �����е��ֶ��Ƿ��С�batch���������ǵ�ָ��Ƕ�ָ� 
 **/
{
    "batch": [
        {
            "sid": "switch",
            "data": {
                "on": 1
            }
        },
        {
            "sid": "brightness",
            "data": {
                "value": 100
            }
        },
        {
            "sid": "colour",
            "data": {
                "white": 100
            }
        }
    ]
}

/****
 * 5.1 ��ѯ�豸����
 * Wi-Fi req: getDevInfo,0\n
 * MCU ack:getDevInfo,432,{��������\n
 * Wi-Fi ack: getDevInfo,2,ok\n ���� Wi-Fi ack: getDevInfo,5,error\n
 ****/
 getDevInfo,0\n
 
 
getDevInfo,0,
{
    "v": "1.0.1",
    "dv": "1.0.0",
    "prodId": "25PK",
    "deviceTypeId": "01D",
    "manufacturerId": "416",
    "deviceModel": "OHOS002",
    "deviceTypeNameEn": "Socket",
    "manufacturerNameEn": "c-chip",
    "networkType": "BLE",
    "acKey": "702B20206F2468203956502B3A6A673A734AE2CC6BE3B7A4CDD9BA27DD889661DB9D818EED21F46CD159B2AAAAAC16C8",
    "productSeries": "Certificate",
    "productKey": "f2b80c7c77b840e4b7017029baab9bf6",
    "marketName": "о��о����",
    "brand": "c-chip"
}


/**
 * 5.2 MCU ==> WIFI 
 * �ϱ��������ԣ��õ��˱���body
 * MCU send:reportService,109,{...}
 * MCU����Ҫ�����˱��ĵ�����
 **/
{
    "sId": [
        "switch",
        "fan",
        "childLockSwitch",
        "filterElement1"
    ],
    "sType": [
        "switch",
        "fan",
        "switch",
        "filterElement"
    ]
}

/**
 * 5.10 MCU ==> WIFI
 * �� Rssi
 * MCU send: getRssi,0\n
 * WIFI ACK: getRssi,3,-35\n
 **/
 
/**
 * 5.16 MCU ==> WIFI
 * �� IP
 * MCU send: getIp,0\n
 * WIFI ACK: getIp,34,{"status": 1,"ip":"192.168.43.86"}\n
 **/
 
/**
 * 5.17 MCU ==> WIFI
 * �� MAC
 * MCU send: getMac,0\n
 * WIFI ACK: getMac,38,{"status":1,"mac":"fc:58:22:4f:32:c9"}\n
 **/
 
/**
 * 5.18 MCU ==> WIFI
 * �� SSID
 * MCU send: getSsid,0\n
 * WIFI ACK: getSsid,24,{"status":1,"ssid":"K2"}\n
 **/
 
 /**
 * 5.18 MCU ==> WIFI
 * �� getWifiStatus
 * MCU send: getWifiStatus,0\n
 * WIFI ACK: getSsid,24,{"status":1,"ssid":"K2"}\n
 **/
 
/*********************************************************************************************************************/
ȷ��״̬������:
Ϊ�˾������Ķ�ԭ���Ĵ��룬�����ò� sysvar/ ״̬�ķ�ʽ��ȡ����Ϣ���������ڶ�Ӧ״̬����Ϣ! (���������� �����ڶ�Ӧ�Ĵ����߼�������Ϣ���� ��)


a.��������״̬�ж�����(��������)��
����mode_determine()�����еı仯Ϊ׼����ο� wifi_Upload_STATUS()
  sysvar.Modes
  MODE_1/MODE_2/MODE_3/MODE_RINSE/MODE_CLEANING --- ��Ӧ��׼ģʽ��ǿ��ģʽ��ǿ��ģʽ����ϴģʽ������ϴģʽ
  
  �˳�����ʱ��״̬��
  sysvar.sysfang &= ~OFF_ON; ���ݲ���������ϴģʽ��


b.��Ͳ����(�豸����)  �ο� wifi_Upload_FAULT() ����
  (sysvar.sysfang & MOTO_ERR_1)

c.��ص������ж�����(�����ĵ�����1��2��)��
���� perform() �����еı仯Ϊ׼��
    (sysvar.batfang & (BACH_low|BACH_lowin|BACH_lowin2) 
    
    (BACH_low|BACH_lowin|BACH_lowin2)�ĸ�����Ϣ�����Դ� perform() ���������һ�εõ���
    sysvar.batfang & BACH_low --- ��Ӧ sysvar.BAT_soc < 1 (����ǿ���״̬������ BATpowers_ERR)
    sysvar.batfang & BACH_lowin2 --- ��Ӧ sysvar.BAT_soc < 3
    sysvar.batfang & BACH_lowin --- ��Ӧ sysvar.BAT_soc < 5
    
    
d.���״̬
  ���� CHruning() �����е�
   (sysvar.CH_state==CH_DETECTION || sysvar.CH_state==CH_over) --- �����(�ǳ��״̬)
   (sysvar.CH_state==CH_STANDBY) --- ����(�ǳ��״̬)
   (sysvar.CH_state==CH_ING) --- �����()
   (sysvar.CH_state==CH_ERR) --- ������
   ����ֻ����3��״̬���ǳ��״̬(������)������У�������
   
   CH_task2()�����е� (sysvar.batfang & BATCHining) ����  (sysvar.batfang & BACH_over) �Ǹ�ɶ��(������֮)
   
     
e.��ˮˮλ(����)
  ���� mode_determine()�����е� IdSensor_fang(IdSensor_CLEAR) Ϊ׼
  ���⣬���ӿ������ҷǴ�ˮ��ϴģʽ (sysvar.sysfang & OFF_ON) && (sysvar.Modes!=MODE_RINSE)
  
  IDs_Judge() �е��ж������� ��
  (IdSensor_fCmp(IdSensor_CLEAR)) ��Ҫ��һ���˽⣺IdSensor_fCmp(IdSensor_CLEAR)���ڱȽ����μ��ǰ��ı仯��IdSensor_fang(IdSensor_CLEAR)�ǵ�ǰ״̬
  

f.ˮ��״̬(���ء�����С)
  ˮ�ù��ϣ����� (IdSensor_fang(IdSensor_PUMPERR) �� (IdSensor_fang(IdSensor_PUMP)  ����Ϊ׼���ο� wifi_Upload_FAULT()
   ����֪�߼�����(sysvar.sysfang & (OFF_ON)) �������Ҫ����
  
  moto_run() �����е� (sysvar.sysfang & MOTO_ERR_2) ��Ҫ��һ���˽�
  
/*********************************************************************************************************************/
�豸״̬�ϱ���Э�鶨�����£�

4.2 MCU״̬�ϱ�(5.3 MCU�ظ�getChar��ѯҲ������ͬ�ı�����)
reportChar,xx,{"mop":{"status":0}},            // unknow δ֪״̬(�����Ͽ�������)
reportChar,xx,{"mop":{"status":1}},            // standby ����
reportChar,xx,{"mop":{"status":2}},            // standard ��׼ģʽ
reportChar,xx,{"mop":{"status":3}},            // highPowerǿ��ģʽ
reportChar,xx,{"mop":{"status":4}},            // rinse ��ˮ��ϴģʽ(�����ò���)
reportChar,xx,{"mop":{"status":5}},            // cleaning ����ϴģʽ(�����ò���)

reportChar,xx,{"roller":{"status":1}},       // normal ��Ͳ�������
reportChar,xx,{"roller":{"status":2}},       // error ��Ͳ�������

reportChar,xx,{"clearWater":{"status":1}},   // clear water normal ��ˮ����
reportChar,xx,{"clearWater":{"status":2}},   // clear water shortage ��ˮ����

reportChar,xx,{"pump":{"status":1}},           // ˮ������     pumpNormal
reportChar,xx,{"pump":{"status":2}},           // ˮ�ù���     pumpOverload
reportChar,xx,{"pump":{"status":3}},           // ˮ�õ�����С pumpCurrentTooSmall

reportChar,xx,{"battery":{"status":1}},        // ��ص�ѹ��������Χ
reportChar,xx,{"battery":{"status":2}},        // ��ص�ѹ����
reportChar,xx,{"battery":{"level":%u}}         // ��ص��� (0-100)

reportChar,xx,{"charge":{"status":1}},         // û���
reportChar,xx,{"charge":{"status":2}},         // ���ڳ��
reportChar,xx,{"charge":{"status":3}},         // ������(����û��糡��)
reportChar,xx,{"charge":{"status":4}},         // ������
/*********************************************************************************************************************/