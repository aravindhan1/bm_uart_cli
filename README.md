# Bare-Metal UART Command Line Interface with Cooperative Scheduler on STM32F401RE

## Project Overview

This project implements a **bare-metal firmware system** on the **STM32F401RE microcontroller** without using any vendor-provided hardware abstraction layers (HAL/LL) or RTOS.

The firmware provides:

* An **interrupt-driven UART-based Command Line Interface (CLI)** for interactive control
* A **cooperative, time-triggered task scheduler** using the SysTick timer
* Modular **register-level drivers** for core peripherals

This project demonstrates advanced embedded systems concepts, including **interrupt handling, real-time scheduling, UART communication, low-level peripheral control, and modular firmware architecture**.

---

## Project Objectives

* Implement firmware using **direct register-level programming**
* Develop an **interactive CLI** to control peripherals in real time
* Build a **cooperative scheduler** to manage multiple tasks without blocking
* Design a **scalable and modular firmware architecture** suitable for extension
* Gain practical understanding of **Cortex-M NVIC, SysTick, timers, and peripheral integration**

---

## System Architecture

```
                 +------------------------+
                 |   UART RX Interrupt    |
                 +-----------+------------+
                             |
                             v
                 +------------------------+
                 |      RX Ring Buffer     |
                 +-----------+------------+
                             |
                             v
                 +------------------------+
                 |        CLI Task        |
                 +-----------+------------+
                             |
        +--------------------+--------------------+
        |                    |                    |
        v                    v                    v
   LED Control           PWM Control          Status Reporting

------------------------------------------------------------
              SysTick Timer (1ms Tick)
------------------------------------------------------------
                             |
                             v
                    Cooperative Scheduler
```

---

## Hardware Configuration

* **Development Board:** NUCLEO-F401RE
* **Microcontroller:** STM32F401RET6 (Cortex-M4)
* **LED:** On-board LED (PA5)
* **UART:** USART2 for communication over ST-Link Virtual COM port

---

## Peripherals Utilized

| Peripheral | Function                                   |
| ---------- | ------------------------------------------ |
| GPIO       | LED output and button input                |
| USART2     | UART CLI communication                     |
| SysTick    | Scheduler timing and task management       |
| TIMx       | PWM signal generation                      |
| NVIC       | Interrupt handling and priority management |
| RCC        | System and peripheral clock configuration  |

---

## Software Architecture

```
.
├── Core
│   ├── main.c
│   ├── startup.s
│   └── system.c
│
├── drivers
│   ├── gpio.c / gpio.h
│   ├── uart.c / uart.h
│   ├── timer.c / timer.h
│   └── systick.c / systick.h
│
├── kernel
│   ├── scheduler.c / scheduler.h
│
├── app
│   ├── cli.c / cli.h
│   ├── led_app.c
│   └── status_app.c
│
└── README.md
```

---

## Scheduler Design

* **Type:** Cooperative, time-triggered
* **Tick Source:** SysTick configured at 1 millisecond intervals
* **Task Execution:** Non-blocking, periodic, and deterministic

### Task Model

Each task is defined as:

```c
typedef struct {
    void (*task)(void);
    uint32_t period_ms;
    uint32_t last_run;
} task_t;
```

### Example Tasks

| Task        | Execution Period | Function                        |
| ----------- | ---------------- | ------------------------------- |
| CLI Task    | 10 ms            | Parse commands from UART buffer |
| LED Task    | 500 ms           | Toggle onboard LED              |
| Status Task | 1000 ms          | Print system status over UART   |

The scheduler iterates over tasks, checking if their configured period has elapsed and executes the ready tasks accordingly.

---

## UART CLI Design

* **Baud Rate:** 115200
* **Mode:** TX and RX
* **Reception:** Interrupt-driven, non-blocking
* **Buffering:** Ring buffer to handle asynchronous input
* **Command Termination:** Line-based (`\r`)

### Supported Commands

| Command       | Description                         |
| ------------- | ----------------------------------- |
| `help`        | Displays available commands         |
| `led on`      | Turns the onboard LED on            |
| `led off`     | Turns the onboard LED off           |
| `pwm <0-100>` | Sets PWM duty cycle as a percentage |
| `status`      | Prints system and task status       |

---

## Interrupt Management

* **SysTick:** Provides a 1 ms tick for scheduler timing
* **USART2 RX:** Handles incoming UART characters asynchronously
* **(Optional) EXTI:** Handles external button interrupts

Interrupt priorities are configured manually using NVIC to ensure that critical tasks (e.g., UART RX) are responsive and non-blocking.

---

## Timing Strategy

* All timing and scheduling are derived from the SysTick timer
* No blocking delays (`while` loops or `delay()` functions) are used
* Ensures deterministic execution and minimal CPU idle time

---

## Testing and Debugging

* UART logging for debugging and interaction
* GDB debugging using OpenOCD
* Register inspection during ISR execution
* LED state verification and PWM signal validation (oscilloscope optional)

---

## Learning Outcomes

* ARM Cortex-M architecture and peripheral registers
* NVIC configuration and interrupt priority management
* SysTick timer and cooperative scheduling concepts
* UART driver development without HAL
* Modular, scalable firmware design suitable for embedded systems

---

## Future Improvements

* Implementation of preemptive scheduling
* Dynamic task creation and management
* Software timers for delayed execution
* Command auto-completion and advanced CLI features
* CPU load measurement and performance profiling

---

## Author

**Aravindhan TV**
