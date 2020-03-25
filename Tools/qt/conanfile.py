from conans import ConanFile, CMake, tools
import os

class QtWebKitConan(ConanFile):
    name = "qtwebkit"
    version = "5.212.0-alpha4"
    license = "LGPL-2.0-or-later, LGPL-2.1-or-later, BSD-2-Clause"
    url = "https://github.com/qtwebkit/qtwebkit"
    description = "Qt port of WebKit"
    topics = ( "qt", "browser-engine", "webkit", "qt5", "qml", "qtwebkit" )
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
                self.build_requires('pkg-config_installer/0.29.2@bincrafters/stable')

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
        cmake = CMake(self,set_cmake_flags=True)
        cmake.generator = "Ninja"
        cmake.verbose = False
        cmake.definitions["QT_CONAN_DIR"] = self.build_folder
        cmake.definitions["CMAKE_BUILD_TYPE"]="Release"

        os.environ["CC"]="cl"
        os.environ["CXX"]="cl"
        # if self.options.use_ccache:
        #    cmake.definitions["CMAKE_C_COMPILER_LAUNCHER"] = "ccache"
        #    cmake.definitions["CMAKE_CXX_COMPILER_LAUNCHER"] = "ccache"
        #conan build ..\Tools\qt\conanfile.py --source-folder C:\qtwebkit

        if "Qt5_dir" in os.environ:
            cmake.definitions["Qt5_DIR"] = os.environ["Qt5_dir"]
        else:
            self.output.error("Specify path to Qt5 directory by setting environment variable Qt5_dir")
        
        print(cmake.definitions["CONAN_CXX_FLAGS"])
        
        #Retrieve cmake args set by wrapper
        if "Temp_D_Flags_For_Dev" in os.environ:
            ddef=os.environ["Temp_D_Flags_For_Dev"]
            ddef_list=ddef.split(';')
            for ddef_it in ddef_list:
                dtemp=ddef_it.split('#')
                if len(dtemp)==2:
                    cmake.definitions[dtemp[0]]=dtemp[1]

        
        if "Temp_File_FLags_For_Dev" in os.environ:
            defs=" "+os.environ["Temp_File_FLags_For_Dev"]+" "
            #print(cmake.definitions["CONAN_CXX_FLAGS"]+defs)
            cmake.definitions["CONAN_CXX_FLAGS"]=defs+cmake.definitions["CONAN_CXX_FLAGS"]

        if self.settings.os == "Windows":
            print(tools.vcvars_command(self.settings))
        
        print(self.source_folder)
        print()
        print(self.build_folder)

        #self.run(r'cmake -DQT_CONAN_DIR=. -DCMAKE_BUILD_TYPE=Release -GNinja -DQt5_DIR=C:\Qt\5.13.1\msvc2017_64\lib\cmake\Qt5 ..')
        cmake.configure()
        cmake.build()
        

    def package(self):
        pass

    def package_info(self):
        pass
