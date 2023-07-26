#ifndef QMC_H_DEBUGSEC
#   define QMC_H_DEBUGSEC

#   include <cstdio>
#   include <cstdlib>

#   include "../base.hxx"

namespace qmc
{
    namespace debug
    {

        [[noreturn]]
        void nullptr_handler(qmc::uint32_t line , qmc::ccstring_t func_name ,
                             qmc::ccstring_t source_name)
        {
            fprintf(stderr , "FATAL ERROR:Nullptr Occurred in line %i, func %s , source %s , " \
                "calling abort() ..." , line , func_name , source_name);
            abort();
        }

        [[noreturn]]
        void asserted_false_handler(qmc::uint32_t line , qmc::ccstring_t func_name ,
                                 qmc::ccstring_t source_name)
        {
            fprintf(stderr , "FATAL ERROR:Asserted `0`(false) in line %i, func %s , source %s , " \
                "calling abort() ..." , line , func_name , source_name);
            abort();      
        }
                                 
    } // namespace debug
    
} // namespace qmc

#   undef  __QMC_ASSERT__
#   define __QMC_ASSERT__(exp) ( \
        if(!exp) \
            qmc::debug::asserted_false_handler(__LINE__ , __func__ , __FILE__) \
    )

#   undef  __QMC_ASSERT_NULLPTR__
#   define __QMC_ASSERT_NULLPTR__(ptr)  ( \
        if(!ptr) \
            qmc::debug::nullptr_handler(__LINE__ , __func__ , __FILE__) \
    )


#endif // QMC_H_DEBUGSEC