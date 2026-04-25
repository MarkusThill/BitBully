/**
 * @file version.h
 * @brief Build-time project version macros.
 *
 * The values defined here are kept in sync with the rest of the build by the
 * commitizen pre-bump script (`scripts/cz_pre_bump_hook.sh`).
 */
#ifndef BITBULLY_VERSION_H
#define BITBULLY_VERSION_H

/// @brief Full semver version string (e.g.\ "0.0.78").
#define PROJECT_VERSION "0.0.78"
/// @brief Major version component as a string.
#define PROJECT_MAJOR_VERSION "0"
/// @brief Minor version component as a string.
#define PROJECT_MINOR_VERSION "0"
/// @brief Patch version component as a string.
#define PROJECT_PATCH_VERSION "78"

#endif  // BITBULLY_VERSION_H
