list(APPEND PAL_SOURCES
    crypto/qt/CryptoDigestQt.cpp

    system/ClockGeneric.cpp

    system/qt/SoundQt.cpp

    text/KillRing.cpp
)

if (USE_GCRYPT)
    list(APPEND PAL_PUBLIC_HEADERS
        crypto/gcrypt/Handle.h
        crypto/gcrypt/Initialization.h
        crypto/gcrypt/Utilities.h

        crypto/tasn1/Utilities.h
    )
    list(APPEND PAL_SOURCES
        crypto/tasn1/Utilities.cpp
    )
    list(APPEND PAL_LIBRARIES
        ${LIBGCRYPT_LIBRARIES}
        ${LIBTASN1_LIBRARIES}
    )
    list(APPEND PAL_SYSTEM_INCLUDE_DIRECTORIES
        ${LIBGCRYPT_INCLUDE_DIRS}
        ${LIBTASN1_INCLUDE_DIRS}
    )
endif ()

if (APPLE AND ENABLE_WEB_CRYPTO)
    list(APPEND PAL_PUBLIC_HEADERS
        spi/cocoa/CommonCryptoSPI.h
    )
endif ()
