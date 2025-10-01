#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned char uint8;
typedef unsigned long uint32;

#define HIGH_TIME 40        // 高电平延时可稍微调整
#define MAX_WAIT_LOOPS 10000 // 超时循环次数

int pinNumber = 7;  // wiringPi pin 7 对应 BCM GPIO4

// 读取 DHT11 数据
uint8 readSensorData(int *temperature, int *humidity) {
    uint32 databuf = 0;
    uint8 crc = 0;
    uint8 i;
    int loop;

    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, 0);  // 拉低 25ms
    delay(25);
    digitalWrite(pinNumber, 1);  // 拉高
    pinMode(pinNumber, INPUT);
    pullUpDnControl(pinNumber, PUD_UP);

    delayMicroseconds(27);

    // 等待 DHT11 响应
    loop = 0;
    while (digitalRead(pinNumber) == 1) {
        if (++loop > MAX_WAIT_LOOPS) return 0; // 超时
    }

    // 等待拉高
    loop = 0;
    while (digitalRead(pinNumber) == 0) {
        if (++loop > MAX_WAIT_LOOPS) return 0; // 超时
    }

    // 读取 32 bit 数据
    for (i = 0; i < 32; i++) {
        loop = 0;
        while (digitalRead(pinNumber)) if (++loop > MAX_WAIT_LOOPS) return 0;
        loop = 0;
        while (!digitalRead(pinNumber)) if (++loop > MAX_WAIT_LOOPS) return 0;

        delayMicroseconds(HIGH_TIME);
        databuf <<= 1;
        if (digitalRead(pinNumber)) databuf |= 1;
    }

    // 读取 CRC
    crc = 0;
    for (i = 0; i < 8; i++) {
        loop = 0;
        while (digitalRead(pinNumber)) if (++loop > MAX_WAIT_LOOPS) return 0;
        loop = 0;
        while (!digitalRead(pinNumber)) if (++loop > MAX_WAIT_LOOPS) return 0;

        delayMicroseconds(HIGH_TIME);
        crc <<= 1;
        if (digitalRead(pinNumber)) crc |= 1;
    }

    // 解析数据
    *humidity = ((databuf >> 24) & 0xff) * 10 + ((databuf >> 16) & 0xff);
    *temperature = ((databuf >> 8) & 0xff) * 10 + (databuf & 0xff);

    return 1;
}

int main(void) {
    int temperature = 0, humidity = 0;

    printf("PIN: %d\n", pinNumber);

    if (wiringPiSetup() == -1) {
        printf("wiringPi 初始化失败!\n");
        return 1;
    }

    printf("开始读取 DHT11 数据...\n");

    while (1) {
        if (readSensorData(&temperature, &humidity)) {
            printf("读取成功! 湿度: %d.%d%% 温度: %d.%d°C\n",
                humidity / 10, humidity % 10,
                temperature / 10, temperature % 10);
        } else {
            printf("传感器未响应或超时!\n");
        }

        delay(3000); // 每 3 秒读取一次
    }

    return 0;
}
