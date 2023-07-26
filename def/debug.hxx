#ifndef QMC_H_DEBUG
#   define QMC_H_DEBUG

#   define __QMC_ASSERT__(exp)
#   define __QMC_ASSERT_NULLPTR__(ptr)

#   if defined(__QMC_DEBUG__)
#       include "debug_section.hxx"
#   endif // defined(__QMC_DEBUG__)

#endif // QMC_H_DEBUG