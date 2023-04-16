#ifndef QMC_H_MACROS
#   define QMC_H_MACROS

#   include "types.hxx"

namespace qmc
{
    enum midi_fmt
    {
        single_trk,
        muilt_trk,
        smpte_time
    };
    
    constexpr const size_t expected_header_size = 6;
} // namespace qmc


#endif // QMC_H_MACROS