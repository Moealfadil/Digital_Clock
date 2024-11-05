#include <LiquidCrystal.h>
#define SW 8
#define set 9
#define change 10
#define record A0
#define reset A1
#define stop A2
#define show A3
LiquidCrystal lcd1(12, 11, 5, 4, 3, 2);
volatile bool x=true;
volatile int y=0;
volatile int z=0;
volatile bool stp=false;
volatile bool sh=false;
volatile int tcs=0;
volatile int ts=0;
volatile int tm=0;
int s=0;
volatile int m=00;
volatile int h=00;//setting clock to zero 
unsigned long prevousTime=millis();
unsigned long currentTime;
unsigned long prevousTime2=millis();
unsigned long currentTime2;
unsigned long prevousTime3=millis();
unsigned long currentTime3;
String laps[100];//array to record the laps
volatile int id=0;
int i=0;
void setup()
{
  lcd1.begin(16, 2); // Set up the number of columns and rows on the LCD.
  PCICR|=B00000001; // Interrupt from port B
  PCMSK0|=B00000111;// Enable pin change interrupt for pins 8,9 and 10
  PCICR|=B00000010; // Interrupt from port c
  PCMSK1|=B00001111;// Enable pin change interrupt for pins A0, A1,A2 and A3
  Serial.begin(9600);
  pinMode(SW,INPUT);
  pinMode(set,INPUT);
  pinMode(change,INPUT);
  pinMode(reset,INPUT);
  pinMode(record,INPUT);
  pinMode(stop,INPUT);
  pinMode(show,INPUT);
}
void loop()
{ currentTime=millis();
  currentTime3=millis();
  if(digitalRead(reset)==1){ 
    lcd1.setCursor(0,1);
    lcd1.print("           ");}//clear screen
  if(digitalRead(SW)==1){ 
    lcd1.setCursor(0,0);
    lcd1.print("           ");
    lcd1.setCursor(0,1);
    lcd1.print("           ");}//clear screen
 
 
 if(digitalRead(show)==1){ 
    lcd1.setCursor(0,0);
    lcd1.print("                ");
    lcd1.setCursor(0,1);
    lcd1.print("           ");}//clear screen
 if(sh==true)//show laps
 { if(id==0){
 lcd1.setCursor(0,0);
    lcd1.print("No laps recorded");
 }
   else if(((currentTime3-prevousTime3)>1000) && i<id){
   prevousTime3=currentTime3;
   lcd1.setCursor(0,0);
    lcd1.print("Lap");
   lcd1.setCursor(3,0);
    lcd1.print(i);   
   lcd1.setCursor(0,1);
     lcd1.print(laps[i]);
     i++;}}
 
  
 if(x==false || sh==true)// keep clock count while stopwatch is working
 {
   if((currentTime-prevousTime)>1000){
   prevousTime=currentTime;
    s++;
   if(s==60){
    s=0;
    m++;
    }
   if(m==60){
    s=0;
    m=0;
    h++;
   }}}
 
  if(x==true && sh==false)//clock
  {
  lcd1.setCursor(0,0);
  lcd1.print("Clock  ");
  if((currentTime-prevousTime)>1000){
    prevousTime=currentTime;
    s++;
    lcd1.setCursor(6,1);
    lcd1.print(s);}
  if(s==60){
    s=0;
    m++;//increase minutes every 60 seconds
    lcd1.setCursor(6,1);
    lcd1.print("0 ");}
  if(m==60){
    s=0;
    m=0;
    h++;//increase hours every 60 minutes
    lcd1.setCursor(3,1);
    lcd1.print("0 ");}
  if(h==24){
    s=0;
    m=0;
    h=0;
    lcd1.setCursor(0,1);
    lcd1.print("           ");}//reset to zero every 24 hours
  lcd1.setCursor(3,1);
  lcd1.print(m);
  lcd1.setCursor(0,1);
  lcd1.print(h);
  lcd1.setCursor(2,1);
  lcd1.print(":");
  lcd1.setCursor(5,1);
  lcd1.print(":");
  lcd1.setCursor(0,0);
  if(z==0){
    lcd1.setCursor(7,0);
    lcd1.print("     ");
  }
  else if(z==1){
    lcd1.setCursor(7,0);
    lcd1.print("SET H");
  }
  else if(z==2){
    lcd1.setCursor(7,0);
    lcd1.print("SET M");
  }}
 
 
 else if (x==false && sh==false)// stop watch 
 {
  lcd1.setCursor(0,0);
  lcd1.print("Stop Watch");
  currentTime2=millis();
   if(stp==false){ 
  if((currentTime2-prevousTime2)>10){
  prevousTime2=currentTime2;
  tcs++;// centi seconds
  if(tcs==100)
    tcs=0;
  lcd1.setCursor(5,1);
  lcd1.print(tcs);}
  if((currentTime3-prevousTime3)>1000){
   prevousTime3=currentTime3;
   ts++;// seconds in stop watch
   lcd1.setCursor(2,1);
   lcd1.print(ts);}
   if(ts==60){
    ts=00;
    tm++;
    lcd1.setCursor(2,1);
    lcd1.print("0 ");} 
   lcd1.setCursor(0,1);
   lcd1.print(tm);}
 }
 }
ISR(PCINT0_vect) // Interrupt for Port B
{
  if(digitalRead(SW)==1){
    x=!x;
    tcs=0;
    ts=0;
    tm=0;}
  if(digitalRead(set)==1){
    y++;
    z=y%3;
  }
  
  if(digitalRead(change)==1 && z==1)
    h++;//increase hours
  if(digitalRead(change)==1&& z==2)
    m++;//increase minutes
}
ISR(PCINT1_vect) // Interrupt for Port C
{ if(digitalRead(record)==1)//record laps 
{String A=String(tm);
String B=String(ts);
String C=String(tcs);
String lap=A+" "+B+" "+C;
laps[id]=lap;
Serial.println(laps[id]);
 id++;}
 if(digitalRead(show)==1){
   sh=!sh;
   i=0;}
 if(digitalRead(reset)==1){
    ts=0;
    tcs=0;
    tm=0;
   while(i<id){
     laps[i]="";
   i++;}
   id=0;
 }
 if(digitalRead(stop)==1)
    stp=!stp;
 }
