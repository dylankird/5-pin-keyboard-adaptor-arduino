//Pin 2 is clock, pin 3 is data

word data_in = 0;

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  pinMode(13, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  if(digitalRead(2) == LOW)
  {
    data_in = 0;
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
    Serial.println(data_in);
  }


}
