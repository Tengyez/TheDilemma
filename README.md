# TheDilemma

When the world is ending in 7 days the last question you probably have is, do we deserve it?
How about we find out by using the most wellknown and iconic psycological dilemma, the prisoner dilemma.

## Description

This is a psychological research game made based of the prisoner dilemma. We made this based of the theme "what would you make if the world's ending in 7 days" 

## Getting Started

### Usage

* Playing requires two player

### Assembling

* Print out the 3d parts (body,slider,topper,boostcover and please use the 3mf file I beg you)
* Put the esp-32 into the main body
* Stack the esp32 cover lid over the esp32 to lock it in place
* stick all the pin header into it own slot
* follow the wiring guide below
* put the microphone into the 6 headerpin slot
* stuff all the wiring jumper wires into the case
* connect the led strip into the external headerpin
* charge the lipo with the Tp4056 module (not include in the circuit and the case)
* connect the lipo to the external header pin
* cut your 2m led strip to your piano size
* use a multimeter to check the boost converter and adjust first using a screw prior to adding it to the circuit
* take the sliding lid and close it
* mount the led and the component box on your piano
* The only point require soldering are the INMP441 mic and the GND wires. 

### Wiring
* INMP441 - ESP32
* VDD  - 3.3V
* GND  - GND
* SCK  - GPIO32
* WS   - GPIO25
* SD   - GPIO34
* L/R  - GND
* WS2812B LED STRIP
* DIN  - 300Ω → GPIO13
* GND  - GND
* MT3608 boost converter (use a multimeter to check the boost and adjust first prior to adding electricity)
* +Vout - led strip 5v pin
* -Vout - GND
* Lipo battery (preferably 20 000mah)
* Splice the red wire before connecting it to the boost converter and connect it to ESP-32 VIN pin
* Red wire   - MT3608 +Vin pin
* Black wire - GND
* Splice all GND together and add solder and a electrical tape!!!
* Refers to the schematic down below for more indepth guide.

### Firmware

* The code is in C++ made for Xiao ESP-32 C3
* The library version might change over time, please recheck it again.
* Sidenote: I'm not really sure about the code but I tried my best to polish it because my friend quit half way :(
* Check out the [Firmware](firmware/Firmware.ino)
## Materials

This is the BOM of the entire project, just buy the normal one not the pcb version and just use jumper wire with minimal soldering required except for slicing GND together.
```
ESP-32 devkit v1           1 pcs. 8usd
INMP441 mic                1 pcs. 1usd
WS2812B led                2m 144/m 20usd
LIPO-battery               1 (reccomend atleast 10 000mah) 7usd
Tp4056 usb-c               1 (does not include in the build, only use the charge lipo externally) 1usd
MT3608 boost converter     1 pcs. 1usd
Female Jumper wires        30 pcs. 1usd
Male Jumper wires          15 pcs. 50cent
Pin headers                5 pcs. 25cent
Electrical tape            1 roll -
PLA 3d printing/time       ≈15g ≈45min -
Total: 39.75 usd
```

## Assembled Pictures

## Rendered Pictures



## Captive Portal interface



## Wiring Diagram 
<img width="1380" height="969" alt="image" src="https://github.com/user-attachments/assets/7be862da-b0e5-4416-b377-62f610ba3e29" />



# Zine Poster


