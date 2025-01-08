This is a universal firmware for ESP32/ESP32-S2 devices to connect an ePaper display (with an SPI connection) to OpenEpaperLink.
The software is designed to be compiled using VSCode/PlatformIO.

Currently, it includes drivers for the following displays:

- 7,3" Spectra 6 color Epaper Display (buy it here: https://s.click.aliexpress.com/e/_oDk19jD)
- 4.01" ACeP color Epaper Display (discontinued)

![IMG_2872](https://github.com/user-attachments/assets/63864c09-9611-4113-bf48-1980ebcf7a32)

Select the display type in `/include/main.h`. It's relatively easy to add support for other display types, as long as they use SPI and you have the code to send an image buffer to the display. If you add support for a new display type, feel free to submit a pull request so others can benefit from your work.

When ordering a display, make sure to include an adapter board to access the SPI pins. Avoid purchasing an "ESP32 demo kit" with it, as it is not necessary.
In addition to the ePaper display and adapter board, you'll need an ESP32. For larger or full-color displays, PSRAM is required. A Wemos S2 mini (buy it here: https://s.click.aliexpress.com/e/_oFBoFr1) 
with 2 MB of PSRAM works well and is very affordable.

Connecting the pins is straightforward. You can use a header connector instead of wires. Refer to the pinout in `platformio.ini` to determine which pin numbers to use.

![IMG_2873](https://github.com/user-attachments/assets/8a38a5f7-4447-413e-8db0-b4478f6511da)

⚠️ Since the bare displays are very fragile, it is highly recommended to provide some protection as a first step. A basic 3D-printed case for the 7.3" Spectra display is also included in this repository. It consists of a bezel and an inset. No printing supports needed.

To add rigidity, the case is designed to embed a 9x15 cm prototype board (buy it here: https://s.click.aliexpress.com/e/_oDEHPhz), which can be incorporated during the printing process.
Configure your slicer to pause the print just before the bridges are printed, insert the prototype board, and then resume printing.

![pause](https://github.com/user-attachments/assets/914c4678-9b6b-4ccc-b5a7-f47e9a3a2b32)

![IMG_2886](https://github.com/user-attachments/assets/b8605516-7ed5-48f1-8253-3a10afb3c08a)

(Some of the links in this documentation are affiliate links. This means that if you click on the link and make a purchase, a small commission may be earned at no additional cost to you)
