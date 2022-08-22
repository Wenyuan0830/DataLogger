 /* 
 * File:   lab3.c
 * Author: Wenyuan Zhu
 * Created on November 16, 2021, 3:53 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//define a function that transmit the value to display window
void USART_Transmit( uint8_t data ) {
/* Wait for empty transmit buffer */ 
    while ( !( UCSRA & (1<<UDRE)) );
/* Put data into buffer, sends the data */ 
    UDR = data;
}

//data format: D3D2D1 H2H1:M2M1:S2S1.T
volatile uint8_t D1=0;
volatile uint8_t D2=0;
volatile uint8_t D3=0;
volatile uint8_t H1=0;
volatile uint8_t H2=0;
volatile uint8_t M1=0;
volatile uint8_t M2=0;
volatile uint8_t S1=0;
volatile uint8_t S2=0;
volatile uint8_t T=0;

//define the interrupt
//every 0.1s interrupt would happen
ISR(TIMER1_COMPA_vect){
    //every 0.1s, T++
    T++;
    //when t=1s
    //got 1 on S1
    if(T==10){
        T=0;
        S1++;
    }
    //the limit of S1 is 10
    //when S1=10, got 1 on S2
    //so on and so forth
    if(S1==10){
        S1=0;
        S2++;
        }
    if(S2 == 6){
        S2=0;
        M1++;
    }
    if(M1 == 10){
        M1=0;
        M2++;
    }
    if(M2 == 6){
        M2=0;
        H1++;
    }
    if(H1 == 10){
        H1=0;
        H2++;
    }
    //when reach 24hrs, got 1 on D1
    if((H1==4)&&(H2==2)){
        H1=0;
        H2=0;
        D1++;
    }
    if(D1 == 10){
        D1=0;
        D2++;
    }
    if(D2 == 10){
        D2=0;
        D3++;
    }
    if(D3 == 10){
        D3=0;
    }  
}

//begin the main body
int main(void) {
TIMSK = ((1<<TOIE1)|(1<<OCIE1A));
TCCR1A = 0x40;//set CTC mode
OCR1A = 23040;//set OCR1A value: (184320/8)
TCCR1B = 0x0A;//prescaler is 8
UCSRA |= (1<<UDRE); //indicate the UDR is ready to receive data
UCSRB |= (1<<TXEN);//enable USART transmitter
UBRRL = 2;

sei();// interrupt turn on

//declare four inputs represent for four channels
DDRD &= ~(1<<DDD3);
DDRD &= ~(1<<DDD4);
DDRD &= ~(1<<DDD5);
DDRD &= ~(1<<DDD6);

//set up for initial external pull-up values
PORTD |= (1<<PORTD3);
PORTD |= (1<<PORTD4);
PORTD |= (1<<PORTD5);
PORTD |= (1<<PORTD6);

while(1){
    //declare variables
    int8_t button_3 = 0;
    int8_t button_4 = 0;
    int8_t button_5 = 0;
    int8_t button_6 = 0;
    
    //initial variables
    button_3 = PIND & (1<<PIND3);
    button_4 = PIND & (1<<PIND4);
    button_5 = PIND & (1<<PIND5);
    button_6 = PIND & (1<<PIND6);
    
    //button_3 is pressed,represent channel A
    if(button_3==0){
        //use transmit function to transmit the content to the window
        USART_Transmit(D3+48);
        USART_Transmit(D2+48);
        USART_Transmit(D1+48);
        USART_Transmit(' ');
        USART_Transmit(H2+48);
        USART_Transmit(H1+48);
        USART_Transmit(':');
        USART_Transmit(M2+48);
        USART_Transmit(M1+48);
        USART_Transmit(':');
        USART_Transmit(S2+48);
        USART_Transmit(S1+48);
        USART_Transmit('.');
        USART_Transmit(T+48);
        USART_Transmit(' ');
        USART_Transmit('C');
        USART_Transmit('h');
        USART_Transmit('a');
        USART_Transmit('n');
        USART_Transmit('n');
        USART_Transmit('e');
        USART_Transmit('l');
        USART_Transmit(' ');
        USART_Transmit('A');
        USART_Transmit('\n');
        USART_Transmit('\r');
        //delat function, which prevent executing going to the following code when the button is pressed
        while(button_3==0){
            button_3 = PIND & (1<<PIND3);
            }
    }
    //when button_4 is pressed, which represent channel B
    if(button_4==0){
        USART_Transmit(D3+48);
        USART_Transmit(D2+48);
        USART_Transmit(D1+48);
        USART_Transmit(' ');
        USART_Transmit(H2+48);
        USART_Transmit(H1+48);
        USART_Transmit(':');
        USART_Transmit(M2+48);
        USART_Transmit(M1+48);
        USART_Transmit(':');
        USART_Transmit(S2+48);
        USART_Transmit(S1+48);
        USART_Transmit('.');
        USART_Transmit(T+48);
        USART_Transmit(' ');
        USART_Transmit('C');
        USART_Transmit('h');
        USART_Transmit('a');
        USART_Transmit('n');
        USART_Transmit('n');
        USART_Transmit('e');
        USART_Transmit('l');
        USART_Transmit(' ');
        USART_Transmit('B');
        USART_Transmit('\n');
        USART_Transmit('\r');
        while(button_4==0){
            button_4 = PIND & (1<<PIND4);
            }
    }
    
    //when button_5 is pressed, which represent channel C
    if(button_5==0){
        USART_Transmit(D3+48);
        USART_Transmit(D2+48);
        USART_Transmit(D1+48);
        USART_Transmit(' ');
        USART_Transmit(H2+48);
        USART_Transmit(H1+48);
        USART_Transmit(':');
        USART_Transmit(M2+48);
        USART_Transmit(M1+48);
        USART_Transmit(':');
        USART_Transmit(S2+48);
        USART_Transmit(S1+48);
        USART_Transmit('.');
        USART_Transmit(T+48);
        USART_Transmit(' ');
        USART_Transmit('C');
        USART_Transmit('h');
        USART_Transmit('a');
        USART_Transmit('n');
        USART_Transmit('n');
        USART_Transmit('e');
        USART_Transmit('l');
        USART_Transmit(' ');
        USART_Transmit('C');
        USART_Transmit('\n');
        USART_Transmit('\r');
        while(button_5==0){
            button_5 = PIND & (1<<PIND5);
            }
    }
    
    //when button_6 is pressed, which represent channel D
    if(button_6==0){
        USART_Transmit(D3+48);
        USART_Transmit(D2+48);
        USART_Transmit(D1+48);
        USART_Transmit(' ');
        USART_Transmit(H2+48);
        USART_Transmit(H1+48);
        USART_Transmit(':');
        USART_Transmit(M2+48);
        USART_Transmit(M1+48);
        USART_Transmit(':');
        USART_Transmit(S2+48);
        USART_Transmit(S1+48);
        USART_Transmit('.');
        USART_Transmit(T+48);
        USART_Transmit(' ');
        USART_Transmit('C');
        USART_Transmit('h');
        USART_Transmit('a');
        USART_Transmit('n');
        USART_Transmit('n');
        USART_Transmit('e');
        USART_Transmit('l');
        USART_Transmit(' ');
        USART_Transmit('D');
        USART_Transmit('\n');
        USART_Transmit('\r');
        while(button_6==0){
            button_6 = PIND & (1<<PIND6);
            }
    }
}
return (0);
}



