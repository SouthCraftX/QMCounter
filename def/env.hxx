#ifndef QMC_H_DEF_ENV
#   define QMC_H_DEF_ENV

// platform
#   if defined(_MSDOS)
#       error "libQMCounter does not support MS-DOS"
#   elif (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)
#       define __QMC_WINDOWS__
#   endif



#   if defined(__QMC_WINDOWS__)
#       if defined(_WIN64)
#           define __QMC_64BIT__
#       endif
#   elif defined(POSIX)
#       if (defined(__x86_64__) || defined(__LP64__) || defined(__amd64) || defined(__aarch64__))
#           define __QMC_64BIT__
#       endif
#   endif //

//endian
#   if defined(__BYTE_ORDER__)
#       if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#           define _QMC_LITTLE_ENDIAN_
#       elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#           define _QMC_BIG_ENDIAN_
#       endif //#ifdef __BYTE_ORDER__
#   elif (defined(__linux__)  || defined(__OpenBSD__) || defined(__DragonFly__) )
#       include <endian.h>
#       if (__BYTE_ORDER == __LITTLE_ENDIAN)
#           define _QMC_LITTLE_ENDIAN_
#       elif (__BYTE_ORDER == __BIG_ENDIAN)
#           define _QMC_BIG_ENDIAN_
#       endif
#   elif defined(__hppa__) || defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
        (defined(__MIPS__) && defined(__MIPSEB__)) || defined(__ppc__) || defined(__POWERPC__) ||\
        defined(__powerpc__) || defined(__PPC__) || defined(__sparc__)
#       define _QMC_BIG_ENDIAN_
#   endif 

#   if !(defined(_QMC_BIG_ENDIAN_)||defined(_QMC_LITTLE_ENDIAN_))
#       error "libQMCounter:unsupported endianness!"
#   endif

#endif