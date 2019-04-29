#ifndef __FENWICK_BITF_HPP__
#define __FENWICK_BITF_HPP__
#include <iostream>
#include "fenwick_tree.hpp"

namespace hft::fenwick {

/**
 * class BitF - bit compression and classical node layout.
 * @sequence: sequence of integers.
 * @size: number of elements.
 * @BOUND: maximum value that @sequence can store.
 *
 */
template <size_t BOUND> class BitF : public FenwickTree {
public:
  static constexpr size_t BOUNDSIZE = ceil_log2_plus1(BOUND);
  static constexpr size_t STARTING_OFFSET = 65;
  static constexpr size_t END_PADDING = 56;
  static_assert(BOUNDSIZE >= 1 && BOUNDSIZE <= 55, "Some nodes will span on multiple words");

protected:
  size_t Size;
  DArray<uint8_t> Tree;

public:
  BitF(uint64_t sequence[], size_t size)
      : Size(size), Tree((first_bit_after(size) + END_PADDING + 7) >> 3) {
    for (size_t idx = 1; idx <= size; idx++)
      addToPartialFrequency(idx, sequence[idx - 1]);

    for (size_t m = 2; m <= size; m <<= 1)
      for (size_t idx = m; idx <= size; idx += m)
        addToPartialFrequency(idx, getPartialFrequency(idx - m / 2));
  }

  virtual uint64_t prefix(size_t idx) const {
    uint64_t sum = 0;

    while (idx != 0) {
      sum += getPartialFrequency(idx);
      idx = clear_rho(idx);
    }

    return sum;
  }

  virtual void add(size_t idx, int64_t inc) {
    while (idx <= Size) {
      addToPartialFrequency(idx, inc);
      idx += mask_rho(idx);
    }
  }

  using FenwickTree::find;
  virtual size_t find(uint64_t *val) const {
    size_t node = 0;

    for (size_t m = mask_lambda(Size); m != 0; m >>= 1) {
      if (node + m > Size)
        continue;

      const uint64_t value = getPartialFrequency(node + m);

      if (*val >= value) {
        node += m;
        *val -= value;
      }
    }

    return node;
  }

  using FenwickTree::compFind;
  virtual size_t compFind(uint64_t *val) const {
    size_t node = 0;

    for (size_t m = mask_lambda(Size); m != 0; m >>= 1) {
      if (node + m > Size)
        continue;

      const int height = rho(node + m);
      const uint64_t value = (BOUND << height) - getPartialFrequency(node + m);

      if (*val >= value) {
        node += m;
        *val -= value;
      }
    }

    return node;
  }

  virtual size_t size() const { return Size; }

  virtual size_t bitCount() const {
    return sizeof(BitF<BOUNDSIZE>) * 8 + Tree.bitCount() - sizeof(Tree);
  }

private:
  inline static size_t holes(size_t idx) { return STARTING_OFFSET + (idx >> 14) * 64; }

  inline static size_t first_bit_after(size_t idx) {
    return (BOUNDSIZE + 1) * idx - popcount(idx) + holes(idx);
  }

  inline uint64_t getPartialFrequency(size_t idx) const {
    const uint64_t r = rho(idx);
    const uint64_t mask = (UINT64_C(1) << (BOUNDSIZE + r)) - 1;
    const uint64_t end = (BOUNDSIZE + 1) * idx - popcount(idx) - 1 + holes(idx - 1);
    const uint64_t byte_pos = end / 8 - 7;
    return (*(reinterpret_cast<auint64_t *>(&Tree[0] + byte_pos)) >> ((end - (r + BOUNDSIZE) + 1) - byte_pos * 8)) & mask;
  }

  inline void addToPartialFrequency(size_t idx, uint64_t value) {
    const uint64_t r = rho(idx);
    const uint64_t mask = (UINT64_C(1) << (BOUNDSIZE + r)) - 1;
    const uint64_t end = (BOUNDSIZE + 1) * idx - popcount(idx) - 1 + holes(idx - 1);
    const uint64_t byte_pos = end / 8 - 7;
    (*(reinterpret_cast<auint64_t *>(&Tree[0] + byte_pos)) += value << ((end - (r + BOUNDSIZE) + 1) - byte_pos * 8));
  }

  friend std::ostream &operator<<(std::ostream &os, const BitF<BOUND> &ft) {
    const uint64_t nsize = hton((uint64_t)ft.Size);
    os.write((char *)&nsize, sizeof(uint64_t));

    return os << ft.Tree;
  }

  friend std::istream &operator>>(std::istream &is, BitF<BOUND> &ft) {
    uint64_t nsize;
    is.read((char *)(&nsize), sizeof(uint64_t));
    ft.Size = ntoh(nsize);

    return is >> ft.Tree;
  }
};

} // namespace hft::fenwick

#endif // __FENWICK_BITF_HPP__
