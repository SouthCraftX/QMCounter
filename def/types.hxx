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

    using aint8_t = std::atomic_int8_t;
    using aint16_t = std::atomic_int16_t;
    using aint32_t = std::atomic_int32_t;
    using aint64_t = std::atomic_int64_t;

#   if __cplusplus > 202300L
    using flt32_t = std::float32_t;
    using flt64_t = std::float64_t;
#   else
    using flt32_t = std::float_t;
    using flt64_t = std::double_t;
#   endif

    using auint8_t = std::atomic_uint8_t;
    using auint16_t = std::atomic_uint16_t;
    using auint32_t = std::atomic_uint32_t;
    using auint64_t = std::atomic_uint64_t;

    using flag8_t = int8_t;
    using flag16_t = int16_t;
    using flag32_t = int32_t;

    using byte_t = uint8_t;
    using errno_t = flag32_t;

    using fmode_t = flag8_t;    

    class CounterBase
    {
        public:
            qmc::uint64_t polyphony;
            qmc::uint64_t notes;
            qmc::uint64_t ticks;
            qmc::flt64_t  bpm;
    };
    
    class Counter : public qmc::CounterBase
    {
        public:
            qmc::uint64_t nps;

    };
    
    class GlobalInfo
    {
        public:
            qmc::flag8_t  fmt;
            qmc::uint16_t trk_n;
            qmc::uint16_t ppq;
    };
    

    

} // namespace qmc


#endif // QMC_H_TYPES