#ifndef QMC_H
#   define QMC_H

#   include <stdlib.h>
#   include <wchar.h>

// platform
#   if defined(_MSDOS)
#       error "libQMC does not support MS-DOS"
#   elif (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__QMC_WINDOWS__)
#       define __QMC_WINDOWS__
#   endif

// 64-bit or 32-bit
#   if defined(__QMC_WINDOWS__)
#       if defined(_WIN64)
#           define __QMC_64BIT__
#       endif
#   elif defined(POSIX)
#       if (defined(__x86_64__) || defined(__LP64__) || defined(__amd64) || defined(__aarch64__))
#           define __QMC_64BIT__
#       endif
#   endif // defined(__QMC_WINDOWS__)

//endian
#   if defined(__BYTE_ORDER__)
#       if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#           define _QMC_LITTLE_ENDIAN_
#       elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#           define _QMC_BIG_ENDIAN_
#       endif //#ifdef __BYTE_ORDER__
#   elif ( defined(__linux__)  || defined(__OpenBSD__) || defined(__DragonFly__) )
#       include <endian.h>
#       if (__BYTE_ORDER == __LITTLE_ENDIAN)
#           define _QMC_LITTLE_ENDIAN_
#       elif (__BYTE_ORDER == __BIG_ENDIAN)
#           define _QMC_BIG_ENDIAN_
#       endif
#   elif defined(__hppa__) || defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
        (defined(__MIPS__) && defined(__MIPSEB__)) || defined(__ppc__) || defined(__POWERPC__) ||\
        defined(__powerpc__) || defined(__PPC__) || defined(__sparc__)
#       define _QMC_BIG_ENDIAN_
#   endif 

#   if !(defined(_QMC_BIG_ENDIAN_)||defined(_QMC_LITTLE_ENDIAN_))
#       error "libQMC:unsupported endianness!"
#   endif

// Force Inline
#   if defined(__GNUC__)
#       define __QMC_FORCEINLINE__ __attribute__((always_inline))
#   elif defined(_MSC_VER)
#       define __QMC_FORCEINLINE__ __forceinline
#   else
#       define __QMC_FORCEINLINE__ inline
#       if __STDC_VER__ >= 202305L
#           warning "libQMC: inline may fail"
#       endif // __STDC_VER__ >= 202305L
#   endif // defined(__GNUC__)

#   define __QMC_ABSTRACT__ 
#   define __QMC_INTERNAL__ static
#   if defined(__GNUC__)
#       define __QMC_GLOBAL_CONSTRUCTOR__ __attribute__((constructor)) 
#   endif


#   if defined(__cplusplus)
extern "C" {
#   endif // defined(__cplusplus)

#   include <limits.h>

// Define your own qmc_uint8_t to override them
#   if !defined(qmc_uint8_t)
#
#       include <stdint.h>
#       include <stdbool.h>
#       include <math.h>

typedef     uint8_t         qmc_uint8_t;
typedef     int8_t          qmc_int8_t;
typedef     uint16_t        qmc_uint16_t;
typedef     int16_t         qmc_int16_t;
typedef     uint32_t        qmc_uint32_t;
typedef     int32_t         qmc_int32_t;
typedef     uint64_t        qmc_uint64_t;
typedef     int64_t         qmc_int64_t;

typedef     qmc_int8_t      qmc_flag8_t;
typedef     qmc_int16_t     qmc_flag16_t;
typedef     qmc_int32_t     qmc_flag32_t;

typedef     qmc_uint8_t     qmc_byte_t;

typedef     char*           qmc_str_t;
typedef     const char*     qmc_cstr_t;
typedef     wchar_t*        qmc_wstr_t;
typedef     const wchar_t*  qmc_cwstr_t;

typedef     float_t         qmc_fp32_t;
typedef     double_t        qmc_fp64_t;

typedef     qmc_flag32_t    qmc_errno_t;
typedef     qmc_uint32_t    qmc_size_t;
#       if defined(__QMC_64BIT__)
typedef     qmc_uint64_t    qmc_longsize_t;
#       else
typedef     qmc_size_t      qmc_longsize_t; 
#       endif // defined(__QMC_64BIT__)
typedef     qmc_uint16_t    qmc_thrd_count_t;

typedef     qmc_int32_t(*qmc_func_t)(void *);

#   endif // !defined(qmc_uint8_t) 

// Define your own __QMC_ASSERT__ to override them
#   if !defined(__QMC_ASSERT__)
#       if (defined(DEBUG) || defined(_DEBUG)) && !defined(NDEBUG)
#           define __QMC_ASSERT__(source , line)
#       else

#       endif // defined(DEBUG) ...
#   endif // !defined(__QMC_ASSERT__)

enum
{
    QMC_OK = 0,
    QMC_BAD_PATH,
    QMC_PERMISSION_DENIED,
    QMC_OPEN_FAILED,
    QMC_GET_FILE_SIZE_FAILED,
    QMC_BAD_MIDI_HEADER_SIZE,
    QMC_BAD_MIDI_HEADER,
    QMC_BAD_TRACK_HEADER_SIZE,
    QMC_BAD_TRACK_HEADER,
    QMC_BAD_MIDI,
    QMC_OUT_OF_MEM,
    QMC_CREATE_THRD_FAILED,
    QMC_UNKNOWN_ERR
};



// Error Descriptor
struct qmc_errdesc
{
    qmc_errno_t id;
    qmc_str_t further_info;
};

// <===========Backends===========>
// ----Input File Stream----
__QMC_ABSTRACT__
struct qmc_ifile
{
    qmc_longsize_t file_size;
    void* context;   // Could be handle of anything , likes ordinary file, pipe, even file in zip
    qmc_errno_t(*fn_open)(qmc_ifile*,qmc_cstr_t);
    qmc_longsize_t(*fn_read)(void*,qmc_byte_t**,qmc_longsize_t);
    void(*fn_close)(void*);
};

#   define qmc_fopen    qmc_open_ifile
#   define qmc_fread    qmc_read_ifile
#   define qmc_fclose   qmc_close_ifile
#   define qmc_finit    qmc_init_ifile

#   define QMC_DEFINE_IFILE(var_name) (struct qmc_ifile var_name[1] , qmc_finit(var_name)) 

__QMC_FORCEINLINE__
qmc_errno_t qmc_open_ifile(struct qmc_ifile* p_ifile , qmc_cstr_t path)
{
    return (*p_ifile->fn_open)(p_ifile , path);
}

__QMC_FORCEINLINE__
qmc_longsize_t qmc_read_ifile(struct qmc_ifile* p_ifile , qmc_byte_t** p_buf ,qmc_longsize_t size)
{
    return (*p_ifile->fn_read)(p_ifile->context , p_buf , size);
}

__QMC_FORCEINLINE__
void qmc_close_ifile(struct qmc_ifile* p_ifile)
{
    return (*p_ifile->fn_close)(p_ifile->context);
}

#   if !defined(QMC_DISABLE_SYSTEM_FILE_API)
#       if defined(__QMC_WINDOWS__) 
#           include <fileapi.h>         // CreateFile , ReadFile , ...
#           include <handleapi.h>       // CloseHandle
#           include <errhandlingapi.h>  // GetLastError

__QMC_INTERNAL__
qmc_errno_t __qmc_win_open_file(struct qmc_ifile* p_ifile , qmc_cstr_t path)
{
    HANDLE file_handle;

    // Convert char* to wchar_t*
    wchar_t wc_path[MAX_PATH] = { };
    mbstate_t state = mbstate_t();
    mbsrtowcs(wc_path , &path , MAX_PATH , &state );

    file_handle = CreateFile(wc_path , GENERIC_READ , FILE_SHARE_READ , NULL ,
                        OPEN_EXISTING , FILE_FLAG_SEQUENTIAL_SCAN , NULL);

    if(file_handle)
    {
        LARGE_INTEGER file_size;
        if(GetFileSizeEx(file_handle , &file_size))
        {
            p_ifile->context = file_handle;
            p_ifile->file_size = file_size.QuadPart;
            return QMC_OK;
        }
        return QMC_GET_FILE_SIZE_FAILED;
    }

    switch (GetLastError())
    {
        case ERROR_FILE_NOT_FOUND: 
        case ERROR_INVALID_DRIVE:  
        case ERROR_PATH_NOT_FOUND:  return QMC_BAD_PATH;
        case ERROR_ACCESS_DENIED:   return QMC_PERMISSION_DENIED;
    
        default:
            return QMC_OPEN_FAILED;
    }
}

__QMC_INTERNAL__ __QMC_FORCEINLINE__
qmc_size_t __qmc_win_read_file_32(void* handle , qmc_byte_t* buf , qmc_size_t size)
{
    qmc_size_t total_read;
    return ReadFile(handle , buf , size , &total_read , NULL) ? total_read : 0;
}

#           if defined(__QMC_64BIT__)
__QMC_INTERNAL__
qmc_longsize_t __qmc_win_read_file(void* handle , qmc_byte_t** p_buf ,qmc_longsize_t size)
{
    qmc_longsize_t n_read = 0;
    qmc_byte_t* buf = *p_buf;

    while(size > UINT32_MAX)
    {
        n_read += __qmc_win_read_file_32(handle , buf , UINT32_MAX);
        buf += UINT32_MAX , size -= UINT32_MAX;
    }
    n_read += __qmc_win_read_file_32(handle , buf , UINT32_MAX);
    return n_read;
}
#           endif // defined(__QMC_64BIT__)

__QMC_INTERNAL__
void __qmc_win_close_file(void* handle)
{
    CloseHandle(handle);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_init_ifile(struct qmc_ifile* p_qmc_ifile)
{
    p_qmc_ifile->fn_close = __qmc_win_close_file;
    p_qmc_ifile->fn_open  = __qmc_win_open_file;
    p_qmc_ifile->fn_read  = __qmc_win_read_file;
}

#       elif defined(POSIX)
#           include <unistd.h>



#       else
#           error "libQMC:No system macros detected.Have you configured your build environment properly?"
#       endif // defined(__QMC_WINDOWS__)

#   elif !defined(QMC_DISABLE_LIBC_FILE_API)
#       include <stdio.h>
#       include <errno.h>

#       if defined(_MSC_VER)
#           define __FSEEK  _fseeki64_nolock
#           define __FTELL  _ftelli64_nolock
#           define __FREAD  _fread_nolock
#           define __FCLOSE _fclose_nolock
#       else
#           define __FSEEK  fseek
#           define __FTELL  ftell
#           define __FREAD  fread
#           define __FCLOSE fclose
#       endif // defined(_MSC_VER)

__QMC_FORCEINLINE__
void qmc_init_ifile(struct qmc_ifile* p_ifile)
{
    p_ifile->fn_open = __qmc_libc_open_file;
    p_ifile->fn_read = __qmc_libc_read_file;
    p_ifile->fn_close = __FCLOSE; 
}

__QMC_INTERNAL__
qmc_errno_t __qmc_libc_open_file(struct qmc_ifile* p_ifile , qmc_cstr_t path)
{
    FILE* file_handle = fopen(path , "rb");
    if(file_handle)
    {
        if(__FSEEK(file_handle , 0 , SEEK_END))
        {
            __FCLOSE(file_handle);
            return QMC_GET_FILE_SIZE_FAILED;
        }
        p_ifile->file_size = __FTELL(file_handle);
        if(__FSEEK(file_handle , 0 , SEEK_SET)) // Move FPI back
        {
            __FCLOSE(file_handle);
            return QMC_OPEN_FAILED;
        }
        p_ifile->context = file_handle;
        return QMC_OK;
    }
    return QMC_OPEN_FAILED;
}

qmc_longsize_t __qmc_libc_read_file(void* handle , qmc_byte_t** p_buf , qmc_longsize_t size)
{
    return __FREAD((void*)*p_buf , 1 , size , (FILE*)handle);
}

__QMC_FORCEINLINE__
qmc_errno_t qmc_open_ifile(struct qmc_ifile* p_ifile , qmc_cstr_t path)
{
    (*p_ifile->fn_open)(p_ifile , path);
}

__QMC_FORCEINLINE__
qmc_longsize_t qmc_read_ifile(struct void* handle , qmc_byte_t** p_buf , qmc_longsize_t size)
{
    (*p_ifile->fn_read)(handle , p_buf , size);
}

__QMC_FORCEINLINE__
void qmc_close_ifile(void* handle)
{
    (*p_ifile->fn_close)(handle);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_init_ifile(qmc_ifile* p_qmc_ifile)
{
    p_qmc_ifile->fn_close = __FCLOSE;
    p_qmc_ifile->fn_open  = __qmc_libc_open_file;
    p_qmc_ifile->fn_read  = __qmc_libc_read_file;
}

#       undef __FSEEK 
#       undef __FCLOSE
#       undef __FREAD
#       undef __FTELL

#   else
#       if !(defined(qmc_fopen) && defined(qmc_fread) && defined(qmc_fclose) && defined(qmc_finit))
#           error 
#       endif 
#   endif

// ----Compressed File Stream
#   if !defined(QMC_DISABLE_COMPRESSED_FILE_STREAM)

struct lib7z
{

};

struct qmc_compressed_file_context
{
    struct lib7z lib7z_api;

};

#   endif // !defined(QMC_DISABLE_COMPRESSED_FILE_STREAM)

// ----Threading----

__QMC_INTERNAL__ qmc_thrd_count_t __qmc_cpu_thrd_n = 0;


#   if !defined(QMC_DISABLE_SYSTEM_THREADING_API)
#       if defined(__QMC_WINDOWS__)

#           include <sysinfoapi.h>  // GetSystemInfo
#           include <processthreadsapi.h> // CreateThread , ExitThread , ...

#           if defined(__QMC_GLOBAL_CONSTRUCTOR__)
__QMC_GLOBAL_CONSTRUCTOR__ __QMC_INTERNAL__
void qmc_get_cpu_thrd_count_from_system()
{
    SYSTEM_INFO sys_info;
    GetNativeSystemInfo(&sys_info);
    __qmc_cpu_thrd_n = sys_info.dwNumberOfProcessors;
}

qmc_thrd_count_t qmc_get_cpu_thrd_count()
{
    return __qmc_cpu_thrd_n;
}
#           else
__QMC_INTERNAL__
void qmc_get_cpu_thrd_count_from_system()
{
    SYSTEM_INFO sys_info;
    GetNativeSystemInfo(&sys_info);
    __qmc_cpu_thrd_n = sys_info.dwNumberOfProcessors;
}

qmc_thrd_count_t qmc_get_cpu_thrd_count()
{
    if(__qmc_cpu_thrd_n)
        return __qmc_cpu_thrd_n;

    qmc_get_cpu_thrd_count_from_system();
    return __qmc_cpu_thrd_n;
}
#           endif // defined(__QMC_GLOBAL_CONSTRUCTOR__)

struct qmc_thrd
{
    HANDLE thread_handle;
};

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_errno_t qmc_thrd_start(struct qmc_thrd* p_thrd , qmc_func_t func , void* arg)
{
    p_thrd->thread_handle = CreateThread(nullptr , init_thrd_stack_size ,
                                        (LPTHREAD_START_ROUTINE)func , arg , 0 , NULL);
    return (p_thrd->thread_handle ? QMC_OK : QMC_CREATE_THRD_FAILED);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_thrd_exit(qmc_errno_t ret)
{
    ExitThread(ret);
}

#       else
#           include <pthread.h>

struct qmc_thrd
{
    pthread_t thread_handle;
};

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_errno_t qmc_thrd_start(struct qmc_thrd* p_thrd ,qmc_func_t func , void* arg)
{
    return (pthread_create(&p_thrd->thread_handle , NULL , func , arg)) ?  
           QMC_CREATE_THRD_FAILED : QMC_OK;
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_thrd_exit(qmc_errno_t ret)
{
    pthread_exit(ret);
}

#       endif // defined(__QMC_WINDOWS__)
#   elif !define(QMC_DISABLE_LIBC_THREADING_API)
#       include <threads.h>

struct qmc_thrd
{
    thrd_t thread;
};

__QMC_INTERNAL__
qmc_errno_t qmc_thrd_create(struct qmc_thrd* p_thrd , qmc_func_t func , void* arg)
{
    switch(thrd_create(&p_thrd->thread , func , arg ))
    {
        case thrd_success:  return QMC_OK;
        case thrd_nomem:    return QMC_OUT_OF_MEM;
        case thrd_error:    return QMC_CREATE_THRD_FAILED;
    }
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_thrd_exit(qmc_errno_t ret)
{
    thrd_exit(ret);
}



#   elif defind(qmc_thrd) && defined(qmc_thrd_create) && defined(qmc_thrd_stop)

#   else
#       error "libQMC:Threading-related methods are not impled!"
#   endif

struct qmc_thrd_group
{

};

struct qmc_allocator
{

};

// ---- Lock ----
__QMC_ABSTRACT__
struct qmc_lock
{
    void* lock_context;
    void(*fn_init)(void*);
    void(*fn_acquire)(void*);
    void(*fn_release)(void*);
};

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_lock_init(struct qmc_lock* p_lock)
{
    (*p_lock->fn_init)(p_lock->lock_context);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_lock_acquire(struct qmc_lock* p_lock)
{
    (*p_lock->fn_acquire)(p_lock->lock_context);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_lock_release(struct qmc_lock* p_lock)
{
    (*p_lock->fn_release)(p_lock->lock_context);
}

// ----Conditional Lock----
#   if !defined(QMC_DIABLE_SYSTEM_CONDLOCK_API)


struct qmc_condlock
{

};
#   elif defined(qmc_condlock) && defined(qmc_condlock_acquire) && \\
         defined(qmc_condlock_init) && defined(qmc_condlock_release)

#   else

#   endif // !defined(QMC_DIABLE_SYSTEM_CONDLOCK_API)


// Define your own qmc_spinlock , relative methods , 
// QMC_DISABLE_SYSTEM_SPINLOCK_API , QMC_DISABLE_BUILDIN_SPINLOCK to override them
#   if !defined(QMC_DISABLE_SYSTEM_SPINLOCK_API)
#       if defined(__QMC_WINDOWS__)
#           include<wdm.h>

struct qmc_spinlock
{
    KSPIN_LOCK lock;
    KIRQL irql;
};

__QMC_INTERNAL__
void qmc_init_lock_as_spinlock(struct qmc_lock* p_lock)
{

}

void qmc_spinlock_init(struct qmc_spinlock* p_lock)
{
    KeInitializeSpinLock(&p_lock->lock);
}

void qmc_spinlock_acquire(struct qmc_spinlock* p_lock)
{
    KeAcquireSpinLock(&p_lock->lock , &p_lock->irql);
}

void qmc_spinlock_release(struct qmc_spinlock* p_lock)
{
    KeReleaseSpinLock(&p_lock->lock , p_lock->irql);
}
#       elif defined(__linux__)
#           include <linux/spinlock.h>
struct qmc_spinlock
{
    raw_spinlock_t lock;
};

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_spinlock_init(struct qmc_spinlock* p_lock)
{
    p_lock->lock = __RAW_SPIN_LOCK_UNLOCKED(p_lock->lock);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_spinlock_acquire(struct qmc_spinlock* p_lock)
{
    raw_spin_lock(&p_lock->lock);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_spin_lock_release(struct qmc_spinlock* p_lock)
{
    raw_spin_unlock(&p_lock->lock);
}
#       endif // defined(__QMC_WINDOWS__)
#   elif !defined(QMC_DISABLE_BUILDIN_SPINLOCK)



#   elif defined(qmc_spinlock) && defined(qmc_spinlock_init) && \\
         defined(qmc_spinlock_acquire) && defined(qmc_spinlock_release)

#   else
#       error "libQMC:No spinlock defined!"
#   endif



#   if defined(__cplusplus)
}
#   endif // extern "C"

#endif //