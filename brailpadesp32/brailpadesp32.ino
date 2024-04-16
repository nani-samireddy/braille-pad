#include <BleKeyboard.h>
#include <map>
#include <ESP.h>

BleKeyboard bleKeyboard;

// Define input pins for Braille inputs
#define BRAILLE_INPUT_1 15
#define BRAILLE_INPUT_2 2
#define BRAILLE_INPUT_3 0
#define BRAILLE_INPUT_4 12
#define BRAILLE_INPUT_5 14
#define BRAILLE_INPUT_6 27

// Define control button input pins
#define CONTROL_BUTTON_1 4
#define CONTROL_BUTTON_2 16
#define CONTROL_BUTTON_3 26

// Vibration motor pin
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
    {"100100", "3"},
    {"100110", "4"},
    {"100010", "5"},
    {"110100", "6"},
    {"110110", "7"},
    {"110010", "8"},
    {"010100", "9"},
    {"010110", "0"},
};

// Map to store the conversion between braille patterns and alphabets
std::map<std::string, std::string> alphabets_map = {
    {"100000", "a"},
    {"110000", "b"},
    {"100100", "c"},
    {"100110", "d"},
    {"100010", "e"},
    {"110100", "f"},
    {"110110", "g"},
    {"110010", "h"},
    {"010100", "i"},
    {"010110", "j"},
    {"101000", "k"},
    {"111000", "l"},
    {"101100", "m"},
    {"101110", "n"},
    {"101010", "o"},
    {"111100", "p"},
    {"111110", "q"},
    {"111010", "r"},
    {"011100", "s"},
    {"011110", "t"},
    {"101001", "u"},
    {"111001", "v"},
    {"010111", "w"},
    {"101101", "x"},
    {"101111", "y"},
    {"101011", "z"},
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
  pinMode(BRAILLE_INPUT_3, INPUT_PULLUP);
  pinMode(BRAILLE_INPUT_4, INPUT_PULLUP);
  pinMode(BRAILLE_INPUT_5, INPUT_PULLUP);
  pinMode(BRAILLE_INPUT_6, INPUT_PULLUP);
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
  Serial.println("Sending Haptic Feedback");
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
  braille_input += digitalRead(BRAILLE_INPUT_1) == LOW ? "1" : "0";
  braille_input += digitalRead(BRAILLE_INPUT_2) == LOW ? "1" : "0";
  braille_input += digitalRead(BRAILLE_INPUT_3) == LOW ? "1" : "0";
  braille_input += digitalRead(BRAILLE_INPUT_4) == LOW ? "1" : "0";
  braille_input += digitalRead(BRAILLE_INPUT_5) == LOW ? "1" : "0";
  braille_input += digitalRead(BRAILLE_INPUT_6) == LOW ? "1" : "0";
  return braille_input;
}

/**
 * @brief Get the control inputs by reading the digital states of control button pins.
 *
 * This function reads the digital states of the control button pins CONTROL_BUTTON_1 to CONTROL_BUTTON_3
 * and constructs a binary string representing the control input.
 *
 * @return std::string - The control input as a binary string.
 */
std::string get_controls_input()
{
  std::string controls_input = "";
  controls_input += digitalRead(CONTROL_BUTTON_1) == LOW ? "1" : "0";
  controls_input += digitalRead(CONTROL_BUTTON_2) == LOW ? "1" : "0";
  controls_input += digitalRead(CONTROL_BUTTON_3) == LOW ? "1" : "0";
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
  if (braille_input != "000000" && modes_map.find(braille_input) != modes_map.end())
  {
    Serial.println("Changing mode");
    // Toggle the mode between ToCharacter and ToCapital
    if ((current_mode == Mode::ToCharacter || current_mode == Mode::ToCapital) && (modes_map[braille_input] == Mode::ToCharacter || modes_map[braille_input] == Mode::ToCapital))
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
  if (controls_input == "111")
  {
    disconnectBluetooth();
    return;
  }

  // Check for other control combinations
  if (controls_map.find(controls_input) != controls_map.end())
  {
    bleKeyboard.write(static_cast<uint8_t>(controls_map[controls_input]));
    Serial.println(controls_input.c_str());
    return;
  }

  std::string braille_input = decode_braille_input();
  if (braille_input == "000000")
  {
    return;
  }

  // Log the braille input and current mode
  Serial.println("Braille Input");
  Serial.println(braille_input.c_str());

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
    /**
     * @brief Send braille input as numbers.
     */
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
    /**
     * @brief Send braille input as characters.
     */
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
    /**
     * @brief Send braille input as capital characters.
     */
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
    // Delay before restarting to ensure proper disconnection
    delay(600);
    // Restart the ESP32 to disconnect Bluetooth
    ESP.restart();
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
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  // Set input pins to pull-up mode and VBP pin to output
  set_input_pins_pullup();
  // Start the BLE keyboard service
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
  if (bleKeyboard.isConnected())
  {
    Serial.println("Bluetooth connected");
    // Process braille input if Bluetooth is connected
    process_braille_input();
  }
  else
  {
    Serial.println("Bluetooth Not connected");
    Serial.println("Waiting 600ms...");
  }

  // Wait for 600 milliseconds before checking again
  delay(600);
}
