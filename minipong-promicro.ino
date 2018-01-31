 //for simplicity the pins are given variable names so that it is easier to understand which light should turn on.
const int col1 = 9 ; //column 1 is on pin 1...this is 1 in the data sheet
const int col2 = 7 ; //column 2 is on pin 3
const int col3 = 14; //column 3 is on pin 10
const int col4 = 19 ; //column 4 is on pin 7
const int col5 = 18; //column 5 is on pin 8
const int row1 = 10 ; //row 1 is on pin 12
const int row2 = 16 ; //row 1 is on pin 11
const int row3 = 8 ; //row 1 is on pin 2
const int row4 = 15 ; //row 1 is on pin 9
const int row5 = 6 ; //row 1 is on pin 4
const int row6 = 5 ; //row 1 is on pin 5
const int row7 = 4 ; //row 1 is on pin 6

const int rows = 7; //number of rows
const int cols = 5; //number of columns

const int button = 3;
boolean lastButton = LOW;    //Last Button State
boolean currentButton = LOW; //Current Button State
int mode = 8;
int modes = 9; //number of led modes
int mycol = 0; int mydata = 23;
int nc = 1; //number column
int nr = 1; //number row
int dr = 1; //direction column (move forward or backward
int dc = 1; //direction row (move forward or backward)
int pongball = 1;
int p1=0;
int p2=0;
unsigned long t2;
int t3;

void setup() {
  pinMode (button, INPUT_PULLUP);  //Set button as input (not required)
  for (int i = 4; i < 21; i++) {
    pinMode (i, OUTPUT); //Set all pins to output
  }
  pinMode (20, HIGH);
  t2 = 0;
  t3 = 1;
  Serial.begin(9600);
}


void loop() {
  currentButton = debounce(lastButton);           //read debounced state
  if (lastButton == LOW && currentButton == HIGH) //if it was pressed...
  {
    mode = mode % modes + 1;
    //mode++;                                    //increment the LED value
  }
  lastButton = currentButton;
  //if you’ve cycled through the different options, reset the counter to 0
  //if (mode == modes + 1) mode = 1;
  //  setMode(mode);//change the LED state
  if (mode == 9 && millis() % 12 < 1) {
  mycol = mycol % 7 + 1;
  xorMode(mycol,mydata);
  //xorMode(mycol,random(0,(1 << 5) - 1));
  }
  //when clock reaches wakeup time call random pixle mode
  if (millis() > t2) {
    if (mode == 1) {
      stripeScrollMode();
    }
    if (mode == 2) {
      rowScrollMode();
    }
    if (mode == 3) {
      columnScrollMode();
    }
    if (mode == 4) {
      randomPixelMode ();
    }
    if (mode == 5) {
      randomPladMode();
    }
    if (mode == 6) {
      bounce90Mode();
    }
    if (mode == 7) {
      if (nr == 1 || nr == rows ) {
        pongball = random(1, 5);
      }
      if (nr==1){p1=random(-1, 1);}
      if (nr==cols){p2=random(-1, 1);}
      if (pongball == 1) {
        columnBounceMode();
      }
      else {bounce90Mode();}
    }
    if (mode == 8) {
      onMode();
    }
    if (mode == 9) {
      mydata = mydata ^ random(0,(1 << 12) - 1);
      //offMode();//xorMode(random(0,(1 << 5) - 1));
    }

    //read in time
    //calculate time to wakeup
    t2 = millis() + 200;
    t3++;
    Serial.print(nr);
    Serial.print(' ');
    Serial.print(nc);
    Serial.print(' ');
    Serial.print(pongball);
    Serial.print(' ');
    Serial.println(mode);
  }
  if (mode == 7) {
    pongMode();
  }
}


boolean debounce(boolean last)
{
  boolean current = digitalRead(button);       //Read the button state
  if (last != current)                         //if it's different...
  {
    delay(5);                                  //wait 5ms
    current = digitalRead(button);             //read it again
  }
  return current;                              //return the current value
}

int pongMode() {
  offMode();
  setMode(nr, nc);//ball determined by bounce90Mode
  delay(1);

  //paddle1
  offMode();
  if (nc + p1 <= 1){
    setMode(1, 1);
    delay(1);
    offMode();
    setMode(1,2);
    delay(1);
    offMode();
    setMode(1,3);
    delay(1);
    }
  else if (nc + p1 >= cols) {
    setMode(1, cols);
    delay(1);
    offMode();
    setMode(1, cols-1);
    delay(1);
    offMode();
    setMode(1, cols-2);
    delay(1);
  }
  else {
    setMode(1, nc+p1-1);
    delay(1);
    offMode();
    setMode(1, nc+p1);
    delay(1);
    offMode();
    setMode(1, nc+p1+1);
    delay(1);
  }
 
  //paddle2
  offMode();
  if (nc + p2 <= 1){
    setMode(rows, 1);
    delay(1);
    offMode();
    setMode(rows,2);
    delay(1);
    offMode();
    setMode(rows,3);
    delay(1);
    }
  else if (nc + p2 >= cols) {
    setMode(rows, cols);
    delay(1);
    offMode();
    setMode(rows, cols-1);
    delay(1);
    offMode();
    setMode(rows, cols-2);
    delay(1);
  }
  else {
    setMode(rows, nc+p2-1);
    delay(1);
    offMode();
    setMode(rows, nc+p2);
    delay(1);
    offMode();
    setMode(rows, nc+p2+1);
    delay(1);
  }
}

int columnBounceMode() {
  if (nr <= 1) {
    dr = 1;
  }
  if (nr >= rows) {
    dr = -1;
  }
  nr = nr + dr;
  offMode();
  setMode(nr, nc);
}

int bounce90Mode() {
  if (nc <= 1) {
    dc = 1;
  }
  if (nc >= cols) {
    dc = -1;
  }
  if (nr <= 1) {
    dr = 1;
  }
  if (nr >= rows) {
    dr = -1;
  }
  nc = nc + dc;
  nr = nr + dr;
  offMode();
  setMode(nr, nc);
}

int randomPladMode() {
  digitalWrite(random(4, 15), HIGH);
  digitalWrite(random(4, 15), LOW);
}


int randomPixelMode () {

  int nr = random(1, rows + 1);
  int nc = random (1, cols + 1);
  offMode();
  setMode(nr, nc);
}

int stripeScrollMode () {
  nc = nc + 1;
  nr = nr + 1;
  if (nc > cols ) {
    nc = 1;
  }
  if (nr > rows ) {
    nr = 1;
  }
  offMode();
  setMode(nr, nc);
}

int rowScrollMode() {
  nc = nc + 1;
  if (nc > cols) {
    nc = 1;
    nr = nr + 1;
  }
  if (nr > rows ) {
    nr = 1;
  }
  offMode();
  setMode(nr, nc);
}

int columnScrollMode() {
  nr = nr + 1;
  if (nr > rows ) {
    nr = 1;
    nc = nc + 1;
  }
  if (nc > cols) {
    nc = 1;
  }
  offMode();
  setMode(nr, nc);
}

void xorMode(int col, const unsigned int in)
{
  offMode();
  unsigned int t; unsigned int i = in;
  for (int c = 7; c > 0; c--) {
    if (c == col) {
      t = i;
      for (int r = 1; r <= 5; r++) {
        if (t & 1 != 0) {
          setMode(c,r);
        }
        t = t >> 1;
      }
    }
    i = i ^ (i >> 1);
  }
}

void onMode()
{
  digitalWrite(col1, HIGH);
  digitalWrite(col2, HIGH);
  digitalWrite(col3, HIGH);
  digitalWrite(col4, HIGH);
  digitalWrite(col5, HIGH);
  digitalWrite(row1, LOW);
  digitalWrite(row2, LOW);
  digitalWrite(row3, LOW);
  digitalWrite(row4, LOW);
  digitalWrite(row5, LOW);
  digitalWrite(row6, LOW);
  digitalWrite(row7, LOW);
}

void offMode()
{
  digitalWrite(col1, LOW);
  digitalWrite(col2, LOW);
  digitalWrite(col3, LOW);
  digitalWrite(col4, LOW);
  digitalWrite(col5, LOW);
  digitalWrite(row1, HIGH);
  digitalWrite(row2, HIGH);
  digitalWrite(row3, HIGH);
  digitalWrite(row4, HIGH);
  digitalWrite(row5, HIGH);
  digitalWrite(row6, HIGH);
  digitalWrite(row7, HIGH);
}

int  setMode(int nr, int nc)
{
  if (nc == 1)
  {
    digitalWrite(col1, HIGH);
  }
  if (nc == 2)
  {
    digitalWrite(col2, HIGH);
  }
  if (nc == 3)
  {
    digitalWrite(col3, HIGH);
  }
  if (nc == 4)
  {
    digitalWrite(col4, HIGH);
  }
  if (nc == 5)
  {
    digitalWrite(col5, HIGH);
  }


  if ( nr == 1)
  {
    digitalWrite(row1, LOW);
  }
  if ( nr == 2)
  {
    digitalWrite(row2, LOW);
  }
  if ( nr == 3)
  {
    digitalWrite(row3, LOW);
  }
  if ( nr == 4)
  {
    digitalWrite(row4, LOW);
  }
  if ( nr == 5)
  {
    digitalWrite(row5, LOW);
  }
  if ( nr == 6)
  {
    digitalWrite(row6, LOW);
  }
  if ( nr == 7)
  {
    digitalWrite(row7, LOW);
  }
}
int  setOffMode(int nr, int nc)
{
  if (nc == 1)
  {
    digitalWrite(col1, LOW);
  }
  if (nc == 2)
  {
    digitalWrite(col2, LOW);
  }
  if (nc == 3)
  {
    digitalWrite(col3, LOW);
  }
  if (nc == 4)
  {
    digitalWrite(col4, LOW);
  }
  if (nc == 5)
  {
    digitalWrite(col5, LOW);
  }


  if ( nr == 1)
  {
    digitalWrite(row1, HIGH);
  }
  if ( nr == 2)
  {
    digitalWrite(row2, HIGH);
  }
  if ( nr == 3)
  {
    digitalWrite(row3, HIGH);
  }
  if ( nr == 4)
  {
    digitalWrite(row4, HIGH);
  }
  if ( nr == 5)
  {
    digitalWrite(row5, HIGH);
  }
  if ( nr == 6)
  {
    digitalWrite(row6, HIGH);
  }
  if ( nr == 7)
  {
    digitalWrite(row7, HIGH);
  }
}
