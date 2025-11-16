"""Setup script for building the BitBully C++ extension module using CMake.

This module defines custom setuptools extension and build classes to integrate
CMake into the Python packaging and distribution workflow.
"""

import os
import subprocess
import sys
from pathlib import Path

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

# Convert distutils Windows platform specifiers to CMake -A arguments
PLAT_TO_CMAKE = {
    "win32": "Win32",
    "win-amd64": "x64",
    "win-arm32": "ARM",
    "win-arm64": "ARM64",
}


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
            f"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY={extdir}",
            f"-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY={extdir}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE={cfg}",
        ]

        build_args = []

        # CMake lets you override the generator - we need to check this.
        # Can be set with Conda-Build, for example.
        cmake_generator = os.environ.get("CMAKE_GENERATOR", "")
        # TODO: Windows specific:
        if self.compiler.compiler_type != "msvc":
            pass
            # Not sure if we have to do something here. Check:
            # https://github.com/pybind/cmake_example/blob/master/setup.py
        else:
            # Single config generators are handled "normally"
            single_config = any(x in cmake_generator for x in {"NMake", "Ninja"})

            # CMake allows an arch-in-generator style for backward compatibility
            contains_arch = any(x in cmake_generator for x in {"ARM", "Win64"})

            # Specify the arch if using MSVC generator, but only if it doesn't
            # contain a backward-compatibility arch spec already in the
            # generator name.
            if not single_config and not contains_arch:
                cmake_args += ["-A", PLAT_TO_CMAKE[self.plat_name]]

            # Multi-config generators have a different way to specify configs
            if not single_config:
                # Config-specific dirs (critical for MSVC multi-config)
                cmake_args += [
                    f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}",
                    f"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}",
                    f"-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}",
                ]
                build_args += ["--config", cfg]

        # Create the build directory
        if not Path(self.build_temp).exists():
            Path(self.build_temp).mkdir(parents=True)

        # Run CMake
        subprocess.check_call(["cmake", ext.sourcedir, *cmake_args], cwd=self.build_temp)
        # Run the build
        subprocess.check_call(
            ["cmake", "--build", ".", "--target", "bitbully_core", *build_args],
            cwd=self.build_temp,
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
    # version="0.0.58",  # already defined in the pyproject.toml
    # packages=["bitbully"],
    ext_modules=[CMakeExtension("bitbully.bitbully_core")],
    cmdclass={"build_ext": CMakeBuildExtension},
    zip_safe=False,
)
