#include "mbed.h"
#include <iostream>
#include <cmath> 
#include <string>
Serial pc(USBTX, USBRX);
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
DigitalIn  Switch(SW3);
DigitalOut greenLED(LED1);
DigitalOut redLED(LED2);


int i;
float freqcnt = 0;
int sample = 600;
float ADCinput[600];
float freq = 0;

BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char dot[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};
int main(){
    for(i = 0; i < sample; i++) {
        Aout = Ain;
        ADCinput[i] = Ain;
        wait(1./sample);
    }
    for(i = 0; i < sample; i++) {
        if(i != 0) {
            if(ADCinput[i-1] < 0.3 && ADCinput[i] >= 0.3) freqcnt = freqcnt + 1; 
        }
        pc.printf("%1.3f\r\n", ADCinput[i]);
    }
    freq = round(freqcnt);
    int digit = (int) freq;
    float k;
    float waittime = (freq*40);
    int x = 0;
    if((int)freq >= 100) x = 100;
    else x = 10;
    while(1) {
        if(Switch == 0) {
            if(x == 1) {
                display = dot[digit];
                digit = (int) freq;
                if((int)freq >= 100) x = 100;
                else x = 10;
            }
            else {
                display = table[digit/x];
                digit %= x;
                x /= 10;
            }
            wait(1);
            redLED = 1;
            greenLED = 0;
        }
        else if(Switch == 1) {
            display = 0x00;
            redLED = 0;
            greenLED = 1;
        }
        for( k=0; k<2; k+=0.05 ){

        Aout = 0.5 + 0.5*sin(k*3.14159);

        wait(1./waittime);
        }
    }
}