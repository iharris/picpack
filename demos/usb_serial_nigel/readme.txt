readme.txt for usb_serial demo

20 Jan 09

Windows will ask for a driver - just point it at the directory picpack\tools\inf and it will
load the driver from there

If you want to see all the cool stuff that's going on under the hood, you can simply
uncomment the USB_DEBUG and CDC_DEBUG #defines in config.h, and include pic_serial.c in the project.
This will churn out lots of interesting debug out the serial (uart) port.
