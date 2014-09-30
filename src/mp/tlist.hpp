#ifndef LYZA_MP_TLIST__
# define LYZA_MP_TLIST__

# include "if.hpp"
# include "bool.hpp"
# include "arithmetic.hpp"

namespace lyza { namespace mp {

struct null {
};

template <typename Thd, typename Ttl>
struct tlist {
    typedef Thd hd;
    typedef Ttl tl;
};

# ifndef NO_VTEMPLATES
template <typename Thd, typename ...Ttl>
struct make_tlist__ {
    typedef tlist< Thd, typename make_tlist__<Ttl...>::type > type;
};

template <typename ...Ttl>
struct make_tlist__<null, Ttl...> {
    typedef null type;
};

template <typename ...Ttl>
struct make_tlist {
    typedef
        typename make_tlist__<Ttl..., null>::type
        type;
};
#endif

template <typename TL, typename Tmatcher>
struct find {
    typedef
        typename mp_if <
            Tmatcher::template match<typename TL::hd>::value,
            typename TL::hd,
            typename find<typename TL::tl, Tmatcher>::type
        >::type type;
};

template <typename Tunused>
struct find<null, Tunused> {
    typedef null type;
};

template <typename TL, typename Tmatcher>
struct find_count {
    enum {
        value =
            mp_if <
                Tmatcher::template match<typename TL::hd>::value,
                zero,
                add<1, find_count<typename TL::tl, Tmatcher>::value >
            >::type::value
    };
};

template <typename Tunused>
struct find_count<null, Tunused> {
    enum { value = -1 };
};

template <typename TL, typename Tpred>
struct tlist_for_all {
    enum {
        value =
            mp_if <
                Tpred::template pred<typename TL::hd>::value,
                mp_true,
                tlist_for_all<typename TL::tl, Tpred>
            >::type::value
    };
};

template <typename Tunused>
struct tlist_for_all<null, Tunused> {
    enum { value = false };
};

template <typename TL>
struct tlist_length {
    enum { value = 1 + tlist_length<typename TL::tl>::value };
};

template <>
struct tlist_length<null> {
    enum { value = 0 };
};

}}

#endif
