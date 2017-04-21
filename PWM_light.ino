const int ledPin = 3;
const int resistorPin = A0;
const int infraredPin = 2;

const int SWING_RANGE = 10;
const int MV_SIZE = 5;
int valueSum = 0;
int values[MV_SIZE];
int lastValue = -1;
bool light_on = false;

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
  }
  else if (lightCommand==On || (abs(valueSum - lastValue) > SWING_RANGE)) {
    analogWrite(ledPin, valueSum/2);
    Serial.println("Switched");
  }
  
  lastValue = valueSum;
}

LightCommand check_light_status() {

  if (Serial.available() > 0) {
    if (Serial.read() == '1'){
      Serial.println("command on");
      return On;
    }
    else {
      Serial.println("off");
      return Off;             
    }
  } else if ( abs(valueSum - lastValue) > SWING_RANGE ) {
    Serial.println("range on");
    Serial.println(valueSum);
    return On;    
  } else if (valueSum < 10) {
    Serial.println("off");
    return Off;    
  }

  return Remain;
}

void read_value() {
  valueSum = 0;
  for (int i=0 ; i<MV_SIZE ; i++){
    values[i] = analogRead(resistorPin);
    valueSum += values[i];
    delay(20); 
  }

  valueSum /= MV_SIZE;
  
  if (valueSum > 500) valueSum = 500; 
}

