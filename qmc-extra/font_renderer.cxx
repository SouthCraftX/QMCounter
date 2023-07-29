#ifndef QMC_EH_FONTRDER
#   define QMC_EH_FONTRDER

#    define STB_TRUETYPE_IMPLEMENTATION
#    include "stb_truetype.h"

#   include "../base.hxx"
#   include "../backends/prefetched_stream.hxx"
#   include "../backends/fstream.hxx"

namespace qmc
{
    namespace extra
    {
        
        class font_renderer
        {
            private:
                using font_stream = qmc::backend::prefetched_stream<qmc::backend::input_file_stream>;
            
            public:
                qmc::errno_t init_font();
                font_renderer(/* args */);
                ~font_renderer();
        };
        
        font_renderer::font_renderer(/* args */)
        {
        }
        
        font_renderer::~font_renderer()
        {
        }
        
    } // namespace extra
    
} // namespace qmc


#endif // QMC_EH_FONTRDER