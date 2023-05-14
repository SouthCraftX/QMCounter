#ifndef QMC_H_MOD_THRD
#   define QMC_H_MOD_THRD



#   include "def.hxx"

namespace qmc
{
    namespace mod
    {
        
        class Thread
        {
        private:
            static qmc::uint16_t _mac_thrd_n = 0;
        public:
            Thread(/* args */);
            ~Thread();
            qmc::uint16_t mac_thrd_n() { return _mac_thrd_n; }
        };
        
        Thread::Thread(/* args */)
        {
            if(!_mac_thrd_n)
            
        }
        
        Thread::~Thread()
        {
        }
        

    } // namespace mod
    
} // namespace qmc


#endif // QMC_H_MOD_THRD