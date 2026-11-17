# Bare-Metal UART Command-Line Interface on STM32F401RE

A register-level embedded firmware project for the **STM32F401RE** on the **NUCLEO-F401RE** development board. It implements a UART command-line interface (CLI), interrupt-driven serial reception, a ring buffer, and a lightweight cooperative scheduler driven by SysTick.

The firmware is written in C and uses direct peripheral-register access through CMSIS device definitions. It does not use STM32 HAL, LL, or an RTOS.

## Implemented functionality

- **USART2 CLI:** Interactive serial interface over the ST-LINK Virtual COM port.
- **Interrupt-driven RX:** USART2 RXNE interrupt receives characters without polling the peripheral in the application loop.
- **Ring buffer:** A 64-byte circular buffer decouples the UART receive ISR from CLI processing.
- **Cooperative scheduler:** Periodic task dispatch based on a 1 ms SysTick counter.
- **GPIO control:** Control the NUCLEO-F401RE user LED connected to PA5.
- **Status reporting:** Display the firmware uptime through the CLI.

## Hardware and configuration

| Item | Configuration |
|---|---|
| Development board | NUCLEO-F401RE |
| Microcontroller | STM32F401RETx, Arm Cortex-M4 |
| Firmware language | C |
| Peripheral access | Direct register-level programming using CMSIS definitions |
| System clock assumption | 16 MHz HSI default clock; no PLL setup is performed in the current `rcc_init()` |
| UART peripheral | USART2 |
| UART TX | PA2, alternate function AF7 |
| UART RX | PA3, alternate function AF7 |
| UART baud rate | 115200 baud (USART2 BRR configured for a 16 MHz peripheral clock) |
| User LED | PA5 |
| Scheduler timebase | SysTick, 1 ms tick |

Connect to the board's ST-LINK Virtual COM port using a serial terminal configured for **115200 baud, 8 data bits, no parity, 1 stop bit (115200 8-N-1)**.

## CLI commands

Type a command in the serial terminal and press Enter.

| Command | Behaviour |
|---|---|
| `help` | Prints the available commands |
| `led on` | Turns the PA5 user LED on |
| `led off` | Turns the PA5 user LED off |
| `status` | Prints uptime and scheduler status |

The command parser currently supports these commands only. A PWM command is not implemented in the current source.

## Firmware flow

```text
             USART2 RXNE Interrupt
                      |
                      v
                RX Ring Buffer
                      |
                      v
              CLI scheduled task
                (10 ms period)
                      |
             +--------+--------+
             |        |        |
             v        v        v
           help     led on/off  status

                 SysTick ISR
                 (1 ms tick)
                      |
                      v
            Cooperative scheduler
```

The application initializes RCC, GPIO, USART2, SysTick, and the scheduler. It registers the CLI task with a 10 ms period and continuously calls `scheduler_run()`.

## Repository layout

```text
bm_uart_cli/
├── .cproject                     # STM32CubeIDE build configuration
├── .project                      # Eclipse/STM32CubeIDE project metadata
├── .settings/                    # IDE language and project settings
├── .gitignore                    # Excludes local build artifacts
├── Inc/
│   ├── cli.h
│   ├── gpio.h
│   ├── rcc.h
│   ├── ring_buffer.h
│   ├── scheduler.h
│   ├── systick.h
│   └── uart.h
├── Src/
│   ├── cli.c                     # Command parsing and CLI task
│   ├── gpio.c                    # PA5 LED control
│   ├── main.c                    # Firmware initialization and main loop
│   ├── rcc.c                     # Peripheral clock gates
│   ├── ringbuffer.c              # Circular receive buffer
│   ├── scheduler.c               # Cooperative task scheduler
│   ├── syscalls.c                # C library system-call stubs
│   ├── sysmem.c                  # C library memory support
│   ├── systick.c                 # 1 ms system tick
│   └── uart.c                    # USART2 driver and RX interrupt handler
├── Startup/
│   └── startup_stm32f401retx.s   # Reset handler and interrupt vector table
├── STM32F401RETX_FLASH.ld        # Flash-target linker memory layout
└── README.md
```

## Build with STM32CubeIDE

1. Install STM32CubeIDE and the Arm GNU Toolchain supported by that IDE.
2. Ensure the STM32F401RE device support and compatible CMSIS core/device headers are available.
3. Clone the repository:
   ```bash
   git clone https://github.com/aravindhan1/bm_uart_cli.git
   cd bm_uart_cli
   ```
4. In STM32CubeIDE, import the repository as an existing project.
5. Select the **Debug** or **Release** configuration and build the project.
6. Flash the generated firmware to a NUCLEO-F401RE board.
7. Open the ST-LINK Virtual COM port at 115200 8-N-1 and enter `help`.

### CMSIS header dependency

The source includes `stm32f401xe.h`. The current `.cproject` configuration points to CMSIS headers in an absolute directory on the original development machine:

- `CMSIS/Device/ST/STM32F4xx/Include`
- `CMSIS/Include`

Those header files are not stored in this repository. On another machine, install or provide compatible CMSIS headers and update the include paths in STM32CubeIDE before building.

## Important implementation notes

- The Flash linker script and startup assembly file are required for the current firmware target and are intentionally retained.
- Build products such as ELF files, object files, dependency files, map/list files, and the generated `Debug/` and `Release/` directories are excluded by `.gitignore`; they are not source files.
- The ring buffer drops newly received characters when it becomes full.
- The source contains `task_led()` and `task_status()` functions, but the current `main()` registers only `task_cli()` with the scheduler.
- The RCC code enables the TIM2 clock, but a PWM driver and PWM CLI command are not implemented in this version.
- The repository does not currently include a dedicated automated test suite. Validate the firmware on the target board and check UART behaviour with a serial terminal.

## Learning topics

- STM32F4 register-level peripheral configuration
- RCC clock gating
- GPIO mode and output data registers
- USART2 alternate-function configuration
- Interrupt-driven UART reception
- Circular buffer design
- SysTick interrupt and timebase implementation
- Cooperative scheduling
- Command parsing in embedded C

## Author

**Aravindhan TV**  
GitHub: [@aravindhan1](https://github.com/aravindhan1)
