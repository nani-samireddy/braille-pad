# braille-pad
A simple braille pad for the visually impaired. It uses a 3x2 grid of buttons to input braille characters. The characters are then displayed on a 2x3 grid of LEDs. The pad is designed to be used with a ESP32 microcontroller.

## Installation
1. Clone the repository
2. Open the project in [Arduino IDE](https://www.arduino.cc/en/software)
3. Add ESP32 support to the Arduino IDE
   1. Go to File > Preferences
   2. Add `https://dl.espressif.com/dl/package_esp32_index.json` to the Additional Board Manager URLs
   3. Go to Tools > Board > Boards Manager
   4. Search for `esp32` and install the package
   5. Select the ESP32 board from Tools > Board
4. Import the required libraries
   1. ESP32-BLE-Keyboard.zip (Provided in the repository folder) 
   2. By going into Arduino> Sketch > Include Library > Add .ZIP Library
5. Connect the ESP32 to your computer
6. Upload the code to the ESP32 by clicking the upload button

---
### Components
- ESP32
- 9x Push buttons
- Jump wires
- USB cable
- Vibration motor (Optional)

---
###Connection
- Connect the push buttons to the ESP32 as shown in the diagram below
