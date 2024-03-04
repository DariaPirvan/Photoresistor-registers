void init_adc()  //conversie analog digitala
{
    ADMUX = 0b01000101;
    ADCSRA= 0b10000111; // Activare ADC - bit 7    si     Setare  prescaler = 128 
}
int adc_l,adc_h;

int readADC (char ch){
     ADMUX &= 0b11100000;   //Resetează canalul de conversie 
     ADMUX |= ch;	          //Setează canalul conversiei
  
    
    ADCSRA |= (1<<6);   //incepe conversia
    while(ADCSRA & (1<<6)); //asteapta finalizarea conversiei

     adc_l = ADCL;
     adc_h = ADCH;
     return ((adc_h << 8) | adc_l);
}
void init_Buzzer(){ //initializare buzzer
DDRD |=(1<<6);
TCCR1A = 0b00100010; 
TCCR1B = 0b00011010;
TCCR1C = 0;
}
void reset_Buzzer()
{
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1C = 0;
}
void init_timer0() 
 {
  cli();
 TCCR0A=0;
 TCCR0B=0;
 TCNT0=0;
 OCR0A=124;
 TCCR0A = 0b00000010;
 TCCR0B = 0b00000011; 
 TIMSK0 = 0b00000010;
 }
ISR(TIMER0_COMPA_vect){  //intrerupere
  int duration, distance;
  PORTD |=(1<<3);
  delayMicroseconds(10);
  PORTD &= ~(1<<3); //digitalWrite(trigPin, LOW);
  //duration = pulseIn(2, HIGH);
  while (!(PIND & (1<<2)));  // Wait until Echo HIGH
  TCNT0 = 0; 
  while (PIND & (1<<2));  // Wait until Echo LOW
  duration = TCNT0;
  distance = (duration/2) / 29.1;
  if(distance<=50)
{
  PORTB |= (1<<3); //led on
  init_Buzzer(); //buzzer on
  delay(10);
  
}
else
{
   PORTB &=~(1<<3); //led off
   reset_Buzzer();
}
    if(distance<50 && distance>25)
    {
      PORTB &=~(1<<3); //led off
      PORTB &=~(1<<4); //led off
      PORTB |= (1<<5); //led on
    }
    if(distance<25 && distance>10)
    {
      PORTB |= (1<<4); //led on
      PORTB &=~(1<<3); //led off
      PORTB &=~(1<<5); //led off  
    }
    if(distance<10)
    {
      PORTB |= (1<<3); //led on
      PORTB &=~(1<<4); //led off
      PORTB &=~(1<<5); //led off
      init_Buzzer();
    }
 }

int main(){
  DDRB |= (1<<3); //ROSU pin11
  DDRB |= (1<<4); //galben pin12
  DDRB |= (1<<5); //VERDE pin13
  DDRD |= (1<<6);  //buzzer PIN8
  DDRD |= (1<<3);   
  DDRD &=~(1<<2); 
init_adc();
init_timer0();
for(;;);
}



