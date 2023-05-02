#include <Keyboard.h> //Include the keyboard emulation library for the teensy

//Pin 2 is clock, pin 3 is data

typedef struct {
  word keycode;
  word keychar;
} keydict;

//The dictionary for normal keys
const keydict normal[] {
  //Letters
  {1080, 'a'},
  {1124, 'b'},
  {1602, 'c'},
  {1094, 'd'},
  {1608, 'e'},
  {1622, 'f'},
  {1128, 'g'},
  {1638, 'h'},
  {1158, 'i'},
  {1142, 'j'},
  {1668, 'k'},
  {1686, 'l'},
  {1652, 'm'},
  {1122, 'n'},
  {1672, 'o'},
  {1690, 'p'},
  {1066, 'q'},
  {1626, 'r'},
  {1590, 's'},
  {1112, 't'},
  {1656, 'u'},
  {1108, 'v'},
  {1594, 'w'},
  {1604, 'x'},
  {1642, 'y'},
  {1076, 'z'},

  //Numbers
  {1068, '1'},
  {1596, '2'},
  {1100, '3'},
  {1098, '4'},
  {1628, '5'},
  {1644, '6'},
  {1146, '7'},
  {1148, '8'},
  {1164, '9'},
  {1162, '0'},

  //Characters
  {1052, '`'},
  {1692, '-'},
  {1706, '='},
  {1210, '\\'},
  {1192, '['},
  {1206, ']'},
  {1176, ';'},
  {1188, '\''},
  {1666, ','},
  {1170, '.'},
  {1172, '/'},

  //Function keys
  {1260, KEY_ESC},
  {1546, KEY_F1},
  {1548, KEY_F2},
  {1032, KEY_F3},
  {1560, KEY_F4},
  {1542, KEY_F5},
  {1046, KEY_F6},
  {1286, KEY_F7},
  {1556, KEY_F8},
  {1026, KEY_F9},
  {1554, KEY_F10},
  {1776, KEY_F11},
  {1038, KEY_F12},

  //Modifiers
  {1572, KEY_LEFT_SHIFT},
  {1576, KEY_LEFT_CTRL},
  {1570, KEY_LEFT_ALT},
  {1714, KEY_RIGHT_SHIFT},
  {1788, KEY_SCROLL_LOCK},
  {1774, KEY_NUM_LOCK},
  
  //Keypad
  {1272, KEYPAD_ASTERIX},
  {1782, KEYPAD_MINUS},
  {1266, KEYPAD_PLUS},
  {1746, KEYPAD_1},
  {1764, KEYPAD_2},
  {1268, KEYPAD_3},
  {1238, KEYPAD_4},
  {1254, KEYPAD_5},
  {1768, KEYPAD_6},
  {1752, KEYPAD_7},
  {1258, KEYPAD_8},
  {1786, KEYPAD_9},  //KEY_KP_9
  {1248, KEYPAD_0},
  {1762, KEYPAD_PERIOD},

  //Special keys
  {1050, KEY_TAB},
  {1200, KEY_CAPS_LOCK},
  {1740, KEY_BACKSPACE},
  {1106, ' '},
  {1716, KEY_RETURN} //Enter key

};

//This is the dictionary for keys with the 1472 code
const keydict modified[] {

  {1576, KEY_RIGHT_CTRL},
  {1570, KEY_LEFT_GUI}, //This is actually the key right alt, but I have remapped it to meta

  {1172, KEYPAD_SLASH},

  {1716, KEYPAD_ENTER},

  {1272, KEY_PRINTSCREEN},
  {1248, KEY_INSERT},
  {1762, KEY_DELETE},
  {1752, KEY_HOME},
  {1746, KEY_END},
  {1786, KEY_PAGE_UP},
  {1268, KEY_PAGE_DOWN},
  {1258, KEY_UP_ARROW},
  {1764, KEY_DOWN_ARROW},
  {1238, KEY_LEFT_ARROW},
  {1768, KEY_RIGHT_ARROW}

};


word keyin;   //The data recieved from the keyboard

bool releasekey= false;     //the boolean value that turns true when the code for releasing a key is detected
bool modifiercode = false;  //the boolean value that turns true when the modifier code is detected

bool pausekey = false;

int keyarraysize;
int modkeyarraysize;

void setup() {
  pinMode(2, INPUT);    //Clock pin
  pinMode(3, INPUT);    //Data pin

  pinMode(13, OUTPUT);  //Status LED

delay(500);       //Wait half a second to give time to program the arduino before sketch runs
Keyboard.begin(); //Start the keyboard output of the arduino

keyarraysize = sizeof(normal) / (sizeof(word)*2);
modkeyarraysize = sizeof(modified) / (sizeof(word)*2);

//Serial.begin(9600); //Open serial port for debugging
}

void loop() {
  if(digitalRead(2) == LOW)     //The clock pin going low means a key has been pressed
  {
    digitalWrite(13, HIGH);     //Turn on status LED so we can see that data is transferring
    keyin = read_keyboard();    //Read the 
    if(keyin == 2016)           //This keycode means the following keycode has been released
    {
      releasekey = true;        //set the releasekey boolean to true to indicate that the next keycode will be released
    }
    else if(keyin == 1472)      //This is the modifier code, which means the key will be looked up in the secondary dictionary
    {
      modifiercode = true;      //Set the modifiercode bool to true so we look up the key in the secondary dictionary
    }
    else if(keyin == 1986)      //Unfinished code to handle the PAUSE key
    {                           //The PAUSE key sends a bunch of weird keycodes 
      if(!pausekey)
      {
        Keyboard.write(KEY_PAUSE); 
        pausekey = true;
      }
      else pausekey = false;
    }
    else if(releasekey == false)  //If the releasekey bool is false and keyin is not one of the previous codes, we have a keycode for a key that has been pressed
    {
      Keyboard.press(findchar(keyin,modifiercode));   //Find the character based on the keycode and press it down on the computer
      modifiercode = false;                           //reset the modifiercode bool since the modified key has been selected
    }
    else    //If the releasekey bool is true and it's not one of the previous codes, we have a keycode for a key that has been released
    {
      Keyboard.release(findchar(keyin,modifiercode)); //Find the character based on the keycode and release it
      releasekey=false;                               //reset the releasekey bool since the key has been released on the computer
      modifiercode = false;                           //reset the modifiercode bool since the modified key has been selected
    }
    digitalWrite(13, LOW);  //Turn off the status LED, as the data has been read
    //For debugging:
    //Serial.print(keyin);
    //Serial.print('\t');
    //Serial.println(printparse(keyin, modifiercode)); //Prints the code from the keyboard and its corresponding character
  }
}

word read_keyboard()  //Reads in a word from the keyboard
{
  word data_in = 0;   //Start with an empty word
  for(int i=0; i<12; i++) //Loop 12 times since there are 12 clock pulses
  {
    while(digitalRead(2) == HIGH);  //wait until clock pin goes low
    if(digitalRead(3) == HIGH)      //If the data pin is high, set bit i of the word
    {
      bitSet(data_in, i);
    }
    else                            //If the data pin is low, clear bit i
    {
      bitClear(data_in, i);
    }
    while(digitalRead(2) == LOW);   //Wait until the clock goes high again before looping back
  }  
  return data_in;   //When done, return the word recieved 
}  

word findchar(word inputval, bool modifier) //Find the character from the dictionary based on the number from the keyboard
{
  if(!modifier)   //If there is no modifier code, get the character from the regular dictionary
  {
    for(int i=0; i<keyarraysize; i++) //Iterate through the dictionary until the keycode is found
    {
      if(normal[i].keycode == inputval) //If the keycode matches the code from the keyboard, we found it
      {
        return normal[i].keychar;       //Return the character found
      }
    }
  }
  else  //If the modifier code is present, look it up in the other dictionary
  {
    for(int i=0; i<modkeyarraysize; i++)   //Same as before, but with the modifier dictionary
    {
      if(modified[i].keycode == inputval)
      {
        return modified[i].keychar;
      }
    }
  }

  return 0; //If the keycode was not found, return 0.
}

String printparse(word inputval, bool modifier)
{
  word outputchar = findchar(inputval, modifier);
  if(outputchar)
  {
    return String(char(outputchar));
  }
  else
  {
    switch (inputval)
    {
      case 2016:
        return "release";
        break;
      case 1472:
        return "modifier";
        break;
      default:
        return "UNKNOWN";
        break;
    }
  }
}
