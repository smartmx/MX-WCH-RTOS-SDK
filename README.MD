# MX-WCH-RTOS-SDK

本仓库存放个人关于WCH系列芯片的一些开源项目的移植和库文件的优化修改。

## 移植特点

1. 中断向量表修改，默认中断向量表不同于RISC-V官方，在一部分异常发生时都是进入HardFault中断。所以依旧使用Qingke的中断向量表模式来进行中断管理。但是将用户编写的外部中断都统一使用汇编函数入口进行管理，所以不在需要使用__attribute__((interrupt("WCH-Interrupt-fast")))或者__attribute__((interrupt()))修饰。在汇编总入口中，第一级中断进入时，会打开中断嵌套，切换为中断栈。

2. 使能中断嵌套会导致每个中断执行会多约10个指令周期，中断嵌套使能通过工程右键 -> properties -> c/c++ Build -> settings -> tool settings -> GNU RISC-V Cross Assembler -> Preprocessor 右边输入框Defined symbols中的 ENABLE_INTERRUPT_NEST=0 修改为 ENABLE_INTERRUPT_NEST=1 即可。

3. 使用硬件压栈（不可关闭），保证运行速度。

## 移植目录

目前已经移植的目录

### CH583/2/1

| RTOS | 状态 |
| :---- | :---- |
| [`FreeRTOS`](CH583_2_1/FreeRTOS/) | 已完成 |
| [`RT-Thread`](CH583_2_1/RT-Thread-Nano/) | 已完成 |
| [`liteos_m`](CH583_2_1/LiteOS_m/) | 已完成 |

### CH585/4

| RTOS | 状态 |
| :---- | :---- |
| [`FreeRTOS`](CH585_4/FreeRTOS/) | 已完成 |
| [`RT-Thread`](CH585_4/RT-Thread-Nano/) | 已完成 |
| [`liteos_m`](CH585_4/LiteOS_m/) | 已完成 |

### CH592/1

| RTOS | 状态 |
| :---- | :---- |
| [`FreeRTOS`](CH592/FreeRTOS/) | 已完成 |
| [`RT-Thread`](CH592/RT-Thread-Nano/) | 已完成 |
| [`liteos_m`](CH592/LiteOS_m/) | 已完成 |

### CH32V208

| RTOS | 状态 |
| :---- | :---- |
| [`FreeRTOS`](CH32V208/FreeRTOS/) | 已完成 |
| [`RT-Thread`](CH32V208/RT-Thread-Nano/) | 已完成 |
| [`liteos_m`](CH32V208/LiteOS_m/) | 已完成 |

### CH32V203

| RTOS | 状态 |
| :---- | :---- |
| `FreeRTOS` | 进行中 |
| `RT-Thread` | 进行中 |
| `liteos_m` | 进行中 |

### CH32V303/5/7

| RTOS | 状态 |
| :---- | :---- |
| `FreeRTOS` | 进行中 |
| `RT-Thread` | 进行中 |
| `liteos_m` | 进行中 |

## WCH芯片内核版本

WCH Qingke内核是基于RISC-V的内核。

| 芯片型号 | 内核版本 | 浮点 |
| :---- | :---- | :---- |
| CH32V103 | Qingke V3A | 不支持 |
| CH583/2/1 | Qingke V4A | 不支持 |
| CH585/4 | Qingke V3C | 不支持 |
| CH32V203 | Qingke V4B | 不支持 |
| CH592/1 | Qingke V4C | 不支持 |
| CH32V208 | Qingke V4C | 不支持 |
| CH32L103 | Qingke V4C | 不支持 |
| CH32V303/5/7 | Qingke V4F | 支持 |
