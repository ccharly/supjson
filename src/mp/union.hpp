#ifndef LYZA_MP_UNION__
# define LYZA_MP_UNION__

namespace lyza { namespace mp {

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
