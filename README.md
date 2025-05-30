This was forked from https://github.com/seguinyan/Esp32wroom/tree/main which was already a fully working version. Many thanks to seguinyan and also to https//github.com/jbyrns2000/ELAZ-Satellite-Tracker :)

I use it with this antenna rotator: https://www.thingiverse.com/thing:4664558/

## Setup
* Install packages for rfcomm (e.g. ``bluez``) and rotctld (e.g. ```libhamlib-utils```)
* Install gpredict
* Bind/open Bluetooth serial:
  * ```rfcomm bind /dev/rfcomm0 xx:xx:xx:xx:xx:xx 1```(Replace xx... with the device's MAC address, find it via Bluetooth scan.)
* Start rotctld (accepts TCP commands, controls rotator via Bluetooth serial): 
  * ```rotctld -m 202 -r /dev/rfcomm0 -s 9600  -C timeout=200 -vvvvv```
* Start gpredict, add antenna rotator:
  *   Edit -> Preferences -> Interfaces -> Rotator Tab -> Add New
  *   Enter Name, click OK
* Control antenna:
  * Upper Right Corner Burger Menu -> Antenna Control
