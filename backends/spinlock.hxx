#ifndef QMC_H_SPINLOCK
#   define QMC_H_SPINLOCK

#   include "../base.hxx"

#   if defined(__QMC_WINDOWS__)
#       include <wdm.h>
#   else
#       include <linux/spinlock.h>
#   endif // defined(__QMC_WINDOWS__)

namespace qmc
{   
    namespace backend
    {
        class spinlock
        {
            private:
#   if defined(__QMC_WINDOWS__)
                ::KSPIN_LOCK _lock;
                ::KIRQL _irql;
#   else
                DEFINE_RAW_SPINLOCK(_lock);
#   endif
            public:
                void acquire();
                void release();
                spinlock();
                ~spinlock() = default;
        };

#   if defined(__QMC_WINDOWS__)     

        spinlock::spinlock()
        {
            ::KeInitializeSpinLock(&this->_lock);
        }

        void spinlock::acquire()
        {
            KeAcquireSpinLock(&this->_lock , &this->_irql);
        }

        void spinlock::release()
        {
            ::KeReleaseSpinLock(&this->_lock , this->_irql);
        }

#   else

        spinlock::spinlock()
        {
            // There is nothing to do.
        }

        void spinlock::acquire()
        {
            raw_spin_lock(this->_lock);
        }

        void spinlock::release()
        {
            raw_spin_unlock(this->_lock);
        }

#   endif
    } // namespace backend
    
} // namespace qmc


#endif // QMC_H_SPINLOCK