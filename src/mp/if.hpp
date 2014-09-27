#ifndef LYZA_MP_IF__
# define LYZA_MP_IF__

namespace lyza { namespace mp {

template <bool Tb, typename T, typename Telse>
struct mp_if {
    typedef Telse type;
};

template <typename T, typename Telse>
struct mp_if<true, T, Telse> {
    typedef T type;
};

}}

#endif
