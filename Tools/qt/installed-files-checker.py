#!/usr/bin/env python3
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

args = parser.parse_args()

template_abspath = os.path.abspath(args.template)
template_folder = os.path.dirname(template_abspath)
template_name = os.path.basename(template_abspath)

file_loader = FileSystemLoader(template_folder) # directory of template file
env = Environment(loader=file_loader)

template = env.get_template(template_name) # load template file

major, minor, patch = args.version.split('.')

check_list = template.render(os=args.os,
    major=major, version=args.version, release=args.release, debug=args.debug).split('\n')

file_count = {"linux_Release": 108, "windows_Debug": 118,"windows_Release":110, "macos_Release": 170}


def verify_linux(check_list):
    error_list = []
    count = 0

    for line in check_list:
        if line.rstrip():
            if line.startswith('include/'):
                chk_path = os.path.join(args.qt_install_header, line[len('include/'):])
            elif line.startswith('lib/'):
                chk_path = os.path.join(args.qt_install_libs, line[len('lib/'):])
            elif line.startswith('mkspecs/') or line.startswith('qml/'):
                chk_path = os.path.join(args.qt_install_archdata, line)
            elif line.startswith('libexec/'):
                chk_path = os.path.join(args.qt_install_libexecs, line[len('libexec/'):])

            count+=1

            if not os.path.exists(chk_path):
                error_list.append(chk_path)

    return [error_list, count]

def verify_windows_mac(check_list):
    error_list = []
    count = 0

    build=os.path.join(os.getcwd(),args.build)
    for line in check_list:
        if line.rstrip():
            if line.startswith('bin'):
                chk_path = os.path.join(build, line)
            else:
                chk_path = os.path.join(args.qt, line)

            count+=1
            if not os.path.exists(chk_path):
                error_list.append(chk_path)

    return [error_list, count]


if args.os == 'linux':
    res = verify_linux(check_list)
elif args.os == 'windows' or args.os == 'macos':
    res = verify_windows_mac(check_list)

build_type = 'Debug' if args.debug else 'Release'

print("Verified {0}/{1} files".format(res[1],file_count[args.os+'_'+build_type]))
if len(res[0])!=0:
    print("Errors found files below are missing:")
    for err in res[0]:
        print(err)
    exit(1)

print("All files are installed properly")


#python3 checker.py --version 5.212.0 --build /mnt/c/qtwebkit/build --os linux
#
# py checker.py --version 5.20.0 --qt "C:/Qt/5.14.2/msvc2017_64" --build "C:/qtwebkit/build/" --os windows
