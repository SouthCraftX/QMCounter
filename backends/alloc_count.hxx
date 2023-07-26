#ifndef QMC_H_MDE_MEMORY
#define QMC_H_MDE_MEMORY

#   if defined(__QMC_WINDOWS__)
#       include <sysinfoapi.h>
#   elif defined(POSIX)
#       include <sys/sysinfo.h>
#   endif // if defined(__WINDOWS__)

#include "def.hxx"

namespace qmc
{
    namespace backend
    {

        class alloc_count
        {
        private:
            qmc::uint64_t _alloc_size;
            qmc::uint64_t _commit_limit;

        public:
            static qmc::errno_t get_free_physical_mem(qmc::uint64_t& free_count);
            bool allocatable(qmc::uint64_t desired_alloc_size);
            void operator+=(qmc::uint64_t x);
            void operator-=(qmc::uint64_t x);
            alloc_count();
            ~alloc_count() = default;
        };

        bool alloc_count::allocatable(qmc::uint64_t desired_alloc_size)
        {
            return ((desired_alloc_size + this->_alloc_size) <= this->_commit_limit);
        }

        void alloc_count::operator+=(qmc::uint64_t x)
        {
            this->_alloc_size += x;
        }

        void alloc_count::operator-=(qmc::uint64_t x)
        {
            this->_alloc_size -= x;
        }

        alloc_count::alloc_count(/* args */)
        {
            this->_alloc_size = 0;
            this->_commit_limit = this->get_free_physical_mem(this->_commit_limit);
        }


#if defined(_WIN32)

        qmc::errno_t alloc_count::get_free_physical_mem(qmc::uint64_t& free_count)
        {
            MEMORYSTATUSEX mem_stat = { .dwLength = sizeof(MEMORYSTATUSEX) } ;
            if(::GlobalMemoryStatusEx(&mem_stat))
            {
                free_count = mem_stat.ullAvailPhys;
                return qmc::err::ok;
            }
            return qmc::err::get_free_mem;
        }

#elif defined(POSIX)

        qmc::errno_t alloc_count::get_free_physical_mem(qmc::uint64_t& free_count)
        {
            struct sysinfo sys_info;
            if(::sysinfo(&sys_info))
            {
                return qmc::err::get_free_mem;
            }
            free_count = sys_info.freeram;
            return qmc::err::ok;
        }

#else

#endif

    template<class AllocatorT>
    class memory
    {
    private:
        /* data */
    public:
        memory(/* args */);
        ~memory();
    };
    
    memory::memory(/* args */)
    {
    }
    
    memory::~memory()
    {
    }
    
#   if defined(__WINDOWS__)

#   elif defined(POSIX)

    class PosixAllocator
    {
    private:
        /* data */
    public:
        void* alloc(qmc::uint64_t size);
        PosixAllocator() = default;
        PosixAllocator() = default;
    };
    
    void* PosixAllocator::alloc(qmc::uint64_t size)
    {
        
    }
    

#   else

#   endif


    } // namespace backend

} // namespace qmc

#endif // QMC_H_MDE_MEMORY