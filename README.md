# Pinkpad_Keyboard_Controller
 This repo contains all the files that are needed to make the [Pinkpad](https://crafty.moe/pinkpad.htm)'s thinkpad X61 keyboard controller.
 
 BOM:
```
1x 0543630489 connector
1x Teensy LC
2x BSS138 (or compatible) sot23 NFET
4x 0603 10K Resistor
2x 0.1uf 0603 Capacitor
```

If you'd like to order the board directly, I recommend ordering [This OSHPark project](https://oshpark.com/shared_projects/3l395XbG)

This board is based on [HDR/USB_Laptop_Keyboard_Controller](https://github.com/HDR/USB_Laptop_Keyboard_Controller), I've just removed the need for a reset generator in exchange for getting rid of the caps lock LED.
