#ifndef _HID_LINUX_LIBUSB_H_
#define _HID_LINUX_LIBUSB_H_


#include <libusb-1.0/libusb.h>


class CHid
{
	public:
	bool WriteHid(unsigned char ucTxBuffer[], unsigned int ucTxLength);
	bool ReadHid(unsigned char ucDataBuffer[]);
	bool OpenHidDevice(unsigned short vid, unsigned short pid);
	bool CloseHidDevice(void);
	bool IsOpen(void);
	void SetTimeOut(unsigned int wait_time);

	CHid();
	~CHid();

	protected:
	//HANDLE HidDevHandle;
	libusb_device_handle *dev_handle;
	unsigned char ReportID;
	unsigned int ReportSize;
	unsigned int TimeOut;
};


#endif

