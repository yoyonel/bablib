#ifndef ITER_RANGE_H_
#define ITER_RANGE_H_

#include "internal/iterbase.hpp"

#include <exception>
#include <type_traits>
#include <iterator>
#include <cassert>

namespace iter {
  namespace impl {
    template <typename T>
    class Range;
  }

  template <typename T>
  constexpr impl::Range<T> range(T) noexcept;
  template <typename T>
  constexpr impl::Range<T> range(T, T) noexcept;
  template <typename T>
  constexpr impl::Range<T> range(T, T, T) noexcept;
}

namespace iter {
  namespace detail {
    template <typename T, bool IsFloat = std::is_floating_point<T>::value>
    class RangeIterData;

    // everything except floats
    template <typename T>
    class RangeIterData<T, false> {
     private:
      T value_{};
      T step_{};

     public:
      constexpr RangeIterData() noexcept = default;
      constexpr RangeIterData(T in_value, T in_step) noexcept
          : value_{in_value},
            step_{in_step} {}

      constexpr T value() const noexcept {
        return this->value_;
      }

      constexpr T step() const noexcept {
        return this->step_;
      }

      void inc() noexcept {
        this->value_ += step_;
      }

      constexpr bool operator==(const RangeIterData& other) const noexcept {
        return this->value_ == other.value_;
      }

      constexpr bool operator!=(const RangeIterData& other) const noexcept {
        return !(*this == other);
      }
    };

    // float data
    template <typename T>
    class RangeIterData<T, true> {
     private:
      T start_{};
      T value_{};
      T step_{};
      std::size_t steps_taken{};

     public:
      constexpr RangeIterData() noexcept = default;
      constexpr RangeIterData(T in_start, T in_step) noexcept
          : start_{in_start},
            value_{in_start},
            step_{in_step} {}

      constexpr T value() const noexcept {
        return this->value_;
      }

      constexpr T step() const noexcept {
        return this->step_;
      }

      void inc() noexcept {
        ++this->steps_taken;
        value_ = this->start_ + (this->step_ * this->steps_taken);
      }

      constexpr bool operator==(const RangeIterData& other) const noexcept {
        // if the difference between the two values is less than the
        // step size, they are considered equal
        return (this->value_ < other.value_ ? other.value_ - this->value_
                                            : this->value_ - other.value_)
               < this->step_;
      }

      constexpr bool operator!=(const RangeIterData& other) const noexcept {
        return !(*this == other);
      }
    };
  }
}

template <typename T>
class iter::impl::Range {
  // see stackoverflow.com/questions/32174186 about why only specializations
  // aren't marked as friend
  template <typename U>
  friend constexpr Range<U> iter::range(U) noexcept;
  template <typename U>
  friend constexpr Range<U> iter::range(U, U) noexcept;
  template <typename U>
  friend constexpr Range<U> iter::range(U, U, U) noexcept;

 private:
  const T start;
  const T stop;
  const T step;

  constexpr Range(T in_stop) noexcept : start{0}, stop{in_stop}, step{1} {}

  constexpr Range(T in_start, T in_stop, T in_step = 1) noexcept
      : start{in_start},
        stop{in_stop},
        step{in_step} {}

 public:
  // the reference type here is T, which doesn't strictly follow all
  // of the rules, but std::vector<bool>::iterator::reference isn't
  // a reference type either, this isn't any worse

  class Iterator : public std::iterator<std::forward_iterator_tag, T,
                       std::ptrdiff_t, T*, T> {
   private:
    iter::detail::RangeIterData<T> data;
    bool is_end;

    // compare unsigned values
    static bool not_equal_to_impl(const Iterator& iter,
        const Iterator& end_iter, std::true_type) noexcept {
      assert(!iter.is_end);
      assert(end_iter.is_end);
      return iter.data.value() < end_iter.data.value();
    }

    // compare signed values
    static bool not_equal_to_impl(const Iterator& iter,
        const Iterator& end_iter, std::false_type) noexcept {
      assert(!iter.is_end);
      assert(end_iter.is_end);
      return !(iter.data.step() > 0
                 && iter.data.value() >= end_iter.data.value())
             && !(iter.data.step() < 0
                    && iter.data.value() <= end_iter.data.value());
    }

    static bool not_equal_to_end(
        const Iterator& lhs, const Iterator& rhs) noexcept {
      if (rhs.is_end) {
        return not_equal_to_impl(lhs, rhs, std::is_unsigned<T>{});
      }
      return not_equal_to_impl(rhs, lhs, std::is_unsigned<T>{});
    }

   public:
    constexpr Iterator() noexcept = default;

    constexpr Iterator(T in_value, T in_step, bool in_is_end) noexcept
        : data(in_value, in_step),
          is_end{in_is_end} {}

    constexpr T operator*() const noexcept {
      return this->data.value();
    }

    constexpr ArrowProxy<T> operator->() const noexcept {
      return {**this};
    }

    Iterator& operator++() noexcept {
      this->data.inc();
      return *this;
    }

    Iterator operator++(int) noexcept {
      auto ret = *this;
      ++*this;
      return ret;
    }

    // This operator would more accurately read as "in bounds"
    // or "incomplete" because exact comparison with the end
    // isn't good enough for the purposes of this Iterator.
    // There are two odd cases that need to be handled
    //
    // 1) The Range is infinite, such as
    // Range (-1, 0, -1) which would go forever down toward
    // infinitely (theoretically).  If this occurs, the Range
    // will instead effectively be empty
    //
    // 2) (stop - start) % step != 0.  For
    // example Range(1, 10, 2).  The iterator will never be
    // exactly equal to the stop value.
    //
    // Another way to think about it is that the "end"
    // iterator represents the range of values that are invalid
    // So, if an iterator is not equal to that, it is valid
    //
    // Two end iterators will compare equal
    //
    // Two non-end iterators will compare by their stored values
    bool operator!=(const Iterator& other) const noexcept {
      if (this->is_end && other.is_end) {
        return false;
      }

      if (!this->is_end && !other.is_end) {
        return this->data != other.data;
      }
      return not_equal_to_end(*this, other);
    }

    bool operator==(const Iterator& other) const noexcept {
      return !(*this != other);
    }
  };

  constexpr Iterator begin() const noexcept {
    return {start, step, false};
  }

  constexpr Iterator end() const noexcept {
    return {stop, step, true};
  }
};

template <typename T>
constexpr iter::impl::Range<T> iter::range(T stop) noexcept {
  return {stop};
}

template <typename T>
constexpr iter::impl::Range<T> iter::range(T start, T stop) noexcept {
  return {start, stop};
}

template <typename T>
constexpr iter::impl::Range<T> iter::range(T start, T stop, T step) noexcept {
  return step == T(0) ? impl::Range<T>{0} : impl::Range<T>{start, stop, step};
}

#endif
