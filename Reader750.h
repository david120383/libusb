#ifndef _READER_750_H_ 
#define _READER_750_H_


#include "HID_LinuxLibusb.h"


//连接读卡器，成功返回0，失败返回2，已经打开过了返回1
//HidDevHandle，返回一个句柄
//Index，如果需要在一台电脑上连接几个读卡器，这个参数用于指定连接第几个读卡器
//如果只有一台读卡器，此参数设置为0
unsigned char OpenReader(CHid *Dev);


//成功返回0，失败返回1
//关闭后HidDevHandle会被设置成无效
unsigned char CloseReader(CHid *Dev);


//HidDevHandle有效返回0，无效返回1
unsigned char IsReaderOpen(CHid Dev);


//设置蜂鸣器
//time,单位 ms
unsigned char Sys_SetBuzzer(CHid Dev, unsigned char time);


//设置LED
unsigned char Sys_SetLight(CHid Dev, unsigned char color);


//设置蜂鸣器
//time,单位 ms
unsigned char Sys_SetBuzzer(CHid Dev, unsigned char time);


//设置LED
unsigned char Sys_SetLight(CHid Dev, unsigned char color);

//鎵撳紑鍏抽棴璇诲崱鍣ㄥぉ绾?
unsigned char Sys_SetAntenna(CHid Dev, unsigned char ant_on);

//璁剧疆璇诲崱鍣ㄥ皠棰戞ā寮?
unsigned char Sys_InitType(CHid Dev, unsigned char type);


//14443A瀵诲崱
unsigned char TyA_Request(CHid Dev, unsigned char mode, unsigned char *tag_type);


//14443A闃插啿鎾?
unsigned char TyA_Anticollision(CHid Dev, unsigned char *snr);

//閫夊畾鍗?
unsigned char TyA_Select(CHid Dev, unsigned char *snr, unsigned char *sak);

//浼戠湢鍗＄墖
unsigned char TyA_Halt(CHid Dev);

//楠岃瘉瀵嗙爜
unsigned char TyA_CS_Authentication2(CHid Dev, unsigned char model, unsigned char block, unsigned char *key);

//楠岃瘉瀵嗙爜
unsigned char TyA_CS_Read(CHid Dev, unsigned char block, unsigned char *block_data);

//楠岃瘉瀵嗙爜
unsigned char TyA_CS_Write(CHid Dev, unsigned char block, unsigned char *block_data);


#endif