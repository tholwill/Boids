# Schooling Fish Simulation

An implementation of the boids artificial life algorithm written in C using **RayLib**

---

<img width="910" height="661" alt="Boids" src="https://github.com/user-attachments/assets/9d9c63fd-3763-4507-9015-7825f921324a" />

---

# Background
Boids was developed by Craig Reynolds as an artificial life algorithm to simulate the flocking of birds, however it can be applied to other life such as fish. The algorithm has 3 primary components:
 - Separation:
    Avoid other boids that get too close
 - Alignment
    Travel in the same direction as other nearby boids
 - Cohesion
    Travel towards the centre of mass of other nearby boids
Since its original development in 1986, the basic model has been extended, however this implementation does not follow any additional rules.

## Requirements
 * Raylib 5.0+
 * GCC / MinGW-w64

## Building

#### Clone the repository

```bash
git clone https://github.com/tholwill/Boids.git
cd Boids
git clone https://github.com/raysan5/raylib.git
```

### Linux
#### Build raylib
```bash
cd raylib/src
make
cd ../..
```

#### Build boids
```bash
make
./boids
```

---

### Windows
#### Build raylib
```bash'
cd raylib/src
mingw32-make
cd ../..
```

#### Build boids
```bash
mingw32-make win
.\boids.exe
```

