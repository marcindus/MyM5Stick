
PI_USER=ute
PI_ADDRESS=192.168.1.2
PROJ_DIR=/home/pi/xxx

tags_all: 
	ctags -R --langmap=C++:.C.h.c.cpp.hpp.ino

rsync:


upload:
	$(shell ssh ${PI_USER}@${PI_ADDRESS} "cd $PROJ_DIR pio -t upload")

ssh:
	@ssh ${PI_USER}@${PI_ADDRESS}


