#ifndef QMC_HF_MDE_ARGPACK
#   define QMC_HF_MDE_ARGPACK

#   include "def.hxx"

namespace qmc
{
    class ArgPack
    {
        private:
            qmc::uint16_t _count;
            void*         _arg;

        public:
            ArgPack(/* args */);
            ~ArgPack();
    };
    
    ArgPack::ArgPack(/* args */)
    {
    }
    
    ArgPack::~ArgPack()
    {
    }
    
} // namespace qmc


#endif // QMC_HF_MDE_ARGPACK