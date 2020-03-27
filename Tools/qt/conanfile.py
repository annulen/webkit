# Copyright (C) 2020 Konstantin Tokarev <annulen@yandex.ru>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

from conans import ConanFile, CMake, tools
import os


class QtWebKitConan(ConanFile):
    name = "qtwebkit"
    version = "5.212.0-alpha4"
    license = "LGPL-2.0-or-later, LGPL-2.1-or-later, BSD-2-Clause"
    url = "https://github.com/qtwebkit/qtwebkit"
    description = "Qt port of WebKit"
    topics = ("qt", "browser-engine", "webkit", "qt5", "qml", "qtwebkit")
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake", "virtualenv"
    exports_sources = "../../*"
    no_copy_source = True
    requires = (
        "icu/65.1@qtproject/stable",
        "libxml2/2.9.10@qtproject/stable",
        "libxslt/1.1.34@qtproject/stable",
        "libjpeg-turbo/2.0.3@qtproject/stable",
        "zlib/1.2.11",

        "libpng/1.6.37",
        "sqlite3/3.31.1"
    )
    default_options = {
        "icu:shared": True,
        "icu:data_packaging": "library",

        "libxml2:shared": True,
        "libxml2:iconv": False,
        "libxml2:icu": True,
        "libxml2:zlib": False,

        "libxslt:shared": True,

        "libjpeg-turbo:shared": False
    }
# For building with conan
#    options = {
#        "use_ccache": [True, False],
#        "qt5_dir": "ANY"
#    }
#    build_requires = (
#        "ninja/1.9.0",
#        "cmake/3.16.4"
#    )

    def build_requirements(self):
        if self.settings.os == 'Linux':
            if not tools.which('pkg-config'):
                self.build_requires(
                    'pkg-config_installer/0.29.2@bincrafters/stable')

        # gperf python perl bison ruby flex
        if not tools.which("gperf"):
            self.build_requires("gperf_installer/3.1@conan/stable")
        if not tools.which("perl"):
            self.build_requires("strawberryperl/5.30.0.1")
        if not tools.which("ruby"):
            self.build_requires("ruby_installer/2.6.3@bincrafters/stable")
        if not tools.which("bison"):
            self.build_requires("bison_installer/3.3.2@bincrafters/stable")
        if not tools.which("flex"):
            self.build_requires("flex_installer/2.6.4@bincrafters/stable")

    def build(self):
        cmake = CMake(self, set_cmake_flags=True)
        cmake.generator = "Ninja"
        cmake.verbose = False
        cmake.definitions["QT_CONAN_DIR"] = self.build_folder

        # if self.options.use_ccache:
        #    cmake.definitions["CMAKE_C_COMPILER_LAUNCHER"] = "ccache"
        #    cmake.definitions["CMAKE_CXX_COMPILER_LAUNCHER"] = "ccache"

        if "QTDIR" in os.environ:
            cmake.definitions["Qt5_DIR"] = os.environ["QTDIR"] + \
                r'\lib\cmake\Qt5'
            print("Qt5 directory:" + cmake.definitions["Qt5_DIR"])

        if "CMAKEFLAGS" in os.environ:
            cmake_flags = os.environ["CMAKEFLAGS"].split(' ')
        else:
            cmake_flags = None

        if "NINJAFLAGS" in os.environ:
            ninja_flags = os.environ["NINJAFLAGS"]
        else:
            ninja_flags = ""

        if self.settings.os == "Windows":
            print(tools.vcvars_command(self.settings))

        print(self.source_folder)
        print()
        print(self.build_folder)

        cmake.configure(args=cmake_flags)
        
        self.run(tools.vcvars_command(self.settings))
        self.run("ninja "+ninja_flags)

    def package(self):
        pass

    def package_info(self):
        pass
