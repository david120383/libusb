#ifndef _MY_COMMAND_
#define _MY_COMMAND_


class MyCommand
{
public:
	MyCommand();
	~MyCommand();

	unsigned short dev_addr;
	unsigned char para_buf[512];//数据域缓存，最�?12个字�?
	short para_len;//数据域长�?
	short cmd_len;//指令长度
	unsigned char cmd_buf[1024];

	unsigned char response_buf[1024] ;//接收应答缓存
	short rece_res_cnt;//接收应答缓存计数�?
	unsigned char response_parity;//计算应答校验�?
	unsigned char ReceAA;//收到0xAA数据标志


	//生成数据域，有些指令有几个参数，可以多次调用这个方法来生成数据域
	//para_in，输入参�?
	//len，para_in长度
	void MakePara(unsigned char para_in[], short len);


	//para_in，输入单字节参数
	void MakePara(unsigned char para_in);

	//把c保存到cmd_out，并且在AA后面�?0
	//返回c
	unsigned char MakeCmd(unsigned char c, unsigned char cmd_out[]);


	//组装指令，数据域不为0的，先用调用MakePara生成指令数据�?
	//addr，输入设备地址，一般输�?x0000
	//cmd，输入命�?
	//cmd_out，返回组装后的指�?
	//返回指令总长�?
	void MakeCmd(short cmd);


	//�?50协议接收数据
	//response_in，输入应答数�?
	//len，应答数据长�?
	//未接收完返回0，返�?接收成功，返�?校验错误
	unsigned char ReceResponse(unsigned char response_in[], int len);

	//返回应答指令状�?
	unsigned char GetResStatus();


	//返回应答数据域长度，不包含状态字�?
	short GetResDataLen();


	//返回应答数据�?
	//response_in，输入应答数�?
	//data_out，输出数据域数据
	//返回数据域长�?
	short GetResData(unsigned char data_out[]);

	void SetDevAddr(unsigned short addr);
	unsigned short GetDevAddr(void);
};


#endif

