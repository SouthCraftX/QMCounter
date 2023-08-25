struct qmc_errdesc
{
    int id;
    void* further_info;
};

#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>

struct qmc_exception_context
{
    bool stat;
    struct qmc_errdesc error_desc;
    jmp_buf jmp_context;
} ;

#       define __QMC_TRY__(__exc , __exp) (__exc.stat = setjmp(__exc.jmp_context) , __exp)
#       define __QMC_CATCH__(__exc) if(__exc.stat)
#       define __QMC_THROW__(__exc , __errdesc) (__exc.error_desc = __errdesc , longjmp(__exc.jmp_context , true))
#       define __QMC_PTHROW__(__pexc , __errdesc) (__pexc->error_desc = __errdesc , longjmp(__pexc->jmp_context , true))

void do_throw(struct qmc_exception_context* p_ctx)
{
    struct qmc_errdesc ed = {5 , 0};
    __QMC_PTHROW__(p_ctx,ed);
}

int main()
{
    struct qmc_exception_context ctx;
    __QMC_TRY__(ctx , do_throw(&ctx));
    __QMC_CATCH__(ctx)
    {
        printf("threw:%i,%p",ctx.error_desc.id,ctx.error_desc.further_info);
    }
}