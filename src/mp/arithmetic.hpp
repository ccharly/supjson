#ifndef LYZA_MP_ARITHMETIC__
# define LYZA_MP_ARITHMETIC__

namespace lyza { namespace mp {

struct zero {
    enum { value = 0 };
};

struct one {
    enum { value = 1 };
};

template <int Ta, int Tb>
struct add {
    enum { value = Ta + Tb };
};

}}

#endif
