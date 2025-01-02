import os
import subprocess
import sys
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


class CMakeBuildExtension(build_ext):
    def build_extension(self, ext):
        # Get the extension's build directory
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cfg = "Debug" if self.debug else "Release"
        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE={cfg}",
        ]

        # Create the build directory
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        # Run CMake
        subprocess.check_call(
            ["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp
        )
        # Run the build
        subprocess.check_call(
            ["cmake", "--build", ".", "--target", "bitbully_core"], cwd=self.build_temp
        )


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


setup(
    # name="bitbully",
    # version="0.0.16",  # already defined in the pyproject.toml (might get rid of it here)
    # packages=["bitbully"],
    ext_modules=[CMakeExtension("bitbully.bitbully_core")],
    cmdclass={"build_ext": CMakeBuildExtension},
    zip_safe=False,
)
