
// For interrupt timer
int timer1_counter;

int button1 = 13;
int button2 = 1;

int val1 = 0;
int val2 = 0;
int secondsOverload = 0;

// Giving all of the 7-Segment-Array pin numbers
// For easier code reading
int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;

// For button
int dMode = 0;
long button1Timer = 0;
boolean bTimer1 = LOW;
long buttonDelay = 150;

// Our array with numbers to show on the 7-Segment-Display
  int dtabNums[4] = {1,1,1,1};
  
// Tab which digits are which pins
  int dtabDigit[4] = {12,11,10,9};
  
// Also, we need to know which one to activate now
int digitOn = 0;

void setup() {

  // Settings for timer interrupt
 // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 15624;
  TCCR1B |= (1 << WGM12);
  
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  
  
  
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
  
  interrupts();             // enable all interrupts
}

void loop() {
  
  // Read buttons
  val1 = digitalRead(button1);
  val2 = digitalRead(button2);
  
  // If button pressed
  if(val1 == HIGH){
    
    if((millis() - button1Timer) > buttonDelay){
      bTimer1 = LOW;
      button1Timer = millis(); 
    }
    
    if(bTimer1 == LOW){
      bTimer1 = HIGH;
      dMode++;
      setTimer();
      changeTime();
      secondsOverload = 0;
    }
  }
  
  if(val2 == HIGH){
      // Another button usage! Unused!
  }
  
  // --------- Drawing Logic -----------\\
  // If last digit was displayed, back to the first
  if(digitOn > 3) digitOn = 0;
  
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
    
    // Here is the logic for timer
    // Good luck reading it
 
  // We display next digit in next cycle. 
  // Not sure if we wont need to check for specific time.
  digitOn++;
}


// Function for changing number to its output combination
// So it transformes 0 to 0000001-0
// Additional zero is for the LFD039BSR-109 7-segment-display,
// it uses additional pin, for e segment of first digit
// Zeroes are ON, cause its common anode display

void changeNumbersToPins(int n){
  switch(n){
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
    
    default:
    digitalWrite(a, HIGH);
    digitalWrite(b, HIGH);
    digitalWrite(c, HIGH); 
    digitalWrite(d, HIGH); 
    digitalWrite(e, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(g, HIGH);
    break;
  }
}


  ISR(TIMER1_COMPA_vect)      // interrupt service routine 
{
  TCNT1 = timer1_counter;   // preload timer

  // Odejmuje jeden od ostatniej cyfry, jezeli spadlem ponizej 0 to robie z niej 9, a odejmuje jeden od dziesiątek
  // Jeżeli dziesiątki są == 0 to skoncz liczenie
  
  changeTime();
}

void setTimer(){
  switch(dMode){
    
        case 1:
          dtabNums[0] = 0;
          dtabNums[1] = 1;
          dtabNums[2] = 0;
          dtabNums[3] = 0;
           
        break;
        
        case 2:
          dtabNums[0] = 0;
          dtabNums[1] = 1;
          dtabNums[2] = 3;
          dtabNums[3] = 0;
        break;
        
        case 3:

          dtabNums[0] = 0;
          dtabNums[1] = 2;
          dtabNums[2] = 0;
          dtabNums[3] = 0;
        break;
        
        case 4:

          dtabNums[0] = 0;
          dtabNums[1] = 3;
          dtabNums[2] = 0;
          dtabNums[3] = 0;
        break;
        
        case 5:

          dtabNums[0] = 0;
          dtabNums[1] = 4;
          dtabNums[2] = 0;
          dtabNums[3] = 0;
        break;
        
        case 6:

          dtabNums[0] = 0;
          dtabNums[1] = 5;
          dtabNums[2] = 0;
          dtabNums[3] = 0;
        break;
        
        case 7:
          dtabNums[0] = 1;
          dtabNums[1] = 0;
          dtabNums[2] = 0;
          dtabNums[3] = 0;
        break;
        
        case 8:
          dMode = 0;
          dtabNums[0] = 0;
          dtabNums[1] = 0;
          dtabNums[2] = 3;
          dtabNums[3] = 0;
        break;
      } 
}

void changeTime(){
  if(dtabNums[3] != 0){
    dtabNums[3]--;
  } 
  else 
  if(dtabNums[3] == 0){
    if(dtabNums[2]!=0){
      dtabNums[2]--;
      dtabNums[3] = 9; 
      }
    else 
    if(dtabNums[2] == 0){
       if(dtabNums[1]!=0){
          dtabNums[1]--;
          dtabNums[2] = 5;
          dtabNums[3] = 9;
        }
     else
        if(dtabNums[1] == 0){
          if(dtabNums[0] != 0){
            dtabNums[0]--;
            dtabNums[1] = 9;
            dtabNums[2] = 5;
            dtabNums[3] = 9; 
          }
        else
          if(dtabNums[0] == 0){
            //BOOM!
            noInterrupts();
          }
        }
      }
    }
}
