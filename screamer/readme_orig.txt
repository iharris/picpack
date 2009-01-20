Bloader and Screamer v1.4 for Microchip PIC boot loading
Spark Fun Electronics (www.sparkfun.com)

6-16-04

The purpose of a boot loader is to load programs onto a microcontroller over a serial line without the need of a high voltage or otherwise hardware orientated programmer. What this means to you is that you will now be able to load programs much faster and much easier onto PICs then you ever dreamed of.

Bloader is the program that resides on the PIC. Screamer is the windows program that parses the HEX program file. The two work in unison to make for very fast downloads. 8K can be downloaded in under 9 seconds.

=======================================================

How to get Bloader and Screamer to work:

1) Download the latest Bloader and Screamer versions from www.sparkfun.com

2) Find the appropriate "Bloader-XXX-###.hex" file where XXX is the chip type and ### is the external oscillator speed. Ex: "Bloader-873A-20MHz.hex"

3) You must burn Bloader onto the PIC with a hardware programmer. This is generally done with a PG2C or MCP programmer.

4) Put the PIC into your breadboard if you had to remove it for programming. Your application must have a serial connection, MCLR pull-up resistor, reset button, and 5V power. We use the RS232 Shifter for all of our development but a MAX232 is always a better way to go. All the Olimex development boards have a MAX232 circuit built in, but you will have to connect the UART on the PIC to the MAX232 chip. Click here for more information.

5) Plug in the power and serial connection. Open hyperterminal and verify that the PIC is outputting 'Ok' to the terminal screen at 9600bps 8-N-1. This verifies that Bloader was loaded correctly and that your serial connection is working correctly. Close the hyperterminal window.

6) Now open Screamer. Select the Chip Type, Osc Speed, Comm Port, and Baud Rate (start with 9600). Click 'Open' and find the file "SerialTest-XXX-###.hex" file where XXX matches your chip and ### matches your oscillator speed. Click download - hit the reset button on your board. Screamer should load the new HEX file.

7) Open hyperterminal again. Hit a button on the keyboard. The new program should display "Waiting for input:" and repeat any button you press. Now hit the reset button - the same thing should appear. The PIC can be reset and if Screamer is not active, the PIC will automatically run the currently loaded program.

=======================================================

Bloader works because a select set of PIC Microcontrollers have the ability to change their program space via software. In other words, you can tell the PIC to re-program itself. Currently the only PICs that can do this are:
16F87x
16F87xA
16F88
16F819

The 16F819 is troublesome because it does not have an onboard UART so all serial routines must be done in software. This is more difficult to setup and takes more code words. The 16F819 is also limited to 2K of code words, with 300 of these words (%15) taken up by the boot loader - a sizeable chunk.

The 16F88 is a wonderful PIC! With 4K of space and nano-watt power control features, it is my highest recommendation for beginning users. Bloader can even be instituted on a 16F88 using the internal 8MHz variable oscillator

The 16F87x family is old and is being phased out. Go with the cheaper and improved 'A' revision.

The 16F87xA family have been proven over years of applications and embedded products. Bloader has currently been written for the 873A, 876A, and the 877A.

The 18F family also has the ability for a boot loader but we haven't had the time or reason (yet) to implement Bloader on this range of PICs.

=======================================================

What's a boot loader?

A boot loader is a little program running on a PIC that takes incoming input and records the incoming data into the program area of the PIC's memory. In other words, we send the PIC a serial signal of 1s and 0s and the PIC records these into the program memory.

The purpose of a boot loader is to load programs onto a microcontroller over a serial line without the need of a high voltage or otherwise hardware orientated programmer. What this means to you is that you will now be able to load programs much faster and much easier onto PICs then you ever dreamed of.

Bloader is the program that resides on the PIC. Screamer is the windows program that parses the HEX program file. The two work in unison to make for very fast downloads. 8K can be downloaded in under 9 seconds.

=======================================================

What chips does Bloader work with?

Bloader works because a select set of PIC Micocontrollers have the ability to change their program space via software. In other words, you can tell the PIC to re-program itself. Currently the only PICs that can do this are:
16F87x
16F87xA
16F88
16F819

The 16F819 is troublesome because it does not have an onboard UART so all serial routines must be done in software. This is more difficult to setup and takes more code words. The 16F819 is also limited to 2K of code words, with 300 of these words (%15) taken up by the boot loader - a sizeable chunk.

The 16F88 is a wonderful PIC! With 4K of space and nano-watt power control features, it is my highest recommendation for beginning users. Bloader can even be instituted on a 16F88 using the internal 8MHz variable oscillator!

The 16F87x family is old and is being phased out. Go with the cheaper and improved 'A' revision.

The 16F87xA family have been proven over years of applications and embedded products. Bloader has currently been written for the 873A, 876A, and the 877A.

The 18F family also has the ability for a boot loader but we haven't had the time or reason (yet) to implement Bloader on this range of PICs.

=======================================================

Things you should know:

Once you've loaded Bloader onto your PIC, it will run it's default program and transmit 'Ok' at 9600bps as well as blink all pins on all ports at roughly 1Hz. This way you can tell if your PIC is powered correctly and is running, and if you've got the serial interface wired correctly.

You must have all programs start at memory location 0x0004. This allows a boot vector to sit in spots 0x00-0x02 and the jumper vector to sit at 0x03. Interrupt vectors are still allowed and operate without problems from 0x0004. Read Code Bumping for more information about inserting the necessary commands into your compiler.

There is always a very small chance that Bloader will bit the dust in the event of some unknown and unforeseeable error. Keep your hardware programmer around in case you need to re-load Bloader.

Bloader will turn off the UART (CREN = 0 and TXEN = 0) upon exiting the boot loader program. If you program uses the UART, you will need to re-initialize the UART with your own settings.

Bloader can only update the program space. There is no way to change the configuration word without an external hardware programmer. The Oscillator setting, the Watch Dog Timer, and code protection bits are all set when Bloader is installed onto a PIC and cannot be changed during a boot load. They must be changed by re-programming the PIC. Keep this in mind.

Debugging abilities of the PIC are turned off. If you're using a debugger like the ICD2, don't even try to use Bloader as well - it'll just get too hairy. Who needs a hardware debugger anyways when you can try a new program within a second or so?

That's it! Have fun! 

=======================================================

All information is free for instructional purposes! If you use any of this information, or find any of it useful - please let us know! We don't limit the use any of our code, but it's always nice to hear when it does get used. If you really like using our boot loader, please consider buying some parts or hardware from our store at www.sparkfun.com. You may not use any part of this information for commercial deployments without permission.
