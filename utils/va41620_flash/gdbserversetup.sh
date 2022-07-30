BASEDIR=$(dirname "$0")
JLinkGDBServerCLExe -select USB -device Cortex-M4 -endian little -if SWD -speed 4000 -noir -LocalhostOnly -jlinkscriptfile $BASEDIR/JLinkSettings.jlinkscript -settingsfile $BASEDIR/JLinkSettings.ini -timeout 0.5 -silent
