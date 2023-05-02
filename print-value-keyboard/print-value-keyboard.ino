
word data_in = 0;

void setup() {
  pinMode(2, INPUT);  //clock pin
  pinMode(3, INPUT);  //data pin

  Serial.begin(9600); //start serial connection
}

void loop() {
  if(digitalRead(2) == LOW)   //start reading when the clock pin goes low
  {
    data_in = 0;              //start with an empty variable
    for(int i=0; i<12; i++)   //There are 12 clock pulses 
    {
      while(digitalRead(2) == HIGH);  //wait until the clock pin goes low
      if(digitalRead(3) == HIGH)      //If the data pin is high, set bit i
      {
        bitSet(data_in, i);
      }
      else                            //If the data pin is low, clear bit i
      {
        bitClear(data_in, i);
      }
      while(digitalRead(2) == LOW);   //Wait until the clock pin goes back to high
    }
    Serial.println(data_in);          //print the value over serial
  }
}
