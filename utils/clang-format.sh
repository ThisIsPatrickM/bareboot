#!/bin/bash
pushd "${MESON_SOURCE_ROOT}"

python3 "$1" --recursive "${@:2}"

RETURN_CODE=$?
popd
case $RETURN_CODE in

	0)
	printf "\nclang-format check was successful \n\n"
	exit 0
	;;

	1)
	printf "\nclang-format check failed \n\n"
	exit 1
	;;

	2)
	printf "\nSomething went horribly wrong! \n\n"
	exit 2
	;;
esac
