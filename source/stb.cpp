#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#if !defined(_MSC_VER)
    auto sprintf_s = sprintf;
#endif 
#include <stb/stb_image_write.h>
