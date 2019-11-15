import sys
import math
import matplotlib.pyplot as plt
import os
import subprocess
import time

VERBOSE = False

nil_device = open(os.devnull, 'w')

# If this does not build, make sure your _OBJS line looks like '_OBJS ?= ...'
subprocess.run([
    'make',
    '_OBJS=crt0.o lib/soft_impl.o lib/fxp.o lib/fxp_gen.o example_sim_harness.o',
    'build'
], check=True)

p = subprocess.Popen(['hardware-src/verilator/build/sim_tl45_io',  'obj/a.out'],
                     stderr=sys.stderr if VERBOSE else nil_device,
                     stdin=subprocess.PIPE,
                     stdout=subprocess.PIPE,
                     bufsize=1,
                     universal_newlines=True)

time.sleep(0.5)
print(p.stdout.readline())
time.sleep(0.5)


def raw_send(num):
    x = '{:08x}\n'.format(num)
    p.stdin.write(x)


def raw_recv():
    r = int('0x' + p.stdout.readline().strip(), 16)
    if VERBOSE:
        print('recv() ->', r)
    return r


def call(*args, num_returns=None):
    for arg in args:
        assert type(arg) == int
        raw_send(arg)

    if num_returns is None:
        return raw_recv()

    return [raw_recv() for _ in range(num_returns)]


def to_fixed(flt):
    return int(math.floor(flt * 0x1000))


def from_fixed(num):
    return num / 0x1000


angles = []
expected = []
actual = []

for i in range(1, 500):
    angle = i / 500
    angles.append(angle)

    response = from_fixed(call(to_fixed(angle)))
    actual.append(response)

    expected.append(100 / angle)

plt.scatter(angles, expected)
plt.scatter(angles, actual)
plt.show()


