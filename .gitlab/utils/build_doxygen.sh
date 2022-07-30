#!/bin/bash
pushd "${MESON_SOURCE_ROOT}"

DOXYFILE=${1}
shift
echo "Doxyfile=${DOXYFILE}"
echo "Modifications:"
printf "%s\n" "$@"
( cat ${DOXYFILE} ; printf "%s\n" "$@" ) | doxygen -

popd
