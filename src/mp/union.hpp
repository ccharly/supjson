#ifndef LYZA_MP_UNION__
# define LYZA_MP_UNION__

# include "defines.hpp"
# include "tlist.hpp"

namespace lyza { namespace mp {

# ifdef NO_VTEMPLATES
template <typename TL>
struct make_union__ {
    typedef union U {
        U() : hd() {}
        ~U() {}

		typename TL::hd hd;
        typename make_union__<typename TL::tl>::type tl;
    } type;

    typedef typename TL::hd hd;
    typedef make_union__<typename TL::tl> tl;
};

template <>
struct make_union__<mp::null> {
    typedef union {
    } type;

    typedef void hd;
};

template <typename TL>
struct make_union {
    typedef
        make_union__<TL>
        type;
};

# else
template <typename Thd, typename ...Ttl>
struct make_union__ {
    typedef union U {
        U() : hd() {}
        ~U() {}

        Thd hd;
        typename make_union__<Ttl...>::type tl;
    } type;

    typedef Thd hd;
    typedef make_union__<Ttl...> tl;
};

template <typename ...Ttl>
struct make_union__<void, Ttl...> {
    typedef union {
    } type;

    typedef void hd;
};

template <typename ...Ttl>
struct make_union {
    typedef
        make_union__<Ttl..., void>
        type;
};
# endif

template <typename Tunion, int Tn>
struct union_nth {
    typedef
        typename union_nth<typename Tunion::tl, Tn - 1>::type
        type;

    static type& get(typename Tunion::type& u)
    {
        return union_nth<typename Tunion::tl, Tn - 1>::get(u.tl);
    }

    static const type& get(const typename Tunion::type& u)
    {
        return union_nth<typename Tunion::tl, Tn - 1>::get(u.tl);
    }
};

template <typename Tunion>
struct union_nth<Tunion, 0> {
    typedef typename Tunion::hd type;

    static type& get(typename Tunion::type& u)
    {
        return u.hd;
    }

    static const type& get(const typename Tunion::type& u)
    {
        return u.hd;
    }
};
/*
*/


}}

#endif
