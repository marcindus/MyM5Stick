
SYNC_PATH_LOCAL=${CURDIR}
PI_USER=pi
PI_ADDRESS=192.168.1.2
PROJ_DIR=MyM5Stick
PORT=/dev/ttyUSB0
BAUDRATE=115200

tags_all: 
	ctags -R --langmap=C++:.C.h.c.cpp.hpp.ino

upload:
	pio run -t upload

ssh:
	@ssh ${PI_USER}@${PI_ADDRESS}

rsync:
	@rsync -Ctazv --exclude ".git/" \
					--exclude ".gitignore" \
					--exclude ".pio" \
					--exclude "unit_tests/" \
					--exclude "build/" \
					--exclude "tmp/" \
					${SYNC_PATH_LOCAL}/ ${PI_USER}@${PI_ADDRESS}:/home/${PI_USER}/${PROJ_DIR}/
miniterm:
	python3 -m serial.tools.miniterm --raw --encoding ascii ${PORT} ${BAUDRATE}
