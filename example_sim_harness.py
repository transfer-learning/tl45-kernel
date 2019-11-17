import json
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
    '_OBJS=crt0.o lib/soft_impl.o lib/fxp.o lib/fxp_gen.o lib/divmnu.o example_sim_harness.o',
    'build'
], check=True)

p = subprocess.Popen(['hardware-src/verilator/build/sim_tl45_io', 'obj/a.out'],
                     # stderr=sys.stderr,
                     stdin=subprocess.PIPE,
                     stdout=subprocess.PIPE,
                     bufsize=1,
                     universal_newlines=True)

received_messages = []


def wait_for_message(matcher=None):
    if matcher is None:
        matcher = lambda x: True

    for msg in received_messages:
        if matcher(msg):
            received_messages.remove(msg)
            return msg

    while True:
        item = json.loads(p.stdout.readline())
        if item['type'] == 'log':
            print('[SIM]', item['message'])
            continue

        if item['type'] == 'output' and int(item['address']) % 256 == 3:
            print('[SIM] Output:', hex(int(item['data'])))

        if matcher(item):
            return item

        received_messages.append(item)


def raw_send(num):
    item = wait_for_message(lambda x: x['type'] == 'input_request')

    json.dump({
        'type': 'input_response',
        'address': item['address'],
        'data': str(num),
    }, p.stdin)
    p.stdin.write('\n')


# def raw_recv():
#     r = int('0x' + p.stdout.readline().strip(), 16)
#     if VERBOSE:
#         print('recv() ->', r)
#     return r


def call(*args, num_returns=None):
    for arg in args:
        assert type(arg) == int
        raw_send(arg)

    if num_returns is None:
        return int(wait_for_message(lambda x: x['type'] == 'output' and int(x['address']) % 256 == 0)['data'])

    return [int(wait_for_message(lambda x: x['type'] == 'output' and int(x['address']) % 256 == 0)['data']) for _ in range(num_returns)]


def to_fixed(flt):
    return int(math.floor(flt * (1 << 16)))


def from_fixed(num):
    return num / (1 << 16)


angles = []
expected = []
actual = []

for i in range(1, 1000):
    angle = i / 500
    angles.append(angle)

    raw_response = call(to_fixed(9.5), to_fixed(angle))
    # print(hex(raw_response), hex(to_fixed(9.5 / 5)))
    response = from_fixed(raw_response)
    actual.append(response)

    expected.append(9.5 / angle)


# raw_response = call(to_fixed(9.5), to_fixed(5))
# print(hex(raw_response), hex(to_fixed(9.5 / 5)))
# response = from_fixed(raw_response)


#
# got 0x4ccb, expected 0x1e666
#
# a - 0x98000
# b - 0x50000
#
#



# for x, y in zip(expected, actual):
#     print(y / x)

print(hex(to_fixed(9.5)))
print(hex(to_fixed(5)))

plt.plot(angles, expected)
plt.plot(angles, actual)
plt.show()
