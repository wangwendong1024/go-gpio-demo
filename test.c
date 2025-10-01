#include <stdio.h>
#include "C/dht11.h"

int main() {
    float t, h;
    int ret = read_dht11(7, &t, &h); // wiringPi pin 7 对应物理 GPIO 4
    if (ret == 0) {
        printf("温度: %.1f°C 湿度: %.1f%%\n", t, h);
    } else {
        printf("读取失败: %d\n", ret);
    }
    return 0;
}
