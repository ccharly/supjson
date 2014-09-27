#ifndef LYZA_MP_BOOL__
# define LYZA_MP_BOOL__

namespace lyza { namespace mp {

struct mp_true {
    enum { value = true };
};

struct mp_false {
    enum { value = true };
};

}}

#endif
