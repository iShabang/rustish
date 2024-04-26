#ifndef _shabang_option_option_storage_h_
#define _shabang_option_option_storage_h_

#include <type_traits>
#include <utility>

namespace option {

template <typename T, typename U> struct IsSameDecayType {
  static constexpr bool value =
      std::is_same<typename std::decay<T>::type,
                   typename std::decay<U>::type>::value;
};

template <typename T> class OptionStorage {
  enum State {
    SOME,
    NONE,
  };

public:
  using ret_t = T;
  using ref_t = T &;
  using cref_t = const T &;
  using param_t = T;

  inline static T *cast(char *buff) { return reinterpret_cast<T *>(buff); }
  inline static const T *cast_const(const char *buff) {
    return reinterpret_cast<const T *>(buff);
  }

  OptionStorage() : m_state(NONE) {}

  template <typename U, typename V = typename std::enable_if<
                            IsSameDecayType<T, U>::value, void>::type>
  OptionStorage(U &&value) : m_state(SOME) {
    new (m_buff) T(std::forward<U>(value));
  }

  OptionStorage(const OptionStorage &other) : m_state(other.m_state) {
    new (m_buff) T(other.cref());
  }

  OptionStorage &operator=(const OptionStorage &other) {
    if (this == &other)
      return *this;

    if (is_some())
      cast(m_buff)->~T();

    m_state = other.m_state;
    if (is_some())
      new (m_buff) T(other.cref());

    return *this;
  }

  OptionStorage(OptionStorage &&other) : m_state(other.m_state) {
    if (is_some())
      new (m_buff) T(other.get());

    other.m_state = NONE;
  }

  OptionStorage &operator=(OptionStorage &&other) {
    if (this == &other)
      return *this;

    if (is_some())
      cast(m_buff)->~T();

    m_state = other.m_state;
    if (is_some())
      new (m_buff) T(other.get());

    other.m_state = NONE;

    return *this;
  }

  ~OptionStorage() {
    if (is_some())
      cast(m_buff)->~T();
  }

  bool is_some() const { return m_state == SOME; }

  bool is_none() const { return m_state == NONE; }

  T &&get() {
    m_state = NONE;
    return std::move(*cast(m_buff));
  }

  T &ref() { return *cast(m_buff); }

  const T &cref() const { return *cast_const(m_buff); }

private:
  State m_state;
  char m_buff[sizeof(T)];
};

template <typename T> class OptionStorage<T &> {
public:
  using ret_t = T &;
  using ref_t = T &;
  using cref_t = const T &;
  using param_t = T &;

  OptionStorage(T &value) : m_ptr(&value) {}

  OptionStorage() : m_ptr(nullptr) {}

  OptionStorage(const OptionStorage &other) = default;
  OptionStorage &operator=(const OptionStorage &other) = default;
  OptionStorage &operator=(OptionStorage &&other) = default;
  OptionStorage(OptionStorage &&other) = default;

  bool is_some() const { return m_ptr != nullptr; }

  bool is_none() const { return m_ptr == nullptr; }

  T &get() {
    T *ret = m_ptr;
    m_ptr = nullptr;
    return *ret;
  }

  T &ref() { return *m_ptr; }

  const T &cref() const { return *m_ptr; }

private:
  T *m_ptr;
};

template <typename T> class OptionStorage<const T &> {
public:
  using ret_t = const T &;
  using ref_t = const T &;
  using cref_t = const T &;
  using param_t = const T &;

  OptionStorage(const T &value) : m_ptr(&value) {}

  OptionStorage() : m_ptr(nullptr) {}

  OptionStorage(const OptionStorage &other) = default;
  OptionStorage &operator=(const OptionStorage &other) = default;
  OptionStorage &operator=(OptionStorage &&other) = default;
  OptionStorage(OptionStorage &&other) = default;

  bool is_some() const { return m_ptr != nullptr; }

  bool is_none() const { return m_ptr == nullptr; }

  const T &get() {
    const T *ret = m_ptr;
    m_ptr = nullptr;
    return *ret;
  }

  const T &ref() const { return *m_ptr; }

  const T &cref() const { return *m_ptr; }

private:
  const T *m_ptr;
};
} // namespace option

#endif //_shabang_option_option_storage_h_
