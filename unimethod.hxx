#ifndef QMC_H_UNIMETHOD
#   define QMC_H_UNIMETHOD

#   include "def.hxx"

namespace qmc
{

    template<class FlagT>
    constexpr bool has_flag(FlagT x , FlagT flag)
    {
        return x & flag;
    }
    
    // Variable-length quantity --> Unsigned 32-bit interger
    qmc::uint32_t vlq_to_u32(qmc::byte_t* p_vlq)
    {
        qmc::uint32_t n = *p_vlq & 0x7FU;
        while ((*p_vlq)++ & 0x80u)
        {
            (n <<= 7) |= *p_vlq & 0x7FU;
        }
        return n;
    }

} // namespace qmc


#   endif // QMC_H_UNIMETHOD
