test_port:
	@if [ -z "$(PORT)" ]; then echo "PORT is not set, please set PORT using export, example: 'export PORT=/dev/ttyACM0'"; exit 1; fi

setup:
	idf.py set-target esp32s3
	pre-commit install

compile:
	idf.py build

flash:
	make test_port
	idf.py -p $(PORT) flash

monitor:
	make test_port
	idf.py -p $(PORT) monitor

clean:
	rm -rf build/ managed_components/
	rm sdkconfig
	make setup

ff:
	make test_port
	idf.py -p $(PORT) erase-flash
		make flash monitor

all: flash monitor

.PHONY: test_port setup compile flash monitor clean all
