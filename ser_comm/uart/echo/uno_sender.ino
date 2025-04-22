const int buttonPin = 2;  // the number of the pushbutton pin

int buttonState = 0;  // variable for reading the pushbutton status

void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  // Wait for serial connection to be established
  while (!Serial) {
    ; // Wait for serial to be ready
  }
}

void loop() {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      Serial.println("BUTTON_PRESSED");
    } else {
      Serial.println("BUTTON_RELEASED");
    }

  delay(100); // Debounce and rate limit
}
