import json
import subprocess
import sys
import os
import re

def getSerialNum(mpname):
    lsblk = subprocess.run(['lsblk','-J','-o','SERIAL,MOUNTPOINT'],stdout=subprocess.PIPE)
    upper_serial = None
    for dev in json.loads(lsblk.stdout.decode())['blockdevices']:
        # print(dev.items())
        serial, mountpoint = dev['serial'], dev['mountpoint']
        if mountpoint and mpname in mountpoint:
            return serial or upper_serial
        upper_serial = serial
    return None

if __name__ == '__main__':
    # mpname = 'pocksensor'
    p = os.path.split(os.path.realpath(sys.argv[0]))[0]
    f = os.path.join(p, 'anchor.cfg')
    _file = open(f)
    mpname = _file.readline().strip('\n')
    s = getSerialNum(mpname)
    s = re.findall('\d+', s)[0]
    print("serial: {}".format(s))
