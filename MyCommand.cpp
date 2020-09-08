#include "MyCommand.h"


MyCommand::MyCommand()
{	
	dev_addr = 0;
	para_len = 0;//数据域长�?
	cmd_len = 0;//指令长度

	rece_res_cnt = 0;//接收应答缓存计数�?
	response_parity = 0;//计算应答校验�?
	ReceAA = 0;//收到0xAA数据标志
}


MyCommand::~MyCommand()
{
}

//生成数据域，有些指令有几个参数，可以多次调用这个方法来生成数据域
//para_in，输入参�?
//len，para_in长度
void MyCommand::MakePara(unsigned char para_in[], short len)
{
	for (short i = 0; i<len; i++)
	{
		MakePara(para_in[i]);
	}
}

//para_in，输入单字节参数
void MyCommand::MakePara(unsigned char para_in)
{
	para_buf[para_len] = para_in;
	para_len++;
}

//把c保存到cmd_out，并且在AA后面�?0
//返回c
unsigned char MyCommand::MakeCmd(unsigned char c, unsigned char cmd_out[])
{
	cmd_out[cmd_len++] = c;
	if (c == 0xAA)
	{
		cmd_out[cmd_len++] = 0;
	}
	return c;
}

//组装指令，数据域不为0的，先用调用MakePara生成指令数据�?
//addr，输入设备地址，一般输�?x0000
//cmd，输入命�?
//cmd_out，返回组装后的指�?
//返回指令总长�?
void MyCommand::MakeCmd(short cmd)
{
	unsigned char parity = 0;
	cmd_len = 0;

	cmd_buf[cmd_len++] = (unsigned char)0xAA;
	cmd_buf[cmd_len++] = (unsigned char)0xCC;

	//设备地址
	parity ^= MakeCmd((unsigned char)(dev_addr >> 8), cmd_buf);
	parity ^= MakeCmd((unsigned char)(dev_addr & 0xFF), cmd_buf);

	//命令
	parity ^= MakeCmd((unsigned char)(cmd >> 8), cmd_buf);
	parity ^= MakeCmd((unsigned char)(cmd & 0xFF), cmd_buf);

	//长度
	parity ^= MakeCmd((unsigned char)(para_len >> 8), cmd_buf);
	parity ^= MakeCmd((unsigned char)(para_len & 0xFF), cmd_buf);

	//数据�?
	for (short i = 0; i<para_len; i++)
	{
		parity ^= MakeCmd(para_buf[i], cmd_buf);
	}

	//校验
	MakeCmd(parity, cmd_buf);
}

//�?50协议接收数据
//response_in，输入应答数�?
//len，应答数据长�?
//未接收完返回0，返�?接收成功，返�?校验错误
unsigned char MyCommand::ReceResponse(unsigned char response_in[], int len)
{
	for (int i = 0; i<len; i++)
	{
		if (ReceAA == 1)//收到转义字符
		{
			ReceAA = 0;
			if (response_in[i] == (unsigned char)0xCC)
			{
				response_buf[0] = (unsigned char)0xAA;
				response_buf[1] = (unsigned char)0xCC;
				rece_res_cnt = 2;
				response_parity = 0;
			}
		}
		else//保存数据
		{
			response_buf[rece_res_cnt++] = response_in[i];
			response_parity ^= response_in[i];
			if (response_in[i] == (unsigned char)0xAA)
			{
				ReceAA = 1;
			}
		}

		if ((rece_res_cnt > 9)//最短的应答�?0个字�?
			&& (response_buf[0] == (unsigned char)0xAA)//�?
			&& (response_buf[1] == (unsigned char)0xCC)
			&& (GetResDataLen() == (rece_res_cnt - 10)))//长度
			//&&(response_parity == 0))//校验
		{
			if (response_parity == 0)//校验
			{
				return 1;
			}
			else
			{
				return 2;//返回数据的校验错�?
			}
		}
	}
	return 0;
}

//返回应答指令状�?
unsigned char MyCommand::GetResStatus()
{
	return response_buf[8];//返回应答指令状�?
}

//返回应答数据域长度，不包含状态字�?
short MyCommand::GetResDataLen()
{
	short data_len = response_buf[6];
	data_len <<= 8;
	data_len += response_buf[7];
	return (short)(data_len - 1);//减去状态字�?
}


//返回应答数据�?
//response_in，输入应答数�?
//data_out，输出数据域数据
//返回数据域长�?
short MyCommand::GetResData(unsigned char data_out[])
{
	short data_cnt;
	short data_len = GetResDataLen();

	for (data_cnt = 0; data_cnt<data_len; data_cnt++)
	{
		data_out[data_cnt] = response_buf[9 + data_cnt];//返回应答数据�?
	}
	return data_cnt;//数据域长�?
}


void MyCommand::SetDevAddr(unsigned short addr)
{
	dev_addr = addr;
}


unsigned short MyCommand::GetDevAddr(void)
{
	return dev_addr;
}
