#!/bin/bash

# https://commitizen-tools.github.io/commitizen/commands/bump/#pre_bump_hooks
# Variable	Description
# CZ_PRE_IS_INITIAL	True when this is the initial release, False otherwise
# CZ_PRE_CURRENT_VERSION	Current version, before the bump
# CZ_PRE_CURRENT_TAG_VERSION	Current version tag, before the bump
# CZ_PRE_NEW_VERSION	New version, after the bump
# CZ_PRE_NEW_TAG_VERSION	New version tag, after the bump
# CZ_PRE_MESSAGE	Commit message of the bump
# CZ_PRE_INCREMENT	Whether this is a MAJOR, MINOR or PATH release
# CZ_PRE_CHANGELOG_FILE_NAME	Path to the changelog file, if available

# Get the new version from Commitizen
NEW_VERSION=$CZ_PRE_NEW_VERSION

# Extract major, minor, and patch versions
IFS='.' read -r MAJOR MINOR PATCH <<< "$NEW_VERSION"

# Update version.h
sed -i.bak \
    -e "s/^#define PROJECT_VERSION \".*\"/#define PROJECT_VERSION \"$NEW_VERSION\"/" \
    -e "s/^#define PROJECT_MAJOR_VERSION .*/#define PROJECT_MAJOR_VERSION $MAJOR/" \
    -e "s/^#define PROJECT_MINOR_VERSION .*/#define PROJECT_MINOR_VERSION $MINOR/" \
    -e "s/^#define PROJECT_PATCH_VERSION .*/#define PROJECT_PATCH_VERSION $PATCH/" \
    src/version.h

# Remove the backup file created by sed (on macOS)
rm -f version.h.bak
