from __future__ import print_function

import glob
import sys
import time
import argparse
import struct
import serial

IO_SEVEN_SEG = 0x1000000

VERBOSE = False

# hexes = []
# with open(sys.argv[1]) as f:
#     for line in f:
#         if line.startswith(':'):
#             hexes.append(line.strip())


if sys.platform == 'darwin':
    # mac_irl
    serial_ifs = glob.glob('/dev/cu.usbserial*')
else:
    serial_ifs = glob.glob('/dev/ttyUSB*')

if not serial_ifs:
    print('No serial interfaces found!')
    exit(1)

print('Using serial:', serial_ifs[0])


def vprint(*args, **kwargs):
    if VERBOSE:
        print(*args, **kwargs)


def set_base_addr(ser, base):
    checksum = 0x06
    checksum += (base & 0xFF) + ((base >> 8) & 0xFF)
    checksum = (~checksum + 1) & 0xFF
    payload = ":02000004{:04X}{:02X}".format(base, checksum)
    ser.write(payload.encode('ascii'))
    response = ser.read(1).decode('ascii')
    vprint(payload)
    return response == "S"

def write_ack(ser, address, data):
    assert(len(data) <= 0xFF)
    base = (address >> 16) & 0xFFFF
    offset = address & 0xFFFF
    if not set_base_addr(ser, base):
        print("FAILED TO SET BASE ADDR")
        exit(1)

    checksum = len(data)
    checksum += (offset & 0xFF) + ((offset >> 8) & 0xFF)
    payload = ":{:02X}{:04X}00".format(len(data), offset)
    for b in data:
        checksum += b
        payload += "{:02X}".format(b)
    payload += "{:02X}".format((~checksum + 1) & 0XFF)
    vprint(payload)
    retry = 0
    while True:
        ser.write(payload.encode('ascii'))
        response = ser.read(1).decode('ascii')
        if not response or response == "E":
            print("try: {} CHECKSUM ERROR at 0x{:08X}\n HEX: {}".format(retry, address, payload))
            retry += 1
            time.sleep(.1)
            continue
        break
    return response



def parsehex(num):
    return int('0x' + num, 16)

def dump_file(ser, filename, base=0, chunksize=240):
    prog = open(filename, 'rb').read()
    print('FILE SIZE {}, CHUNKSIZE = {}'.format(len(prog), chunksize))
    for addr in range(0, len(prog), chunksize):
        if not VERBOSE:
            print('Writing File {}/{}'.format(addr, len(prog)), end='\r')
        res = write_ack(ser, base + addr, prog[addr:addr+chunksize])
        if res == "K":
            continue
        elif res == "E":
            print("CHECKSUM ERROR")
            exit(1)
        else:
            print("RES = ", res)
            exit(1)
    # write_ack(ser, base + addr, [0]*32)

def auto_int(x):
        return int(x, 0)

argp = argparse.ArgumentParser()
argp.add_argument('--base', type=auto_int, nargs='?', default=0)
argp.add_argument('file', type=str, nargs='?')
args = argp.parse_args()

with serial.Serial(serial_ifs[0], 115200, timeout=0.1) as ser:
    print('Serial Name:', ser.name)

    ser.reset_input_buffer()
    ser.reset_output_buffer()

    while True:
        d = ser.read(size=1000000)
        if not d:
            break
        print(d)

    if args.file:
        print('Dump file: ', args.file)
        dump_file(ser, args.file)
        exit(0)
    print(args)
    exit(0)

    # dump_file(ser, '/Users/will/Work/transfer-learning/llvm-tl45/llvm/bbb/a.out')