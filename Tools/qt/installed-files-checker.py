#!/usr/bin/env python3
# Copyright (C) 2020 Konstantin Tokarev <annulen@yandex.ru>
# Copyright (C) 2020 Rajagopalan-Gangadharan <g.raju2000@gmail.com>
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

import sys
from jinja2 import Environment, FileSystemLoader
import argparse
import os

parser = argparse.ArgumentParser(description='Checker for Qtwebkit Binaries')
parser.add_argument("--version", help=r"Version history of the form {major_version}.{minor_version}.{ver_patch}", required=True)
parser.add_argument("--qt", help="Root of Qt installation")
parser.add_argument("--build", help="Root of build directory")
parser.add_argument("--os", help="Operating system", required=True, choices=[ "linux", "macos", "windows" ])
parser.add_argument("--template", help='Relative path to template file', default="template/QtBinaryChecklist.txt")
parser.add_argument("--release", help='Release build', action='store_true')
parser.add_argument("--debug", help='Debug build', action='store_true')
parser.add_argument("--qt_install_header", help='Qt headers install path')
parser.add_argument("--qt_install_libs", help='Qt libraries install path')
parser.add_argument("--qt_install_archdata", help='Qt archdata install path')
parser.add_argument("--qt_install_libexecs", help='Qt libexecs install path')
parser.add_argument("--force_debug_info", help='Enable debug symbols for release builds', action='store_true')
parser.add_argument("--icu_version", help='ICU version')
parser.add_argument("--toolchain", help='Toolchain used e.g. msvc, mingw for windows')

args = parser.parse_args()

if not args.release and not args.debug:
    print("Please specify at least one build type!")
    exit(1)

template_abspath = os.path.abspath(args.template)
template_folder = os.path.dirname(template_abspath)
template_name = os.path.basename(template_abspath)

file_loader = FileSystemLoader(template_folder)  # directory of template file
env = Environment(loader=file_loader)

template = env.get_template(template_name)  # load template file

major, minor, patch = args.version.split('.')

check_list = template.render(os=args.os,
    major=major, version=args.version, release=args.release, debug=args.debug,
    icu_version=args.icu_version, force_debug_info=args.force_debug_info, toolchain=args.toolchain).split('\n')


def verify_linux(check_list):
    error_list = []

    for line in check_list:
        if line.rstrip():
            line = line.lstrip()
            if line.startswith('include/'):
                chk_path = os.path.join(args.qt_install_header, line[len('include/'):])
            elif line.startswith('lib/'):
                chk_path = os.path.join(args.qt_install_libs, line[len('lib/'):])
            elif line.startswith('mkspecs/') or line.startswith('qml/'):
                chk_path = os.path.join(args.qt_install_archdata, line)
            elif line.startswith('libexec/'):
                chk_path = os.path.join(args.qt_install_libexecs, line[len('libexec/'):])

            if not os.path.exists(chk_path):
                error_list.append(chk_path)

    return error_list

def verify_windows_mac(check_list):
    error_list = []

    build = os.path.join(os.getcwd(), args.build) if args.build else None
    for line in check_list:
        if line.rstrip():
            line = line.lstrip()
            chk_path = None
            if line.startswith('bin'):
                if build:
                    chk_path = os.path.join(build, line)
            else:
                chk_path = os.path.join(args.qt, line)

            if chk_path and (not os.path.exists(chk_path)):
                error_list.append(chk_path)

    return error_list


if args.os == 'linux':
    res = verify_linux(check_list)
elif args.os == 'windows' or args.os == 'macos':
    res = verify_windows_mac(check_list)

if len(res) != 0:
    print("Errors found files below are missing:")
    for err in res:
        print(err)
    exit(1)

print("All files are installed properly")


#python3 installed-files-checker.py --version 5.212.0 --build /mnt/c/qtwebkit/build --os linux
#
# py installed-files-checker.py --version 5.20.0 --qt "C:/Qt/5.14.2/msvc2017_64" --build "C:/qtwebkit/build/" --os windows --icu_version=65
