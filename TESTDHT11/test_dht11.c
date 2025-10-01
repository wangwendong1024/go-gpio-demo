#include <stdio.h>
#include "dht11_wiringpi.h"

int main() {
    float t, h;
    int ret = read_dht11(7, &t, &h); // wiringPi pin 7 对应 GPIO4
    if (ret == 0) {
        printf("温度: %.1f°C  湿度: %.1f%%\n", t, h);
    } else if (ret == -1) {
        printf("wiringPi 初始化失败\n");
    } else if (ret == -2) {
        printf("数据校验失败\n");
    } else {
        printf("读取失败, 错误码: %d\n", ret);
    }
    return 0;
}
