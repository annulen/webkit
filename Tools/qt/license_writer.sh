#!/bin/bash -x
if [[ "${TOOLCHAIN}" =~ "win64_mingw" ]]; then
        SUBDIR="${TOOLCHAIN/win64_/}_64"
    elif [[ "${TOOLCHAIN}" =~ "win32_mingw" ]]; then
        SUBDIR="${TOOLCHAIN/win32_/}_32"
    elif [[ "${TOOLCHAIN}" =~ "win64_msvc" ]]; then
        SUBDIR="${TOOLCHAIN/win64_/}"
    elif [[ "${TOOLCHAIN}" =~ "win32_msvc" ]]; then
        SUBDIR="${TOOLCHAIN/win32_/}"
    else
        SUBDIR="${TOOLCHAIN}"
    fi

CONF_FILE="${QTDIR}/bin/qt.conf"
echo "[Paths]" > ${CONF_FILE}
echo "Prefix = .." >> ${CONF_FILE}

# Adjust the license to be able to run qmake
# sed with -i requires intermediate file on Mac OS
PRI_FILE="${QTDIR}/mkspecs/qconfig.pri"
sed -i.bak 's/Enterprise/OpenSource/g' "${PRI_FILE}"
sed -i.bak 's/licheck.*//g' "${PRI_FILE}"
rm "${PRI_FILE}.bak"

# Print the directory so that the caller can
# adjust the PATH variable.
echo $(dirname "${CONF_FILE}")
