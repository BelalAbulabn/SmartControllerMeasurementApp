# Smart Controller Measurement App
## Java Desktop Application
### Description
This is a Java-based desktop application that provides an interface for taking various measurements and interacting with serial ports. The application allows users to control different parameters like voltage, frequency, and pulses. It also enables users to plot graphs, connect to serial ports, and import/export data.

### Features
- Interactive GUI with user-friendly controls
- Ability to set and control different parameters including voltage, frequency, and pulses
- Serial port communication
- Plot graphs based on the input data
- Import and Export data
- Save data functionality

### Prerequisites
- Java JDK 8 or higher
- Serial communication library (e.g., jSerialComm)
- JFreeChart library for plotting graphs

### Setting Up
1. Clone the repository or download the source code.
2. Ensure that Java JDK is installed on your machine.
3. Include the required libraries in your project (jSerialComm, JFreeChart, etc.).
4. Set the file paths in the source code to the appropriate locations on your machine.

### How to Run
1. Open the project in your preferred Java IDE (e.g., Eclipse, IntelliJ).
2. Resolve any dependencies by adding the necessary libraries.
3. Run the main class `Windo`.
4. The application's graphical user interface should open. 
5. Use the controls to connect to a serial port, take measurements, and plot graphs.

## Hardware Part

The hardware part is implemented using STM32 microcontroller. The code is written in C and handles hardware communication and data manipulation.

### Description
This hardware part interacts with different peripherals such as ADCs (Analog to Digital Converters), DACs (Digital to Analog Converters), USART for serial communication and Timers for precise timing events. The ADC reads data from different channels, while the DAC is used to output data. The USART (Universal Synchronous Asynchronous Receiver Transmitter) is used for serial communication. This hardware is controlled and communicated with the Java-based desktop application.

### Prerequisites
- STM32F4 Microcontroller
- STM32F4xx standard peripheral library

### Code Overview
- `main.c` is the entry point of the program and contains the main function. This file initializes the hardware and runs an infinite loop to read user inputs and handle other functions.
- `config.c` contains the setup for different peripherals like Timer, USART, DMA, ADC, etc. This file is crucial for configuring the hardware.
- `control.c`is responsible for handling Analog to Digital Conversion (ADC), serial communication through RS232, the main control loop of the program, timers, LED control, DAC outputs, and power control.
-  `pwm.c` contains the implementation for controlling the two full bridges. The change of the PWM speed can be adjusted by changing the `tim_period` value.


### control.c

#### Key Components

- **ADC**: The file is responsible for handling Analog to Digital Conversion (ADC).
- **RS232**: Serial communication through RS232 is implemented here.
- **Control Loop**: The control loop essential for the actuator regulation is found in this file.
- **Timers**: This file contains functions and configurations for various timers.
- **LED Control**: Functions to control LEDs are included.
- **DAC**: Functions for setting DAC outputs.
- **Power Control**: Functions for enabling and disabling power supplies.

#### Functions

Some key functions included in this file are:

- `init_SmartControl`: Initializes the hardware for the SmartControl project.
- `controlLoop`: Main control loop of the program. Additional functions for actuator regulation should be added here.
- `dummyLoop`: A demo function for LED control.
- `USART_SendText`: Function to send text over USART.
- `usartReceived`: Evaluates user input commands via RS232 connection.
- `DAC_Set_Output`: Sets the output for the DAC.
- `EN_PWR`: Enables the power supply.
- `Disable_PWR`: Disables the power supply.
- `ADC_Save_Values_In_Cache`: Saves ADC values in cache

### USART Configuration
- Configuration for USART1 on GPIOA is done to use Pin 9 for transmitting data and Pin 10 for receiving data.

### ADC Configuration
- The ADC (Analog to Digital Converter) is configured to read values from different channels.
- The ADC values are read into a buffer, and the DMA (Direct Memory Access) is configured to automatically move these values into memory so the CPU doesn't have to do these operations.

### DAC Configuration
- The DAC (Digital to Analog Converter) is used to output analog values. The output values are loaded from a buffer.

### Timer Configuration
- Timers are used for precise timing events. One is configured for ADC DMA use and another for controlLoop interrupt.

### Control Loop
- The control loop handles periodic tasks that are to be executed in a timely manner. 

### How to Compile
1. Set up the STM32F4 development environment.
2. Include the STM32F4xx standard peripheral library.
3. Compile the C code using an ARM cross-compiler (e.g., arm-none-eabi-gcc).
4. Upload the compiled code to the STM32F4 microcontroller using a programming tool (e.g., ST-Link).

### Integration with Java Application
The STM32 microcontroller communicates with the Java application through serial communication. The Java application sends commands to the microcontroller, and the microcontroller sends back data. This data can be parameters such as voltage, frequency, and pulses that the Java application will then process or display.


## Full Project Structure
The full project consists of two main parts:
1. Java-based desktop application
2. Hardware implementation using STM32 microcontroller

Both parts work together to create a complete Smart Controller Measurement system. The desktop application serves as the user interface, allowing for control and visualization of different parameters. The STM32 microcontroller interacts with the hardware, collecting data and executing commands from the desktop application.


## Author
Belal Abulabn

## Support
For support, please contact [Belal.abulabn@gmail.com]
