# Esp32wroom
Gpredict control AZ/EL via ROTCTLD and Esp-32 Wrooom32 with bluetooth


rotctld -m 202 -r /dev/rfcomm0 -s 9600  -C timeout=200 -vvvvv

Bind Bluetooth
rfcomm bind /dev/rfcomm0 xx:xx:xx:xx:xx:xx 1

project work with:
https://www.thingiverse.com/thing:4664558

thanks you to:
https//github.com/jbyrns2000/ELAZ-Satellite-Tracker
