#include <Keyboard.h>

//Pin 2 is clock, pin 3 is data

typedef struct {
  word keycode;
  word keychar;
} keydict;

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


word keyin;

bool releasekey= false;
bool modifiercode = false;

bool pausekey = false;

int keyarraysize;

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  pinMode(13, OUTPUT);

delay(500);
Keyboard.begin();

keyarraysize = sizeof(normal) / (sizeof(word)*2);

//Serial.begin(9600);
}

void loop() {

  if(digitalRead(2) == LOW)
  {
    digitalWrite(13, HIGH);
    keyin = read_keyboard();
    if(keyin == 2016)
    {
      releasekey = true;
    }
    else if(keyin == 1472)
    {
      modifiercode = true;
    }
    else if(keyin == 1986)
    {
      if(!pausekey)
      {
        Keyboard.write(KEY_PAUSE); //For some reason this presses H. I have no idea why
        pausekey = true;
      }
      else pausekey = false;
    }
    else if(keyin != 2016 && releasekey == false)
    {
      Keyboard.press(findchar(keyin,modifiercode));  
      modifiercode = false;
    }
    else
    {
      Keyboard.release(findchar(keyin,modifiercode));
      releasekey=false;
      modifiercode = false;
    }
    digitalWrite(13, LOW);
    //Serial.print(keyin);
    //Serial.print('\t');
    //Serial.println(printparse(keyin, modifiercode));
  }
}

word read_keyboard()
{
  word data_in = 0;
  for(int i=0; i<12; i++)
  {
    while(digitalRead(2) == HIGH);
    if(digitalRead(3) == HIGH)
    {
      bitSet(data_in, i);
    }
    else
    {
      bitClear(data_in, i);
    }
    while(digitalRead(2) == LOW);
  }  
  return data_in;
}  

word findchar(word inputval, bool modifier)
{
  if(!modifier)
  {
    for(int i=0; i<keyarraysize; i++)
    {
      if(normal[i].keycode == inputval)
      {
        return normal[i].keychar;
      }
    }
  }
  else
  {
    for(int i=0; i<keyarraysize; i++)
    {
      if(modified[i].keycode == inputval)
      {
        return modified[i].keychar;
      }
    }
  }

  return 0;
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
