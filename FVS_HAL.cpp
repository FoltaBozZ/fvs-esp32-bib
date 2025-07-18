/***************************************************************************
 * Ferdinand von Steinbeis-Schule Reutlingen
 *
 * Funktionen für Elektroniker Geräte und Systeme
 *
 * Autor: Löhe
 * Datum:
 *
 * Bearbeitet: Tress
 * Datum: 09.03.2025
 *
***************************************************************************/

#define  _FVS_HAL_C

#include "FVS_HAL.h"


//Funktionen aus HAL-Bibliothek Löhe

//*********************** I/O Funktionen ***********************************

hw_timer_t* timer1ms = NULL;

void delay_ms(uint16_t zeit)
{
  delay(zeit);
}

void delay_100us(uint16_t zeit)
{
  delay(zeit/10);
}

void bit_init(uint8_t port, uint8_t pin, uint8_t value)
{
if (port == 0)
  {
  if(pin>=0 && pin<=2)
    {pin = pin+25;}
  else if(pin>=3 && pin<=7)
    {pin = pin+29;}
  pinMode(pin, INPUT);
  }
else if (port == 1)
  {
  if(pin>=0 && pin<=4)
    {pin = pin+1;}
  else if(pin>=5 && pin<=7)
    {pin = pin+9;}
  pinMode(pin, OUTPUT);
  }
}

void byte_init(uint8_t port, uint8_t value)
{
uint8_t i;
  if (port == 0)
  {
    for ( i=25; i<=27 ; i++ )
      {pinMode(i,INPUT);}
    for ( i=32; i<=36 ; i++ )
      {pinMode(i,INPUT);}
  }
  else if (port == 1)
  {
    for ( i=1; i<=5 ; i++ )
      {pinMode(i,OUTPUT);}
    for ( i=14; i<=16 ; i++ )
      {pinMode(i,OUTPUT);}
  }
}

uint8_t bit_read(uint8_t port, uint8_t pin)
{
    uint8_t wert;
    if(pin>=0 && pin<=2)
      {wert = digitalRead(pin+25);}
    else if(pin>=3 && pin<=4)
      {wert = digitalRead(pin+29);}
    else if(pin>=5 && pin<=7)
      {wert = !digitalRead(pin+29);}
    return wert;
}

void bit_write(uint8_t lost, uint8_t pin, uint8_t value)
{
    if(pin>=0 && pin<=4)
        pin = pin+1;
    else if(pin>=5 && pin<=7)
        pin = pin+9;
    digitalWrite(pin,value);
}

uint8_t byte_read(uint8_t port)
{
    uint8_t wert=0;
    wert = digitalRead(25);
    wert = wert + 2*digitalRead(26);
    wert = wert + 4*digitalRead(27);
    wert = wert + 8*digitalRead(32);
    wert = wert + 16*digitalRead(33);
    wert = wert + 32*!digitalRead(34);
    wert = wert + 64*!digitalRead(35);
    wert = wert + 128*!digitalRead(36);
    return wert;
}

void byte_write(uint8_t port, uint8_t value)
{
    bit_write(port, 0,   1&value);
    bit_write(port, 1,   2&value);
    bit_write(port, 2,   4&value);
    bit_write(port, 3,   8&value);
    bit_write(port, 4,  16&value);
    bit_write(port, 5,  32&value);
    bit_write(port, 6,  64&value);
    bit_write(port, 7, 128&value);
}

//*********************** LCD Funktionen ***********************************

void lcd_init(void)
{
    Tft.begin();
}

void lcd_clear(void)
{
    Tft.begin();
}

void lcd_setcursor(uint8_t row, uint8_t column)
{
    Tft.setCursorCharacter(row,column);
}

void lcd_char(char data)
{
    Tft.print(data);
}

void lcd_print(uint8_t text[])
{
    char txt[171];
    uint8_t i;

    for(i=0;i<170;i++)
        txt[i]=' ';
    for(i=0;i<170;i++)
        txt[i]=text[i];
    txt[170]='\0';
    Tft.print(txt);
}

void lcd_byte(uint8_t data)
{
    char txt[5];
    sprintf(txt,"%3d",data);
    Tft.print(txt);
}

void lcd_int(uint16_t data)
{
    char txt[10];
    sprintf(txt,"%5d",data);
    Tft.print(data);
}

//*********************** PWM Funktionen ***********************************

void pwm_init(void)
{
   /* ledcSetup(0,10000,8);
    ledcAttachPin(5,0);*/
    ledcAttach(5,10000,8);
    ledcWrite(5,127);
}

void pwm_start(void)
{
}

void pwm_stop(void)
{
    ledcDetach(5);
}

void pwm_duty_cycle(uint8_t value)
{
    ledcWrite(5,value);
}


//*********************** ADC Funktionen ***********************************

void adc_init(void)
{
    delay_ms(1);
}

uint8_t adc_in1(void)
{
    uint8_t value;
    const int analogPin = A0;

    value = analogRead(analogPin)/16;
    return value;
}

uint8_t adc_in2(void)
{
    uint8_t value;
    const int analogPin = A1;

    value = analogRead(analogPin)/16;
    return value;
}

//*********************** IR Funktionen ***********************************

void ext_interrupt_enable(void)
{}

void ext_interrupt_disable(void)
{}

void ext_interrupt1_enable(void)
{}

void ext_interrupt1_disable(void)
{}

void ext_interrupt_isr(void);

void ext_interrupt1_isr(void);

void ext_interrupt_init(void)
{
    attachInterrupt(34,ext_interrupt_isr,FALLING);
}

void ext_interrupt1_init(void)
{
    attachInterrupt(35,ext_interrupt1_isr,FALLING);
}

//*********************** TIMER Funktionen ***********************************

void timer1ms_isr();

void timer1ms_enable(void)
{

    //timerAlarmEnable(timer1ms);
    timerRestart(timer1ms);
    timerStart(timer1ms);
}

void timer1ms_disable(void)
{
    timerStop(timer1ms);
}

void timer1ms_init(void)
{
    timer1ms = timerBegin(1000000); //1Mhz
    timerAttachInterrupt(timer1ms,&timer1ms_isr);
    timerAlarm(timer1ms,1000,true,0);//1ms
}

//*********************** I2C Funktionen ***********************************

void i2c_init(void)
{
    Wire.begin();
}

void i2c_start(void)
{
}

void i2c_write(uint8_t value)
{
Wire.beginTransmission(value);
Wire.write(0);
Wire.endTransmission();
}

uint8_t i2c_read(uint8_t value)
{
Wire.requestFrom(0x48,2);
value = Wire.read();
Wire.read();
return value;
}

void i2c_stop(void)
{
}

//*********************** RS232 Funktionen ***********************************
