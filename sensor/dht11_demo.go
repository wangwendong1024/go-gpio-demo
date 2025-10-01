package sensor

/*
#cgo CFLAGS: -I${SRCDIR}/../C
#cgo LDFLAGS: -L${SRCDIR}/../C -ldht11_demo -lwiringPi
#include "dht11_demo.h"
*/
import "C"






import (
    "fmt"
    "time"
)

// ReadDHT11Demo 调用 C 层读取 DHT11
func ReadDHT11Demo(pin int) (float32, float32, error) {
    var temp C.int
    var hum C.int
    ret := C.read_dht11(C.int(pin), &temp, &hum)
    if ret != 1 {
        return 0, 0, fmt.Errorf("读取失败，错误码: %d", int(ret))
    }
    // 返回温度和湿度，保留一位小数
    return float32(temp) / 10.0, float32(hum) / 10.0, nil
}

func MonitorDHT11Demo(pin int, interval time.Duration, stop <-chan struct{}) {
	for {
		select {
		case <-stop:
			fmt.Println("DHT11 监控停止")
			return
		default:
			var temperature C.int
			var humidity C.int
			ret := C.read_dht11(C.int(pin), &temperature, &humidity)
			if ret != 1 {
				fmt.Println("读取失败或传感器未响应")
			} else {
				fmt.Printf("温度: %.1f°C  湿度: %.1f%%\n",
					float32(temperature)/10.0,
					float32(humidity)/10.0)
			}
			time.Sleep(interval)
		}
	}
}