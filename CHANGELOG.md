## v0.0.55-a0 (2025-09-20)

### Fix

- updated cibuildwheel to 3.1.4 in github workflow
- remove pypy build for old python versions 3.8 and 3.9
- added copy function and tests to board class

## v0.0.54 (2025-09-17)

### Fix

- added some more tests for opening books
- changed test to work without numpy

## v0.0.54-a6 (2025-09-16)

### Fix

- added tests and removed irrelevant bindings

## v0.0.54-a5 (2025-09-15)

### Fix

- wheels workflow
- wheels workflow

## v0.0.54-a4 (2025-09-15)

### Fix

- cmake multi platform workflow

## v0.0.54-a3 (2025-09-15)

### Fix

- cmake multi platform workflow

## v0.0.54-a2 (2025-09-15)

### Fix

- added few tests

## v0.0.54-a1 (2025-09-15)

### Fix

- do not build pre-releases for macos (expensive)
- added pydoclint
- added some docs on mkdocs
- added badge to notebooks

## v0.0.53 (2025-09-14)

### Fix

- updated several dependencies

## v0.0.52 (2025-09-14)

### Fix

- remove numpy from tests due to slow install on certain runners

## v0.0.51 (2025-09-14)

### Fix

- loosen version requirement for numpy in cibuildwheel tests

## v0.0.50 (2025-09-14)

### Fix

- add accidentally removed win32-skips again

## v0.0.49 (2025-09-14)

### Fix

- pinned numpy version for cibuildwheel tests

## v0.0.54-a0 (2025-09-15)

### Fix

- do not build pre-releases for macos (expensive)
- added pydoclint
- added some docs on mkdocs
- added badge to notebooks

## v0.0.53 (2025-09-14)

### Fix

- updated several dependencies

## v0.0.52 (2025-09-14)

### Fix

- remove numpy from tests due to slow install on certain runners

## v0.0.51 (2025-09-14)

### Fix

- loosen version requirement for numpy in cibuildwheel tests

## v0.0.50 (2025-09-14)

### Fix

- add accidentally removed win32-skips again

## v0.0.49 (2025-09-14)

### Fix

- pinned numpy version for cibuildwheel tests

## v0.0.49-a0 (2025-09-14)

## v0.0.48 (2025-09-14)

### Fix

- change github workflow for pre-releases

## v0.0.47 (2025-09-14)

## v0.0.47-a4 (2025-09-13)

### Fix

- ruff fixes

## v0.0.47-a3 (2025-09-13)

### Fix

- ruff fixes

## v0.0.47-a2 (2025-09-13)

### Fix

- add mssing tests

## v0.0.47-a1 (2025-09-13)

### Fix

- allow fast builds with prereleases 2

## v0.0.47-a0 (2025-09-13)

### Fix

- allow fast builds with prereleases

## v0.0.46 (2025-09-13)

### Fix

- remove support for python3.8 and 3.9
- remove builds for python versions which reached eol

## v0.0.45 (2025-08-13)

### Fix

- make sure bitbully_core is created and added to wheel under windows

## v0.0.44 (2025-08-08)

### Fix

- addressed license issue

## v0.0.43 (2025-08-08)

### Fix

- addressed chrono issue under windows builds

## v0.0.42 (2025-08-08)

### Fix

- addressed chrono issue under windows builds

## v0.0.41 (2025-08-07)

### Fix

- cleanup setup.py for ruff and mypy

## v0.0.40 (2025-08-04)

### Fix

- smaller changes in docs
- added two jupyter notebooks
- clean-up c4 widget

## v0.0.39 (2025-02-17)

### Fix

- run full build pipeline

## v0.0.38 (2025-02-17)

### Fix

- run full build pipeline

## v0.0.37 (2025-02-17)

### Fix

- simple build for pypi

## v0.0.36 (2025-02-17)

### Fix

- simple build for pypi

## v0.0.35 (2025-02-16)

### Fix

- increase build time for cibuildwheels

## v0.0.34 (2025-02-16)

### Fix

- for the first time push to main pypi
- added mkdocs support

## v0.0.33 (2025-02-16)

### Fix

- command line tool for runtime comparisons
- added several cmd line args for main

## v0.0.32 (2025-02-11)

### Fix

- remove fierz' legacy code
- remove legacy code
- add macos-13 to cmake github workflow
- add macos-13 to cmake github workflow
- adapted min macosx deployment target
- increase timeout for build wheels
- adapted min macosx deployment target

## v0.0.31 (2025-02-02)

### Fix

- full cibuildwheel activated again

## v0.0.30 (2025-02-02)

### Fix

- added missing includes

## v0.0.29 (2025-02-02)

### Fix

- add missing databases

## v0.0.28 (2025-02-02)

### Fix

- issues with python bindings in bitbully_core
- various issues with the 8-ply and 12-ply databases
- let gui use opening book

## v0.0.27 (2025-01-28)

### Fix

- always delete artifacts, even if no tag is pushed

## v0.0.26 (2025-01-28)

### Fix

- handle edge cases in opening book access
- added filesystem header for pybind11

## v0.0.25 (2025-01-27)

### Fix

- exposed a few more functions to pybind11

## v0.0.24 (2025-01-27)

### Fix

- added opening book to bitbully agent
- more pedantic compile warnings

## v0.0.23 (2025-01-26)

### Fix

- added some tests for the opening books

## v0.0.22 (2025-01-25)

### Fix

- add fundamental opening book support

## v0.0.21 (2025-01-22)

### Fix

- added scoreMoves function

## v0.0.20 (2025-01-19)

### Fix

- corrected win detection in jupyter gui

## v0.0.19 (2025-01-19)

### Fix

- improve the readme
- add stl support to pybind module
- added haswin bindings and a new setboard function

## v0.0.18 (2025-01-18)

### Fix

- added c4 gui for notebooks
- cleanup in board class

## v0.0.17 (2025-01-15)

### Fix

- minimal build for now
- added new copy button to doxy config
- adapted logo
- added a github badge
- add logo
- added more modern doxygen theme
- fixed cmake build in github workflow
- add doxygen support
- updated connect4 formatting
- removed snake casing from google test dir
- avoid moving under own threats

### Refactor

- renamed example to bitbully_core

## v0.0.16 (2025-01-01)

### Fix

- updated version.h
- fixed update_version script

## v0.0.15 (2025-01-01)

### Fix

- add bindings for bitbully to example

### Refactor

- moved cpp src files

### Perf

- optimzed some code

## v0.0.14 (2024-12-30)

### Fix

- added include-guard
- corrected messy includes

## v0.0.13 (2024-12-30)

### Fix

- minor cleanups
- manually bumped version in setup.py

## v0.0.12 (2024-12-30)

### Fix

- add example file

## v0.0.11 (2024-12-27)

### Fix

- adjusted changelog

## v0.0.10 (2024-12-27)

### Fix

- added pyproject.toml to commitizen version_files

## v0.0.9 (2024-12-27)

### Fix

- fixed some commitizen options
- test version for pypi

## v0.0.8 (2024-12-27)

### Fix

- test for clang compiler fixed
- test for clang compiler fixed
- fix version in script
- fix version in script
- fixed test
- added some config
- added some config
- added a few config files for commitizen
- cleaned up test
- fixed bug getMask function
- added several tests
- ensure that tests pass for now
- add macos support to workflow
- add macos support to workflow
- test binaries all in same folder across op systems
- if-condition for running google tests
- move into google_test directory
- move into google_test directory
- try different path for gtest
- corrected path for binary in test stage
- figure out working directory
- renamed target in github pipeline
- some fixes of the solver
- try to remove gtest_force_shared_crt flag
- try to remove gtest_force_shared_crt flag
- try to remove gtest_force_shared_crt flag
- adjusted cmake files for msvc
- adjusted cmake files for msvc
- adjusted cmake files for msvc
- adjusted cmake files for msvc
- adjusted cmake files for msvc
- adjusted cmake files for msvc
- adjusted cmake files for msvc
- added missing include in third-party lib
