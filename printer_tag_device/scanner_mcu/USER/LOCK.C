#include"HeadType.h"

#define LOCK_SHORT_TIME 		20
#define LOCK_LONG_TIME			300
#define LOCK_STATE_COUNT    3			//������˲��������ڶ�ʱ�����þ��൱���˲�ʱ��

u8 Check_State;					//��λ��������ѯ״̬����������λ������
u8 Lock_Check_state;		//��״̬����
Lock_Type Lock;
//=============================================================================
//��������: LOCK_GPIO_Config
//���ܸ�Ҫ:LOCK��������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void LOCK_GPIO_Config(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	//LOCK1��������ź�
	RCC_APB2PeriphClockCmd(LOCK1_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LOCK1_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LOCK1_PORT, &GPIO_InitStructure);
	//LOCK2��������ź�
	RCC_APB2PeriphClockCmd(LOCK2_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LOCK2_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LOCK2_PORT, &GPIO_InitStructure);
		//LOCK3��������ź�
	RCC_APB2PeriphClockCmd(LOCK3_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LOCK3_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LOCK3_PORT, &GPIO_InitStructure);
	//LOCK4��������ź�
	RCC_APB2PeriphClockCmd(LOCK4_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LOCK4_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LOCK4_PORT, &GPIO_InitStructure);
	
	//LOCK1״̬��������ź�
	RCC_APB2PeriphClockCmd(LOCK1_CHECK_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LOCK1_CHECK_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LOCK1_CHECK_PORT, &GPIO_InitStructure);
	//LOCK2״̬��������ź�
	RCC_APB2PeriphClockCmd(LOCK2_CHECK_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LOCK2_CHECK_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LOCK2_CHECK_PORT, &GPIO_InitStructure);
	//LOCK3״̬��������ź�
	RCC_APB2PeriphClockCmd(LOCK3_CHECK_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LOCK3_CHECK_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LOCK3_CHECK_PORT, &GPIO_InitStructure);
	//LOCK4״̬��������ź�
	RCC_APB2PeriphClockCmd(LOCK4_CHECK_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LOCK4_CHECK_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LOCK4_CHECK_PORT, &GPIO_InitStructure);	
	
	//LOCK1����ָʾ��1
	RCC_APB2PeriphClockCmd(LOCK1_LIGHT1_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LOCK1_LIGHT1_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LOCK1_LIGHT1_PORT, &GPIO_InitStructure);
	
	//LOCK1����ָʾ��2,��������ǵƵ����ȵ���IO
	RCC_APB2PeriphClockCmd(LOCK1_LIGHT2_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LOCK1_LIGHT2_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LOCK1_LIGHT2_PORT, &GPIO_InitStructure);

//	//LOCK2����ָʾ��1
//	RCC_APB2PeriphClockCmd(LOCK2_LIGHT1_RCC,ENABLE);
//	GPIO_InitStructure.GPIO_Pin = LOCK2_LIGHT1_IO;			 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(LOCK2_LIGHT1_PORT, &GPIO_InitStructure);

//	//LOCK2����ָʾ��1
//	RCC_APB2PeriphClockCmd(LOCK2_LIGHT2_RCC,ENABLE);
//	GPIO_InitStructure.GPIO_Pin = LOCK2_LIGHT2_IO;			 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(LOCK2_LIGHT2_PORT, &GPIO_InitStructure);
	
	
	LOCK1_LIGHT_OFF();
	LOCK1_OFF;
	LOCK2_OFF;
	Lock.lock1.lock_check_state = 0;
	Lock.lock2.lock_check_state = 0;
	Lock.lock3.lock_check_state = 0;
	Lock.lock4.lock_check_state = 0;
	Check_State =0 ;
}

//���Ŀ��ƺͼ��ֿ�����Ϊ���Ŀ�����Ҫ���Ƶƣ������˹�������ʱ��Ҫ�Զ��صƣ���������״̬�ı���Ҫ�ϴ���PC
//=============================================================================
//��������: Lock_control_12
//���ܸ�Ҫ:��12���ƺ���
//��������:��
//��������:��
//ע��    :���ڶ�ʱ������
//=============================================================================
static void Lock_control_12(void )
{
	 static u8 lock1_triggerstate;
	 static u16 lock1_timercount;
	 static u8 lock2_triggerstate;
	 static u16 lock2_timercount;
	 //LOCK1
	 if(Lock.lock1.lock_state == 1){	
			if(READ_LOCK1_CHECK == READLOW){   //���ǹصģ��Ŵ򿪣���Ȼû������
				if(lock1_triggerstate == 0){
					lock1_triggerstate = 1;
				}else{
					lock1_timercount++;
				}
				if(lock1_timercount >= LOCK_SHORT_TIME){
						LOCK1_LIGHT_ON_H();//��⵽�����ˣ����Ʋ��ҹرտ����ź�,ֻ�йر��˿����źţ��ſ����˹�����
						LOCK1_OFF;
						Lock.lock1.lock_state = 2;
						lock1_triggerstate = 0;
						lock1_timercount = 0;
				}
			}
	 }else if(Lock.lock1.lock_state == 2){//����֮��һֱ�������״̬��������Ѿ��������ϣ�Ӧ�����
			if(READ_LOCK1_CHECK == READHIGH){
				if(lock1_triggerstate == 0){
					lock1_triggerstate = 1;
				}else{
					lock1_timercount++;
				}
				if(lock1_timercount >= LOCK_SHORT_TIME){
//						LOCK1_LIGHT_OFF();//�ص�
						LOCK1_OFF;
						Lock.lock1.lock_state = 0;
						lock1_triggerstate = 0;
						lock1_timercount = 0;
				}
			}
	 }else{
				lock1_triggerstate = 0;
				lock1_timercount = 0;
	 }
	 	 //LOCK2
	 if(Lock.lock2.lock_state == 1){
			if(READ_LOCK2_CHECK == READLOW){
				if(lock2_triggerstate == 0){
					lock2_triggerstate = 1;
				}else{
					lock2_timercount++;
				}
				if(lock2_timercount >= LOCK_SHORT_TIME){
//						LOCK2_LIGHT_ON();//��⵽�����ˣ����Ʋ��ҹرտ����ź�,ֻ�йر��˿����źţ��ſ����˹�����
					  LOCK1_LIGHT_ON_H();//��⵽�����ˣ����Ʋ��ҹرտ����ź�,ֻ�йر��˿����źţ��ſ����˹�����
						LOCK2_OFF;
						Lock.lock2.lock_state = 2;
						lock2_triggerstate = 0;
						lock2_timercount = 0;
				}
			}
	 }else if(Lock.lock2.lock_state == 2){//����֮��һֱ�������״̬��������Ѿ��������ϣ�Ӧ�����
			if(READ_LOCK2_CHECK == READHIGH){
				if(lock2_triggerstate == 0){
					lock2_triggerstate = 1;
				}else{
					lock2_timercount++;
				}
				if(lock2_timercount >= LOCK_SHORT_TIME){
//						LOCK2_LIGHT_OFF();//�ص�
//				  	LOCK1_LIGHT_OFF();//�ص�
						LOCK2_OFF;
						Lock.lock2.lock_state = 0;
						lock2_triggerstate = 0;
						lock2_timercount = 0;
				}
			}
	 }else{
				lock2_triggerstate = 0;
				lock2_timercount = 0;
	 }
	 
	 if(Lock.lock1.lock_state > 0){   //�����ʱ�䣬������ǿ��ģ���λ����MCU��������ʱ��״̬��λ
			 Lock.lock1.lock_time--;
			 if(Lock.lock1.lock_time <= 0){
				 LOCK1_OFF;
				 Lock.lock1.lock_time = LOCK_TIME;
			}
	}
	if(Lock.lock2.lock_state > 0){
			 Lock.lock2.lock_time--;
			 if(Lock.lock2.lock_time <= 0){
				 LOCK2_OFF;
				 Lock.lock2.lock_time = LOCK_TIME;
			}
	}
}
//���Ŀ��ƺͼ��ֿ�����Ϊ���Ŀ�����Ҫ���Ƶƣ������˹�������ʱ��Ҫ�Զ��صƣ���������״̬�ı���Ҫ�ϴ���PC
//=============================================================================
//��������: Lock_control_34
//���ܸ�Ҫ:��34���ƺ���
//��������:��
//��������:��
//ע��    :���ڶ�ʱ������
//=============================================================================
static void Lock_control_34(void )
{
	 static u8 lock3_triggerstate;
	 static u16 lock3_timercount;
	 static u8 lock4_triggerstate;
	 static u16 lock4_timercount;
	 //LOCK3
	 if(Lock.lock3.lock_state == 1){	
			if(READ_LOCK3_CHECK == READLOW){
				if(lock3_triggerstate == 0){
					lock3_triggerstate = 1;
				}else{
					lock3_timercount++;
				}
				if(lock3_timercount >= LOCK_SHORT_TIME){
						LOCK1_LIGHT_ON_H();//��⵽�����ˣ����Ʋ��ҹرտ����ź�,ֻ�йر��˿����źţ��ſ����˹�����
						LOCK3_OFF;
						Lock.lock3.lock_state = 2;
						lock3_triggerstate = 0;
						lock3_timercount = 0;
				}
			}
	 }else if(Lock.lock3.lock_state == 2){//����֮��һֱ�������״̬��������Ѿ��������ϣ�Ӧ�����
			if(READ_LOCK3_CHECK == READHIGH){
				if(lock3_triggerstate == 0){
					lock3_triggerstate = 1;
				}else{
					lock3_timercount++;
				}
				if(lock3_timercount >= LOCK_SHORT_TIME){
//						LOCK1_LIGHT_OFF();//�ص�
						LOCK3_OFF;
						Lock.lock3.lock_state = 0;
						lock3_triggerstate = 0;
						lock3_timercount = 0;
				}
			}
	 }else{
				lock3_triggerstate = 0;
				lock3_timercount = 0;
	 }
	 	 //LOCK4
	 if(Lock.lock4.lock_state == 1){
			if(READ_LOCK4_CHECK == READLOW){
				if(lock4_triggerstate == 0){
					lock4_triggerstate = 1;
				}else{
					lock4_timercount++;
				}
				if(lock4_timercount >= LOCK_SHORT_TIME){
//						LOCK2_LIGHT_ON();//��⵽�����ˣ����Ʋ��ҹرտ����ź�,ֻ�йر��˿����źţ��ſ����˹�����
					  LOCK1_LIGHT_ON_H();//��⵽�����ˣ����Ʋ��ҹرտ����ź�,ֻ�йر��˿����źţ��ſ����˹�����
						LOCK4_OFF;
						Lock.lock4.lock_state = 2;
						lock4_triggerstate = 0;
						lock4_timercount = 0;
				}
			}
	 }else if(Lock.lock4.lock_state == 2){//����֮��һֱ�������״̬��������Ѿ��������ϣ�Ӧ�����
			if(READ_LOCK4_CHECK == READHIGH){
				if(lock4_triggerstate == 0){
					lock4_triggerstate = 1;
				}else{
					lock4_timercount++;
				}
				if(lock4_timercount >= LOCK_SHORT_TIME){
//						LOCK2_LIGHT_OFF();//�ص�
//				  	LOCK1_LIGHT_OFF();//�ص�
						LOCK4_OFF;
						Lock.lock4.lock_state = 0;
						lock4_triggerstate = 0;
						lock4_timercount = 0;
				}
			}
	 }else{
				lock4_triggerstate = 0;
				lock4_timercount = 0;
	 }
	 
	 if(Lock.lock3.lock_state > 0){
			 Lock.lock3.lock_time--;
			 if(Lock.lock3.lock_time <= 0){
				 LOCK3_OFF;
				 Lock.lock3.lock_time = LOCK_TIME;
			}
	}
	if(Lock.lock4.lock_state > 0){
			 Lock.lock4.lock_time--;
			 if(Lock.lock4.lock_time <= 0){
				 LOCK4_OFF;
				 Lock.lock4.lock_time = LOCK_TIME;
			}
	}
}
//=============================================================================
//��������: Lock_check
//���ܸ�Ҫ:��״̬��麯��
//��������:��
//��������:��
//ע��    :��
//=============================================================================
static void Lock_check(void )
{
	static u8 old_lock1_state,old_lock2_state,old_lock3_state,old_lock4_state;
  static u8 lock1_state_count,lock2_state_count,lock3_state_count,lock4_state_count;	
	
	if(Lock.lock1.lock_check_state == 0){
		if(old_lock1_state != READ_LOCK1_CHECK){
				lock1_state_count++;
				if(lock1_state_count >= LOCK_STATE_COUNT)
				{
					Lock.lock1.lock_check_state = 1;		//��״̬�仯λ��Ϊ1��ʱ����Ҫ�����ϴ����ݣ������ϴ���֮������
					Lock.lock1.lock_check_value = READ_LOCK1_CHECK;
					lock1_state_count = 0;
					old_lock1_state = Lock.lock1.lock_check_value;
				}
			}else{
					lock1_state_count = 0;
			}
	}
	if(Lock.lock2.lock_check_state == 0){
		if(old_lock2_state != READ_LOCK2_CHECK){
				lock2_state_count++;
				if(lock2_state_count >= LOCK_STATE_COUNT)
				{
					Lock.lock2.lock_check_state = 1;
					Lock.lock2.lock_check_value = READ_LOCK2_CHECK;
					lock2_state_count = 0;
					old_lock2_state = Lock.lock2.lock_check_value;
				}
			}else{
					lock2_state_count = 0;
			}
	 }
	if(Lock.lock3.lock_check_state == 0){
			if(old_lock3_state != READ_LOCK3_CHECK){
				lock3_state_count++;
				if(lock3_state_count >= LOCK_STATE_COUNT)
				{
					Lock.lock3.lock_check_state = 1;
					Lock.lock3.lock_check_value = READ_LOCK3_CHECK;
					lock3_state_count = 0;
					old_lock3_state = Lock.lock3.lock_check_value;
				}
			}else{
					lock3_state_count = 0;
			}
	 }
	 if(Lock.lock4.lock_check_state == 0){
			if(old_lock4_state != READ_LOCK4_CHECK){
				lock4_state_count++;
				if(lock4_state_count >= LOCK_STATE_COUNT)
				{
					Lock.lock4.lock_check_state = 1;
					Lock.lock4.lock_check_value = READ_LOCK4_CHECK;
					lock4_state_count = 0;
					old_lock4_state = Lock.lock4.lock_check_value;
					lock4_state_count = 0;
				}
			}else{
					lock4_state_count = 0;
			}
	}	
	if(Lock_Check_state == 0){
		if((Lock.lock1.lock_check_state ==1)||(Lock.lock2.lock_check_state ==1)||(Lock.lock2.lock_check_state ==1)||(Lock.lock4.lock_check_state ==1)){
				Lock_Check_state = 1;				
		}
	}
}
//=============================================================================
//��������: Lock_control
//���ܸ�Ҫ:�����ƺ���
//��������:��
//��������:��
//ע��    :���ڶ�ʱ������
//=============================================================================
void Lock_control(void )
{
	Lock_control_12();
	Lock_control_34();
	if((Lock.lock1.lock_state > 0)||(Lock.lock2.lock_state > 0)||(Lock.lock3.lock_state > 0)||(Lock.lock4.lock_state > 0)){
		 LOCK1_LIGHT_ON_H();//��⵽�����ˣ����Ʋ��ҹرտ����ź�,ֻ�йر��˿����źţ��ſ����˹�����
	}else{//�ĸ������ǹص�״̬���Զ��ص�
		LOCK1_LIGHT_OFF();//�ص�
	}
	Lock_check();
}












