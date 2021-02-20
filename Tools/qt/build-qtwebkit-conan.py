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
import subprocess
from conans.client.conf.detect import _get_default_compiler
from conans.client.output import ConanOutput


def run_command(command):
    print("Executing:", command)
    exit_code = os.system(command)
    print("Exit code:", exit_code)
    if exit_code:
        sys.exit(1)


def run_comman_output(command):
    print("Executing:", command)
    output = os.popen(command)
    return output.read()


class ConanProfile:
    def __init__(self, profile_name):
        self.name = profile_name

    def create(self):
        run_command("conan profile new {0} --detect --force".format(self.name))

    def get(self, setting):
        return subprocess.check_output(f"conan profile get settings.{setting} {self.name}", shell=True).rstrip().decode('ascii')

    def update(self, setting, value):
        run_command("conan profile update settings.{0}={1} {2}".format(setting, value, self.name))


def set_env_variable(var, value, override):
    if var in os.environ and len(os.environ[var]) != 0 and not os.environ[var].isspace():
        old_value = os.environ[var]
        if old_value != value:
            if override:
                print(f"Warning: overriding environment variable '{var}': '{old_value}' -> '{value}'")
                os.environ[var] = value
            else:
                print(f"Note: using environment variable '{var}' = '{old_value}'. Undefine it to use '{value}' instead")

    else:
        os.environ[var] = value


def set_compiler_environment(cc, cxx, override):
    set_env_variable("CC", cc, override)
    set_env_variable("CXX", cxx, override)


def get_cc_cxx(compiler):
    compiler_preset = {
        "msvc": ["cl", "cl"],
        "clang": ["clang", "clang++"],
        "gcc": ["gcc", "g++"]
    }

    return compiler_preset[compiler]


def create_profile(compiler, arch):
    if not compiler:
        if platform.system() == "Windows":
            detected_toolchain = _get_default_compiler(ConanOutput(sys.stdout))
            if not detected_toolchain:
                sys.exit("Error: No toolchain detected")

            detected_compiler = detected_toolchain[0]
            if detected_compiler == "Visual Studio":
                compiler = "msvc"
            else:
                compiler = detected_compiler

        elif platform.system() == "Darwin":
            compiler = "clang"
        elif platform.system() == "Linux":
            compiler = "gcc"

    try:
        cc, cxx = get_cc_cxx(compiler)
    except KeyError:
        sys.exit(f"Error: Unsupported compiler '{compiler}' specified")

    profile = ConanProfile('qtwebkit_{0}_{1}'.format(compiler, arch))  # e.g. qtwebkit_msvc_x86

    if compiler == "msvc":
        profile.create()
        set_compiler_environment(cc, cxx, override=True)
    else:
        set_compiler_environment(cc, cxx, override=True)
        profile.create()

    if arch == 'default':
        arch = profile.get('arch_build')

    profile.update('arch', arch)
    profile.update('arch_build', arch)

    if platform.system() == "Windows" and compiler == "gcc":
        profile.update('compiler.threads', 'posix')
        if arch == 'x86':
            profile.update('compiler.exception', 'dwarf2')
        if arch == 'x86_64':
            profile.update('compiler.exception', 'seh')

    return profile.name


def set_environment_for_profile(profile_name):
    profile = ConanProfile(profile_name)
    compiler = profile.get('compiler')

    if compiler == "Visual Studio":
        compiler = "msvc"

    try:
        cc, cxx = get_cc_cxx(compiler)
    except KeyError:
        sys.exit(f"Error: Unsupported compiler '{compiler}' specified in profile '{profile_name}'")
    set_compiler_environment(cc, cxx, override=False)


def get_qt_version(qt_path):
    qmake_path = os.path.join(qt_path, "bin", "qmake")
    qt_version = run_comman_output(f"{qmake_path} -query QT_VERSION").rstrip()

    if not qt_version:
        if qt_path:
            sys.exit(f"Can't find working qmake in {qt_path}")
        else:
            sys.exit("Can't find working qmake in PATH")

    return qt_version


parser = argparse.ArgumentParser(description='Build QtWebKit with Conan. For installation of build product into Qt, use --install option')

parser.add_argument("--qt", help="Root directory of Qt Installation", type=str, metavar="QTDIR")
parser.add_argument(
    "--cmakeargs", help="Space separated values that should be passed as CMake arguments", default="", type=str)
parser.add_argument("--ninjaargs", help="Ninja arguments",
                    default="", type=str)
parser.add_argument(
    "--build_directory", help="Name of build dirtectory (defaults to build)", default="build", type=str)
parser.add_argument("--compiler", help="Specify compiler for build (msvc, gcc, clang)", default=None, choices=['gcc', 'msvc', 'clang'], type=str)
parser.add_argument("--configure", help="Execute the configuration step. When specified, build won't run unless --build is specified", action="store_true")
parser.add_argument("--build", help="Execute the build step. When specified, configure won't run unless --configure is specified", action="store_true")
parser.add_argument("--install", help="Execute the install step. When specified, configure and build steps WILL run without changes", action="store_true")
parser.add_argument("--profile", help="Name of conan profile provided by user. Note: compiler and profile options are mutually exclusive", type=str)
parser.add_argument("--arch", help="32 bit or 64 bit build, leave blank for autodetect", default="default", choices=['x86', 'x86_64'])
parser.add_argument("--build_type", help="Name of CMake build configuration to use", default="Release", choices=['', 'Release', 'Debug'])
parser.add_argument("--install_prefix", help="Set installation prefix to the given path (defaults to Qt directory)", default=None)
parser.add_argument("--ignore-qt-bundled-deps",
                    help="Don't try to match versions of dependencies bundled with Qt and use latest versions of them", action="store_true")

args = parser.parse_args()

# Always print commands run by conan internally
os.environ["CONAN_PRINT_RUN_COMMANDS"] = "1"

src_directory = str(pathlib.Path(__file__).resolve().parents[2])

if os.path.isabs(args.build_directory):
    build_directory = args.build_directory
else:
    build_directory = os.path.join(src_directory, args.build_directory)

conanfile_path = os.path.join(src_directory, "Tools", "qt", "conanfile.py")

print("Path of build directory:" + build_directory)

run_command("conan remote add -f bincrafters https://api.bintray.com/conan/bincrafters/public-conan")
run_command("conan remote add -f qtproject https://api.bintray.com/conan/qtproject/conan --insert")

if args.profile and args.compiler:
    sys.exit("Error: --compiler and --profile cannot be specified at the same time")

if not args.profile:
    profile_name = create_profile(args.compiler, args.arch)
else:
    profile_name = args.profile
    set_environment_for_profile(profile_name)

build_vars = f'-o qt="{args.qt}" -o cmakeargs="{args.cmakeargs}" -o build_type="{args.build_type}"'

if args.qt and not args.ignore_qt_bundled_deps:
    qt_version = get_qt_version(args.qt)
    build_vars += f' -o qt_version="{qt_version}"'

if args.install_prefix:
    build_vars += ' -o install_prefix="{}"'.format(args.install_prefix)
elif args.qt:
    build_vars += ' -o install_prefix="{}"'.format(args.qt)

if args.ninjaargs:
    os.environ["NINJAFLAGS"] = args.ninjaargs

if not args.configure and not args.build:
    # If we have neither --configure nor --build, we should do both configure and build (but install only if requested)
    args.configure = True
    args.build = True

if args.configure:
    run_command('conan install {0} -if "{1}" --build=missing --profile={2} {3}'.format(conanfile_path, build_directory, profile_name, build_vars))

configure_flag = "--configure" if args.configure else ""
build_flag = "--build" if args.build else ""
install_flag = "--install" if args.install else ""

run_command('conan build {0} {1} {2} -sf "{3}" -bf "{4}" "{5}"'.format(configure_flag, build_flag, install_flag, src_directory, build_directory, conanfile_path))
