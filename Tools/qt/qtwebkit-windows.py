import os
import argparse
import pathlib


def parse_qt(qt):
    if qt != None:
        os.environ['QTDIR'] = qt


def parse_cmake(cmake):
    ## Eg -Wno-dev -DFLag1=val -DFlag2=val2 -fsanitize=address
    Dflags = ""
    flags = ""
    if cmake == "":
        return
    cl = cmake.split(' ')
    for cc in cl:
        if cc[1] == 'D':
            tc = cc.split('=')
            Dflags += tc[0][2:] + "#" + tc[1] + ";"
        else:
            flags += cc + " "

    os.environ["DCMAKEFLAGS"] = Dflags
    os.environ["CMAKEFLAGS"] = flags


parser = argparse.ArgumentParser("QtWebkit building with Conan")

parser.add_argument("--qt", help="Root Directory of Qt Installation", type=str)
parser.add_argument(
    "--cmakeargs", help="Space seperated values that should be passed as CMake arguments", default="", type=str)
parser.add_argument("--ninjaargs", help="Ninja arguments",
                    default="", type=str)
parser.add_argument(
    "--install", help="Pass this flag if you want to invoke install script", action="store_true")
parser.add_argument(
    "--build_folder", help="name of build folder defaults to build", default="build", type=str)

args = parser.parse_args()

src_directory = str(pathlib.Path(__file__).resolve().parents[2])
install_folder = src_directory + "\\" + args.build_folder

print("Path of build directory:" + install_folder)

if args.install == True:
    script = 'conan install conanfile.py -if "{0}"'.format(install_folder)
    os.system(script)

parse_qt(args.qt)
parse_cmake(args.cmakeargs)

script = 'conan build conanfile.py -sf "{0}" -bf "{1}"'.format(
    src_directory, install_folder)
os.system(script)
