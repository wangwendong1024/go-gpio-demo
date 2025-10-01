#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef unsigned char uint8;
typedef unsigned long uint32;

#define HIGH_TIME 40
#define MAX_WAIT_LOOPS 10000

// Go 调用用的函数，返回 int：1成功，0未响应
int read_dht11(int pin, int *temperature, int *humidity) {
    uint32 databuf = 0;
    uint8 crc = 0;
    uint8 i;
    int loop;

    // 初始化 wiringPi
    if (wiringPiSetup() == -1) {
        return -1; // 初始化失败
    }

    pinMode(pin, OUTPUT);
    digitalWrite(pin, 0);  // 拉低 25ms
    delay(25);
    digitalWrite(pin, 1);  // 拉高
    pinMode(pin, INPUT);
    pullUpDnControl(pin, PUD_UP);

    delayMicroseconds(27);

    // 等待 DHT11 响应
    loop = 0;
    while (digitalRead(pin) == 1) if (++loop > MAX_WAIT_LOOPS) return 0;

    loop = 0;
    while (digitalRead(pin) == 0) if (++loop > MAX_WAIT_LOOPS) return 0;

    // 读取 32 bit 数据
    for (i = 0; i < 32; i++) {
        loop = 0;
        while (digitalRead(pin)) if (++loop > MAX_WAIT_LOOPS) return 0;
        loop = 0;
        while (!digitalRead(pin)) if (++loop > MAX_WAIT_LOOPS) return 0;

        delayMicroseconds(HIGH_TIME);
        databuf <<= 1;
        if (digitalRead(pin)) databuf |= 1;
    }

    // 读取 CRC
    crc = 0;
    for (i = 0; i < 8; i++) {
        loop = 0;
        while (digitalRead(pin)) if (++loop > MAX_WAIT_LOOPS) return 0;
        loop = 0;
        while (!digitalRead(pin)) if (++loop > MAX_WAIT_LOOPS) return 0;

        delayMicroseconds(HIGH_TIME);
        crc <<= 1;
        if (digitalRead(pin)) crc |= 1;
    }

    // 解析数据
    *humidity = ((databuf >> 24) & 0xff) * 10 + ((databuf >> 16) & 0xff);
    *temperature = ((databuf >> 8) & 0xff) * 10 + (databuf & 0xff);

    return 1; // 成功
}
