#include "Reader750.h"
#include "MyCommand.h"


//���ͺͽ�������
unsigned char ExecCommand(CHid Dev, MyCommand cmd, unsigned char data_out[], unsigned int *data_len)
{
	unsigned char output[64];

	//����
	if (!Dev.WriteHid(cmd.cmd_buf, cmd.cmd_len))
	{
		return (unsigned char)0xC5;//����ʧ��
	}

	//����
	unsigned char res;
	while (Dev.ReadHid(output))
	{
		res = cmd.ReceResponse(output, 63);//����report ID�󳤶�Ϊ63
		if (res == 1)//���ճɹ�
		{
			cmd.GetResData(data_out);
			*data_len = cmd.GetResDataLen();
			return cmd.GetResStatus();
		}
		else if (res == 2)//У�����
		{
			return (unsigned char)0xE1;
		}
	}

	return (unsigned char)0xC4;//����ʧ��
}


//���Ӷ��������ɹ�����0��ʧ�ܷ���2���Ѿ��򿪹��˷���1
//HidDevHandle������һ�����
//Index�������Ҫ��һ̨���������Ӽ����������������������ָ�����ӵڼ���������
//���ֻ��һ̨���������˲�������Ϊ0
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


//�ɹ�����0��ʧ�ܷ���1
//�رպ�HidDevHandle�ᱻ���ó���Ч
unsigned char CloseReader(CHid *Dev)
{
	if (Dev->CloseHidDevice())
	{
		return 0;
	}
	return 1;
}


//HidDevHandle��Ч����0����Ч����1
unsigned char IsReaderOpen(CHid Dev)
{
	if (Dev.IsOpen())
	{
		return 0;
	}
	return 1;
}


//���÷�����
//time,��λ ms
unsigned char Sys_SetBuzzer(CHid Dev, unsigned char time)
{
	//�������
	MyCommand cmd;
	unsigned char data_out[512];
	unsigned int data_len;

	//��װָ��
	cmd.MakePara(time);
	short cmd_code = 0x0106;
	cmd.MakeCmd(cmd_code);
	Dev.SetTimeOut(time);
	unsigned char res = ExecCommand(Dev, cmd, data_out, &data_len);
	Dev.SetTimeOut(200);
	return res;
}


//����LED
unsigned char Sys_SetLight(CHid Dev, unsigned char color)
{
	//�������
	MyCommand cmd;
	unsigned char data_out[512];
	unsigned int data_len;

	//��װָ��
	cmd.MakePara(color);
	short cmd_code = 0x0107;
	cmd.MakeCmd(cmd_code);
	return ExecCommand(Dev, cmd, data_out, &data_len);
}

//打开关闭读卡器天�?
unsigned char Sys_SetAntenna(CHid Dev, unsigned char ant_on)
{
		//�������
		MyCommand cmd;
		unsigned char data_out[512];
		unsigned int data_len;

		//��װָ��
		cmd.MakePara(ant_on);
		short cmd_code = 0x0108;
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, data_out, &data_len);
}

    //设置读卡器射频模�?
unsigned char Sys_InitType(CHid Dev, unsigned char type)
{
		//�������
		MyCommand cmd;
		unsigned char data_out[512];
		unsigned int data_len;

		//��װָ��
		cmd.MakePara(type);
		short cmd_code = 0x0109;
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, data_out, &data_len);
}


    //14443A寻卡
unsigned char TyA_Request(CHid Dev, unsigned char mode, unsigned char *tag_type)
{
		//�������
		MyCommand cmd;
		//unsigned char data_out[512];
		unsigned int data_len;

		//��װָ��
		cmd.MakePara(mode);
		short cmd_code = 0x1001;
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, tag_type, &data_len);
}


    //14443A防冲�?
unsigned char TyA_Anticollision(CHid Dev, unsigned char *snr)
{
		//�������
		MyCommand cmd;
		//unsigned char data_out[512];
		unsigned int data_len;

		//��װָ��
		//cmd.MakePara(mode);
		short cmd_code = 0x1002;
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, snr, &data_len);
}

    //选定�?
unsigned char TyA_Select(CHid Dev, unsigned char *snr, unsigned char *sak)
{
		//�������
		MyCommand cmd;
		//unsigned char data_out[512];
		unsigned int data_len;

		//��װָ��
		short cmd_code = 0x1003;
		cmd.MakePara(snr, (short)4);
		
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, sak, &data_len);
}

    //休眠卡片
unsigned char TyA_Halt(CHid Dev)
{
		//�������
		MyCommand cmd;
		unsigned char data_out[512];
		unsigned int data_len;

		//��װָ��
		short cmd_code = 0x1004;
		//cmd.MakePara(snr, (short)4);
		
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, data_out, &data_len);
}

    //验证密码
unsigned char TyA_CS_Authentication2(CHid Dev, unsigned char model, unsigned char block, unsigned char *key)
{
		//�������
		MyCommand cmd;
		unsigned char data_out[512];
		unsigned int data_len;

		//��װָ��
		short cmd_code = 0x100A;
		 cmd.MakePara(model);
		 cmd.MakePara(block);
		 cmd.MakePara(key, 6);
		
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, data_out, &data_len);
}

    //验证密码
unsigned char TyA_CS_Read(CHid Dev, unsigned char block, unsigned char *block_data)
{
		//�������
		MyCommand cmd;
		//unsigned char data_out[512];
		unsigned int data_len;

		//��װָ��
		short cmd_code = 0x100B;
		cmd.MakePara(block);
		
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, block_data, &data_len);
}

    //验证密码
unsigned char TyA_CS_Write(CHid Dev, unsigned char block, unsigned char *block_data)
{
		//�������
		MyCommand cmd;
		unsigned char data_out[512];
		unsigned int data_len;

		//��װָ��
		short cmd_code = 0x100c;
		cmd.MakePara(block);
		cmd.MakePara(block_data, 16);
		
		cmd.MakeCmd(cmd_code);
		return ExecCommand(Dev, cmd, data_out, &data_len);
}