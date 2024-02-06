#ifndef __ADC_H
#define __ADC_H

// Files includes
#include "main.h"
#include "string.h"
#include "types.h"



#define VPWM_(i)				(i*4.095*10/(10+51)/3.3)
#define VBAT_gat(i)			(i*170)
#define SENSVAR_A(i)    (i*241)//((4095/3.3)*1*0.015*100/5.1*(10/(5.1+10)))//1V=161

#define VIN_gat(i)			(i*2*4095/5/(10+2))


#define	Vin_CHdown			16.2F
#define	Vin_CHup 				18.5F




static const u16 BATcompare_CHing[]={
		VBAT_gat(12.76255),//0
		VBAT_gat(13.09547),//1
		VBAT_gat(13.33912),//2
		VBAT_gat(13.52697),//3
		VBAT_gat(13.68506),//4
		VBAT_gat(13.77805),//5
		VBAT_gat(13.81525),//6
		VBAT_gat(13.84687),//7
		VBAT_gat(13.87477),//8
		VBAT_gat(13.90453),//9
		VBAT_gat(13.938),//10
		VBAT_gat(13.97148),//11
		VBAT_gat(14.00496),//12
		VBAT_gat(14.04588),//13
		VBAT_gat(14.08865),//14
		VBAT_gat(14.13515),//15
		VBAT_gat(14.17793),//16
		VBAT_gat(14.22257),//17
		VBAT_gat(14.26348),//18
		VBAT_gat(14.3044),//19
		VBAT_gat(14.3416),//20
		VBAT_gat(14.38066),//21
		VBAT_gat(14.41599),//22
		VBAT_gat(14.44761),//23
		VBAT_gat(14.47923),//24
		VBAT_gat(14.50527),//25
		VBAT_gat(14.53131),//26
		VBAT_gat(14.55177),//27
		VBAT_gat(14.57409),//28
		VBAT_gat(14.59268),//29
		VBAT_gat(14.61128),//30
		VBAT_gat(14.62802),//31
		VBAT_gat(14.64662),//32
		VBAT_gat(14.66522),//33
		VBAT_gat(14.68196),//34
		VBAT_gat(14.70056),//35
		VBAT_gat(14.71916),//36
		VBAT_gat(14.73776),//37
		VBAT_gat(14.76007),//38
		VBAT_gat(14.77867),//39
		VBAT_gat(14.80099),//40
		VBAT_gat(14.82145),//41
		VBAT_gat(14.84749),//42
		VBAT_gat(14.87167),//43
		VBAT_gat(14.89771),//44
		VBAT_gat(14.92746),//45
		VBAT_gat(14.9535),//46
		VBAT_gat(14.98512),//47
		VBAT_gat(15.0186),//48
		VBAT_gat(15.05022),//49
		VBAT_gat(15.0837),//50
		VBAT_gat(15.11903),//51
		VBAT_gat(15.15623),//52
		VBAT_gat(15.19343),//53
		VBAT_gat(15.23249),//54
		VBAT_gat(15.2734),//55
		VBAT_gat(15.31432),//56
		VBAT_gat(15.35524),//57
		VBAT_gat(15.39616),//58
		VBAT_gat(15.43707),//59
		VBAT_gat(15.47799),//60
		VBAT_gat(15.51705),//61
		VBAT_gat(15.55239),//62
		VBAT_gat(15.58772),//63
		VBAT_gat(15.6212),//64
		VBAT_gat(15.65468),//65
		VBAT_gat(15.68444),//66
		VBAT_gat(15.71606),//67
		VBAT_gat(15.74582),//68
		VBAT_gat(15.77371),//69
		VBAT_gat(15.80161),//70
		VBAT_gat(15.82951),//71
		VBAT_gat(15.85927),//72
		VBAT_gat(15.88717),//73
		VBAT_gat(15.91506),//74
		VBAT_gat(15.94668),//75
		VBAT_gat(15.9783),//76
		VBAT_gat(16.00992),//77
		VBAT_gat(16.04526),//78
		VBAT_gat(16.08245),//79
		VBAT_gat(16.11965),//80
		VBAT_gat(16.15871),//81
		VBAT_gat(16.19963),//82
		VBAT_gat(16.24055),//83
		VBAT_gat(16.27774),//84
		VBAT_gat(16.31866),//85
		VBAT_gat(16.35772),//86
		VBAT_gat(16.39492),//87
		VBAT_gat(16.43025),//88
		VBAT_gat(16.46001),//89
		VBAT_gat(16.49163),//90
		VBAT_gat(16.51767),//91
		VBAT_gat(16.53627),//92
		VBAT_gat(16.55487),//93
		VBAT_gat(16.575),//94
		VBAT_gat(16.585),//95
		VBAT_gat(16.595),//96
		VBAT_gat(16.61),//97
		VBAT_gat(16.621),//98
		VBAT_gat(16.63),//99
		VBAT_gat(16.635),//100


};


static const u16 BATcompare_Load[]={
		VBAT_gat(16.08617),//0
		VBAT_gat(16.00806),//1
		VBAT_gat(15.95598),//2
		VBAT_gat(15.91693),//3
		VBAT_gat(15.88717),//4
		VBAT_gat(15.86299),//5
		VBAT_gat(15.84253),//6
		VBAT_gat(15.82207),//7
		VBAT_gat(15.80347),//8
		VBAT_gat(15.78859),//9
		VBAT_gat(15.76627),//10
		VBAT_gat(15.74767),//11
		VBAT_gat(15.7235),//12
		VBAT_gat(15.70118),//13
		VBAT_gat(15.67886),//14
		VBAT_gat(15.65282),//15
		VBAT_gat(15.6212),//16
		VBAT_gat(15.58772),//17
		VBAT_gat(15.55239),//18
		VBAT_gat(15.51333),//19
		VBAT_gat(15.47241),//20
		VBAT_gat(15.43335),//21
		VBAT_gat(15.39616),//22
		VBAT_gat(15.35896),//23
		VBAT_gat(15.31804),//24
		VBAT_gat(15.27526),//25
		VBAT_gat(15.23249),//26
		VBAT_gat(15.20087),//27
		VBAT_gat(15.16367),//28
		VBAT_gat(15.12833),//29
		VBAT_gat(15.09857),//30
		VBAT_gat(15.06882),//31
		VBAT_gat(15.0372),//32
		VBAT_gat(15.00744),//33
		VBAT_gat(14.97768),//34
		VBAT_gat(14.94606),//35
		VBAT_gat(14.91259),//36
		VBAT_gat(14.88283),//37
		VBAT_gat(14.84749),//38
		VBAT_gat(14.81587),//39
		VBAT_gat(14.78053),//40
		VBAT_gat(14.74706),//41
		VBAT_gat(14.71172),//42
		VBAT_gat(14.67638),//43
		VBAT_gat(14.6429),//44
		VBAT_gat(14.60384),//45
		VBAT_gat(14.56851),//46
		VBAT_gat(14.52945),//47
		VBAT_gat(14.48667),//48
		VBAT_gat(14.44203),//49
		VBAT_gat(14.40484),//50
		VBAT_gat(14.3695),//51
		VBAT_gat(14.32858),//52
		VBAT_gat(14.28766),//53
		VBAT_gat(14.25047),//54
		VBAT_gat(14.21513),//55
		VBAT_gat(14.18165),//56
		VBAT_gat(14.15189),//57
		VBAT_gat(14.12213),//58
		VBAT_gat(14.09423),//59
		VBAT_gat(14.06448),//60
		VBAT_gat(14.03658),//61
		VBAT_gat(14.00868),//62
		VBAT_gat(13.9845),//63
		VBAT_gat(13.95846),//64
		VBAT_gat(13.93428),//65
		VBAT_gat(13.91011),//66
		VBAT_gat(13.88593),//67
		VBAT_gat(13.86361),//68
		VBAT_gat(13.83757),//69
		VBAT_gat(13.81525),//70
		VBAT_gat(13.79293),//71
		VBAT_gat(13.77061),//72
		VBAT_gat(13.74644),//73
		VBAT_gat(13.72226),//74
		VBAT_gat(13.69808),//75
		VBAT_gat(13.6739),//76
		VBAT_gat(13.64786),//77
		VBAT_gat(13.62182),//78
		VBAT_gat(13.59392),//79
		VBAT_gat(13.56789),//80
		VBAT_gat(13.54185),//81
		VBAT_gat(13.51023),//82
		VBAT_gat(13.48233),//83
		VBAT_gat(13.45257),//84
		VBAT_gat(13.42095),//85
		VBAT_gat(13.38934),//86
		VBAT_gat(13.35772),//87
		VBAT_gat(13.3261),//88
		VBAT_gat(13.29076),//89
		VBAT_gat(13.24984),//90
		VBAT_gat(13.21451),//91
		VBAT_gat(13.17359),//92
		VBAT_gat(13.12895),//93
		VBAT_gat(13.08245),//94
		VBAT_gat(13.02852),//95
		VBAT_gat(12.97086),//96
		VBAT_gat(12.90949),//97
		VBAT_gat(12.82579),//98
		VBAT_gat(12.70862),//99
		VBAT_gat(12.56169),//100

};



typedef struct{
	u16 DMA_Vin;//0
	u16 DMA_Ntc2;//1
	u16 DMA_sens;//3
	u16 DMA_Vout;//8
	u16 DMA_Vbat;//9
	u16 DMA_Temp;//
}DMAread_TypeDef;

typedef struct{
	__IO	DMAread_TypeDef ADCread;
	u16 PWMcompare;
	u16 benchmarkIout;
	s16 Calibrattion_Val;

	u16 VbatAvg;
	u16 VinAvg;
	u16 IoutAvg;
	u8 SOC;


}adc_TypeDef;


#ifdef _ADC_C_
#define GLOBAL

#else
#define GLOBAL extern

#endif
GLOBAL	adc_TypeDef  		ADC_;

#undef GLOBAL

void Gat_ADC_Avg(void);
void Get_ADC_RefValue(void);
void perform(void);
u16 Compare_(const u16* const a,const u8 n,const u16 var);
u16 Compare_Size(const u16* const a,const u8 n);
void DMAInit(void);
void ADC1_ChannelInit(void);
void ADC_AnyChanInit(void);


#endif


