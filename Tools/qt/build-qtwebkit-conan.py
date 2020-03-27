import os
import argparse
import pathlib


def parse_qt(qt):
    if qt != None:
        os.environ['QTDIR'] = qt


def parse_cmake(cmake):
    if cmake == "":
        return
    os.environ["CMAKEFLAGS"] = cmake


def parse_ninja(ninja):
    if ninja == "":
        return
    os.environ["NINJAFLAGS"] = ninja


def parse_compiler(compiler):
    if compiler.lower() == "msvc":
        if "CC" not in os.environ:
            os.environ["CC"] = "cl"
        if "CXX" not in os.environ:
            os.environ["CXX"] = "cl"


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
parser.add_argument("--compiler", help="Which compiler to choose", default="msvc", type=str)
parser.add_argument("--configure", help="Execute Only configuration step", action="store_true")
parser.add_argument("--build", help="Execute Only Build step", action="store_true")

args = parser.parse_args()

src_directory = str(pathlib.Path(__file__).resolve().parents[2])
install_folder = src_directory + "\\" + args.build_folder
conanfile_path = src_directory + r"\Tools\qt\conanfile.py"


print("Path of build directory:" + install_folder)

if args.install == True:
    script = 'conan install {0} -if "{1}"'.format(conanfile_path, install_folder)
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

script = 'conan build {0} {1} {2} -sf "{3}" -bf "{4}"'.format(conanfile_path, cflag, bflag, src_directory, install_folder)

print("Executing:", script)
os.system(script)
