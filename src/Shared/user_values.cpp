#include <Shared/user_values.hpp>

namespace Essentia
{
    ska::flat_hash_map<FILTERS, GLenum> defaultFilters;

    void init()
    {
        defaultFilters[FILTERS::MIN_F] = GL_LINEAR;
        defaultFilters[FILTERS::MAG_F] = GL_LINEAR;
        defaultFilters[FILTERS::WRAP_S] = GL_REPEAT;
        defaultFilters[FILTERS::WRAP_T] = GL_REPEAT;
    }
}
