//包含头文件
#include "HID_LinuxLibusb.h"
#include <stdio.h>


#define ENDPOINT 0x02
#define bInterfaceNumber 0


CHid::CHid()
{
	dev_handle = NULL;
	ReportID = 1;
	ReportSize = 64;
	TimeOut = 200;
}


CHid::~CHid()
{
	int ret = libusb_release_interface(dev_handle, 0);
	if(ret != 0)
	{
		printf("Cannot Released Interface!\n");
	}
	else
	{
		printf("Released Interface!\n");
	}
	
	libusb_close(dev_handle);
	libusb_exit(NULL);
	dev_handle = NULL;
}


bool CHid::IsOpen(void)
{
	if (dev_handle != NULL)
	{
		return true;
	}
	return false;
}


void CHid::SetTimeOut(unsigned int wait_time)
{
	TimeOut = wait_time;
}


bool CHid::CloseHidDevice(void)
{
	int ret = libusb_release_interface(dev_handle, 0);
	if(ret != 0)
	{
		return false;
	}
	
	libusb_close(dev_handle);
	libusb_exit(NULL);
	dev_handle = NULL;
	return true;
}




/**********************************************************
*
*	Function: OpenHidDevice
*	Purpose: tries to open a HID device based on VID and PID
*	Parameters: vid - HID device's vendor ID
*				pid - HID device's product ID
*				HidDevHandle - pointer to a handle to the HID device
*	Returns: TRUE, if device is found
*			 FALSE, if device is not found
*
**********************************************************/
bool CHid::OpenHidDevice(unsigned short Vid, unsigned short Pid)
{
	int ret;
	
	ret = libusb_init(NULL);
	if(ret < 0)
	{
		fprintf(stderr, "failed to initialise libusb\n");
		return false;
	}

	dev_handle = libusb_open_device_with_vid_pid(NULL, Vid, Pid);
	if(dev_handle == NULL)
	{
		perror("Cannot open device\n");
		return false;
	}

	if(libusb_kernel_driver_active(dev_handle, bInterfaceNumber) == 1)
	{
		printf("Kernel Driver Active\n");
		if(libusb_detach_kernel_driver(dev_handle, bInterfaceNumber) == 0)
		{
			printf("Kernel Driver Detached!\n");
		}
	}

	ret = libusb_claim_interface(dev_handle, bInterfaceNumber);
	if(ret < 0)
	{
		perror("Cannot Claim Interface\n");
		return false;
	}
	return true;
}



/**********************************************************
*
*	Function ReadHid
*	Purpose: This thread data from the hid device
*	Parameters: ucDataBuffer->the data buffer to read
*				ucDataLength->the data length to read 
*	Returns: none
*
**********************************************************/
bool CHid::ReadHid(unsigned char ucDataBuffer[])
{
	if (dev_handle == NULL)
	{
		return false;
	}
	
	unsigned char buf[1024] = {0};
	int actual_length;
	
	int ret = libusb_interrupt_transfer(dev_handle, (LIBUSB_ENDPOINT_IN | ENDPOINT), buf, sizeof(buf), &actual_length, TimeOut);
	if ((ret!=0) || (actual_length != ReportSize))
	{
		printf("failed to read \n");
		return false;
	}
	
	for (unsigned int i = 0; i<ReportSize - 1; i++)
	{
		ucDataBuffer[i]	= buf[i+1];
	}
	
	return true;
}


/**********************************************************
*
*	Function WriteAndReadHid
*	Purpose: This thread write data to hid device
*	Parameters: ucTxBuffer->the data buffer to send
*				ucTxLength->the data length to send(If zero,don't send any data) 
				ucRxBuffer->the data buffer to read
*				ucRxLength->the data length to read(If zero,don't read any data) 
*	Returns: none
*
**********************************************************/
bool CHid::WriteHid(unsigned char ucTxBuffer[], unsigned int ucTxLength)
{
	
	int actual;
	unsigned char outbuffer[64];
	int TxSize;
	int offset = 0;
	int ret;
	
	if (dev_handle == NULL)
	{
		return false;
	}
	
	do{
		TxSize = ucTxLength>(ReportSize - 1) ? (ReportSize - 1) : ucTxLength;
		outbuffer[0] = ReportID;	/* this is used as the report ID */
		for(unsigned int i=0; i<TxSize; i++)		//copy data form ucDataBuffer
		{
			outbuffer[i+1]	= ucTxBuffer[i+offset];
		}

		ret = libusb_interrupt_transfer(dev_handle, (ENDPOINT | LIBUSB_ENDPOINT_OUT), outbuffer, ReportSize, &actual, TimeOut);
		
		offset += TxSize;
		ucTxLength -= TxSize;
		if((ucTxLength==0) && ((ret == 0) && (actual == ReportSize)))//发送完成
		{
			return true;
		}
	}while(((ret == 0) && (actual == ReportSize)));//end do
	
	return false;
}


