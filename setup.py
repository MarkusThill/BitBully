"""Setup script for building the BitBully C++ extension module using CMake.

This module defines custom setuptools extension and build classes to integrate
CMake into the Python packaging and distribution workflow.
"""

import subprocess
import sys
from pathlib import Path

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext


class CMakeBuildExtension(build_ext):
    """Custom setuptools build extension using CMake."""

    def build_extension(self, ext: Extension) -> None:
        """Builds the extension using CMake.

        Args:
            ext (Extension): The extension to be built.

        Raises:
            TypeError: If the provided extension is not a CMakeExtension instance.
        """
        if not isinstance(ext, CMakeExtension):
            raise TypeError("Expected a CMakeExtension instance")

        # Get the extension's build directory
        # extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        extdir: str = str(Path(self.get_ext_fullpath(ext.name)).parent.resolve())
        cfg = "Debug" if self.debug else "Release"
        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE={cfg}",
        ]

        # Create the build directory
        if not Path(self.build_temp).exists():
            Path(self.build_temp).mkdir(parents=True)

        # Run CMake
        subprocess.check_call(
            ["cmake", ext.sourcedir, *cmake_args], cwd=self.build_temp
        )
        # Run the build
        subprocess.check_call(
            ["cmake", "--build", ".", "--target", "bitbully_core"], cwd=self.build_temp
        )


class CMakeExtension(Extension):
    """A setuptools extension for building Python modules using CMake."""

    sourcedir: Path

    def __init__(self, name: str, sourcedir: str = "") -> None:
        """Initializes the CMakeExtension.

        Args:
            name (str): The name of the extension.
            sourcedir (str, optional): The source directory of the extension.
                Defaults to "".
        """
        super().__init__(name, sources=[])
        self.sourcedir = Path(sourcedir).resolve()


setup(
    # name="bitbully",
    # version="0.0.41",  # already defined in the pyproject.toml
    # packages=["bitbully"],
    ext_modules=[CMakeExtension("bitbully.bitbully_core")],
    cmdclass={"build_ext": CMakeBuildExtension},
    zip_safe=False,
)
