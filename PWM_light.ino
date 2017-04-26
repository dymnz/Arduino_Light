// Pins
const int ledPin = 3;
const int resistorPin = A0;
const int infraredPin = 2;

// ADC control
const int SWING_RANGE = 10;
const int MV_SIZE = 5;

// ADC variable
int valueSum = 0;
int values[MV_SIZE];
int lastValue = -1;

// To On/Off/Remain
enum LightCommand{On, Off, Remain};

void setup() {
  Serial.begin(9600);
  pinMode(infraredPin, INPUT);
}

void loop() {
  
  read_value();
  LightCommand lightCommand = check_light_status();
  
  if (lightCommand==Off) {
    analogWrite(ledPin, 0);
    lastValue = valueSum;
    Serial.println("Switched off");    
    delay(200);
  }
  else if (lightCommand==On || (abs(valueSum - lastValue) > SWING_RANGE)) {
    analogWrite(ledPin, valueSum/2);
    lastValue = valueSum;
    Serial.println("Switched on");
  }  
}

LightCommand check_light_status() {

  if (Serial.available() > 0) {
    if (Serial.read() == '1'){
      Serial.println("command on");
      return On;
    }
    else {
      Serial.println("command off");
      return Off;             
    }
  } else if ( abs(valueSum - lastValue) > SWING_RANGE ) {
    Serial.println(valueSum);
    Serial.println(lastValue);
    Serial.println("range on");
    return On;    
  } else if (valueSum < 10) {
    Serial.println("value off");
    return Off;    
  }

  return Remain;
}

void read_value() {
  valueSum = 0;
  for (int i=0 ; i<MV_SIZE ; i++){
    values[i] = analogRead(resistorPin);
    valueSum += values[i];
    delay(30); 
  }

  valueSum /= MV_SIZE;
  
  if (valueSum > 500) valueSum = 500; 
}

