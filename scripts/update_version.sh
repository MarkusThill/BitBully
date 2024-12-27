#!/bin/bash

# Get the new version from Commitizen
NEW_VERSION=$(cat version.txt)

# Extract major, minor, and patch versions
IFS='.' read -r MAJOR MINOR PATCH <<< "$NEW_VERSION"

# Update version.h
sed -i.bak \
    -e "s/^#define PROJECT_VERSION \".*\"/#define PROJECT_VERSION \"$NEW_VERSION\"/" \
    -e "s/^#define PROJECT_MAJOR_VERSION .*/#define PROJECT_MAJOR_VERSION $MAJOR/" \
    -e "s/^#define PROJECT_MINOR_VERSION .*/#define PROJECT_MINOR_VERSION $MINOR/" \
    -e "s/^#define PROJECT_PATCH_VERSION .*/#define PROJECT_PATCH_VERSION $PATCH/" \
    version.h

# Remove the backup file created by sed (on macOS)
rm -f version.h.bak
