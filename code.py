import time
import digitalio
import board
import usb_hid
from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keycode import Keycode
from adafruit_hid.keyboard_layout_us import KeyboardLayoutUS

keyboard = Keyboard(usb_hid.devices)



input1_pin = board.GP0
input1 = digitalio.DigitalInOut(input1_pin)
input1.direction = digitalio.Direction.INPUT
input1.pull = digitalio.Pull.DOWN

input2_pin = board.GP1
input2 = digitalio.DigitalInOut(input2_pin)
input2.direction = digitalio.Direction.INPUT
input2.pull = digitalio.Pull.DOWN

input3_pin = board.GP2
input3 = digitalio.DigitalInOut(input3_pin)
input3.direction = digitalio.Direction.INPUT
input3.pull = digitalio.Pull.DOWN

input4_pin = board.GP3
input4 = digitalio.DigitalInOut(input4_pin)
input4.direction = digitalio.Direction.INPUT
input4.pull = digitalio.Pull.DOWN

input5_pin = board.GP4
input5 = digitalio.DigitalInOut(input5_pin)
input5.direction = digitalio.Direction.INPUT
input5.pull = digitalio.Pull.DOWN

input6_pin = board.GP5
input6 = digitalio.DigitalInOut(input6_pin)
input6.direction = digitalio.Direction.INPUT
input6.pull = digitalio.Pull.DOWN

input7_pin = board.GP6
input7 = digitalio.DigitalInOut(input7_pin)
input7.direction = digitalio.Direction.INPUT
input7.pull = digitalio.Pull.DOWN

input8_pin = board.GP7
input8 = digitalio.DigitalInOut(input8_pin)
input8.direction = digitalio.Direction.INPUT
input8.pull = digitalio.Pull.DOWN

layout = KeyboardLayoutUS(keyboard)


while True:
    
    calculate = input1.value*128 + input2.value*64 + input3.value*32 + input4.value*16 + input5.value*8 + input6.value*4 + input7.value*2 + input8.value*1
    
    if (calculate >8):
        layout.write(chr(calculate))
    time.sleep(0.0015)