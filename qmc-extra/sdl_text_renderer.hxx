#ifndef QMC_EXTRA_TEXT_RDER
#   define QMC_EXTRA_TEXT_RDER

#   include <SDL2\SDL.h>

#   include "def.hxx"

namespace qmc
{
    class FontInfo
    {
        public:
            qmc::uint16_t  _ptsize;
            ::SDL_Color _color;
            qmc::cstring_t _path;

            FontInfo(qmc::cstring_t path , ::SDL_Color& color , qmc::uint16_t _ptsize);
            ~FontInfo();
    };
    
    FontInfo::FontInfo(qmc::cstring_t path , ::SDL_Color& color , qmc::uint16_t _ptsize)
    {
        
    }
    
    FontInfo::~FontInfo()
    {
    }
    
} // namespace qmc


#endif // QMC_EXTRA_TEXT_RDER