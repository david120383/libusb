#include <unistd.h>
#include <stdio.h>
#include <libusb-1.0/libusb.h>

static void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0;
	
	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}
		
		printf("%04x:%04x (bus %d, device %d)\n",
		desc.idVendor, desc.idProduct,
		libusb_get_bus_number(dev), libusb_get_device_address(dev));
	}
}

int main()
{
	int r;
	ssize_t cnt;
	libusb_device **devs;                         //devices
	
	r=libusb_init(NULL);                          //init 初始化libusb
	if(r<0) {
		printf("failed to init libusb\n");
		return 1;
	}
	cnt = libusb_get_device_list(NULL,&devs);      //获取设备列表
	if (cnt < 0) {
		printf("failed to get device list\n");
		return 1;
	}
	print_devs(devs);
	return 0;
}