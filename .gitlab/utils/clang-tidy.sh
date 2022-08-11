#!/bin/bash
pushd "${MESON_SOURCE_ROOT}"

# colors
RED=; GREEN=; YELLOW=; RESET=;
case ${TERM} in
    '') ;;
    'dumb') ;;
    *)
        RED=`tput setaf 1`
        GREEN=`tput setaf 2`
        YELLOW=`tput setaf 3`
        RESET=`tput sgr0`;;
esac

# Print the clang-tidy version for reference
clang-tidy --version

# Exclude headers in these directories
HEADER_FILTER="$1"
if [[ -z "$HEADER_FILTER" ]]; then
    HEADER_FILTER="^((?!subprojects/).)*$"
fi
printf "With header filter: $HEADER_FILTER\n"

# Find correct build directory with compile_commands.json
BUILD_PATH=$2
if [[ -z "$BUILD_PATH" ]]; then
    BUILD_PATH="./build"
fi
printf "Build directory path: $BUILD_PATH\n"
BUILD_DIR=`basename $BUILD_PATH`
printf "Build directory name: $BUILD_DIR\n"
printf "\n"

# Find all .cpp files to check and add them to SOURCES.
# Note: SOURCES should only contain source files (no header files!) as clang-tidy cannot resolve include paths for headers!
SOURCES=`grep -o '[^ "]*\.cpp\(\.o\)*' ${BUILD_DIR}/compile_commands.json | \
            grep -v '.cpp.o' | grep -v 'subprojects' | \
            sed -e "/^\.\./! s/^/\.\.\/${BUILD_DIR}\//" | sed 's/^\.\.\/\(.*\)$/\1/g' | \
            grep -v ${BUILD_DIR} | sort | uniq`

# convert SOURCES to real file paths
FILEPATH_SOURCES=()
while IFS='' read -r line; do
    FILEPATH_SOURCES+=(`realpath "$line"`)
done <<< "$SOURCES"

# actual parallelized calls to clang-tidy
touch "$BUILD_DIR/clang-tidy-output"
if [[ ${#FILEPATH_SOURCES[@]} -gt 0 ]]; then
    parallel -m "echo \"parallel job #{#}\"; \
        for file in {}; do \
            echo \"running for file: \" \`realpath --relative-to=. \"\$file\"\`; \
            clang-tidy \
                --extra-arg=-Wno-unknown-warning-option \
                -p \"$BUILD_DIR\" \
                --header-filter='$HEADER_FILTER' \
                \"\$file\" 2>&1; \
        done" \
        ::: "${FILEPATH_SOURCES[@]}" \
    2>&1 | tee "$BUILD_DIR/clang-tidy-output"
fi

# return to initial directory and print out result
popd

printf "${RED}\n\n"
if grep -e "warning:\|error:" "$BUILD_DIR/clang-tidy-output"; then
    printf "\nclang-tidy check failed!\n\n"
    printf "$RESET"
    exit 1
fi

printf "${GREEN}clang-tidy check successful!\n\n${RESET}"
exit 0
