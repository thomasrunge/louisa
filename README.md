# love message USB device

Since I discovered [this lovely project](https://hackaday.com/2013/06/22/heart-shaped-project-takes-no-shortcuts/) on [Hackaday](https://hackaday.com/) many years ago, I wanted to make somenthing similar an my own.

Last year my stepdaughter left us to study on a different continent. So I made [this little reminder](https://www.youtube.com/watch?v=wpCfdtLyOdg) to tell her that we always love her. You just put it in a free USB slot on your laptop and it will scroll messages occasionally or on button press.

Hardware used: Digistumps [Digispark](http://digistump.com/products/1) (well, actually it's a 1$ chinese clone) and a monochrome 8x8 LED matrix driven by an HT16K33 controller (something like [this](https://learn.adafruit.com/adafruit-led-backpack/1-2-8x8-matrix)).

As you can read in the code, I replaced the messages with just her name and a heart symbol. If you want different messages just comment in or out the various text-variables.

