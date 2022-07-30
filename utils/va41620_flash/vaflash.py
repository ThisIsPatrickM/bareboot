#!/usr/bin/python3
from pygdbmi.gdbcontroller import GdbController
import argparse
from sys import exit
from pathlib import Path
import subprocess
from functools import partial
import os
import atexit


parser = argparse.ArgumentParser(description='Flash FRAM of VA41620 MCU')
parser.add_argument('bin', type=argparse.FileType('rb'))
parser.add_argument('-g', '--gdb_path', default='gdb')
args = parser.parse_args()
firmware = args.bin
gdbpath = args.gdb_path
if not firmware.name.endswith('bin'):
    print("please use a .bin file")
    sys.exit(1)

scriptpath = os.path.dirname(__file__)

fram_size = 256*1024
# location to load data into
ram_data_address = 0x1fff8000
ram_data_size = 0x1000
start_address = 0x0


filesize = Path(firmware.name).stat().st_size
if filesize > fram_size:
    print("file too large(max " + str(fram_size) + "bytes)")
    exit(1)


first8bytes = firmware.peek(8)
reset_handler_ptr = int.from_bytes(
    first8bytes[4:8], byteorder='little', signed=False)

if ((reset_handler_ptr % 2 != 1) or not (start_address < reset_handler_ptr < (start_address+filesize))):
    print("binary contains an invalid reset handler address")
    exit(1)


gdbserver = subprocess.Popen(["JLinkGDBServerCLExe", "-select", "USB", "-device", "Cortex-M4", "-endian", "little", "-if", "SWD", "-speed", "4000", "-noir",
                             "-LocalhostOnly", "-jlinkscriptfile", scriptpath+"/JLinkSettings.jlinkscript", "-settingsfile", scriptpath+"/JLinkSettings.ini", "-timeout", "0.5", "-silent"])


def onexit():
    print("exiting")
    gdbserver.terminate()


atexit.register(onexit)


gdbmi = GdbController(command=[gdbpath, '--interpreter=mi3'])
response = gdbmi.write('-gdb-set arch armv3m')
if (response[-1]['message'] != 'done'):
    print("the used gdb version doesn't support the armv3m architecture")
    exit(1)

response = gdbmi.write('-file-exec-and-symbols ' +
                       scriptpath + '/VA416_SPI_FRAM_256KB.FLM')
if (response[-1]['message'] != "done"):
    print("error while loading flash algorithm")
    exit(1)
response = gdbmi.write('-target-select extended-remote :2331', timeout_sec=2)
if (response[-1]['message'] != "connected"):
    print("error while connecting to gdb server")
    exit(1)
gdbmi.write('monitor reset 1')  # reset cpu only
gdbmi.write('-target-download')  # download flashing algorithm onto the target
# for more information about function signatures, see https://www.keil.com/pack/doc/CMSIS/Pack/html/algorithmFunc.html
response = gdbmi.write('-data-evaluate-expression "Init(0, 0, 0)"')
if (response[0]['payload']['value'] != '0'):
    print("error during init")
    exit(1)


current_address = start_address
for chunk in iter(partial(firmware.read, ram_data_size), b''):
    print('writing ' + str(len(chunk)) +
          ' bytes at address 0x%X' % current_address)
    gdbmi.write('-data-write-memory-bytes ' + "0x%X" %
                ram_data_address + ' ' + chunk.hex())
    gdbmi.write('-data-evaluate-expression "ProgramPage(' + "0x%X" % current_address +
                ", " + str(ram_data_size) + ', (uint32_t*) 0x%X)"' % ram_data_address)
    current_address = current_address + len(chunk)


gdbmi.write('-data-evaluate-expression UnInit(0)')
gdbmi.write('monitor reset 2')  # reset via pin
gdbmi.exit()
