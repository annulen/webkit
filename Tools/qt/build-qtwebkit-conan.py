#!/usr/bin/env python3

import os
import argparse
import pathlib
import platform


def parse_qt(qt):
    if qt:
        os.environ['QTDIR'] = qt


def parse_cmake(cmake):
    if cmake:
        os.environ["CMAKEFLAGS"] = cmake


def parse_ninja(ninja):
    if ninja:
        os.environ["NINJAFLAGS"] = ninja


def parse_compiler(compiler):
    if not compiler and not ("CC" in os.environ and "CXX" in os.environ):
        if platform.system() == "Windows":
            compiler = "msvc"
        elif platform.system() == "Darwin":
            compiler = "clang"

    if compiler == "msvc":
        os.environ["CC"] = "cl"
        os.environ["CXX"] = "cl"
    elif compiler == "clang":
        os.environ["CC"] = "clang"
        os.environ["CXX"] = "clang++"
    elif compiler == "gcc":
        os.environ["CC"] = "gcc"
        os.environ["CXX"] = "g++"


parser = argparse.ArgumentParser()

parser.add_argument("--qt", help="Root directory of Qt Installation", type=str)
parser.add_argument(
    "--cmakeargs", help="Space seperated values that should be passed as CMake arguments", default="", type=str)
parser.add_argument("--ninjaargs", help="Ninja arguments",
                    default="", type=str)
parser.add_argument(
    "--install", help="Pass this flag if you want to invoke install script", action="store_true")
parser.add_argument(
    "--build_directory", help="Name of build dirtectory (defaults to build)", default="build", type=str)
parser.add_argument("--compiler", help="Specify compiler for build (msvc, gcc, clang)", type=str)
parser.add_argument("--configure", help="Execute Only configuration step", action="store_true")
parser.add_argument("--build", help="Execute Only Build step", action="store_true")

args = parser.parse_args()

src_directory = str(pathlib.Path(__file__).resolve().parents[2])

if os.path.isabs(args.build_directory):
    build_directory = args.build_directory
else:
    build_directory = os.path.join(src_directory, args.build_directory)

conanfile_path = os.path.join(src_directory, "Tools", "qt", "conanfile.py")

print("Path of build directory:" + build_directory)

script = 'conan install {0} -if "{1}"'.format(conanfile_path, build_directory)
os.system(script)

parse_qt(args.qt)
parse_cmake(args.cmakeargs)
parse_ninja(args.ninjaargs)
parse_compiler(args.compiler)

if args.configure == True:
    cflag = "-c"
else:
    cflag = ""

if args.build == True:
    bflag = "-b"
else:
    bflag = ""

script = 'conan build {0} {1} {2} -sf "{3}" -bf "{4}"'.format(conanfile_path, cflag, bflag, src_directory, build_directory)

print("Executing:", script)
os.system(script)
