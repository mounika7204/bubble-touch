# BubbleTouch

![demo](./assets/bubbletouch.mp4)

An interactive 2-player game where players move a colored mark to burst bubbles
that appear on screen. The player to pop the most bubbles wins!

This project was developed as part of my Computer Vision class at ESPOL.

## Dependencies

Make sure you have the following dependencies installed on your system:

- OpenCV 4
- Gtk 3
- Gtkmm 3

## Building

First clone the repository:

```bash
git clone https://github.com/aloussase/BubbleTouch.git --recurse-submodules
```

Now build the application using cmake:

```bash
cd BubbleTouch
cmake -B build -S .
cmake --build build
```

## Running

```bash
./build/bubbletouch
```

You can take a look at the sample configuration included with this repository.
It is used by the application to configure the mark colors and window dimensions.

## License
MIT
