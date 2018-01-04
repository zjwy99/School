#ifndef UTIL_H_
#define UTIL_H_

#include <cstdint>
#include <climits>
#include <string>
#include <type_traits>
#include <vector>

#define array_length(x) (sizeof(x) / sizeof(x[0]))
#define string_length(x) (array_length(x) - 1)

typedef uint8_t byte;

namespace util {
  template <typename T, typename A1, typename A2>
  static inline bool vec_equal(const std::vector<T, A1>& a, const std::vector<T, A2> b) {
    if (a.size() != b.size()) {
      return false;
    }

    for (std::size_t i = 0; i < a.size(); ++i) {
      if (a.at(i) != b.at(i)) {
        return false;
      }
    }

    return true;
  }

  // Generic starts_with method
  template <typename C, typename I> 
    static inline bool starts_with(const C& cont, const I& item) {
      return !cont.empty() && cont[0] == item;
    }

  template <typename C, typename I>
    static inline bool ends_with(const C& cont, const I& item) {
      return !cont.empty() && cont[cont.size() - 1] == item;
    }

  template <typename C, typename I>
    static inline bool ends_with_subseq(const C& cont, const I& items) {
      auto ci = cont.rbegin();
      auto ii = items.rbegin();
      for (; ii != items.rend() && ci != cont.rend(); ++ii, ++ci) {
        if (*ii != *ci) {
          return false;
        }
      }

      return ii == items.rend();
    }

  template <typename A, typename B, bool chooseA>
    struct either {
      typedef A type;
    };

  template <typename A, typename B>
    struct either<A, B, false> {
      typedef B type;
    };

  template <typename A, typename B>
    struct largest_type {
      // Circumvention: blah<sizeof(A) > sizeof(B)> looks like
      // blah<sizeof(A)> garbage;
      static const bool aIsBigger = sizeof(A) > sizeof(B);
      typedef typename either<A, B, aIsBigger>::type type;
    };

  template <typename B, typename S, int offset, std::size_t size, std::size_t preshift>
    static inline bool bit_copy(B& into, S value) {
      // *8 gets us size in bytes
      static_assert(sizeof(B) * CHAR_BIT >= offset + size, "Offset + Size out of range.");
      static_assert(sizeof(B) * CHAR_BIT >= size, "Size of field > size of storage type.");
      static_assert(sizeof(S) * CHAR_BIT > offset, "No point in shifting S into oblivion.");

      B fit_value;

      const int shamt = (offset < 0) ? -offset : offset;
      typedef typename largest_type<B, S>::type Temp;

      if (offset < 0) {
        Temp mask = (((Temp)1 << size) - 1) >> shamt;
        fit_value = ((Temp)value >> shamt) & mask;
      } else {
        Temp mask = (((Temp)1 << size) - 1) << shamt;
        fit_value = ((Temp)value << shamt) & mask;
      }

      into |= fit_value;

      // Rudimentary check to see if the value would be truncated.
      if (sizeof(value) > size && offset > 0 && value != 0) {
        Temp mask = (1 << size) - 1;
        S testS = value >> preshift;
        // Workaround: Compiler warns on trying testS >= 0 when testS is unsigned.
        if (std::is_unsigned<S>::value || testS > 0) {
          return testS == (testS & mask);
        } else {
          return !(testS & ~mask);
        }
      }
      return true;
    }

	template <typename B, typename S, int offset, std::size_t size, std::size_t preshift>
    static inline bool bit_convert(B& into, S value) {
      into = 0;
      return bit_copy<B, S, offset, size, preshift>(into, value);
    }

  template <typename B, typename S>
    static inline bool bit_replace(B& into, S value) {
      into = 0;
      into |= *(B*)&value;
      return true;
    }
  
}

#endif // UTIL_H_
