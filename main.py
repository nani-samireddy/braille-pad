import time
import board
import digitalio
import usb_hid
from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keycode import Keycode

# defining keboard object
keyboard = Keyboard(usb_hid.devices)

#defining button pins
btn1_pin = board.GP0

braille_pins = [digitalio.DigitalInOut(pin) for pin in [board.GP1, board.GP2, board.GP3, board.GP4, board.GP5, board.GP6]]
for pin in braille_pins:
    pin.direction = digitalio.Direction.INPUT
    pin.pull = digitalio.Pull.DOWN

tab = digitalio.DigitalInOut(btn1_pin)
tab.direction = digitalio.Direction.INPUT
tab.pull = digitalio.Pull.DOWN

# numbers braille dict contains the braille combination for numbers and the corresponding keycode
numbers_braille_dict = {
    (False,True,False,True,True,False): Keycode.ZERO,
    (True,False,False,False,False,False): Keycode.ONE,
    (True,True,False,False,False,False): Keycode.TWO,
    (True,False,False,True,True,False): Keycode.THREE,
    (True,False,False,True,True,False): Keycode.FOUR,
    (True,False,False,False,True,False): Keycode.FIVE,
    (True, True, False, True, False, False): Keycode.SIX,
    (True, True, False, True, True, False): Keycode.SEVEN,
    (True, True, False, False, True, False): Keycode.EIGHT,
    (False, True, False, True, False, False): Keycode.NINE
    }

def gpio_to_braille():
    braile_input = tuple(pin.value for pin in braille_pins)
    if(braile_input in numbers_braille_dict):
        print("Braille input detected")
        keyboard.press(numbers_braille_dict[braile_input])
        time.sleep(0.1)
        keyboard.release(numbers_braille_dict[braile_input])
    

# loop to check for button press
while True:
    if(tab.value):
        print("Button 1 pressed")
        keyboard.press(Keycode.GUI, Keycode.TAB)
        time.sleep(0.1)
        keyboard.release(Keycode.GUI, Keycode.TAB)
    else:
        gpio_to_braille()

    time.sleep(0.17)

