#ifndef _READER_750_H_ 
#define _READER_750_H_


#include "HID_LinuxLibusb.h"


//���Ӷ��������ɹ�����0��ʧ�ܷ���2���Ѿ��򿪹��˷���1
//HidDevHandle������һ�����
//Index�������Ҫ��һ̨���������Ӽ����������������������ָ�����ӵڼ���������
//���ֻ��һ̨���������˲�������Ϊ0
unsigned char OpenReader(CHid *Dev);


//�ɹ�����0��ʧ�ܷ���1
//�رպ�HidDevHandle�ᱻ���ó���Ч
unsigned char CloseReader(CHid *Dev);


//HidDevHandle��Ч����0����Ч����1
unsigned char IsReaderOpen(CHid Dev);


//���÷�����
//time,��λ ms
unsigned char Sys_SetBuzzer(CHid Dev, unsigned char time);


//����LED
unsigned char Sys_SetLight(CHid Dev, unsigned char color);


//���÷�����
//time,��λ ms
unsigned char Sys_SetBuzzer(CHid Dev, unsigned char time);


//����LED
unsigned char Sys_SetLight(CHid Dev, unsigned char color);

//打开关闭读卡器天�?
unsigned char Sys_SetAntenna(CHid Dev, unsigned char ant_on);

//设置读卡器射频模�?
unsigned char Sys_InitType(CHid Dev, unsigned char type);


//14443A寻卡
unsigned char TyA_Request(CHid Dev, unsigned char mode, unsigned char *tag_type);


//14443A防冲�?
unsigned char TyA_Anticollision(CHid Dev, unsigned char *snr);

//选定�?
unsigned char TyA_Select(CHid Dev, unsigned char *snr, unsigned char *sak);

//休眠卡片
unsigned char TyA_Halt(CHid Dev);

//验证密码
unsigned char TyA_CS_Authentication2(CHid Dev, unsigned char model, unsigned char block, unsigned char *key);

//验证密码
unsigned char TyA_CS_Read(CHid Dev, unsigned char block, unsigned char *block_data);

//验证密码
unsigned char TyA_CS_Write(CHid Dev, unsigned char block, unsigned char *block_data);


#endif