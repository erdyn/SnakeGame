# Module: Microprocessor Systems
## Title: Game Assignment
Date:  March 2024 <br>
Authors:  Susanna Perkins, Ben McCarthy, Dean O'Brien <br>
Grade:  74% <br>

![Gameplay](./snakeGamePlayGIF.gif)
<br>
<details>
  <summary> <h3> Description: </h3> </summary>
This program is a simple snake game implemented in C for an STM32 microcontroller. It leverages the STM32 HAL for hardware interaction, including GPIO, timers, and interrupts, to enable real-time gameplay on an embedded system. The game features a moving snake, eatable apples, and a wrapping screen effect. The program is modular and includes functions for:
Game mechanics: Movement, score tracking, collision detection, and apple spawning.
Hardware configuration: Clock initialization, GPIO pin setup, and enabling pull-up resistors.
Graphics and sound: Snake and apple sprites for a basic graphical interface, as well as sound effects for key events.
Input handling: Reads button states for directional movement using GPIO.
The project demonstrates a blend of low-level hardware control and game development principles, making it suitable for showcasing embedded system capabilities.
</details>
