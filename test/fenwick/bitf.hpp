#ifndef __TEST_FENWICK_BIT_HPP__
#define __TEST_FENWICK_BIT_HPP__

#include "../utils.hpp"

extern std::uint64_t inc1[];
extern std::uint64_t inc2[];

TEST(bit_fenwick_tree, increments_by_one)
{
    // height  0        1       0         2       0        1       0          3       0        1       0         2       0        1       0
    // node   15       14      13        12      11       10       9          8       7        6       5         4       3        2       1
    //   0000001 00000010 0000001 000000100 0000001 00000010 0000001 0000001000 0000001 00000010 0000001 000000100 0000001 00000010 0000001
    BitF_Test t(inc1, 15);

    std::string tree_str = "000000100000010000000100000010000000010000001000000010000001000000000100000010000000100000010000000010000001000000010";
    EXPECT_EQ(tree_str, tree_tostring(t.Tree.get(), tree_str.length()));

    std::uint64_t seq1[15];
    increments_to_sequence(inc1, seq1, 15);

    // prefix
    for (size_t i = 0; i < 15; i++)
        EXPECT_EQ(seq1[i], t.prefix(i+1)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 15; i++)
        EXPECT_EQ(i+1, t.find(seq1[i])) << "at index " << i;
    for (std::uint64_t i = seq1[14]; i < 100; i++)
        EXPECT_EQ(15, t.find(i)) << "at index " << i;

    // find complement
    for (std::uint64_t i = 0; i < 63; i++)
        EXPECT_EQ(0, t.compFind(i)) << "at index " << i;
    for (std::uint64_t k = 1; k < 15; k++) {
        for (std::uint64_t i = 64*k-seq1[k-1]; i < 64*(k+1)-seq1[k]; i++)
            EXPECT_EQ(k, t.compFind(i)) << "at index " << i;
    }

    // add
    for (size_t i = 0; i < 15; i++)
        t.add(i+1, i);

    size_t sum = 0;
    for (size_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(seq1[i]+sum, t.prefix(i+1)) << "at index " << i;
    }

    // find
    sum = 0;
    for (std::uint64_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(i+1, t.find(seq1[i]+sum)) << "at index " << i;
    }
}


TEST(bit_fenwick_tree, increasing_increments)
{
    // height  0        1       0         2       0        1       0          3       0        1       0         2       0        1       0
    // node   15       14      13        12      11       10       9          8       7        6       5         4       3        2       1
    //   0001111 00011011 0001101 000101010 0001011 00010011 0001001 0000100100 0000111 00001011 0000101 000001010 0000011 00000011 0000001
    BitF_Test t(inc2, 15);

    std::string tree_str = "000111100011011000110100010101000010110001001100010010000100100000011100001011000010100000101000000110000001100000010";
    EXPECT_EQ(tree_str, tree_tostring(t.Tree.get(), tree_str.length()));

    std::uint64_t seq2[15];
    increments_to_sequence(inc2, seq2, 15);

    // prefix
    for (size_t i = 0; i < 15; i++)
        EXPECT_EQ(seq2[i], t.prefix(i+1)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 15; i++)
        EXPECT_EQ(i+1, t.find(seq2[i])) << "at index " << i;
    for (std::uint64_t i = seq2[14]; i < 1000; i++)
        EXPECT_EQ(15, t.find(i)) << "at index " << i;

    // find complement
    for (std::uint64_t i = 0; i < 63; i++)
        EXPECT_EQ(0, t.compFind(i)) << "at index " << i;
    for (std::uint64_t k = 1; k < 15; k++) {
        for (std::uint64_t i = 64*k-seq2[k-1]; i < 64*(k+1)-seq2[k]; i++)
            EXPECT_EQ(k, t.compFind(i)) << "at index " << i;
    }

    // add
    for (size_t i = 0; i < 15; i++)
        t.add(i+1, i);

    size_t sum = 0;
    for (size_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(seq2[i]+sum, t.prefix(i+1)) << "at index " << i;
    }

    // find
    sum = 0;
    for (std::uint64_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(i+1, t.find(seq2[i]+sum)) << "at index " << i;
    }
}


TEST(bit_fenwick_tree, non_complete)
{
    // height 2       0        1       0           4       0        1       0         2       0        1       0          3       0        1       0         2       0        1       0
    // node  20      19       18      17          16      15       14      13        12      11       10       9          8       7        6       5         4       3        2       1
    // 00000100 0000001 00000010 0000001 00000010000 0000001 00000010 0000001 000000100 0000001 00000010 0000001 0000001000 0000001 00000010 0000001 000000100 0000001 00000010 0000001
    BitF_Test t(inc3, 20);

    std::string tree_str = "00000100000000100000010000000100000010000000000100000010000000100000010000000010000001000000010000001000000000100000010000000100000010000000010000001000000010";
    EXPECT_EQ(tree_str, tree_tostring(t.Tree.get(), tree_str.length()));

    std::uint64_t seq3[20];
    increments_to_sequence(inc3, seq3, 20);

    // prefix
    for (size_t i = 0; i < 20; i++)
        EXPECT_EQ(seq3[i], t.prefix(i+1)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 20; i++)
        EXPECT_EQ(i+1, t.find(seq3[i])) << "at index " << i;
    for (std::uint64_t i = seq3[19]; i < 1000; i++)
        EXPECT_EQ(20, t.find(i)) << "at index " << i;

    // find complement
    for (std::uint64_t i = 0; i < 63; i++)
        EXPECT_EQ(0, t.compFind(i)) << "at index " << i;
    for (std::uint64_t k = 1; k < 20; k++) {
        for (std::uint64_t i = 64*k-seq3[k-1]; i < 64*(k+1)-seq3[k]; i++)
            EXPECT_EQ(k, t.compFind(i)) << "at index " << i;
    }

    // add
    for (size_t i = 0; i < 20; i++)
        t.add(i+1, i);

    size_t sum = 0;
    for (size_t i = 0; i < 20; i++) {
        sum += i;
        EXPECT_EQ(seq3[i]+sum, t.prefix(i+1)) << "at index " << i;
    }

    // find
    sum = 0;
    for (std::uint64_t i = 0; i < 20; i++) {
        sum += i;
        EXPECT_EQ(i+1, t.find(seq3[i]+sum)) << "at index " << i;
    }
}

#endif // __TEST_FENWICK_BIT_HPP__
