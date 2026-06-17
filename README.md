# A Digital Clockwork

Viddy well, little brother. This project is queer, queer like a clockwork orange!


**A Digital Clockwork** is a digital clock circuit simulator inspired by a project created by Wagner Rambo and presented on his YouTube channel, **WR Kits**.

This simulator was developed as a way to study digital circuit behavior, low-level hardware concepts, discrete logic, and the internal operation of integrated circuits such as **CD4017**, **CD4029**, **CD4511**, and others used in the original design.

---

## The Original Project

This horrorshow is based on a digital clock circuit designed by Wagner Rambo and showcased on his YouTube channel: **WR Kits**.

Below is an image of the original hardware project:

![Original clock circuit](assets/clockwork-board.png)

---

## Extension



---

## Purpose of This Repository

This repository serves as a personal experimental environment for:

* Studying digital circuit behavior through simulation
* Exploring low-level hardware concepts
* Implementing circuit logic in **C++**
* Experimenting with the simulation of discrete logic components

---

## Project Structure

```bash
digital-clockwork-simulator
├── assets                           # Images and graphical resources
│   ├── clockwork-board.png
│   ├── counter.png
│   └── dividefreq.png
├── docs                             # code and project documentation
│   ├── documentation.pdf
│   └── documentation.tex
├── include                          # Header files
│   ├── chips.hpp 
│   ├── feedback.hpp
│   └── freqGenerator.hpp
├── src                              # Source code files
│   ├── chips.cpp
│   ├── digitalClockwork.cpp
│   ├── feedback.cpp
│   └── freqGenerator.cpp
├── tests                            # Unit tests
│   ├── 4017test.cpp 
│   ├── 4029test.cpp 
│   ├── 4040test.cpp
│   ├── 4511test.cpp
│   └── frequencytest.cpp
├── .gitignore                       # Git ignored files configuration
├── LICENSE                          # Project license
├── Makefile                         # Build automation file
└── README.md                        # Project documentation
```

---

## Build and Run

To compile and run the Digital Clockwork simulator:

```bash
git clone https://github.com/FrankSteps/digital-clockwork-simulator

cd digital-clockwork-simulator

make runClock
```
---

## Important Note

This project is **not intended to function as a real digital clock**, droog.

Its purpose is to validate and explore the behavior of Wagner Rambo's original hardware design through computational simulation. The focus is on reproducing the logical behavior of the circuit rather than achieving precise real-time accuracy.

---

## License

This project is distributed under the **GNU General Public License (GPL)**.

See the `LICENSE` file for more details.

---

## Fun Facts

> This project's name is a reference to the dystopian novel *A Clockwork Orange* and this README was written using Nadsat terms such as "horrorshow" and "droog".

> Building this little horrorshow was almost as pleasurable as the good old in-output, in-output.
