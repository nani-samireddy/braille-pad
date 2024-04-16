Certainly! Here's the updated README file with tables for the braille to text mappings added:

---

# Braille Pad for Visually Impaired

## Overview

This project aims to create a simple braille pad for the visually impaired using an ESP32 microcontroller. The pad consists of a 3x2 grid of buttons to input braille characters, which are then converted into normal text and sent as keyboard inputs using the BLE (Bluetooth Low Energy) Keyboard service. The pad also includes a vibration motor for haptic feedback and supports different modes for inputting numbers, characters, and capital letters.

## Features and Approach

1. **Braille Input Decoding**: The braille input is decoded by reading the digital states of input pins connected to the braille pad buttons (BRAILLE_INPUT_1 to BRAILLE_INPUT_6). Each button press corresponds to a binary digit (1 or 0), forming a 6-bit binary pattern representing a braille character.

2. **Control Buttons and Mode Switching**: Control buttons (CONTROL_BUTTON_1 to CONTROL_BUTTON_3) allow for mode switching and special keyboard functions. The modes include:

   - **ToNumber**: Converts braille input to numbers.
   - **ToCharacter**: Converts braille input to characters (lowercase).
   - **ToCapital**: Converts braille input to capital letters.

3. **Bluetooth Connectivity**: The ESP32 acts as a BLE keyboard, allowing the converted text to be sent wirelessly to a connected device such as a computer or smartphone.

4. **Haptic Feedback**: A vibration motor (VBP) provides haptic feedback to the user, enhancing the interactive experience.

## Installation

1. Clone the repository.
2. Open the project in [Arduino IDE](https://www.arduino.cc/en/software).
3. Add ESP32 support to the Arduino IDE:
   - Go to File > Preferences.
   - Add `https://dl.espressif.com/dl/package_esp32_index.json` to the Additional Board Manager URLs.
   - Go to Tools > Board > Boards Manager.
   - Search for `esp32` and install the package.
   - Select the ESP32 board from Tools > Board.
4. Import the required libraries:
   - ESP32-BLE-Keyboard.zip (Provided in the repository folder).
   - By going into Arduino > Sketch > Include Library > Add .ZIP Library.
5. Connect the ESP32 to your computer.
6. Upload the code to the ESP32 by clicking the upload button.

## Components

- ESP32 microcontroller
- 9x Push buttons for braille input
- Jumper wires
- USB cable for power and programming
- Vibration motor (Optional)

## Connection

Connect the push buttons and control buttons to the ESP32 as follows:

- Braille Input Buttons:
  - BRAILLE_INPUT_1 to BRAILLE_INPUT_6 connected to pins 15, 2, 0, 12, 14, 27 respectively.
- Control Buttons:
  - CONTROL_BUTTON_1 to CONTROL_BUTTON_3 connected to pins 4, 16, 26 respectively.
- Vibration Motor (VBP) connected to pin 22 for haptic feedback.

## Braille to Text Mapping

Here are the mappings from braille patterns to text characters:

### Numbers

| Braille Pattern | Number |
| --------------- | ------ |
| 100000          | 1      |
| 110000          | 2      |
| 101000          | 3      |
| 101100          | 4      |
| 100100          | 5      |
| 110100          | 6      |
| 111000          | 7      |
| 111100          | 8      |
| 110010          | 9      |
| 100110          | 0      |

### Alphabets (Lowercase)

| Braille Pattern | Character |
| --------------- | --------- |
| 100000          | a         |
| 110000          | b         |
| 100100          | c         |
| 100110          | d         |
| 100010          | e         |
| 110100          | f         |
| 110110          | g         |
| 110010          | h         |
| 010100          | i         |
| 010110          | j         |
| 101000          | k         |
| 111000          | l         |
| 101100          | m         |
| 101110          | n         |
| 101010          | o         |
| 111100          | p         |
| 111110          | q         |
| 111010          | r         |
| 011100          | s         |
| 011110          | t         |
| 101001          | u         |
| 111001          | v         |
| 010111          | w         |
| 101101          | x         |
| 101111          | y         |
| 101011          | z         |

### Alphabets (Uppercase)

| Braille Pattern | Character |
| --------------- | --------- |
| 000001          | A         |
| 000011          | B         |
| 001001          | C         |
| 001011          | D         |
| 000101          | E         |
| 001101          | F         |
| 001111          | G         |
| 001011          | H         |
| 010101          | I         |
| 010111          | J         |
| 101001          | K         |
| 111001          | L         |
| 101101          | M         |
| 101111          | N         |
| 101011          | O         |
| 111101          | P         |
| 111111          | Q         |
| 111011          | R         |
| 011101          | S         |
| 011111          | T         |
| 101010          | U         |
| 111010          | V         |
| 010110          | W         |
| 101110          | X         |
| 101111          | Y         |
| 101011          | Z         |

## Usage

1. Power on the braille pad and ESP32.
2. Press the buttons in the 3x2 grid to input braille characters.
3. The characters will be converted based on the current mode (ToNumber, ToCharacter, ToCapital)
4. Connect the ESP32 to a device (like a computer or smartphone) over Bluetooth using the BLE Keyboard service.
5. Use the braille pad to input text on the connected device.

## Additional Notes

- The code includes detailed comments explaining each function and its purpose.
- Customize the braille character mappings, control button functions, and other settings as needed for your specific application.
- Ensure proper power supply and connections for reliable operation of the braille pad.

---
