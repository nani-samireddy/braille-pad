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

# braille options
enable_numbers = False
enable_letters = True
enable_capital_letters = False


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
letters_braille_dict = {
    (True, False, False, False, False, False): Keycode.A,
    (True, True, False, False, False, False): Keycode.B,
    (True, False, False, True, False, False): Keycode.C,
    (True, False, False, True, True, False): Keycode.D,
    (True, False, False, False, False, True): Keycode.E,
    (True, True, False, True, False, False): Keycode.F,
    (True, True, False, True, True, False): Keycode.G,
    (True, True, False, False, True, False): Keycode.H,
    (False, False, True, True, False, False): Keycode.I,
    (False, True, True, True, False, False): Keycode.J,
    (True, False, True, False, False, False): Keycode.K,
    (True, True, True, False, False, False): Keycode.L,
    (True, False, True, True, False, False): Keycode.M,
    (True, False, True, True, True, False): Keycode.N,
    (True, False, True, False, True, False): Keycode.O,
    (True, True, True, True, False, False): Keycode.P,
    (True, True, True, True, True, False): Keycode.Q,
    (True, True, True, False, True, False): Keycode.R,
    (False, True, True, True, False, False): Keycode.S,
    (False, True, True, True, True, False): Keycode.T,
    (True, False, True, False, False, True): Keycode.U,
    (True, True, True, False, False, True): Keycode.V,
    (False, True, False, True, True, True): Keycode.W,
    (True, False, True, True, False, True): Keycode.X,
    (True, False, True, True, True, True): Keycode.Y,
    (True, False, True, False, True, True): Keycode.Z
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

