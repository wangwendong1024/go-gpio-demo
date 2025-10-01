#include <wiringPi.h>
#include <stdio.h>

int main() {
    if (wiringPiSetup() == -1) return 1;
    int pin = 7;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 0);
    delay(20);
    digitalWrite(pin, 1);
    delayMicroseconds(40);
    pinMode(pin, INPUT);
    pullUpDnControl(pin, PUD_UP);
    printf("切换到输入成功\n");
    return 0;
}
