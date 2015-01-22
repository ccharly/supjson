#ifndef LYZA_JSON_VARIANT__
# define LYZA_JSON_VARIANT__

# include <string>
# include <utility>
# include <iostream>
# include "exceptions.hpp"

namespace lyza { namespace json {

template <typename Tnull, typename Tarray, typename Tobject>
class variant {

    private:
        template <typename T>
        struct type_only {
            typedef
                typename std::remove_const<
                typename std::remove_reference<
                    T
                >::type
                >::type
                type;
        };

    private:
        template <size_t Tn, typename Tdummy = void>
        struct index_to_type {
        };

        template <typename T, typename Tdummy = void>
        struct type_to_index {
        };

        template <typename T, typename Tdummy = void>
        struct at {
        };

# define DEF_INDEX_TO_TYPE(N, T)\
        template <typename Tdummy>\
        struct index_to_type<N, Tdummy> {\
            typedef T type;\
        }
# define DEF_TYPE_TO_INDEX(N, T)\
        template <typename Tdummy>\
        struct type_to_index<T, Tdummy> {\
            enum { value = N };\
        }
# define DEF_TYPEDEF(N)\
        typedef typename index_to_type<N>::type T##N;

# define TYPE_HELPERS(N, T) DEF_INDEX_TO_TYPE(N, T); DEF_TYPE_TO_INDEX(N, T); DEF_TYPEDEF(N);

        TYPE_HELPERS(0, Tnull);
        TYPE_HELPERS(1, Tarray);
        TYPE_HELPERS(2, Tobject);
        TYPE_HELPERS(3, bool);
        TYPE_HELPERS(4, double);
        TYPE_HELPERS(5, std::string);

# define TYPE_TO_INDEX(T) type_to_index< typename type_only<T>::type >::value
# define INDEX_TO_TYPE(N) typename index_to_type<N>::type

        union union_type {
            union_type() {
            }

            template <typename T>
            union_type(T&& v) {
                auto& field = at< typename type_only<T>::type >::index(*this);
                new (&field) T(std::forward<T>(v));
            }

            template <typename T>
            union_type(T const& v) {
                auto& field = at< typename type_only<T>::type >::index(*this);
                new (&field) T(v);
            }

            ~union_type() {
            }

            typename index_to_type<0>::type _0;
            typename index_to_type<1>::type _1;
            typename index_to_type<2>::type _2;
            typename index_to_type<3>::type _3;
            typename index_to_type<4>::type _4;
            typename index_to_type<5>::type _5;
        };

# define DEF_AT_INDEX(N)\
        template <typename Tdummy>\
        struct at<T##N, Tdummy> {\
            static T##N& index(union_type& v) {\
                return v._##N;\
            }\
            static const T##N& index(union_type const& v) {\
                return v._##N;\
            }\
        }
# define AT_INDEX(T, UNION)\
        at< typename type_only<T>::type >::index(UNION)

        DEF_AT_INDEX(0);
        DEF_AT_INDEX(1);
        DEF_AT_INDEX(2);
        DEF_AT_INDEX(3);
        DEF_AT_INDEX(4);
        DEF_AT_INDEX(5);

    public:
# define COPY_CASE(N, OTHER)\
        case N: \
            destroy();\
            new (&unsafe_get<T##N>()) T##N(OTHER.unsafe_get<T##N>());\
            tag_ = OTHER.tag_;\
            break;\

# define FORWARD_CASE(N, OTHER)\
        case N: \
            destroy();\
            new (&unsafe_get<T##N>()) T##N(std::forward<T##N>(OTHER.unsafe_get<T##N>()));\
            tag_ = OTHER.tag_;\
            break;\

# define VISIT_CASE(N, VISITOR)\
        case N: \
            return VISITOR(unsafe_get<T##N>());

# define DESTROY_CASE(N)\
        case N: \
            destroyT(unsafe_get<T##N>());\
            break;

        variant& operator=(variant const& other) {
            switch (other.tag_) {
                COPY_CASE(0, other)
                COPY_CASE(1, other)
                COPY_CASE(2, other)
                COPY_CASE(3, other)
                COPY_CASE(4, other)
                COPY_CASE(5, other)
            }
            return *this;
        }

        variant& operator=(variant&& other) {
            switch (other.tag_) {
                FORWARD_CASE(0, other)
                FORWARD_CASE(1, other)
                FORWARD_CASE(2, other)
                FORWARD_CASE(3, other)
                FORWARD_CASE(4, other)
                FORWARD_CASE(5, other)
            }
            return *this;
        }

        variant(variant const& other) {
            switch (other.tag_) {
                COPY_CASE(0, other)
                COPY_CASE(1, other)
                COPY_CASE(2, other)
                COPY_CASE(3, other)
                COPY_CASE(4, other)
                COPY_CASE(5, other)
            }
        }

        variant(variant&& other) {
            switch (other.tag_) {
                FORWARD_CASE(0, other)
                FORWARD_CASE(1, other)
                FORWARD_CASE(2, other)
                FORWARD_CASE(3, other)
                FORWARD_CASE(4, other)
                FORWARD_CASE(5, other)
            }
        }

        template <typename Tvisitor>
        typename Tvisitor::ret_type
        apply_visitor(Tvisitor const& vst) const {
            switch (tag_) {
                VISIT_CASE(0, vst)
                VISIT_CASE(1, vst)
                VISIT_CASE(2, vst)
                VISIT_CASE(3, vst)
                VISIT_CASE(4, vst)
                VISIT_CASE(5, vst)
            }
            throw bad_visit(tag_);
        }

        void destroy() {
            switch (tag_) {
                DESTROY_CASE(0)
                DESTROY_CASE(1)
                DESTROY_CASE(2)
                DESTROY_CASE(3)
                DESTROY_CASE(4)
                DESTROY_CASE(5)
            }
        }

# define CTOR_AND_OPERATOR(N)\
        variant(T##N const& v) : value_(v), tag_(N) {}\
        variant(T##N&& v) : value_(std::forward<T##N>(v)), tag_(N) {}\
        variant& operator=(T##N const& v) {\
            destroy();\
            tag_ = N;\
            new (&unsafe_get<T##N>()) T##N(v);\
            return *this;\
        }\
        variant& operator=(T##N&& v) {\
            destroy();\
            tag_ = N;\
            new (&unsafe_get<T##N>()) T##N(std::forward<T##N>(v));\
            return *this;\
        }

        CTOR_AND_OPERATOR(0);
        CTOR_AND_OPERATOR(1);
        CTOR_AND_OPERATOR(2);
        CTOR_AND_OPERATOR(3);
        CTOR_AND_OPERATOR(4);
        CTOR_AND_OPERATOR(5);

    private:
        template <typename T>
        static void destroyT(T const& t) {
            t.~T();
        }

    public:
        template <typename T>
        const typename type_only<T>::type&
        unsafe_get() const {
            return AT_INDEX(T, value_);
        }

        template <typename T>
        typename type_only<T>::type&
        unsafe_get() {
            return AT_INDEX(T, value_);
        }

        template <typename T>
        const typename type_only<T>::type&
        get() const {
            if (isa<T>()) {
                return AT_INDEX(T, value_);
            }
            throw bad_cast("");
        }

        template <typename T>
        typename type_only<T>::type&
        get() {
            if (isa<T>()) {
                return AT_INDEX(T, value_);
            }
            throw bad_cast("");
        }

        template <typename T>
        bool isa() const {
            return TYPE_TO_INDEX(T) == tag_;
        }

        size_t get_current_tag() const {
            return tag_;
        }

    private:
        union_type value_;
        size_t tag_;

};

}}

#endif
