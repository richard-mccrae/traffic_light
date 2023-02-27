# traffic_light
A simple traffic light implemented on a NXP FRDM-KL25Z board

## SW Design

    - Two "traffic lights" that change color once per second
        - Light timing managed by the SysTick timer
        - Low-level protection so that both lights can never be green at the same time
    - Watchdog timer with a duration of just over 1 second
        - Serviced during SysTick handler
    - ADC with a channel for each of the 6 LEDs
        - When each LED is powered on, ADC takes a measurement between the resistor and diode
        - If the voltage measurement is below a predetermined value, the lights will turn off and 
        the board's own LED will flash red
        - If the voltage measurement is above a predetermined value, the lights will turn off and 
        the board's own LED will flash blue
    - Manual tests are included in the source code
    - All MCU modules controlled via CMSIS-CORE peripheral access layer defines found within MKL25Z4.h
  
## HW Schematic

![Alt text](resources/traffic_light_schem.svg)

## Demonstration

https://photos.app.goo.gl/FKQke2aNuxLzxWqN7
