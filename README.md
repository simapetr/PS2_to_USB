# PS2_to_USB
Converter for PS2 keyboard based on Arduino Leonardo or Micro with MEGA32U4
# HID keyboard
- https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
- https://www.usb.org/hid
# Used technology 
- Arduino IDE (https://www.arduino.cc/en/Main/Software)
- Arduino Leonardo (https://www.arduino.cc/en/Main/Arduino_BoardLeonardo) or Micro (https://store.arduino.cc/arduino-micro)
- Microchip MEGA32U4
- PS2 connector (https://en.wikipedia.org/wiki/PS/2_port)
# Used library
- Paul Carpenter (https://github.com/techpaul/PS2KeyAdvanced)
# Connection
<table>
<tr>
 <th scope="col">Signal</th>
 <th scope="col">Arduino GPIO</th>
 <th scope="col">PS2</th>
</tr>
<tr>
 <td>Data</td>
 <td>2</td>
 <td>1</td>
</tr>
<tr>
 <td>Clock</td>
 <td>3</td>
 <td>5</td>
</tr>
<tr>
 <td>Ground</td>
 <td>Gnd</td>
 <td>3</td>
</tr>
<tr>
 <td>Power</td>
 <td>-5V</td>
 <td>4</td>
</tr>
</table>
