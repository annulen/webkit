#!/usr/bin/env python3
# Copyright (C) 2020 Konstantin Tokarev <annulen@yandex.ru>
# Copyright (C) 2020 Rajagopalan Gangadharan <g.raju2000@gmail.com>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.

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


def set_profile(profile, cc, cxx):
    if profile == "qtwebkit_msvc":
        run_command("conan profile new {0} --detect --force".format(profile))
        os.environ["CC"] = cc
        os.environ["CXX"] = cxx
    else:
        os.environ["CC"] = cc
        os.environ["CXX"] = cxx
        run_command("conan profile new {0} --detect --force".format(profile))
        if platform.system() == "Windows" and "gcc" in profile:
            run_command("conan profile update settings.compiler.threads=posix {0}".format(profile))
            run_command("conan profile update settings.compiler.exception=seh {0}".format(profile))


def parse_compiler(compiler):
    preset_data = {
        "msvc": ["qtwebkit_msvc", "cl", "cl"],
        "clang": ["qtwebkit_clang", "clang", "clang++"],
        "gcc": ["qtwebkit_gcc", "gcc", "g++"]
    }
    if not compiler:
        if platform.system() == "Windows":
            compiler = "msvc"
        elif platform.system() == "Darwin":
            compiler = "clang"

    if compiler in preset_data:
        set_profile(*preset_data[compiler])
        return preset_data[compiler][0]

    sys.exit("Error: Unknown Compiler " + compiler + " specified")


def run_command(command):
    print("Executing:", command)
    exit_code = os.system(command)
    print("Exit code:", exit_code)
    if exit_code:
        sys.exit(1)


parser = argparse.ArgumentParser(description='Build QtWebKit with Conan. For installation of build product into Qt, use --install option')

parser.add_argument("--qt", help="Root directory of Qt Installation", type=str, metavar="QTDIR")
parser.add_argument(
    "--cmakeargs", help="Space separated values that should be passed as CMake arguments", default="", type=str)
parser.add_argument("--ninjaargs", help="Ninja arguments",
                    default="", type=str)
parser.add_argument(
    "--build_directory", help="Name of build dirtectory (defaults to build)", default="build", type=str)
parser.add_argument("--compiler", help="Specify compiler for build (msvc, gcc, clang)", type=str)
parser.add_argument("--configure", help="Execute the configuration step. When specified, build won't run unless --build is specified", action="store_true")
parser.add_argument("--build", help="Execute the build step. When specified, configure won't run unless --configure is specified", action="store_true")
parser.add_argument("--install", help="Execute the install step. When specified, configure and build steps WILL run without changes", action="store_true")
parser.add_argument("--profile", help="Name of conan profile provided by user. Note: compiler and profile options are mutually exclusive", default="default", type=str)

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

if args.profile != "default" and args.compiler != None:
    sys.exit("Error: Both compiler and Conan profile specified")

profile_flag = parse_compiler(args.compiler) if args.profile == "default" else args.profile

script = 'conan install {0} -if "{1}" --build=missing --profile={2}'.format(conanfile_path, build_directory, profile_flag)
run_command(script)

parse_qt(args.qt)
parse_cmake(args.cmakeargs)
parse_ninja(args.ninjaargs)

if not args.configure and not args.build:
    # If we have neither --configure nor --build, we should do both configure and build (but install only if requested)
    args.configure = True
    args.build = True

configure_flag = "--configure" if args.configure else ""
build_flag = "--build" if args.build else ""
install_flag = "--install" if args.install else ""

script = 'conan build {0} {1} {2} -sf "{3}" -bf "{4}" "{5}"'.format(configure_flag, build_flag, install_flag, src_directory, build_directory, conanfile_path)
run_command(script)
