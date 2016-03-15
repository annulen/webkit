#if defined(HAVE_CONFIG_H) && HAVE_CONFIG_H && defined(BUILDING_WITH_CMAKE)
#include "cmakeconfig.h"
#endif

#include <wtf/Assertions.h>
#include <PlatformExportMacros.h>

#undef WEBCORE_EXPORT
#define WEBCORE_EXPORT
