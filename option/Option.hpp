#ifndef _RUSTISH_OPTION_OPTION_HPP_
#define _RUSTISH_OPTION_OPTION_HPP_

#include "OptionStorage.hpp"

#include <exception>
#include <iostream>

namespace rustish {
namespace option {
struct None {};

template <typename T> struct ReturnDefault {
    static inline T &&pass(T &def) { return std::move(def); }
};

template <typename T> struct ReturnDefault<T &> {
    static inline T &pass(T &def) { return def; }
};

template <typename T> struct ReturnDefault<const T &> {
    static inline const T &pass(const T &def) { return def; }
};

template <typename T> class Option {
  public:
    using Storage = OptionStorage<T>;
    using opt_t = T;
    using ref_t = typename Storage::ref_t;
    using cref_t = typename Storage::cref_t;
    using ret_t = typename Storage::ret_t;
    using param_t = typename Storage::param_t;

    Option() {}
    Option(None) {}

    template <typename U, typename V = typename std::enable_if<
                              IsSameDecayType<T, U>::value, void>::type>
    Option(U &&value) : m_storage(std::forward<U>(value)) {}

    Option(const Option &) = default;
    Option &operator=(const Option &) = default;
    Option(Option &&) = default;
    Option &operator=(Option &&) = default;

    bool is_some() const { return m_storage.is_some(); }

    template <typename Func> bool is_some_and(Func &&f) {
        if (is_some())
            return f(m_storage.get());
        return false;
    }

    bool is_none() const { return m_storage.is_none(); }

    Option<cref_t> as_ref() const {
        if (is_some())
            return Option<cref_t>(m_storage.cref());
        return {};
    }

    Option<T &> as_mut() & {
        if (is_some())
            return Option<T &>(m_storage.ref());
        return {};
    }

    // TODO: In later versions of C++ you can use string_view
    ret_t expect(const char *msg) {
        if (is_some())
            return m_storage.get();

        std::cerr << msg << std::endl;
        std::terminate();
    }

    ret_t unwrap() {
        if (is_some())
            return m_storage.get();
        std::cerr << "unwrap() called on Option with None value" << std::endl;
        std::terminate();
    }

    template <typename U, typename V = typename std::enable_if<
                              IsSameDecayType<T, U>::value, void>::type>
    ret_t unwrap_or(U &&def) {
        if (is_some())
            return m_storage.get();
        return std::forward<U>(def);
    }

    template <typename Func> ret_t unwrap_or_else(Func &&f) {
        if (is_some())
            return m_storage.get();
        return f();
    }

    T unwrap_or_default() {
        static_assert(
            !std::is_reference<opt_t>::value,
            "Option::unwrap_or_default() is not available for reference types");
        if (is_some())
            return m_storage.get();
        return T();
    }

    ret_t unwrap_unchecked() { return m_storage.get(); }

    template <typename Func,
              typename U = typename std::result_of<Func(param_t)>::type>
    Option<U> map(Func &&f) {
        if (is_some())
            return Option<U>(f(m_storage.get()));
        return {};
    }

    template <typename Func> Option<T> inspect(Func &&f) {
        if (is_some())
            f(m_storage.cref());
        return std::move(*this);
    }

    template <typename Func,
              typename U = typename std::result_of<Func(param_t)>::type>
    Option<U> map_or(U &&def, Func &&f) {
        if (is_some())
            return Option<U>(f(m_storage.get()));
        return Option<U>(std::forward<U>(def));
    }

    template <typename F, typename D,
              typename U = typename std::result_of<F(param_t)>::type>
    Option<U> map_or_else(D &&def, F &&f) {
        if (is_some())
            return Option<U>(f(m_storage.get()));
        return Option<U>(def());
    }

    template <typename U> Option<U> and_(Option<U> opt) {
        if (is_some())
            return std::move(opt);
        return {};
    }

    template <typename Func,
              typename U = typename std::result_of<Func(param_t)>::type::opt_t>
    Option<U> and_then(Func &&f) {
        if (is_some())
            return f(m_storage.get());
        return {};
    }

    template <typename Pred> Option<T> filter(Pred &&pred) {
        if (is_some() && pred(m_storage.cref()))
            return std::move(*this);
        return {};
    }

    Option<T> or_(Option<T> opt) {
        if (is_some())
            return std::move(*this);
        return std::move(opt);
    }

    template <typename Func> Option<T> or_else(Func &&f) {
        if (is_some())
            return std::move(*this);
        return f();
    }

    Option<T> xor_(Option<T> opt) {
        if (!(is_some() xor opt.is_some()))
            return {};

        if (is_some())
            return std::move(*this);
        else
            return std::move(opt);
    }

    template <typename U, typename V = typename std::enable_if<
                              IsSameDecayType<T, U>::value, void>::type>
    ref_t insert(U &&value) {
        m_storage = Storage(std::forward<U>(value));
        return m_storage.ref();
    }

    template <typename U, typename V = typename std::enable_if<
                              IsSameDecayType<T, U>::value, void>::type>
    ref_t get_or_insert(U &&value) {
        if (is_some())
            return m_storage.ref();
        return insert(std::forward<U>(value));
    }

    ref_t get_or_insert_default() {
        static_assert(!std::is_reference<opt_t>::value,
                      "Option::get_or_insert_default() is not available for "
                      "reference types");
        if (is_some())
            return m_storage.ref();
        m_storage = Storage(T());
        return m_storage.ref();
    }

    template <typename Func> ref_t get_or_insert_with(Func &&f) {
        if (is_some())
            return m_storage.ref();
        m_storage = Storage(f());
        return m_storage.ref();
    }

    Option<T> take() {
        if (is_some())
            return Option<T>(m_storage.get());
        return {};
    }

    template <typename Pred> Option<T> take_if(Pred &&p) {
        if (is_some() && p(m_storage.ref()))
            return Option<T>(m_storage.get());
        return {};
    }

    template <typename U, typename V = typename std::enable_if<
                              IsSameDecayType<T, U>::value, void>::type>
    Option<T> replace(U &&value) {
        Option<T> ret = std::move(*this);
        insert(std::forward<U>(value));
        return ret;
    }

  private:
    OptionStorage<T> m_storage;
};

template <typename T> inline Option<T> Some(T &&val) {
    return Option<T>(std::forward<T>(val));
}

} // namespace option
} // namespace rustish

#endif //_RUSTISH_OPTION_OPTION_HPP_
