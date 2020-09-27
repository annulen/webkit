import requests

dependencies = {
    # "5.6.0" : {"sqlite" : "3.8.10","libpng" : "1.6.20","libjpeg" : "8c" } ,
    # "5.6.3" : {"sqlite" : "3.8.10","libpng" : "1.6.20","libjpeg" : "8c" } ,
    # "5.9.0" : {"sqlite" : "3.16.1","libpng" : "1.6.28","libjpeg" : "8c" } ,
    # "5.9.1" : {"sqlite" : "3.16.2","libpng" : "1.6.28","libjpeg" : "8c" } ,
    # "5.9.2" : {"sqlite" : "3.16.2","libpng" : "1.6.32","libjpeg" : "8c" } ,
    # "5.9.3" : {"sqlite" : "3.16.2","libpng" : "1.6.34","libjpeg" : "8c" } ,
    # "5.9.4" : {"sqlite" : "3.16.2","libpng" : "1.6.34","libjpeg" : "8c" } ,
    # "5.9.5" : {"sqlite" : "3.16.2","libpng" : "1.6.34","libjpeg" : "8c" } ,
    # "5.9.6" : {"sqlite" : "3.16.2","libpng" : "1.6.34","libjpeg" : "8c" } ,
    # "5.9.7" : {"sqlite" : "3.24.0","libpng" : "1.6.34","libjpeg" : "8c" } ,
    # "5.9.8" : {"sqlite" : "3.24.0","libpng" : "1.6.34","libjpeg" : "8c" } ,
    # "5.9.9" : {"sqlite" : "3.24.0","libpng" : "1.6.37","libjpeg" : "8c" } ,
    # "5.10.0" : {"sqlite" : "3.20.1","libpng" : "1.6.34","libjpeg-turbo" : "1.5.2" } ,
    # "5.10.1" : {"sqlite" : "3.20.1","libpng" : "1.6.34","libjpeg-turbo" : "1.5.3" } ,
    # "5.11.0" : {"sqlite" : "3.23.1","libpng" : "1.6.34","libjpeg-turbo" : "1.5.3" } ,
    # "5.11.1" : {"sqlite" : "3.23.1","libpng" : "1.6.34","libjpeg-turbo" : "1.5.3" } ,
    # "5.11.2" : {"sqlite" : "3.24.0","libpng" : "1.6.34","libjpeg-turbo" : "1.5.3" } ,
    # "5.11.3" : {"sqlite" : "3.24.0","libpng" : "1.6.35","libjpeg-turbo" : "1.5.3" } ,
    # "5.12.0" : {"sqlite" : "3.24.0","libpng" : "1.6.35","libjpeg-turbo" : "2.0.0" } ,
    # "5.12.1" : {"sqlite" : "3.26.0","libpng" : "1.6.35","libjpeg-turbo" : "2.0.0" } ,
    # "5.12.2" : {"sqlite" : "3.26.0","libpng" : "1.6.36","libjpeg-turbo" : "2.0.0" } ,
    # "5.12.3" : {"sqlite" : "3.26.0","libpng" : "1.6.36","libjpeg-turbo" : "2.0.0" } ,
    # "5.12.4" : {"sqlite" : "3.28.0","libpng" : "1.6.37","libjpeg-turbo" : "2.0.0" } ,
    # "5.12.5" : {"sqlite" : "3.28.0","libpng" : "1.6.37","libjpeg-turbo" : "2.0.0" } ,
    "5.12.6" : {"sqlite" : "3.29.0","libpng" : "1.6.37","libjpeg-turbo" : "2.0.3" } ,
    "5.12.7" : {"sqlite" : "3.30.1","libpng" : "1.6.37","libjpeg-turbo" : "2.0.3" } ,
    "5.12.8" : {"sqlite" : "3.31.1","libpng" : "1.6.37","libjpeg-turbo" : "2.0.3" } ,
    "5.12.9" : {"sqlite" : "3.32.1","libpng" : "1.6.37","libjpeg-turbo" : "2.0.3" } ,
    "5.13.0" : {"sqlite" : "3.28.0","libpng" : "1.6.37","libjpeg-turbo" : "2.0.2" } ,
    "5.13.1" : {"sqlite" : "3.28.0","libpng" : "1.6.37","libjpeg-turbo" : "2.0.2" } ,
    "5.13.2" : {"sqlite" : "3.29.0","libpng" : "1.6.37","libjpeg-turbo" : "2.0.2" } ,
    "5.14.0" : {"sqlite" : "3.30.1","libpng" : "1.6.37","libjpeg-turbo" : "2.0.3" } ,
    "5.14.1" : {"sqlite" : "3.30.1","libpng" : "1.6.37","libjpeg-turbo" : "2.0.3" } ,
    "5.14.2" : {"sqlite" : "3.31.1","libpng" : "1.6.37","libjpeg-turbo" : "2.0.3" } ,
    "5.15.0" : {"sqlite" : "3.31.1","libpng" : "1.6.37","libjpeg-turbo" : "2.0.4" } ,
    "5.15.1" : {"sqlite" : "3.32.3","libpng" : "1.6.37","libjpeg-turbo" : "2.0.5" } ,
}


def check_version(qt_version):
    return qt_version in dependencies


def get_dependencies(qt_version):
    return dependencies[qt_version]


class Scrapper:
    def __init__(self):
        self.versions = ['5.6.0', '5.6.3', '5.9.0', '5.9.1', '5.9.2', '5.9.3', '5.9.4', '5.9.5', '5.9.6', '5.9.7', '5.9.8', '5.9.9', '5.10.0', '5.10.1', '5.11.0', '5.11.1',
               '5.11.2', '5.11.3', '5.12.0', '5.12.1', '5.12.2', '5.12.3', '5.12.4', '5.12.5', '5.12.6', '5.12.7', '5.12.8', '5.12.9', '5.13.0', '5.13.1', '5.13.2', '5.14.0', '5.14.1', '5.14.2', '5.15.0', '5.15.1']
        self.lib = ["sqlite", "libpng", "libjpeg"]
        self.url_prefix = "https://code.qt.io/cgit/qt/qtbase.git/plain/src/3rdparty/"
        self.url_suffix = "/qt_attribution.json?h=v"

    def get_versions(self,lib_names, qt_version):
        dep = dict({})
        for lib in lib_names:
            con_url = self.url_prefix + lib + self.url_suffix + qt_version
            try:
                source = requests.get(con_url).json(strict=False)
                dep[str(source["Name"]).lower()] = str(source["Version"]).lower()
            except ValueError:
                dep[str(lib).lower()] = None
            except KeyError:
                dep[str(source["Name"]).lower()] = None

        return dep

    def generate_markdown(self):
        ''' Method generates a markdown table of dependencies'''
        md = r'''
        | Qt | sqlite | libpng | libjpeg |
        |----|--------|--------|---------|
        '''

        for ver in self.versions:
            dep = self.get_versions(self.lib, ver)
            md = " | " + ver
            for xx in dep:
                md += f" | {str(dep[xx])}"
        md += " | "
        return md
