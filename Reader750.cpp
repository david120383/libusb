#include "Reader750.h"
#include "MyCommand.h"


//发送和接收数据
unsigned char ExecCommand(CHid Dev, MyCommand cmd, unsigned char data_out[], unsigned int *data_len)
{
	unsigned char output[64];

	//发送
	if (!Dev.WriteHid(cmd.cmd_buf, cmd.cmd_len))
	{
		return (unsigned char)0xC5;//发送失败
	}

	//接收
	unsigned char res;
	while (Dev.ReadHid(output))
	{
		res = cmd.ReceResponse(output, 63);//驱动report ID后长度为63
		if (res == 1)//接收成功
		{
			cmd.GetResData(data_out);
			*data_len = cmd.GetResDataLen();
			return cmd.GetResStatus();
		}
		else if (res == 2)//校验错误
		{
			return (unsigned char)0xE1;
		}
	}

	return (unsigned char)0xC4;//接收失败
}


//连接读卡器，成功返回0，失败返回2，已经打开过了返回1
//HidDevHandle，返回一个句柄
//Index，如果需要在一台电脑上连接几个读卡器，这个参数用于指定连接第几个读卡器
//如果只有一台读卡器，此参数设置为0
unsigned char OpenReader(CHid *Dev)
{
	unsigned short vid = 0x0416;
	unsigned short pid = 0x8020;

	if (Dev->IsOpen())
	{
		return 1;
	}

	if (Dev->OpenHidDevice(vid, pid))
	{
		return 0;
	}
	return 2;
}


//成功返回0，失败返回1
//关闭后HidDevHandle会被设置成无效
unsigned char CloseReader(CHid *Dev)
{
	if (Dev->CloseHidDevice())
	{
		return 0;
	}
	return 1;
}


//HidDevHandle有效返回0，无效返回1
unsigned char IsReaderOpen(CHid Dev)
{
	if (Dev.IsOpen())
	{
		return 0;
	}
	return 1;
}


//设置蜂鸣器
//time,单位 ms
unsigned char Sys_SetBuzzer(CHid Dev, unsigned char time)
{
	//定义变量
	MyCommand cmd;
	unsigned char data_out[512];
	unsigned int data_len;

	//封装指令
	cmd.MakePara(time);
	short cmd_code = 0x0106;
	cmd.MakeCmd(cmd_code);
	Dev.SetTimeOut(time);
	unsigned char res = ExecCommand(Dev, cmd, data_out, &data_len);
	Dev.SetTimeOut(200);
	return res;
}


//设置LED
unsigned char Sys_SetLight(CHid Dev, unsigned char color)
{
	//定义变量
	MyCommand cmd;
	unsigned char data_out[512];
	unsigned int data_len;

	//封装指令
	cmd.MakePara(color);
	short cmd_code = 0x0107;
	cmd.MakeCmd(cmd_code);
	return ExecCommand(Dev, cmd, data_out, &data_len);
}

//鎵撳紑鍏抽棴璇诲崱鍣ㄥぉ绾?
unsigned char Sys_SetAntenna(CHid Dev, unsigned char ant_on)
{
		//定义变量
		MyCommand cmd;
		unsigned char data_out[512];
		unsigned int data_len;

		//封装指令
		cmd.MakePara(ant_on);
		short cmd_code = 0x0108;
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, data_out, &data_len);
}

    //璁剧疆璇诲崱鍣ㄥ皠棰戞ā寮?
unsigned char Sys_InitType(CHid Dev, unsigned char type)
{
		//定义变量
		MyCommand cmd;
		unsigned char data_out[512];
		unsigned int data_len;

		//封装指令
		cmd.MakePara(type);
		short cmd_code = 0x0109;
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, data_out, &data_len);
}


    //14443A瀵诲崱
unsigned char TyA_Request(CHid Dev, unsigned char mode, unsigned char *tag_type)
{
		//定义变量
		MyCommand cmd;
		//unsigned char data_out[512];
		unsigned int data_len;

		//封装指令
		cmd.MakePara(mode);
		short cmd_code = 0x1001;
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, tag_type, &data_len);
}


    //14443A闃插啿鎾?
unsigned char TyA_Anticollision(CHid Dev, unsigned char *snr)
{
		//定义变量
		MyCommand cmd;
		//unsigned char data_out[512];
		unsigned int data_len;

		//封装指令
		//cmd.MakePara(mode);
		short cmd_code = 0x1002;
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, snr, &data_len);
}

    //閫夊畾鍗?
unsigned char TyA_Select(CHid Dev, unsigned char *snr, unsigned char *sak)
{
		//定义变量
		MyCommand cmd;
		//unsigned char data_out[512];
		unsigned int data_len;

		//封装指令
		short cmd_code = 0x1003;
		cmd.MakePara(snr, (short)4);
		
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, sak, &data_len);
}

    //浼戠湢鍗＄墖
unsigned char TyA_Halt(CHid Dev)
{
		//定义变量
		MyCommand cmd;
		unsigned char data_out[512];
		unsigned int data_len;

		//封装指令
		short cmd_code = 0x1004;
		//cmd.MakePara(snr, (short)4);
		
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, data_out, &data_len);
}

    //楠岃瘉瀵嗙爜
unsigned char TyA_CS_Authentication2(CHid Dev, unsigned char model, unsigned char block, unsigned char *key)
{
		//定义变量
		MyCommand cmd;
		unsigned char data_out[512];
		unsigned int data_len;

		//封装指令
		short cmd_code = 0x100A;
		 cmd.MakePara(model);
		 cmd.MakePara(block);
		 cmd.MakePara(key, 6);
		
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, data_out, &data_len);
}

    //楠岃瘉瀵嗙爜
unsigned char TyA_CS_Read(CHid Dev, unsigned char block, unsigned char *block_data)
{
		//定义变量
		MyCommand cmd;
		//unsigned char data_out[512];
		unsigned int data_len;

		//封装指令
		short cmd_code = 0x100B;
		cmd.MakePara(block);
		
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, block_data, &data_len);
}

    //楠岃瘉瀵嗙爜
unsigned char TyA_CS_Write(CHid Dev, unsigned char block, unsigned char *block_data)
{
		//定义变量
		MyCommand cmd;
		unsigned char data_out[512];
		unsigned int data_len;

		//封装指令
		short cmd_code = 0x100c;
		cmd.MakePara(block);
		cmd.MakePara(block_data, 16);
		
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, data_out, &data_len);
}