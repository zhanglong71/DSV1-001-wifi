2023/11/27 10:42:03 添加消息队列、功能栈、定时器
2023/12/6 16:14:43 通过收到再立即发出，验证UART3收发功能正常(中断收)
2023/12/7 8:46:57 通过收到再立即发出，验证UART3收发功能正常(再确认)
          14:01:19 启用systick定时器，设置10ms定时正常
          16:33:32 发送json报文正常(STC_ISP可正常收到对应格式的字符串)
2023/12/8 19:14:15 初验 sm_sendData 工作正常
2023/12/11 8:50:58 sm_sendData工作正常！调整一次向缓冲区发送的数据量
    g_uart3TxQue缓冲区限制了，一次不能发送完所有的数据，sm_sendData(..)函数将数据分批发送：第一次将数据头，数据长度，报文正文的前(U8FIFOSIZE - 32)发送出去；后面再分批将报文正文发出直到全部发送完成！
    考虑到数据接收尚未完成时，缓冲区中的数据不完整！数据处理完成后，将FIFO的头尾指针清零，方便数据检查！
    字符串数据检查：KEY识别，再长度，以接收到的指定长度确定传输的总长度
    字符串识别：
    报文完成识别：
2023/12/13 17:42:31 上报设备信息的报文接收、识别、响应发送 正常(报文body中的最后一个"和\n仍在尾部！暂没处理)
2023/12/14 10:21:39 数据长度改成ASCII码
2023/12/14 13:22:33 确认getDevInfo, heartbeat的响应正常！
2023/12/14 15:00:46 确认reportService的响应正常！
2023/12/15 8:51:35 目前可认为getchar命令接收响应正常(功能代码注释掉导致检查到模式无效没有数据发出！正常模式下数据发出ok)
2023/12/15 10:29:05 接收数据不带引号正常处理
2023/12/15 17:03:51 从电池供电点飞线正常收发数据
2023/12/18 9:10:12 确认PutChar的接收及对应应答动作正常；应答内容待完善；放开编译原功能代码后，收发内容正常！待与WIFI模块联调
2023/12/19 11:26:28 UART3波特率改为9600(原为115200)
2023/12/28 9:53:23 验证新收到的wifi模组:1.能收到getDevInfo数据，并作出应答(发了一个随意构造的消息，返回error)；2.模组能收到ScanWifi数据，并作出应答(返回scanWifi,4,fail及scanWifi,2,72)
2023/12/28 18:42:00 测试connectWifi报文异常！加串口打印，最后的几个字节相同！添加打印输出及分析代码发现是接收缓冲区满导致！
2023/12/29 17:20:04 完成按键双击扫描及配网信息发送; 完成uart3接收超时处理; 完成电池每30秒电量上报动作; 下一步接上洗地机的其它板子(及wifi)进行调试
2024/1/29 11:36:18 追加记录验证，烧录运行正常(wifi串口没测得信号)
2024/1/29 15:12:37 验证确认从正常的DM6主板uart3与PC串口对接收发正常
2024/3/7 17:27:58 修改报文发送对‘\n’的处理(后面自动发送回车换行；原始报文中不加回车换行)
2024/3/18 13:31:42 有了所有功能(水泵不在位状态抓不到)！
    IDs_Judge()中查水泵状态; checkAndReportPumpStatus()后面恢复正常(与原代码状态处理一致)
2024/3/19 11:34:55 设置状态下双击按键复位wifi模组
2024/3/26 11:04:07 修改对getChar命令的处理：不管有没有变化都应答上报！区别reportChar的有变化才上报!
2024/3/27 9:48:51 添加getChar/reportChar status 状态上报：离线与在线
2024/3/28 16:44:36 修改网络信息逻辑：连上路由器后(第一次或断开后的重联)，就要查询网络信息! 响应getChar,7,netInfo/getChar,6,update
2024/3/29 8:36:26 putSync,0\n应答正常
2024/4/10 10:43:28 按D7修改getDevInfo报文内容

/*********************************************************************************************************************/
JSON规则：
1.数据格式: JSON由键值对组成，用大括号{}表示，每个键值对之间用逗号分隔
2.键名：键名必须是字符串类型，用双引号“”括起来
3.值：可以是字符串、数字
4.字符串：字符串必须用双引号“”括起来
5.数字：数字可以是整数或浮点数，不允许用科学计数法
6.布尔值：吸取true/false.
7.数组：用方括号[]表示，数组元素之间用逗号隔开
8.对象：对象用｛｝表示，对象属性之间用逗号隔开，每个属性由键名和值组成，键名和值之间用":"隔开
解析json报文：

MCU需要接收并解析的报文body：
/****
 * 4.1 单个指令下发【命令下发1个数据对象，用户操作APP时下发该指令。】
 * WiFi req: putChar,37,{"switch":{"on":40,"target":65}}\n
 */
{
    "switch": {
        "on": 40,
        "target": 65
    }
}

/** 
 * 4.1 批量指令下发【MCU 端可以通过JSON 对象中的字段是否含有”batch”来区分是单指令还是多指令】 
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
 * 5.1 查询设备配置
 * Wi-Fi req: getDevInfo,0\n
 * MCU ack:getDevInfo,432,{。。。｝\n
 * Wi-Fi ack: getDevInfo,2,ok\n 或者 Wi-Fi ack: getDevInfo,5,error\n
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
    "marketName": "芯中芯插座",
    "brand": "c-chip"
}


/**
 * 5.2 MCU ==> WIFI 
 * 上报服务属性，用到此报文body
 * MCU send:reportService,109,{...}
 * MCU不需要解析此报文的能力
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
 * 查 Rssi
 * MCU send: getRssi,0\n
 * WIFI ACK: getRssi,3,-35\n
 **/
 
/**
 * 5.16 MCU ==> WIFI
 * 查 IP
 * MCU send: getIp,0\n
 * WIFI ACK: getIp,34,{"status": 1,"ip":"192.168.43.86"}\n
 **/
 
/**
 * 5.17 MCU ==> WIFI
 * 查 MAC
 * MCU send: getMac,0\n
 * WIFI ACK: getMac,38,{"status":1,"mac":"fc:58:22:4f:32:c9"}\n
 **/
 
/**
 * 5.18 MCU ==> WIFI
 * 查 SSID
 * MCU send: getSsid,0\n
 * WIFI ACK: getSsid,24,{"status":1,"ssid":"K2"}\n
 **/
 
 /**
 * 5.18 MCU ==> WIFI
 * 查 getWifiStatus
 * MCU send: getWifiStatus,0\n
 * WIFI ACK: getSsid,24,{"status":1,"ssid":"K2"}\n
 **/
 
/*********************************************************************************************************************/
确定状态的依据:
为了尽量不改动原来的代码，偿试用查 sysvar/ 状态的方式的取得信息，而不是在对应状态发消息! (这样合适吗 ？或在对应的代码逻辑处加消息发送 ？)


a.机器工作状态判断依据(基本功能)：
暂以mode_determine()函数中的变化为准：或参考 wifi_Upload_STATUS()
  sysvar.Modes
  MODE_1/MODE_2/MODE_3/MODE_RINSE/MODE_CLEANING --- 对应标准模式、强力模式、强力模式、冲洗模式、自清洗模式
  
  退出工作时的状态：
  sysvar.sysfang &= ~OFF_ON; （暂不考虑自清洗模式）


b.滚筒故障(设备故障)  参考 wifi_Upload_FAULT() 函数
  (sysvar.sysfang & MOTO_ERR_1)

c.电池电量低判断依据(需求文档，第1、2条)：
暂以 perform() 函数中的变化为准：
    (sysvar.batfang & (BACH_low|BACH_lowin|BACH_lowin2) 
    
    (BACH_low|BACH_lowin|BACH_lowin2)的更多信息，可以从 perform() 函数的最后一段得到：
    sysvar.batfang & BACH_low --- 对应 sysvar.BAT_soc < 1 (如果是开机状态，还有 BATpowers_ERR)
    sysvar.batfang & BACH_lowin2 --- 对应 sysvar.BAT_soc < 3
    sysvar.batfang & BACH_lowin --- 对应 sysvar.BAT_soc < 5
    
    
d.充电状态
  暂以 CHruning() 函数中的
   (sysvar.CH_state==CH_DETECTION || sysvar.CH_state==CH_over) --- 充电检测(非充电状态)
   (sysvar.CH_state==CH_STANDBY) --- 充满(非充电状态)
   (sysvar.CH_state==CH_ING) --- 充电中()
   (sysvar.CH_state==CH_ERR) --- 充电故障
   其中只区分3个状态：非充电状态(充电完成)、充电中，充电故障
   
   CH_task2()函数中的 (sysvar.batfang & BATCHining) 还有  (sysvar.batfang & BACH_over) 是干啥的(暂无视之)
   
     
e.清水水位(不足)
  暂以 mode_determine()函数中的 IdSensor_fang(IdSensor_CLEAR) 为准
  另外，附加开机并且非大水冲洗模式 (sysvar.sysfang & OFF_ON) && (sysvar.Modes!=MODE_RINSE)
  
  IDs_Judge() 中的判断流程呢 ？
  (IdSensor_fCmp(IdSensor_CLEAR)) 需要进一步了解：IdSensor_fCmp(IdSensor_CLEAR)用于比较两次检测前后的变化！IdSensor_fang(IdSensor_CLEAR)是当前状态
  

f.水泵状态(过载、电流小)
  水泵故障：暂以 (IdSensor_fang(IdSensor_PUMPERR) 或 (IdSensor_fang(IdSensor_PUMP)  条件为准！参考 wifi_Upload_FAULT()
   从已知逻辑看，(sysvar.sysfang & (OFF_ON)) 这个条件要满足
  
  moto_run() 函数中的 (sysvar.sysfang & MOTO_ERR_2) 需要进一步了解
  
/*********************************************************************************************************************/
设备状态上报，协议定义如下：

4.2 MCU状态上报(5.3 MCU回复getChar查询也可用相同的报文体)
reportChar,xx,{"mop":{"status":0}},            // unknow 未知状态(主机断开及其它)
reportChar,xx,{"mop":{"status":1}},            // standby 待机
reportChar,xx,{"mop":{"status":2}},            // standard 标准模式
reportChar,xx,{"mop":{"status":3}},            // highPower强力模式
reportChar,xx,{"mop":{"status":4}},            // rinse 大水冲洗模式(可能用不上)
reportChar,xx,{"mop":{"status":5}},            // cleaning 自清洗模式(可能用不上)

reportChar,xx,{"roller":{"status":1}},       // normal 滚筒电机正常
reportChar,xx,{"roller":{"status":2}},       // error 滚筒电机故障

reportChar,xx,{"clearWater":{"status":1}},   // clear water normal 清水正常
reportChar,xx,{"clearWater":{"status":2}},   // clear water shortage 清水不足

reportChar,xx,{"pump":{"status":1}},           // 水泵正常     pumpNormal
reportChar,xx,{"pump":{"status":2}},           // 水泵过载     pumpOverload
reportChar,xx,{"pump":{"status":3}},           // 水泵电流过小 pumpCurrentTooSmall

reportChar,xx,{"battery":{"status":1}},        // 电池电压在正常范围
reportChar,xx,{"battery":{"status":2}},        // 电池电压过低
reportChar,xx,{"battery":{"level":%u}}         // 电池电量 (0-100)

reportChar,xx,{"charge":{"status":1}},         // 没充电
reportChar,xx,{"charge":{"status":2}},         // 正在充电
reportChar,xx,{"charge":{"status":3}},         // 充电完成(区别没充电场景)
reportChar,xx,{"charge":{"status":4}},         // 充电故障
/*********************************************************************************************************************/