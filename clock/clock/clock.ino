/* Code by Cipher */

// For interrupt timer
int timer1_counter;

// Buttons for changing hours and minutes
int button1 = 13;
int button2 = 1;

// For button, so it wont jump like hell
int buttonState1; // the current reading from the input pin
int buttonState2;
int lastButtonState1 = LOW; // the previous reading from the input pin
int lastButtonState2 = LOW;

long button1Timer = 0;  // the last time the output pin was toggled
long button2Timer = 0;
long buttonDelay = 150;    // the debounce time; increase if the output flickers

boolean bTimer1 = LOW;
boolean bTimer2 = LOW;

// Giving all of the 7-Segment-Array pin numbers
// For easier code reading
int val1 = 0;
int val2 = 0;
int secondsOverload = 0;
int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;

// Our array with numbers to show on the 7-Segment-Display
int dtabNums[4] = {1, 3, 3, 7};

// Tab which digits are which pins
int dtabDigit[4] = {12, 11, 10, 9};

// Also, we need to know which one to activate now
int digitOn = 0;

void setup() {

  // Settings for timer interrupt at 1Hz
  // initialize timer1
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 15624;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64886;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  //timer1_counter = 32768;   // preload timer 65536-16MHz/256/2Hz
  //timer1_counter = 7812;   // preload timer almost 1s
  //timer1_counter = 1786; // 1s? 65536-16MHz/256/1Hz

  //TCNT1 = timer1_counter;   // preload timer
  //TCCR1B |= (1 << CS12);    // 256 prescaler
  //TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt

  // Pins for selecting which digit is displayed
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);

  // Our outputs for specific 7-segment fragments
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);

  // Button
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  // Enable all interrupts
  interrupts();
}

void loop() {

  val1 = digitalRead(button1);
  val2 = digitalRead(button2);

  // If button pressed
  if (val1 == HIGH) {

    if ((millis() - button1Timer) > buttonDelay) {
      bTimer1 = LOW;
      button1Timer = millis();
    }

    if (bTimer1 == LOW) {
      bTimer1 = HIGH;
      buttonChangeHour();
      secondsOverload = 0;
    }
  }
  // If button pressed
  if (val2 == HIGH) {

    if ((millis() - button2Timer) > buttonDelay) {
      bTimer2 = LOW;
      button2Timer = millis();
    }

    if (bTimer2 == LOW) {
      bTimer2 = HIGH;
      buttonChangeMinute();
      secondsOverload = 0;
    }
  }

  // --------- Drawing Logic -----------\\
  // If last digit was displayed, back to the first
  if (digitOn > 3) digitOn = 0;

  // enable x digit, its incremented each loop
  digitalWrite(dtabDigit[digitOn], HIGH);

  // Change our array with time (hour/minutes or minutes/seconds)
  // we are changing it to output combinations
  // And of course we are using current digit for the number from array.
  changeNumbersToPins(dtabNums[digitOn]);

  // We delay 5 miliseconds, to be able to see all 4 numbers.
  // If we use higher value, we will see flickering
  delay(5);

  // After that, disable the digit, to show next number on next digit.
  digitalWrite(dtabDigit[digitOn], LOW);

  // Here is the logic for the clock
  // Good luck reading it

  // We display next digit in next cycle.
  // Not sure if we wont need to check for specific time.
  digitOn++;
}


// Function for changing number to its output combination
// LFD039BSR-109 7-segment-display
// So it transformes 0 to 0000001
// Zeroes are ON, cause its common anode display

void changeNumbersToPins(int n) {
  switch (n) {
    case 0:
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, HIGH);
      break;

    case 1:
      digitalWrite(a, HIGH);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, HIGH);
      digitalWrite(e, HIGH);
      digitalWrite(f, HIGH);
      digitalWrite(g, HIGH);
      break;

    case 2:
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, HIGH);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, HIGH);
      digitalWrite(g, LOW);
      break;

    case 3:
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, HIGH);
      digitalWrite(f, HIGH);
      digitalWrite(g, LOW);
      break;

    case 4:
      digitalWrite(a, HIGH);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, HIGH);
      digitalWrite(e, HIGH);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;

    case 5:
      digitalWrite(a, LOW);
      digitalWrite(b, HIGH);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, HIGH);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;

    case 6:
      digitalWrite(a, LOW);
      digitalWrite(b, HIGH);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;

    case 7:
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, HIGH);
      digitalWrite(e, HIGH);
      digitalWrite(f, HIGH);
      digitalWrite(g, HIGH);
      break;

    case 8:
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;

    case 9:
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, HIGH);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
  }
}


// Interrupt service routine
ISR(TIMER1_COMPA_vect)
{
  // Preload timer
  TCNT1 = timer1_counter;

  // Add one to the seconds overload
  secondsOverload += 1;
  // If we counted 60 seconds, change display
  // Code here for display change
  if (secondsOverload >= 60) {
    secondsOverload = 0;
    changeTime();
  }
}

void changeTime() {
  // Dodaje jeden do ostatniej cyfry, jezeli bylo 9, to robie z niej 0.
  // Jezeli powstalo zero, dodaje jeden do dziesiatek
  // Jezeli w dziesiatkach jest 6, zamieniam na 0 i dodaje do setek
  // Jezeli w setkach jest 9 zamieniam na 0 i dodaje do tysiaca
  // Jezeli w setkach jest 4 i w tysiacach 2, to zeruje.

  dtabNums[3]++;

  if (dtabNums[3] > 9) {
    dtabNums[3] = 0;
    dtabNums[2]++;
    if (dtabNums[2] >= 6) {
      dtabNums[2] = 0;
      dtabNums[1]++;
      if (dtabNums[1] > 9) {
        dtabNums[1] = 0;
        dtabNums[0] ++;
      }
      else if (dtabNums[0] == 2 && dtabNums[1] >= 4) {
        dtabNums[0] = 0;
        dtabNums[1] = 0;
        dtabNums[2] = 0;
        dtabNums[3] = 0;
      }
    }
  }
}

void buttonChangeHour() {
  dtabNums[1]++;
  if (dtabNums[1] > 9) {
    dtabNums[1] = 0;
    dtabNums[0] ++;
  }
  else if (dtabNums[0] == 2 && dtabNums[1] >= 4) {
    dtabNums[0] = 0;
    dtabNums[1] = 0;
  }
}

void buttonChangeMinute() {
  dtabNums[3]++;
  if (dtabNums[3] > 9) {
    dtabNums[3] = 0;
    dtabNums[2]++;
    if (dtabNums[2] >= 6) {
      dtabNums[2] = 0;
    }
  }
}
