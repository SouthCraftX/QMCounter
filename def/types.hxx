#ifndef QMC_H_TYPES
#   define QMC_H_TYPES

#   include <cstdint>
#   include <atomic>

#   include <cstdlib>

#   if __cplusplus > 202300L
#       include <stdfloat>
#   else
#       include <cmath>
#   endif

namespace qmc
{

    using int8_t = std::int8_t;
    using int16_t = std::int16_t;
    using int32_t = std::int32_t;
    using int64_t = std::int64_t;

    using uint8_t = std::uint8_t;
    using uint16_t = std::uint16_t;
    using uint32_t = std::uint32_t;
    using uint64_t = std::uint64_t;

    using cstring_t = char*;
    using ccstring_t = const char*;

    using am_int8_t = std::atomic_int8_t;
    using am_int16_t = std::atomic_int16_t;
    using am_int32_t = std::atomic_int32_t;
    using am_int64_t = std::atomic_int64_t;

#   if __cplusplus > 202300L
    using flt32_t = std::float32_t;
    using flt64_t = std::float64_t;
#   else
    using flt32_t = std::float_t;
    using flt64_t = std::double_t;
#   endif

    using am_uint8_t = std::atomic_uint8_t;
    using am_uint16_t = std::atomic_uint16_t;
    using am_uint32_t = std::atomic_uint32_t;
    using am_uint64_t = std::atomic_uint64_t;

    class PreCounter
    {
        public:
            qmc::uint64_t polyphony;
            qmc::uint64_t notes;
            qmc::uint64_t ticks;
            qmc::flt64_t  bpm;
    };
    
    class Counter : public qmc::PreCounter
    {
        public:
            qmc::uint64_t nps;

    };
    


} // namespace qmc


#endif // QMC_H_TYPES