ESP8266 wireless serial telemetry.
So, i wanted to have a wireless configuration means for my betaflight drone,
This project uses esp8266 board to transmit serial data over ESP-NOW,

When 2 esp8266 modules are flashed with this firmware, they will instantly share they're serial pin connection.
So if you connect one esp8266 to the computer, via usb, enter the comport in betaflight,
and connect the other one to a free MSB port on your drone (115200 baud), 
you can connect straight away, and configure it wirelessly.
Really convenient in my opinion.
