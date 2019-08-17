# rgbmatrix_smartclock

## Description
A clock to display the time, date, and weather info

### Hardware
* Raspberry Pi 3
	* hzeller has a [great section](https://github.com/hzeller/rpi-rgb-led-matrix/tree/master/bindings/python#performance) on the relative speed of the display based on raspberry pi version  
	However being written in cpp any raspberry pi version should do.
* Hub75 64x32 RGB led matrix
	* Any compatible matrix should do, check Ebay or your equivalent
* Adafruit led matrix hat (optional, but very usefull)
	* [Product](https://www.adafruit.com/product/2345)
	* [Docs](https://learn.adafruit.com/adafruit-rgb-matrix-plus-real-time-clock-hat-for-raspberry-pi)

### Requirements
* rpi-rgb-led-matrix
	* For controlling the matrix  
		* Included as subdirectory
	* `git submodule update --init --recursive`
* libcurl
	* For using openweathermap api
	* `sudo apt install libcurl4-openssl-dev`


## General TODO
* Add real curl command to pull weather data
* Make config.h more configurable
