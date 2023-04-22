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
    
} // namespace qmc


#   endif // QMC_H_UNIMETHOD
