int RED_LED = 13;
int GREEN_LED = 12;
int YELLOW_LED = 8;
int button = 2;

void setup()
{
}

void loop()
{
  
  if(digitalRead(button) == HIGH){
    delay(5000);
    for (int i = 0; i < 3; i++){
  		digitalWrite(GREEN_LED, LOW);
        delay(500);
        digitalWrite(GREEN_LED, HIGH);
        delay(500);
      }
	digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    delay(1000);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    delay(5000);
    digitalWrite(YELLOW_LED, HIGH);
    delay(1000);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    delay(5000);
  } else {
    digitalWrite(GREEN_LED, HIGH);
  }
}