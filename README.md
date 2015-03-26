#UPDATE: 
We are consolidating the code into a single driver as it is expanding quickly ... the new code set is locate here:
https://github.com/sfranzyshen/esp8266_tpm2net

[![Logo](http://goo.gl/w71QcJ)](https://github.com/sfranzyshen/esp8266_tpm2net)
# esp8266_tpm2net_lpd6803
This is a custom firmware for the esp8266 wifi module that will drive a strand of lpd6803 LEDs using the TPM2NET protocol. This is based on work by Frans (Frans-Willem) and popsodav ...

# How to connect hardware

Connect GPIO_0 of ESP8266 to DATA and GPIO_2 to CLCK. I have found mislabled strips ... so if it doesn't work, switch the data and clock wires.

#this needs testing ... upto 256 rgb lpd6803 leds

we need help testing this upto 256 pixels ... I can verify up to 50 pixels (that is all I have)

#How to build a build environment.

see this site from cnlohr: https://github.com/cnlohr/ws2812esp8266
