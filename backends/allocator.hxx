#ifndef QMC_H_ALLOC
#   define QMC_H_ALLOC

#   include "../base.hxx"

#   if defined(__QMC_WINDOWS__)
#       include <heapapi.h>
#   else
#       include <cstdlib>
#       include <linux/vmalloc.h>
#   endif // defined(__QMC_WINDOWS__)

namespace qmc
{
    namespace backend
    {

#   if defined(POSIX)

        class commit_limiter
        {
            private:
                longsize_t _free_size;

            public:
                _QMC_FORCEINLINE_ [[nodiscard]] bool allocatable(longsize_t desired_alloc_size);
                _QMC_FORCEINLINE_ longsize_t operator-=(longsize_t size);
                _QMC_FORCEINLINE_ longsize_t operator+=(longsize_t size);
                _QMC_FORCEINLINE_ void set_limit(longsize_t size);
                commit_limiter() = default;
                ~commit_limiter() = default;
        };

        _QMC_FORCEINLINE_
        void commit_limiter::set_limit(longsize_t size)
        {
            this->_free_size = size;
        }

        [[nodiscard]]
        bool commit_limiter::allocatable(longsize_t desired_alloc_size)
        {
            return (this->_free_size > desired_alloc_size);
        }

        longsize_t commit_limiter::operator-=(longsize_t size)
        {
            return (this->_free_size -= size);
        }

        longsize_t commit_limiter::operator+=(longsize_t size)
        {
            return (this->_free_size += size);
        }

        longsize_t commit_limiter::operator=(longsize_t size)
        {
            this->_free_size = size;
        }

#endif // defined(POSIX)

        class track_allocator
        {
            private:
#   if defined(__QMC_WINDOWS__)
                ::HANLDE _pool_hde;
#   else
                commit_limiter _limiter;
#   endif // defined(__QMC_WINDOWS__)

            public:
                _QMC_FORCEINLINE_ void init(longsize_t max_size);
                _QMC_FORCEINLINE_ [[nodiscard]] byte_t* alloc(longsize_t size);
                _QMC_FORCEINLINE_ [[nodiscard]] void free(byte_t* ptr , [[maybe_unused]] longsize_t size);
                void destroy();
                track_allocator(longsize_t init_size , longsize_t max_size);
                track_allocator();
                ~track_allocator();
        };

#if defined(__QMC_WINDOWS__)

        _QMC_FORCEINLINE_
        void track_allocator::init(longsize_t max_size)
        {
            this->_pool_hde = ::HeapCreate(0 , 0 , max_size);
            __QMC_ASSERT__(this->_pool_hde);
        }

        _QMC_FORCEINLINE_ [[nodiscard]] 
        byte_t* track_allocator::alloc(longsize_t size)
        {
            return (byte_t *)::HeapAlloc(this->_pool_hde , 0 , size);
        }

        _QMC_FORCEINLINE_ [[nodiscard]] 
        void track_allocator::free(byte_t* ptr , [[maybe_unused]] longsize_t size)
        {
            [[maybe_unused]] ::BOOL ret = ::HeapFree(this->_pool_hde , 0 , ptr);
            __QMC_ASSERT__(ret);
        }

        void track_allocator::destroy()
        {
            [[maybe_unused]] ::BOOL ret = ::HeapDestroy(this->_pool_hde);
            __QMC_ASSERT__(ret);
        }

        using reallocatable_allocator = track_allocator;

#else

        _QMC_FORCEINLINE_
        void track_allocator::init(longsize_t max_size)
        {
            this->_limiter.set_limit(max_size);
        }

        _QMC_FORCEINLINE_
        byte_t* track_allocator::alloc(longsize_t size)
        {
            if (this->_limiter.allocatable(size))
            {
                byte_t* ret_ptr = (byte_t*)::__vmalloc(size, __GFP_HIGH | GFP_NOWAIT);
                if (ret_ptr)
                {
                    this->_limiter -= size;
                }
                return ret_ptr;
            }
            return nullptr;
        }

        _QMC_FORCEINLINE_
        void track_allocator::free(byte_t* ptr , longsize_t size)
        {
            ::vfree_atomic(ptr);
            this->_limiter-=size;
        }

        void track_allocator::destroy()
        {
            // There's nothing to do.
        }

        class reallocatable_allocator
        {
            private:
                commit_limiter _limiter;
                
            public:
                _QMC_FORCEINLINE_ void init(longsize_t max_size);
                _QMC_FORCEINLINE_ void destory();
                _QMC_FORCEINLINE_ [[nodiscard]] byte_t* alloc(longsize_t size);
                _QMC_FORCEINLINE_ [[nodiscard]] 
                byte_t* realloc(byte_t* old_ptr , longsize_t old_size , longsize_t new_size);
                _QMC_FORCEINLINE_ void free(byte_t* ptr , longsize_t size);
                reallocatable_allocator() = default;
                ~reallocatable_allocator() = default;
        };
        
        _QMC_FORCEINLINE_
        void reallocatable_allocator::init(longsize_t max_size)
        {
            this->_limiter.set_limit(max_size);
        }

        _QMC_FORCEINLINE_ [[nodiscard]]
        byte_t* reallocatable_allocator::alloc(longsize_t size)
        {
            if(this->_limiter.allocatable(size))
            {
                byte_t* ret = (byte_t*)std::malloc(size);
                if(ret)
                {
                    this->_limiter-=size;
                }
                return ret;
            }
            return nullptr;
        }

        _QMC_FORCEINLINE_ [[nodiscard]]
        byte_t* reallocatable_allocator::realloc(
            byte_t* old_ptr , longsize_t old_size , longsize_t new_size
        ){
            longsize_t delta_size = new_size - old_size;
            if(this->_limiter.allocatable(delta_size))
            {
                byte_t* ret = (byte_t*)::realloc(old_ptr , new_size);
                if(ret)
                {
                    this->_limiter -= delta_size;
                }
                return ret;
            }
            return nullptr;
        }

        _QMC_FORCEINLINE_ 
        void reallocatable_allocator::free(byte_t* ptr , longsize_t size)
        {
            std::free(ptr);
            this->_limiter -= size;
        }

        _QMC_FORCEINLINE_
        void reallocatable_allocator::destory()
        {
            // There's nothing to do.
        }
    /*
        byte_t *track_allocator::unexpandable_alloc(longsize_t size)
        {
            if (this->_limiter.allocatable(size))
            {
                byte_t *ret_ptr = (byte_t *)::__vmalloc(size, __GFP_HIGH | GFP_NOWAIT);
                if (ret_ptr)
                {
                    this->_limiter -= size;
                }
                return ret_ptr;
            }
            return nullptr;
        }

        byte_t *track_allocator::realloc(byte_t *ptr, longsize_t old_size, longsize_t new_size)
        {
            longsize_t delta_size = new_size - old_size;
            if (this->_limiter.allocatable(delta_size))
            {
                byte_t *ret_ptr = (byte_t *)::realloc(ptr, new_size);
                if (ret_ptr)
                {
                    this->_limiter -= delta_size;
                }
                return ret_ptr;
            }
            return nullptr;
        }
    */


#endif // defined(__QMC_WINDOWS__)

    } // namespace backends

} // namespace qmc

#endif // QMC_H_ALLOC