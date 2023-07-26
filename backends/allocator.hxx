#ifndef QMC_H_GBPOOL
#   define QMC_H_GBPOOL

#   if defined(__QMC_WINDOWS__)
#       include <heapapi.h>
#   else
#       include <cstdlib>
#       include <linux/vmalloc.h>

#   endif

#   include "../base.hxx"

namespace qmc
{
    namespace backends
    {

        
        class allocator
        {
            private:
#   if defined(__QMC_WINDOWS__) 
                ::HANLDE _pool_hde;   
#   else
                qmc::uint64_t _free_size;    
#   endif // defined(__QMC_WINDOWS__)

            public:
                void init(qmc::longsize_t max_size);

                qmc::byte_t* alloc(qmc::longsize_t size);
                qmc::byte_t* unexpandable_alloc(qmc::longsize_t size);

                qmc::byte_t* realloc(qmc::byte_t* ptr , qmc::longsize_t size);

                void free(qmc::byte_t* ptr);
                void unexpandable_free(qmc::byte_t* ptr);

                void destory();
                allocator(qmc::longsize_t init_size , qmc::longsize_t max_size);
                allocator();
                ~allocator();
        };

#   if defined(__QMC_WINDOWS__)
        void allocator::init(qmc::longsize_t max_size)
        {
            this->_pool_hde = ::HeapCreate(0 , 0 , max_size);
            __QMC_ASSERT__(this->_pool_hde);
        }

        qmc::byte_t* allocator::alloc(qmc::longsize_t size)
        {
            return (qmc::byte_t*)::HeapAlloc(this->_pool_hde , 0 , size);
        }

        qmc::byte_t* allocator::unexpandable_alloc(qmc::longsize_t size)
        {
            return (qmc::byte_t*)::HeapAlloc(this->_pool_hde , 0 , size);
        }

        qmc::byte_t* allocator::realloc(qmc::byte_t* ptr , qmc::longsize_t size)
        {
            return (qmc::byte_t*)::HeapReAlloc(this->_pool_hde , 0 , ptr , size);
        }    

        void allocator::free(qmc::byte_t* ptr)
        {
            ::BOOL ret = ::HeapFree(this->_pool_hde , ptr)
            __QMC_ASSERT__(ret);
            (void)ret;
        }

        void allocator::unexpandable_free(qmc::byte_t* ptr)
        {
            ::BOOL ret = ::HeapFree(this->_pool_hde , ptr)
            __QMC_ASSERT__(ret);
            (void)ret;
        }

        void allocator::destory()
        {
            ::BOOL ret = ::HeapDestory(this->_pool_hde);
            __QMC_ASSERT__(ret);
            (void)ret;
        }

#   else

        void allocator::init(qmc::longsize_t max_size)
        {     
            this->_free_size = max_size;
        }

        qmc::byte_t* allocator::alloc(qmc::longsize_t size)
        {
            return (qmc::byte_t*)::malloc(size);
        }

        qmc::byte_t* allocator::unexpandable_alloc(qmc::longsize_t size)
        {
            return (qmc::byte_t*)::__vmalloc(size , __GFP_HIGH | GFP_NOWAIT);
        }

        void allocator::unexpandable_free(qmc::byte_t* ptr)
        {
            ::free(ptr);
        }

        void allocator::unexpandable_free(qmc::byte_t* ptr)
        {
            ::vfree(ptr);
        }

        void allocator::destory()
        {
            // There's nothing to do.
        }
#   endif // defined(__QMC_WINDOWS__)

        
        
        

    } // namespace backends
    
} // namespace qmc


#endif // QMC_H_GBPOOL