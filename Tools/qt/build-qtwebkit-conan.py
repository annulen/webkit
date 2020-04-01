#!/usr/bin/env python3

import os
import argparse
import pathlib
import platform
import sys


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


def run_command(command):
    print("Executing:", command)
    exit_code = os.system(command)
    print("Exit code:", exit_code)
    if exit_code:
        sys.exit(1)


parser = argparse.ArgumentParser(description='Build QtWebKit with Conan. For installation of build product into Qt, use --install option')

parser.add_argument("--qt", help="Root directory of Qt Installation", type=str)
parser.add_argument(
    "--cmakeargs", help="Space seperated values that should be passed as CMake arguments", default="", type=str)
parser.add_argument("--ninjaargs", help="Ninja arguments",
                    default="", type=str)
parser.add_argument(
    "--build_directory", help="Name of build dirtectory (defaults to build)", default="build", type=str)
parser.add_argument("--compiler", help="Specify compiler for build (msvc, gcc, clang)", type=str)
parser.add_argument("--configure", help="Execute the configuration step. When specified, build won't run unless --build is specified", action="store_true")
parser.add_argument("--build", help="Execute the build step. When specified, configure won't run unless --configure is specified", action="store_true")
parser.add_argument("--install", help="Execute the install step. When specified, configure and build steps WILL run without changes", action="store_true")

args = parser.parse_args()

src_directory = str(pathlib.Path(__file__).resolve().parents[2])

if os.path.isabs(args.build_directory):
    build_directory = args.build_directory
else:
    build_directory = os.path.join(src_directory, args.build_directory)

conanfile_path = os.path.join(src_directory, "Tools", "qt", "conanfile.py")

print("Path of build directory:" + build_directory)

run_command("conan remote add -f bincrafters https://api.bintray.com/conan/bincrafters/public-conan")
run_command("conan remote add -f qtproject https://api.bintray.com/conan/qtproject/conan")

script = 'conan install {0} -if "{1}" --build=missing'.format(conanfile_path, build_directory)
run_command(script)

parse_qt(args.qt)
parse_cmake(args.cmakeargs)
parse_ninja(args.ninjaargs)
parse_compiler(args.compiler)

if not args.configure and not args.build:
    # If we have neither --configure nor --build, we should do both configure and build (but install only if requested)
    args.configure = True
    args.build = True

configure_flag = "--configure" if args.configure else ""
build_flag = "--build" if args.build else ""
install_flag = "--install" if args.install else ""

script = 'conan build {0} {1} {2} -sf "{3}" -bf "{4}" "{5}"'.format(configure_flag, build_flag, install_flag, src_directory, build_directory, conanfile_path)
run_command(script)
