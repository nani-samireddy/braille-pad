#include <BleKeyboard.h>
#include <map>

BleKeyboard bleKeyboard;

#define BRAILLE_INPUT_1 15
#define BRAILLE_INPUT_2 2
#define BRAILLE_INPUT_3 0
#define BRAILLE_INPUT_4 12
#define BRAILLE_INPUT_5 14
#define BRAILLE_INPUT_6 27
#define CONTROL_BUTTON_1 4
#define CONTROL_BUTTON_2 26
#define CONTROL_BUTTON_3 16

// Vibration motor pin
#define VBP 22

enum class Mode
{
  ToNumber,
  ToCharacter,
  ToCapital
};

Mode current_mode = Mode::ToCharacter;

std::map<std::string, Mode> modes_map = {
    {"001111", Mode::ToNumber},
    {"000011", Mode::ToCharacter},
    {"000001", Mode::ToCapital},
};

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
  pinMode(VBP, OUTPUT);
}

void send_haptic_feedback()
{
  Serial.println("Sending Haptic Feedback");
  digitalWrite(VBP, HIGH);
  delay(800);
  digitalWrite(VBP, LOW);
}

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

// Change the mode based on the input
void change_mode(std::string braille_input)
{
  if (braille_input != "000000" && modes_map.find(braille_input) != modes_map.end())
  {
    Serial.println("Changing mode");
    // if it is to_character mode or to_capital mode, then toggle the mode
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

void send_braille_input()
{
  std::string braille_input = decode_braille_input();

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

  switch (current_mode)
  {
  case Mode::ToNumber:
    if (numbers_map.find(braille_input) != numbers_map.end())
    {
      const char *number_str = numbers_map[braille_input].c_str();
      size_t len = strlen(number_str);
      for (size_t i = 0; i < len; i++)
      {
        bleKeyboard.write(static_cast<uint8_t>(number_str[i]));
      }
    }
    return;
    break;
  case Mode::ToCharacter:
    if (alphabets_map.find(braille_input) != alphabets_map.end())
    {
      const char *char_str = alphabets_map[braille_input].c_str();
      size_t len = strlen(char_str);
      for (size_t i = 0; i < len; i++)
      {
        bleKeyboard.write(static_cast<uint8_t>(char_str[i]));
      }
    }
    return;
    break;
  case Mode::ToCapital:
    if (alphabets_map.find(braille_input) != alphabets_map.end())
    {
      const char *capital_str = alphabets_map[braille_input].c_str();
      size_t len = strlen(capital_str);
      for (size_t i = 0; i < len; i++)
      {
        bleKeyboard.write(static_cast<uint8_t>(toupper(capital_str[i])));
      }
    }
    return;
    break;
  default:
    send_haptic_feedback();
    break;
  }
  send_haptic_feedback();
}

void disconnectBluetooth()
{
  if (bleKeyboard.isConnected())
  {
    bleKeyboard.end();
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  set_input_pins_pullup();
  bleKeyboard.begin();
}

void loop()
{
  if (bleKeyboard.isConnected())
  {
    Serial.println("Bluetooth connected");
    send_braille_input();
    // send_haptic_feedback();
  }
  else
  {
    Serial.println("Bluetooth Not connected");
    Serial.println("Waiting 1 seconds...");
  }

  delay(600);
}
