# TheDilemma

When the world is ending in 7 days the last question you probably have is, do we deserve it?

## Description

This is a controller for a LED strip that lights up a piano key when pressed. I know a version of this had been done before; however, as far as I know, there had only been a version where the input for the LED was from MIDI, which is impossible to do with an older piano or an acoustic piano. I wanted to make this project so much because looking back I would have practiced the piano so much more if it wasn't so boring. So, my goal is to use an actual microphone input and FFT to translate the sound wave signal into individual key notes. So that it becomes fun and usable. I wanted to make this project because I have an acoustic piano but I can't use led on it because it doesn't have a midi output so this project that only use sound will be a game changer for me! Thank you!

## Getting Started

### Usage

* Place the 1.4m led strip over the piano in possition.
* Charge your external LIPO-battery using Tp4056 usb-c charging module.
* Plug the external lipo battery in and rest it on the holder (please be aware of the polarlity).
* Set the setting mode and brightness via esp32-accesspoint captive portal.

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

* The code is in C++ made for ESP-32
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
<img width="1066" height="255" alt="image" src="https://github.com/user-attachments/assets/f935058f-1cd8-41fb-bf4d-d9253fb1b696" />

## Rendered Pictures
<img width="1051" height="238" alt="Screenshot 2026-06-18 172513" src="https://github.com/user-attachments/assets/4d41c5bb-d906-4908-aacb-cc6fe0f857d9" />


## Captive Portal interface
<img width="373" height="445" alt="Screenshot 2026-06-18 032915" src="https://github.com/user-attachments/assets/ff6574ff-b611-4831-ab5b-705859b5a18c" />


## Wiring Diagram 
<img width="1769" height="1251" alt="SCH_Schematic1_2026-06-14 (1)_page-0001" src="https://github.com/user-attachments/assets/8684b099-6665-4e5c-9a5a-702e105f07e2" />


# Zine Poster
<img width="1410" height="2000" alt="waaavecoustic (1)" src="https://github.com/user-attachments/assets/4f861a55-da71-4618-9970-9b8e22a6dd7f" />


