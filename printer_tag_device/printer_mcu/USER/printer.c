#include"HeadType.h"	

#define PRINTER_START_DELAY_TIME  6;
//=============================================================================
//函数名称: Printer_GPIO_Config
//功能概要:打印机引脚配置
//参数名称:无
//函数返回:无
//注意    :无
//=============================================================================
 void Printer_GPIO_Config(void)
{	
	//定义一个GPIO_InitTypeDef 类型的结构体，名字叫GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//打印机故障输入IO配置
	RCC_APB2PeriphClockCmd(PRINTER_ERR_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_ERR_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_ERR_PORT, &GPIO_InitStructure);
	//打印机打印结束输入IO配置
	RCC_APB2PeriphClockCmd(PRINTER_END_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_END_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_END_PORT, &GPIO_InitStructure);
	//打印机在线输入IO配置
	RCC_APB2PeriphClockCmd(PRINTER_INLINE_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_INLINE_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_INLINE_PORT, &GPIO_InitStructure);
	//打印机标签用完输入IO配置
	RCC_APB2PeriphClockCmd(PRINTER_TAGEND_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_TAGEND_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_TAGEND_PORT, &GPIO_InitStructure);
	//打印机色带用完输入IO配置
	RCC_APB2PeriphClockCmd(PRINTER_COEND_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_COEND_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_COEND_PORT, &GPIO_InitStructure);
	//打印机色带少输入IO配置
	RCC_APB2PeriphClockCmd(PRINTER_COLESS_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_COLESS_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_COLESS_PORT, &GPIO_InitStructure);	
	
	//打印机启动输出IO配置
	RCC_APB2PeriphClockCmd(PRINTER_START_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_START_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_START_PORT, &GPIO_InitStructure);
	//打印机重复启动输出IO配置
	RCC_APB2PeriphClockCmd(PRINTER_RESTART_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = PRINTER_RESTART_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(PRINTER_RESTART_PORT, &GPIO_InitStructure);
	

}
static void Printer_Input_Scan(void)
{
   if((Printer.err.state == 0)&&(Printer.tag_end.state == 0)&&(Printer.color_end.state == 0)){
			Printer.input_state = 1;
		}else{ //有错误必须使设备恢复到待机状态或者启动状态才可以取消错误状态
			Printer.input_state = 0;
		}

}
static u8 Printer_Process_Input(void)
{
	 u8 res;
	 Printer_Input_Scan();
	 if(Device_State == 1){  //设备启动
			if(Printer.input_state == 1){  //打印机输入状态OK
//				if(Printer.process == 1){      //打印机启动打印
//					PRINTER_START_ON;
//				}
				res = 0;
			}else{
				Device_State = 3;
				res = 1;
			}
		  return res;
		}else if(Device_State == 2){ //设备停止
			 PRINTER_START_OFF;
			 Printer.err.state = 0;
			 Printer.tag_end.state = 0;
			 Printer.color_end.state = 0;
			 Printer.pinline.state = 0;
		}else{
			 PRINTER_START_OFF;
			 Printer.err.state = 0;
			 Printer.tag_end.state = 0;
			 Printer.color_end.state = 0;
			 Printer.pinline.state = 0;
		}
		res = 1;
		return res;

}
void Printer_Control(void)
{
	static u8 working_err = 0;
	if(1 == Printer_Process_Input()){		//每个过程都需要查询一下打印机状态和设备状态是否OK
			Printer.process = PRINTER_RESERVE;		
	}else{
			Printer.process = Printer.process;	
	}
	switch(Printer.process){
		case PRINTER_RESERVE:		
													if(Device_State == 1){  //设备启动
															if(0 == Printer_Process_Input()){		//打印机OK�
																Printer.process = PRINTER_READY;		
															}
													}
													break ;
		case PRINTER_READY:   
														if(Printer.fluid_bag_timeout==0){
																Control.fluid_bag.state = 0;  //一段时间内打印机没反应，下压气缸有问题，需要清零这个状态，否则下次启动打印机会打印
															}
														if((Air_Control.complete == 1)&&(Control.fluid_bag.state == 1)&&(READ_UPPER_REACH==0)){  //开始打印的时候就把液袋输入信号置位，可以接收下一次信号输入
																Printer.process = PRINTER_WORKING;
																Printer.start_delay_time = PRINTER_START_DELAY_TIME;																																
																AIR_BLOW_ON;
																VACUUM_ON;
																PRINTER_START_ON;
																Air_Control.complete = 0;
															  Printer.printer_work_timeout = 100;
															  MCU_Host_Send.control.err_message &=0xF7;
//																Control.fluid_bag.state = 0;
														}		                  
					break ;
		case PRINTER_WORKING:   if(Printer.end.state == 1){
															Printer.process = PRINTER_END;
															Printer.complete = 1;														
															Control.fluid_bag.state = 0;
															Printer.end.state = 0;
													  }else{
															if(Printer.printer_work_timeout == 0){
																	if(READ_PRINTER_END == SET){//打印中，但是没有任务，所以为高电平
																		Printer.process = PRINTER_END;
																		Printer.complete = 1;
																		AIR_BLOW_OFF;
																		VACUUM_OFF;
																		AIR_BLOW_OFF;
																		Control.fluid_bag.state = 0;
																		Printer.end.state = 0;
																		working_err++;
																		if(working_err > 3){//连续3次发送打印操作，但是没有打印任务，也就是没有打印结束信号，设置为故障报警
																			Device_State = 3;
																			working_err = 0;	
																			MCU_Host_Send.control.err_message |=0x08;
																		}
																	}
															}
														}
					break ;
		case PRINTER_END: if(Device_State == 1){  //设备启动     
													Printer.process = PRINTER_READY; 
											}else{
													Printer.process = PRINTER_RESERVE;	
											}
					break ;
		default :break ;
	}
}




















