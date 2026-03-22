# A Digital Clockwork

**A Digital Clockwork** is a digital clock circuit simulator inspired by a project by Wagner Rambo, presented on his YouTube channel **WR Kits**.

This simulator was created to study digital circuit simulation, low-level hardware concepts, discrete logic, and the internal operation of chips such as **CD4017**, **CD4029**, **CD4511**, and other chips used by Wagner Rambo to build his digital clock.

During my university vacation, this repository will serve as my own experimental laboratory to explore hardware, C++ programming, and digital circuit simulation.

---

## The Original Project

The simulator is based on a digital clock circuit designed by Wagner Rambo and presented on his YouTube channel **WR Kits**.

Below is an image of the original hardware project:

![Original clock circuit](clockwork-board.png)

---

## Purpose of This Repository

This repository serves as a personal experimental environment to:

* Study digital circuit behavior through simulation
* Explore low-level hardware concepts
* Implement circuit logic using **C++**
* Experiment with the simulation of discrete logic components

---



```

# compile clockwork 
g++ src/digitalClockwork.cpp src/chips.cpp src/freqGenerator.cpp -Iinclude -o builds/digitalclockwork

# run clockwork 
./builds/digitalclockwork

```

## License

This project is distributed under the **GNU General Public License (GPL)**.

See the `LICENSE` file for more details.
