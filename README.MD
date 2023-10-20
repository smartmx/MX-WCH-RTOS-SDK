# MX-WCH-RTOS-SDK

本仓库存放个人关于WCH系列芯片的一些开源项目的移植和库文件的优化修改。

## 移植目录

目前已经移植的目录

### CH583/2/1

| RTOS | 状态 |
| :---- | :---- |
| [`FreeRTOS`](CH583_2_1/FreeRTOS/) | 已完成 |
| [`RT-Thread`](CH583_2_1/RT-Thread-Nano/) | 已完成 |
| `liteos_m` | 进行中 |

### CH592/1

| RTOS | 状态 |
| :---- | :---- |
| `FreeRTOS` | 进行中 |
| `RT-Thread` | 进行中 |
| `liteos_m` | 进行中 |

### CH32V208

| RTOS | 状态 |
| :---- | :---- |
| [`FreeRTOS`](CH32V208/FreeRTOS/) | 已完成 |
| [`RT-Thread`](CH32V208/RT-Thread-Nano/) | 已完成 |
| `liteos_m` | 进行中 |

## WCH芯片内核版本

WCH Qingke内核是基于RISC-V的内核。

| 芯片型号 | 内核版本 | 浮点 |
| :---- | :---- | :---- |
| CH32V103 | Qingke V3A | 不支持 |
| CH583/2/1 | Qingke V4A | 不支持 |
| CH32V203 | Qingke V4B | 不支持 |
| CH592/1 | Qingke V4C | 不支持 |
| CH32V208 | Qingke V4C | 不支持 |
| CH32L103 | Qingke V4C | 不支持 |
| CH32V303/5/7 | Qingke V4F | 支持 |
