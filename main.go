package main

import (
    "fmt"
   // "log"
    "os"
    "os/signal"
    "syscall"
    "time"
    "go-gpio-demo/sensor"
    // "periph.io/x/host/v3"
    // "go-gpio-demo/sensor"
)

// func ReadDHT11(pin int) (float32, float32, error) {
//     var temp C.float
//     var hum C.float
//     ret := C.read_dht11(C.int(pin), &temp, &hum)
//     if ret != 0 {
//         return 0, 0, fmt.Errorf("读取失败")
//     }
//     return float32(temp), float32(hum), nil
// }

func main() {
    // 初始化 periph.io
    // if _, err := host.Init(); err != nil {
    //     log.Fatal("periph 初始化失败: ", err)
    // }
    // fmt.Println("periph 初始化成功！开始读取 DHT11...")

    // DHT11 接在 GPIO4 (BCM 引脚编号 4)
    //pin := 4
     // 创建一个退出通道
    stop := make(chan struct{})
    // 捕获 Ctrl+C (SIGINT) 或 kill (SIGTERM) 信号
    sigs := make(chan os.Signal, 1)
    signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)

     // 在 goroutine 中运行传感器监控
   // go sensor.MonitorDHT11(pin, 2*time.Second, stop)
   //go sensor.MonitorDHT11Periph("GPIO4", 2*time.Second, stop)
     go sensor.MonitorDHT11Demo(7, 1*time.Second, stop) // wiringPi pin 7 对GPIO4

      // 等待退出信号
    <-sigs
    fmt.Println("\n收到退出信号，准备退出...")
    close(stop) // 通知 sensor.MonitorDHT11 停止
    time.Sleep(1 * time.Second)

}
