#ifndef QMC_EXTRA_H_STBTTHELPER
#   define QMC_EXTRA_H_STBTTHELPER

#   include "../base.hxx"

extern "C"
{

// Overriding stbtt's types
#   define stbtt_uint8_t  uint8_t 
#   define stbtt_int8_t   int8_t
#   define stbtt_uint16_t uint16_t
#   define stbtt_int16_t  int16_t
#   define stbtt_uint32_t uint32_t

// Overriding stbtt's allocator
#   define STBTT_malloc(size , class) 

}

#endif // QMC_EXTRA_H_STBTTHELPER