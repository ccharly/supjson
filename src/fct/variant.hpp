#ifndef LYZA_FUNCTIONAL_VARIANT__
# define LYZA_FUNCTIONAL_VARIANT__

# include <stdexcept>
# include <tuple>
# include <typeinfo>
# include <type_traits>
# include <functional>
# include <utility>

# include "defines.hpp"
# include "mp/union.hpp"
# include "mp/tlist.hpp"

namespace lyza { namespace functional {

typedef void dummy;

# ifdef NO_VTEMPLATES
template <typename TL>
# else
template <typename ...Targs>
# endif
class variant {
    template <size_t Tn, typename Tdummy>
    struct deduce_copy {
        static inline void copy(size_t n, variant& a, variant const& b)
        {
            if (Tn == n) {
				typedef typename n_to_type<Tn>::type type;

				type& afield = a.unsafe_get<type>();
				new (&afield) type();
				afield = b.unsafe_get<type>();
				a.tag_ = b.tag_;
            } else {
                deduce_copy<Tn - 1, Tdummy>::copy(n, a, b);
            }
        }

        static inline void copy(size_t n, variant& a, variant&& b)
        {
            if (Tn == n) {
				typedef typename n_to_type<Tn>::type type;

				type& afield = a.unsafe_get<type>();
				new (&afield) type();
				afield = std::move(b.unsafe_get<type>());
				a.tag_ = b.tag_;
            } else {
                deduce_copy<Tn - 1, Tdummy>::copy(n, a, b);
            }
        }
    };

    template <typename Tdummy>
    struct deduce_copy<0, Tdummy> {
        static inline void copy(size_t n, variant& a, variant const& b)
        {
            if (n == 0) {
				typedef typename n_to_type<0>::type type;

				type& afield = a.unsafe_get<type>();
				new (&afield) type();
				afield = b.unsafe_get<type>();
				a.tag_ = b.tag_;
            }
        }

        static inline void copy(size_t n, variant& a, variant&& b)
        {
            if (n == 0) {
				typedef typename n_to_type<0>::type type;

				type& afield = a.unsafe_get<type>();
				new (&afield) type();
				afield = std::move(b.unsafe_get<type>());
				a.tag_ = b.tag_;
            }
        }
    };

    public:
        void copy(variant const& v)
        {
            deduce_copy<mp::tlist_length<tlist__>::value - 1, dummy>::copy(v.tag_, *this, v);
        }

        void copy(variant&& v)
        {
            deduce_copy<mp::tlist_length<tlist__>::value - 1, dummy>::copy(v.tag_, *this, std::forward(v));
        }

        template <typename T>
        variant(T&& t)
            : tag_(tag< typename std::remove_reference<T>::type >::value), v_()
        {
			T& field = mp_union<T>::get(v_);
			new (&field) typename std::remove_reference<T>::type();

            field = std::move(t);
        }

        template <typename T>
        variant(T const& t)
            : tag_(tag<T>::value), v_()
        {
			T& field = mp_union<T>::get(v_);
			new (&field) typename std::remove_reference<T>::type();

            field = t;
        }

        variant(variant const& v)
            : tag_(v.tag_), v_()
        {
            copy(v);
        }

        variant(variant&& v)
            : tag_(v.tag_), v_()
        {
            copy(v);
        }

        template <typename T>
        variant& operator=(T&& t)
        {
			T& field = mp_union<T>::get(v_);
			new (&field) typename std::remove_reference<T>::type();

            field = std::move(t);
            tag_ = tag<T>::value;
            return *this;
        }

        template <typename T>
        variant& operator=(T const& t)
        {
			T& field = mp_union<T>::get(v_);
			new (&field) typename std::remove_reference<T>::type();

            field = t;
            tag_ = tag<T>::value;
            return *this;
        }

        variant& operator=(variant&& v)
        {
			copy(v);
            return *this;
        }

        variant& operator=(variant const& v)
        {
			copy(v);
			return *this;
        }

        template <typename T>
        bool isa() const
        {
            return tag<T>::value == tag_;
        }

        template <typename Tret>
        static Tret otherwise(variant& v)
        {
            throw std::runtime_error("type matching failed");
        }

        template <typename T, typename Tret>
        Tret match(
                std::function<Tret (T&)> const& f,
                std::function<Tret (variant&)> const& fo = &variant::otherwise<Tret>
                )
        {
            if (isa<T>())
                return f(unsafe_get<T>());
            return fo(*this);
        }

        template <typename T>
        void match(
                std::function<void (T&)> const& f,
                std::function<void (variant&)> const& fo = &variant::otherwise<void>
                )
        {
            if (isa<T>())
                f(unsafe_get<T>());
            else
                fo(*this);
        }

        size_t get_current_tag() const
        {
            return tag_;
        }

    public:
# ifdef NO_VTEMPLATES
        typedef TL tlist__;

        typedef
            typename mp::make_union<TL>::type
            union__;
# else
        typedef
            typename mp::make_tlist<Targs...>::type
            tlist__;

        typedef
            typename mp::make_union<Targs...>::type
            union__;
# endif

        template <typename T>
        struct type_matcher {
            template <typename U>
            struct match {
                enum { value = std::is_same<T, U>::value };
            };
        };

        template <size_t Tn>
        struct n_to_type {
            typedef
                typename lyza::mp::union_nth<union__, Tn>::type
                type;
        };

    private:
        template <size_t Tn, typename Tvisitor>
        struct deduce_visit {
            static inline
				typename Tvisitor::ret_type
				visit(size_t n, Tvisitor const& vt, variant& v)
            {
                if (Tn == n) {
                    return vt(v.unsafe_get<typename n_to_type<Tn>::type>());
                }
				return deduce_visit<Tn - 1, Tvisitor>::visit(n, vt, v);
            }

            static inline
				typename Tvisitor::ret_type
				visit(size_t n, Tvisitor const& vt, variant const& v)
            {
                if (Tn == n) {
                    return vt(v.unsafe_get<typename n_to_type<Tn>::type>());
                }
				return deduce_visit<Tn - 1, Tvisitor>::visit(n, vt, v);
            }
        };

        template <typename Tvisitor>
        struct deduce_visit<0, Tvisitor> {
            static inline
				typename Tvisitor::ret_type
				visit(size_t n, Tvisitor const& vt, variant& v)
            {
                if (n == 0) {
                    return vt(v.unsafe_get<typename n_to_type<0>::type>());
                }
				throw std::runtime_error("invalid visit");
            }

            static inline
				typename Tvisitor::ret_type
				visit(size_t n, Tvisitor const& vt, variant const& v)
            {
                if (n == 0) {
                    return vt(v.unsafe_get<typename n_to_type<0>::type>());
                }
				throw std::runtime_error("invalid visit");
            }
        };

    public:
        template <typename Tvisitor>
		typename Tvisitor::ret_type
			apply_visitor(Tvisitor const& vt)
        {
            return deduce_visit<mp::tlist_length<tlist__>::value - 1, Tvisitor>::visit(tag_, vt, *this);
        }

        template <typename Tvisitor>
		typename Tvisitor::ret_type
			apply_visitor(Tvisitor const& vt) const
        {
            return deduce_visit<mp::tlist_length<tlist__>::value - 1, Tvisitor>::visit(tag_, vt, *this);
        }

    public:
        template <typename T>
        struct tag {
			// TODO, check if find_count failed
            enum { value = lyza::mp::find_count<tlist__, type_matcher<T> >::value };
        };

    private:
        template <typename T>
        struct mp_union
        : public
          lyza::mp::union_nth<
                union__,
				// TODO, check if find_count failed
                lyza::mp::find_count<tlist__, type_matcher<typename std::remove_reference<T>::type> >::value
          > {};

    public:
        template <typename T>
        const typename mp_union<T>::type&
        unsafe_get() const
        {
            return mp_union<T>::get(v_);
        }

        template <typename T>
        typename mp_union<T>::type&
        unsafe_get()
        {
            return mp_union<T>::get(v_);
        }

        template <typename T>
        const typename mp_union<T>::type&
        get() const
        {
            if (isa<T>())
                return mp_union<T>::get(v_);
            throw std::bad_cast();
        }

        template <typename T>
        typename mp_union<T>::type&
        get()
        {
            if (isa<T>())
                return mp_union<T>::get(v_);
            throw std::bad_cast();
        }

    protected:
        size_t tag_;
        typename union__::type v_;
};

}}


#endif
