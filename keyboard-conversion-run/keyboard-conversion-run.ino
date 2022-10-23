#include <Keyboard.h>

//Pin 2 is clock, pin 3 is data

typedef struct {
  word keycode;
  word keychar;
} keydict;

const keydict zeos[] {
  //Alphabet
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
  //Modifiers
  {1572, KEY_LEFT_SHIFT},
  {1576, KEY_LEFT_CTRL},
  {1570, KEY_LEFT_ALT},
  //{1472, KEY_RIGHT_ALT}, //This key has weird behavior, ignoring for now
  //It appears that 1472 tells the computer that the alt or ctrl on the right side is being pressed vs the left side
  //We should be able to just ignore 1472. Will code in logic for this later
  {1714, KEY_RIGHT_SHIFT},
  //Special keys
  {1106, ' '},
  {1716, KEY_RETURN} //Enter key
};

word keyin;

bool releasekey= false;

int keyarraysize;

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  pinMode(13, OUTPUT);

delay(500);
Keyboard.begin();

keyarraysize = sizeof(zeos) / (sizeof(word)*2);
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
    else if(keyin != 2016 && releasekey == false)
    {
      Keyboard.press(findchar(keyin));  
    }
    else
    {
      Keyboard.release(findchar(keyin));
      releasekey=false;
    }
    digitalWrite(13, LOW);
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

word findchar(word inputval)
{
  for(int i=0; i<keyarraysize; i++)
  {
    if(zeos[i].keycode == inputval)
    {
      return zeos[i].keychar;
    }
  }
}
