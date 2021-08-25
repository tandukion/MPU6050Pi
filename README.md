# MPU6050Pi

MPU6050 library for Raspberry Pi.

The library is written by referring to I2Cdev library code written by Jeff Rowberg [https://github.com/jrowberg/i2cdevlib](https://github.com/jrowberg/i2cdevlib).

> Notes: The library does not provide communication with all regiters on MPU6050 yet.
However, basic usages and DMP feature are already supported.

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
Under `build` directory, do:
```
sudo make install
```

### Calibration
Run the following command to run the auto calibration under `build` directory created on build. The calibration file will be named `calibration.csv`
```
./auto_calibration
```
You can install the calibration file to a shared folder by installing the library again after running the calibration.
```
sudo make install
```
The calibration file will be copied to `"~/.local/share/MPU6050"`

## Run test program
### Test
Run the following command under `build` directory created on build.
```
./read_MPU6050Pi calibration.csv
```
> Notes: the args for cailbration is optional. If not specified, the program will run with default offset.