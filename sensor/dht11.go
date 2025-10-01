package sensor

import (
    "fmt"
    "github.com/d2r2/go-dht"
    "time"
    // "periph.io/x/periph/v3/host"
    // "periph.io/x/periph/v3/conn/gpio/gpioreg"
    // "periph.io/x/periph/v3/devices/dht"
)

// ReadDHT11 读取一次 DHT11 的温湿度
func ReadDHT11(pin int) (float32, float32, int, error) {
    temperature, humidity, retry, err := dht.ReadDHTxxWithRetry(dht.DHT11, pin, false, 10)
    if err != nil {
        return 0, 0, retry, err
    }
    return temperature, humidity, retry, nil
}

// MonitorDHT11 循环读取，并输出到控制台
func MonitorDHT11(pin int, interval time.Duration, stop <-chan struct{}) {
    for {
        select {
        case <-stop:
            fmt.Println("DHT11 采集停止")
            return
        default:
            temperature, humidity, retries, err := dht.ReadDHTxxWithRetry(dht.DHT11, pin, false, 10)
            if err != nil {
                fmt.Println("读取失败:", err)
            } else {
                fmt.Printf("温度: %.1f°C  湿度: %.1f%% (重试次数: %d)\n", temperature, humidity, retries)
            }
            time.Sleep(interval)
        }
    }
}

// ReadDHT11Periph 读取一次 DHT11（使用 periph.io）
// func ReadDHT11Periph(pinName string) (float32, float32, error) {
//     _, err := host.Init()
//     if err != nil {
//         return 0, 0, fmt.Errorf("periph 初始化失败: %v", err)
//     }

//     pin := gpioreg.ByName(pinName)
//     if pin == nil {
//         return 0, 0, fmt.Errorf("找不到 GPIO 引脚: %s", pinName)
//     }

//     sensor := dht.New(pin, dht.DHT11, time.Second*2)
//     temp, hum, err := sensor.Read()
//     if err != nil {
//         return 0, 0, fmt.Errorf("读取 DHT11 失败: %v", err)
//     }

//     return float32(temp.Celsius()), float32(hum), nil
// }

// // MonitorDHT11Periph 循环读取 DHT11（使用 periph.io），输出到控制台
// func MonitorDHT11Periph(pinName string, interval time.Duration, stop <-chan struct{}) {
//     _, err := host.Init()
//     if err != nil {
//         fmt.Println("periph 初始化失败:", err)
//         return
//     }

//     pin := gpioreg.ByName(pinName)
//     if pin == nil {
//         fmt.Println("找不到 GPIO 引脚:", pinName)
//         return
//     }

//     sensor := dht.New(pin, dht.DHT11, time.Second*2)

//     for {
//         select {
//         case <-stop:
//             fmt.Println("DHT11 Periph 采集停止")
//             return
//         default:
//             temp, hum, err := sensor.Read()
//             if err != nil {
//                 fmt.Println("读取失败:", err)
//             } else {
//                 fmt.Printf("Periph 版本 -> 温度: %.1f°C  湿度: %.1f%%\n", temp.Celsius(), hum)
//             }
//             time.Sleep(interval)
//         }
//     }
// }
