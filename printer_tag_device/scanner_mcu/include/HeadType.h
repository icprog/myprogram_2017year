#ifndef __HEADTYPE_H_
#define __HEADTYPE_H_

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>	
#include "led.h"
#include "delay.h"
#include "exti.h"
#include "usart.h"
#include "TIM.h"
#include "communication.h"
#include "baffle.h"

/*************Typedef datatype start*******************/
typedef char int8;
typedef volatile char vint8;
typedef unsigned char uint8;
typedef volatile unsigned char vuint8;
typedef int int16;
typedef unsigned short uint16;
typedef long int32;
typedef unsigned long uint32;
typedef uint8	uchar,u8,BYTE;		/**< 8-bit value */
typedef uint8 	SOCKET;
typedef uint16	ushort,u16,WORD;	/**< 16-bit value */
typedef uint16	uint;		/**< 16-bit value */
typedef uint32	ulong;		/**< 32-bit value */
/*************Typedef datatype end*******************/

/*************define type start*******************/
#define false      (0)
#define true       (!false)

#ifndef NULL
#define NULL  ((void *)(0))
#endif


#define RxBufMax 512
#define TxBufMax 512

/*�Զ���֡��ʱ��ʱʱ��*/
#define AUTO_FRAME_TIMEOUT1  10  //10*2ms
#define AUTO_FRAME_TIMEOUT2  10  //10*2ms
#define AUTO_FRAME_TIMEOUT3  10  //10*2ms
/*�Զ���֡��ʱ��ʱʱ��*/
//��Ӧ��ʱʱ�䣬����λ��ȷ�϶�ά����ȷ��ɨ��ǹһֱɨ��ֱ���õ���һ�����ݵ��ʱ��
#define ANSWER_SCANTIME	 	 4000	  //4000*5ms
//����Ӧ��ʱʱ�䣬������λ���������ݸ���λ��������λ��û����Ӧ���ʱ��
#define NANSWER_TIME	 1000	  //1000*5ms
#define NANSWER_NUMOUT	 	 3	  //1000*5ms

#define	SCAN_CR8000 0
#define BEEP_RIGHT_COUNT  3									//RFID��ȷ����������Ĵ���
#define BEEP_ERROR_COUNT  5									//RFID���󡣷�������Ĵ���
//��Ӧ��ʱʱ�䣬����λ��ȷ�϶�ά����ȷ��ɨ��ǹһֱɨ��ֱ���õ���һ�����ݵ��ʱ��
#define ANSWER_SCANTIME	 	 4000	  //4000*5ms��ɨ����ȷ�Ķ�ά�룬ɨ��ͷһֱ����ɨ���ʱ��
//����Ӧ��ʱʱ�䣬������λ���������ݸ���λ��������λ��û����Ӧ���ʱ��
#define NANSWER_SCANTIME	 400	  //1000*5ms
//Ĭ��ɨ�������е�ɨ��ʱ�䣬ɨ��Ns,��ͣN/2s���ܵ�������3N/2s
#define DEFAULT_SCANTIME	 200

#define IRQ_TIMEOUT							4			//�ж�������ʱʱ��
/*************define type end*******************/

/*************union type start*******************/
typedef union{
	long l;
	unsigned long ul;
	int  i;
	unsigned int ui;
	short si[2];
	char  c[4];
	unsigned char uc[4];	
}UL;

typedef union{
	short i;
	unsigned short ui;
	char c[2];
	unsigned char uc[2];
}UI;

/*************union type end*******************/

/*************enum type start*******************/
enum
{
	False=0,True
};
enum{
	READLOW,READHIGH
};
typedef enum{
	SLAVE,
	HOST
}MCU_State_Type;
typedef enum{
	NO_USART_WORK,
	USART2_WORK,
	USART3_WORK,
}USART_WORK_Type;
typedef enum{
	RESERVE,
	READY,
	WORKING,
	WORKEND,
	END
}CH_Work_Enum_Type;
/*************enum type end*******************/

/*************struct type start*******************/

typedef struct{
    u16 tx_index;        //���͵�ǰ���ݵ�����
    u16 rx_index;        //���յ���ǰ���ݵ�����
    u16 tx_count;        //������������
    u16 rx_count;        //������������
    u8 rx_start;
    u8 rx_aframe;       //�Ѿ����յ�һ֡����
    u8 txbuf[RxBufMax]; //�������ݻ�����
    u8 rxbuf[RxBufMax]; //�������ݻ�����
}Usart_Type;
//������������Э���ֽ�
typedef struct{
	u8  frame_soh;
	u8  frame_x;
	u16 datasize;
	u8  RFID_state;
	u8  RFID_num1;
	u8  RFID_num2;
	u8  RFID_num3;
	u8  RFID_num4;
	u8  lock1_state;
	u8  lock2_state;
	u8  lock3_state;
	u8  lock4_state;
	u8  sensor_state;
	u8  sensor1_state;
	u8  sensor2_state;
	u8  sensor3_state;
	u8  sensor4_state;
	u8  sensor5_state;
	u8  sensor6_state;
	u16 crc16_ccitt; 
	u8  frame_end1;
	u8  frame_end2;
}Communation_Send_Type;
//����������ӦЭ���ֽ�
typedef struct{
	u8  frame_soh;
	u8  frame_x;
	u16 datasize;
	u8  lock1;
	u8  lock2;
	u8  lock3;
	u8  lock4;
	u8  RFID;
	u8  check;
	u16 crc16_ccitt; 
	u8  frame_end1;
	u8  frame_end2;
}Communation_Rec_Type;

typedef union{
	Communation_Send_Type control;
	u8	send_buf[24];	
}COMM_Send_Union_Type;

typedef union{
	Communation_Rec_Type control;
	u8	rec_buf[14];	
}COMM_Rec_Union_Type;

typedef struct{
	u8  answer_state;
	u8 	answer_numout;
	u16 answer_timeout;
	u16 Nanswer_timeout;
}Answer_Type;

typedef struct{
	CH_Work_Enum_Type  state;
	u8 	send_num;			//��Ҫ��ҩ������PC�����͹���
	u8  motor_pulse;	//���ת����Ȧ����ʹ�ô�����
	u8  send_actual;	//ʵ�ʷ�ҩ����ʹ�ô���������
	u8  motor_state;	//����Ĺ���״̬�����ڹ���ʱ������ִ��״̬ת��
	u8 	timeoutstart; //���ʵ�ʷ�ҩ������PC��Ҫ��ҩ��ʱ������ʱ����
	u16  timeout;			//���ʵ�ʷ�ҩ����ʱʱ��
	u8  motor_irqstate;//����ж������˲�״̬
	u8 motor_irqtime;//����ж������˲�ʱ��
	u8 send_actual_irqstate;	//ʵʱ�����ж������˲�״̬
	u8 send_actual_irqtime;		//ʵʱ�����ж������˲�ʱ��
	u8 motor_start_state;			//���������ʱ״̬
	u16 motor_start_time;			//�����ʱ����ʱ��
}CH_Work_Type;

typedef struct{
	CH_Work_Enum_Type  state;
	u8 	 send_time;
	u16  actual_time;
	u8	 speed_step;
	u8   start_signal;
	u8   stop_signal;
	u8   start_ok;
	u8   stop_ok;
	u8   stop_delay_state;
}Belt_Work_Type;
typedef struct{
	CH_Work_Type ch1;
	CH_Work_Type ch2;
	CH_Work_Type ch3;
	CH_Work_Type ch4;
}CH_Work;

typedef struct{
	u8 comm_head;
	u8 comm_word;
	u8 data_len;
	u8 data[4];
	u8 xorsum;
}RFID_REC_Type; 

typedef struct{
	u8  frame_soh;
	u8  frame_x;
	u8  frame_r;
	u8  frame_1;
	u16 data_size;
	u8  beep_state;
	u8  beep_num;
	u8  gled_state;
	u8  gled_num;
	u8  rled_state;
	u8  rled_num;
	u8  scanner_state;
	u8  scanner_time;
	u16 crc16_ccitt; 
}Host_Communation_Type;
typedef struct{
	u8  answer_state;
	u8 start_timeout;
	u16 answer_timeout;
	u16 Nanswer_timeout;
}Host_Answer_Type;

typedef union{
	Host_Communation_Type control;
	u8	host_buf[16];	
}Host_COMM_Union_Type;
typedef struct {
	u8 state;
	u8 irqstate;
	u8 irqtime;
}Printer_Input_Type;
typedef struct{
	Printer_Input_Type baffle_inter;
	Printer_Input_Type baffle_outer;
	Printer_Input_Type scanner;
}Control_Input_Type;
typedef struct{
	u16 Scanner_Err_Time;
	u16 bag_Err_Time;
	u16 process_time;
	u8  bag_input_flag;
	u8  bag_err_flag;
	u8  bag_ok_flag;
	u8  process_flag;
}Control_Baffle_Type;
/*************struct type end*******************/

/*************extern variable start*******************/
extern char Auto_Frame_Time1;
extern char Auto_Frame_Time2;
extern char Auto_Frame_Time3;

extern  Usart_Type Usart1_Control_Data;
extern Usart_Type Usart2_Control_Data;
extern Usart_Type Usart3_Control_Data;
extern  COMM_Send_Union_Type PC_Host_Rec;
extern  COMM_Rec_Union_Type  PC_Host_Send;

extern  COMM_Send_Union_Type MCU_Host_Send;
extern  COMM_Rec_Union_Type  MCU_Host_Rec;

extern  MCU_State_Type MCU_State;
extern  Answer_Type 	 PC_Answer;

extern  CH_Work Channel;
extern	Belt_Work_Type belt11;
extern	Belt_Work_Type belt12;
extern u8 Key_ScanNum;
extern RFID_REC_Type Usart2_RFIDRec;
extern RFID_REC_Type Usart3_RFIDRec;
extern Control_Input_Type Control;
extern Control_Baffle_Type Baffle_Control;
extern u8 Device_State;
/*************extern variable end*******************/

/*************function start*******************/
u16 CRC_GetModbus16(u8 *pdata, int len);
u16 CRC_GetCCITT(u8 *pdata, int len);
u8 Key_Scan(void);
void KEY_GPIO_Config(void);
u16 switch_init_time(void);


/*************function end*******************/
#endif

