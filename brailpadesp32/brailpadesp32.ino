/**
 * Include the libraries
 */
#include <BleKeyboard.h>

//  Bluetooth Keyboard object
BleKeyboard bleKeyboard;

/**
 * Define the pins for the buttons
 * The buttons are connected to the ESP32 pins
 * The pins are defined as INPUT_PULLUP
 * This means that the pin will read HIGH when the button is not pressed
 * and LOW when the button is pressed
 * The buttons are connected to the ESP32 pins as follows:
 * BUTTON_1_PIN 15
 * BUTTON_2_PIN 2
 * BUTTON_3_PIN 0
 * BUTTON_4_PIN 4
 * BUTTON_5_PIN 16
 * BUTTON_6_PIN 17
 * BUTTON_7_PIN 5
 * BUTTON_8_PIN 18
 */
#define BUTTON_1_PIN 15
#define BUTTON_2_PIN 2
#define BUTTON_3_PIN 0
#define BUTTON_4_PIN 4
#define BUTTON_5_PIN 16
#define BUTTON_6_PIN 17
#define BUTTON_7_PIN 5
#define BUTTON_8_PIN 18


/**
 * @brief  - Map of the numbers to the braille representation
 * - The map is a dictionary with the key as the braille representation
 * - and the value as the number
 */
std::map<std::string, std::string> numbers_map = {
  "100000": "1",
  "110000": "2",
  "100100": "3",
  "100110": "4",
  "100010": "5",
  "110100": "6",
  "110110": "7",
  "110010": "8",
  "010100": "9",
  "010110": "0"
};

/**
 * @brief  - Map of the alphabets to the braille representation
 * - The map is a dictionary with the key as the braille representation
 * - and the value as the alphabet
 */
std::map<std::string, std::string> alphabets_map = {
  "100000": "a",
  "110000": "b",
  "100100": "c",
  "100110": "d",
  "100010": "e",
  "110100": "f",
  "110110": "g",
  "110010": "h",
  "010100": "i",
  "010110": "j",
  "101000": "k",
  "111000": "l",
  "101100": "m",
  "101110": "n",
  "101010": "o",
  "111100": "p",
  "111110": "q",
  "111010": "r",
  "011100": "s",
  "011110": "t",
  "101001": "u",
  "111001": "v",
  "010111": "w",
  "101101": "x",
  "101111": "y",
  "101011": "z"
};

/**
 * @brief  - Map of the special characters to the braille representation
 * - The map is a dictionary with the key as the braille representation
 * - and the value as the special character
 */

/**
 * @brief  - function sets the input pins to INPUT_PULLUP
 * - This means that the pin will read HIGH when the button is not pressed
 * - and LOW when the button is pressed
 * @return - void
 */
void set_input_pins_pullup(){
  pinMode(BUTTON_1_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_2_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_3_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_4_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_5_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_6_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_7_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_8_PIN, INPUT_PULLUP); 
}


/**
 * @brief - function triggers haptic feedback
 * @return - void
 */
void trigger_haptic_feedback(){
  // Add code to trigger haptic feedback
}

/**
 * @brief - setup function runs once when you press reset or power the board
 * @return - void 
 */
void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  set_input_pins_pullup();
  bleKeyboard.begin();
}


/**
 * @brief loop function runs over and over again forever
 * 
 * @return * void 
 */
void loop() {
  if(bleKeyboard.isConnected()) {
      Serial.println("Bluetooth connected");
    if(digitalRead(BUTTON_1_PIN) == LOW){
      Serial.println("BUTTON 1 PRESSED");
      bleKeyboard.write('a');
    }
    if(digitalRead(BUTTON_2_PIN) == LOW){
      Serial.println("BUTTON 2 PRESSED");
      bleKeyboard.write('b');
    }
    if(digitalRead(BUTTON_3_PIN) == LOW){
      Serial.println("BUTTON 3 PRESSED");
      bleKeyboard.write('c');
    }
    if(digitalRead(BUTTON_4_PIN) == LOW){
      Serial.println("BUTTON 4 PRESSED");
      bleKeyboard.write('d');
    }
    if(digitalRead(BUTTON_5_PIN) == LOW){
      Serial.println("BUTTON 5 PRESSED");
      bleKeyboard.write('e');
    }
    if(digitalRead(BUTTON_6_PIN) == LOW){
      Serial.println("BUTTON 6 PRESSED");
      bleKeyboard.write('f');
    }
    if(digitalRead(BUTTON_7_PIN) == LOW){
      Serial.println("BUTTON 7 PRESSED");
      bleKeyboard.write('g');
    }
    if(digitalRead(BUTTON_8_PIN) == LOW){
      Serial.println("BUTTON 8 PRESSED");
      bleKeyboard.write('h');
    }
    
  }else{
      Serial.println("Bluetooth Not connected");
      Serial.println("Waiting 1 seconds...");
    }
  
  delay(1000);
}

