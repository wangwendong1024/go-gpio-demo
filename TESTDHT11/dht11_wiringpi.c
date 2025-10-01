#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TIMINGS 85

int read_dht11(int pin, float *temperature, float *humidity) {
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    uint8_t data[5] = {0,0,0,0,0};

    if (wiringPiSetup() == -1) {
        return -1; // wiringPi 初始化失败
    }

    // 发起启动信号
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(18);
    digitalWrite(pin, HIGH);
    delayMicroseconds(40);
    pinMode(pin, INPUT);

    // 读取数据
    for (i = 0; i < MAX_TIMINGS; i++) {
        counter = 0;
        while (digitalRead(pin) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) break;
        }
        laststate = digitalRead(pin);

        if (counter == 255) break;

        // 前 3 次脉冲忽略
        if ((i >= 4) && (i % 2 == 0)) {
            data[j / 8] <<= 1;
            if (counter > 16) data[j / 8] |= 1;
            j++;
        }
    }

    // 校验
    if ((j >= 40) && (data[4] == ((data[0]+data[1]+data[2]+data[3]) & 0xFF))) {
        *humidity = data[0];
        *temperature = data[2];
        return 0;
    } else {
        return -2; // 校验失败
    }
}
