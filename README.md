# rsa-license-key

## dependences

Based on crypto++.

```bash
sudo apt-get install libcrypto++-dev
```

## compile

Compile using "make"

```bash
make
```

The compile result looks like

```shell
g++ app.cpp -lcrypto++ -o app
g++ -c verifylicense.cpp -lcrypto++ 
g++ verify.cpp -lcrypto++ -o verify
```

Compile python runable file

```bash
cd python
pyinstaller -F getserial.py
```

use `pip install pyinstaller` for install `pyinstaller`

## install

coopy these file into LicenseMaker

```
app
verifylicense.o
verify
python/disk/getserial
python/anchor.cfg
```

FYI. `anchor.cfg` content must be the volume name of USB Device, default is `pocksensor`. 


## usaege

1. run `license.sh` script

```
sh license.sh
```
all the file needed should be copy into LicenseManger folder

2. run verify and check license manager

```
cd LicenseManger
./verify
```

if get `ok`, license manager run successfully.


3. copy `LicenseManger` folder into usb device


4. plugin the usb device into target computer, copy LicenseManger folder into the folder you needed, such as `~`, and give option permission to `getserial` and `verify`


```
cp LicenseManger ~/
cd LicenseManger
chmod +x getserial
chmod +x veriff
```

check License manager and get `ok`

```
./verify
```

## more infomation

1. use `verifylicense.o` as libarary