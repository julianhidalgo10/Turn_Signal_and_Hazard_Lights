# Turn_Signal_and_Hazard_Lights
This repo contains a system of directional and stationary signals similar to that of a Tesla Model 3 car.

In summary, the functionalities of the system are:

Non-functional:
- Have 3 buttons: Left Turn, Right Turn, Park.
- Have 3 lights (LEDs): System Heartbeat, Left Light, Right Light.
- Have a debugging port with the PC: USART2

Functional:
- Heartbeat with a frequency of 1Hz to indicate that the system is working.
- If a turn button is pressed 1 time: the light on the corresponding side flashes 3 times.
- If a turn button is pressed 2 times in less than 300ms: the light on the corresponding side flashes indefinitely.
- If a turn button is pressed and the light on the other side is active: the light is deactivated.
- If the button for the parking signal is pressed: it works just like a real car, example: Tesla Model 3
- The flashing frequency of the lights is acceptable according to "The General Traffic Regulations".
- Major system events are identified on a PC serial console.
