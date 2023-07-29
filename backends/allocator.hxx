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

#   if defined(POSIX)

    class commit_limiter
    {
        private:
            qmc::longsize_t _free_size;

        public:
            [[nodiscard]] bool allocatable(qmc::longsize_t desired_alloc_size);
            qmc::longsize_t operator-=(qmc::longsize_t size);
            qmc::longsize_t operator+=(qmc::longsize_t size);
            void set_limit(qmc::longsize_t size);
            commit_limiter() = default;
            ~commit_limiter() = default;
    };
    
    [[nodiscard]]
    bool commit_limiter::allocatable(qmc::longsize_t desired_alloc_size)
    {
        return (this->_free_size > desired_alloc_size);
    }

    qmc::longsize_t commit_limiter::operator-=(qmc::longsize_t size)   
    {
        return (this->_free_size -= size);
    }

    qmc::longsize_t commit_limiter::operator+=(qmc::longsize_t size)   
    {
        return (this->_free_size += size);
    }


#   endif // defined(POSIX)

        class allocator
        {
            private:
#   if defined(__QMC_WINDOWS__) 
                ::HANLDE _pool_hde;   
#   else
                commit_limiter _limiter;    
#   endif // defined(__QMC_WINDOWS__)

            public:
                void init(qmc::longsize_t max_size);

                qmc::byte_t* alloc(qmc::longsize_t size);
                qmc::byte_t* unexpandable_alloc(qmc::longsize_t size);

                qmc::byte_t* realloc(qmc::byte_t* ptr , qmc::longsize_t old_size , qmc::longsize_t new_size);

                void free(qmc::byte_t* ptr , qmc::longsize_t size);
                void unexpandable_free(qmc::byte_t* ptr , qmc::longsize_t size);

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

        qmc::byte_t* allocator::realloc(qmc::byte_t* ptr , qmc::longsize_t old_size , qmc::longsize_t new_size)
        {
            (void)old_size;
            return (qmc::byte_t*)::HeapReAlloc(this->_pool_hde , 0 , ptr , new_size);
        }    

        void allocator::free(qmc::byte_t* ptr , qmc::longsize_t size)
        {
            (void)size;
            ::BOOL ret = ::HeapFree(this->_pool_hde , ptr)
            __QMC_ASSERT__(ret);
            (void)ret;
        }

        void allocator::unexpandable_free(qmc::byte_t* ptr , qmc::longsize_t size)
        {
            (void)size;
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
            this->_limiter.set_limit(max_size);
        }

        qmc::byte_t* allocator::alloc(qmc::longsize_t size)
        {
            if(this->_limiter.allocatable(size))
            {   
                qmc::byte_t* ret_ptr = (qmc::byte_t*)::malloc(size);
                if(ret_ptr)
                {
                    this->_limiter -= size;
                }
                return ret_ptr;
            }
            return nullptr;
        }  

        qmc::byte_t* allocator::unexpandable_alloc(qmc::longsize_t size)
        {
            if(this->_limiter.allocatable(size))
            {
                qmc::byte_t* ret_ptr = (qmc::byte_t*)::__vmalloc(size , __GFP_HIGH | GFP_NOWAIT );
                if(ret_ptr)
                {
                    this->_limiter -= size;
                }
                return ret_ptr;
            }
            return nullptr;
        }

        qmc::byte_t* allocator::realloc(qmc::byte_t* ptr , qmc::longsize_t old_size , qmc::longsize_t new_size)
        {
            qmc::longsize_t delta_size = new_size - old_size;
            if(this->_limiter.allocatable(delta_size))
            {
                qmc::byte_t* ret_ptr = (qmc::byte_t*)::realloc(ptr , new_size);
                if(ret_ptr)
                {
                    this->_limiter -= delta_size;
                }
                return ret_ptr; 
            }
            return nullptr;
        }

        void allocator::unexpandable_free(qmc::byte_t* ptr , qmc::longsize_t size)
        {   
            ::free(ptr);
            this->_limiter += size;
        }

        void allocator::unexpandable_free(qmc::byte_t* ptr , qmc::longsize_t size)
        {
            ::vfree(ptr);
            this->_limiter += size;
        }

        void allocator::destory()
        {
            // There's nothing to do.
        }
#   endif // defined(__QMC_WINDOWS__)

        
        
        

    } // namespace backends
    
} // namespace qmc


#endif // QMC_H_GBPOOL