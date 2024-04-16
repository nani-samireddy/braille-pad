#include <BleKeyboard.h>
#include <map>
#include <ESP.h>

BleKeyboard bleKeyboard;

// Define input pin numbers for Braille inputs
#define BRAILLE_INPUT_1 15
#define BRAILLE_INPUT_2 2
#define BRAILLE_INPUT_3 0
#define BRAILLE_INPUT_4 12
#define BRAILLE_INPUT_5 14
#define BRAILLE_INPUT_6 27

// Define input pin numbers for control buttons
#define CONTROL_BUTTON_1 4
#define CONTROL_BUTTON_2 16
#define CONTROL_BUTTON_3 26

// Define pin number for the vibration motor
#define VBP 22

// Enum to represent different modes
enum class Mode
{
  ToNumber,
  ToCharacter,
  ToCapital
};

// Initialize current mode to ToCharacter
Mode current_mode = Mode::ToCharacter;

// Map to store the conversion between braille patterns and modes
std::map<std::string, Mode> modes_map = {
    {"001111", Mode::ToNumber},
    {"000011", Mode::ToCharacter},
    {"000001", Mode::ToCapital},
};

// Control Inputs map
std::map<std::string, int> controls_map = {
    {"100", KEY_TAB},
    {"010", 0x20},
    {"001", KEY_BACKSPACE},
};

// Map to store the conversion between braille patterns and numbers
std::map<std::string, std::string> numbers_map = {
    {"100000", "1"},
    {"110000", "2"},
    // Add more entries for other numbers
};

// Map to store the conversion between braille patterns and alphabets
std::map<std::string, std::string> alphabets_map = {
    {"100000", "a"},
    {"110000", "b"},
    // Add more entries for other alphabets
};

/**
 * @brief Set the input pins to pull-up mode and the vibration motor pin to output.
 *
 * This function sets the input pins BRAILLE_INPUT_1 to BRAILLE_INPUT_6 and CONTROL_BUTTON_1 to CONTROL_BUTTON_2
 * to INPUT_PULLUP mode, and sets the VBP pin to OUTPUT mode for controlling the vibration motor.
 *
 * @return void
 */
void set_input_pins_pullup()
{
  pinMode(BRAILLE_INPUT_1, INPUT_PULLUP);
  pinMode(BRAILLE_INPUT_2, INPUT_PULLUP);
  // Set other input pins to INPUT_PULLUP mode
  pinMode(CONTROL_BUTTON_1, INPUT_PULLUP);
  pinMode(CONTROL_BUTTON_2, INPUT_PULLUP);
  pinMode(CONTROL_BUTTON_3, INPUT_PULLUP);
  pinMode(VBP, OUTPUT);
}

/**
 * @brief Send haptic feedback by toggling the vibration motor pin.
 *
 * This function toggles the VBP pin HIGH for a brief duration to simulate haptic feedback,
 * then brings it LOW again.
 *
 * @return void
 */
void send_haptic_feedback()
{
  digitalWrite(VBP, HIGH);
  delay(200);
  digitalWrite(VBP, LOW);
}

/**
 * @brief Decode the braille input by reading the digital states of input pins.
 *
 * This function reads the digital states of the input pins BRAILLE_INPUT_1 to BRAILLE_INPUT_6
 * and constructs a binary string representing the braille input.
 *
 * @return std::string - The braille input as a binary string.
 */
std::string decode_braille_input()
{
  std::string braille_input = "";
  // Read each input pin and append '1' or '0' based on its state
  braille_input += digitalRead(BRAILLE_INPUT_1) == LOW ? "1" : "0";
  // Read other input pins and append '1' or '0' based on their states
  return braille_input;
}

/**
 * @brief Get the control inputs by reading the digital states of control button pins.
 *
 * This function reads the digital states of the control button pins
 * and constructs a binary string representing the control inputs.
 *
 * @return std::string - The control inputs as a binary string.
 */
std::string get_controls_input()
{
  std::string controls_input = "";
  // Read each control button pin and append '1' or '0' based on its state
  controls_input += digitalRead(CONTROL_BUTTON_1) == LOW ? "1" : "0";
  // Read other control button pins and append '1' or '0' based on their states
  return controls_input;
}

/**
 * @brief Change the current mode based on the input braille pattern.
 *
 * This function checks the input braille pattern and changes the current mode accordingly.
 * If the pattern corresponds to a mode switch, it toggles between ToCharacter and ToCapital modes.
 *
 * @param braille_input - The input braille pattern to analyze for mode change.
 * @return void
 */
void change_mode(std::string braille_input)
{
  // Check if the braille pattern is in the modes map
  if (braille_input != "000000" && modes_map.find(braille_input) != modes_map.end())
  {
    // Toggle the mode between ToCharacter and ToCapital
    if ((current_mode == Mode::ToCharacter || current_mode == Mode::ToCapital) && 
        (modes_map[braille_input] == Mode::ToCharacter || modes_map[braille_input] == Mode::ToCapital))
    {
      current_mode = current_mode == Mode::ToCharacter ? Mode::ToCapital : Mode::ToCharacter;
    }
    else
    {
      current_mode = modes_map[braille_input];
    }
  }
}

/**
 * @brief Send braille input as keyboard output based on the current mode.
 *
 * This function processes the braille input based on the current mode and sends the corresponding
 * keyboard output using the BLE keyboard.
 *
 * @return void
 */
void process_braille_input()
{
  // Check if the input is a control key
  std::string controls_input = get_controls_input();
  // Check if it is a disconnection input.
  if(controls_input == "111"){
    disconnectBluetooth();
    return;
  }

  // Check if the input is a control combination
  if (controls_map.find(controls_input) != controls_map.end())
  {
    // Send the corresponding keyboard output
    bleKeyboard.write(static_cast<uint8_t>(controls_map[controls_input]));
    return;
  }

  std::string braille_input = decode_braille_input();
  if (braille_input == "000000")
  {
    return;
  }

  // Check if the input is a mode change
  if (modes_map.find(braille_input) != modes_map.end())
  {
    change_mode(braille_input);
    send_haptic_feedback();
    return;
  }

  // Switch statement based on the current mode
  switch (current_mode)
  {
  case Mode::ToNumber:
    // Send braille input as numbers
    if (numbers_map.find(braille_input) != numbers_map.end())
    {
      const char *number_str = numbers_map[braille_input].c_str();
      size_t len = strlen(number_str);
      for (size_t i = 0; i < len; i++)
      {
        bleKeyboard.write(static_cast<uint8_t>(number_str[i]));
      }
      return;
    }
    send_haptic_feedback();
    break;

  case Mode::ToCharacter:
    // Send braille input as characters
    if (alphabets_map.find(braille_input) != alphabets_map.end())
    {
      const char *char_str = alphabets_map[braille_input].c_str();
      size_t len = strlen(char_str);
      for (size_t i = 0; i < len; i++)
      {
        bleKeyboard.write(static_cast<uint8_t>(char_str[i]));
      }
      return;
    }
    send_haptic_feedback();
    break;

  case Mode::ToCapital:
    // Send braille input as capital characters
    if (alphabets_map.find(braille_input) != alphabets_map.end())
    {
      const char *capital_str = alphabets_map[braille_input].c_str();
      size_t len = strlen(capital_str);
      for (size_t i = 0; i < len; i++)
      {
        bleKeyboard.write(static_cast<uint8_t>(toupper(capital_str[i])));
      }
      return;
    }
    send_haptic_feedback();
    break;

  default:
    send_haptic_feedback();
    break;
  }
}

/**
 * @brief Disconnect Bluetooth if connected.
 *
 * This function checks if the BLE keyboard is connected and disconnects it if so.
 *
 * @return void
 */
void disconnectBluetooth()
{
  if (bleKeyboard.isConnected())
  {
    // Disconnect BLE keyboard and restart ESP32
    ESP.restart();
    send_haptic_feedback();
  }
}

/**
 * @brief Setup function to initialize serial communication, pins, and BLE keyboard.
 *
 * This function is called once during startup to initialize serial communication,
 * set input pins to pull-up mode, and start the BLE keyboard service.
 *
 * @return void
 */
void setup()
{
  // Start serial communication
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  // Initialize input pins and BLE keyboard
  set_input_pins_pullup();
  bleKeyboard.begin();
}

/**
 * @brief Loop function to check for Bluetooth connection and send braille input.
 *
 * This function is called repeatedly in a loop. It checks if the BLE keyboard is connected,
 * and if so, it sends the braille input based on the current mode.
 * If the keyboard is not connected, it waits for 600 milliseconds before checking again.
 *
 * @return void
 */
void loop()
{
  // Check if BLE keyboard is connected
  if (bleKeyboard.isConnected())
  {
    // Process braille input if connected
    process_braille_input();
  }
  else
  {
    // Wait for 600 milliseconds if not connected
    delay(600);
  }
}
