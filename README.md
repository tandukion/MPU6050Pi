# MPU6050Pi

MPU6050 library for Raspberry Pi.

## Dependencies
- wiringPi
<br>The driver mainly uses GPIO to control the motor. For using c++ to control the GPIO, wiringPi is needed.
```
sudo apt-get install wiringpi
```

## Compiling the program
### Build libraries and test program at once
```
mkdir build
cd build
cmake ..
make
```

### Installing the library
```
sudo make install
```

## Run test program
### Test
Run the following command under `build` directory created on build.
```

```
