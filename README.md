
Build and run unit tests :

    mkdir build
    cd build
    cmake ../
    make
    ./tests

Build binary (w/o flashing):
    //Given : platformio installed 

    ./.local/bin/platformio run
    or
    pio run

Run integration tests:
    //Given setup from integration_test folder  executed
    ./run_integratio_tests.sh


How to install platformio ?
preffered way to instal into project folder

pip install --target=d:\somewhere\other\than\the\default package_name

default is ~/.local/bin/platformio
