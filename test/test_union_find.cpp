#include <union_find.hpp>
#include "testing.hpp"

// =================================================================================================
BOOST_AUTO_TEST_SUITE(interface_test)

// =================================================================================================
BOOST_AUTO_TEST_CASE(zero_sized)
{
    union_find<int> uf(0);

    BOOST_CHECK_THROW(uf.find(-1), std::out_of_range);
    BOOST_CHECK_THROW(uf.find(0), std::out_of_range);
    BOOST_CHECK_THROW(uf.find(1), std::out_of_range);

    BOOST_CHECK_THROW(uf.join(0, 1), std::out_of_range);
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(size_one)
{
    union_find<int> uf(1);

    BOOST_CHECK_THROW(uf.find(-1), std::out_of_range);
    BOOST_CHECK_EQUAL(uf.find(0), 0);
    BOOST_CHECK_THROW(uf.find(1), std::out_of_range);

    BOOST_CHECK(!uf.join(0, 0));
    BOOST_CHECK_THROW(uf.join(0, 1), std::out_of_range);
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(size_two)
{
    union_find<int> uf(2);

    BOOST_CHECK_THROW(uf.find_opt(-1), std::out_of_range);
    BOOST_CHECK_EQUAL(uf.find_opt(0), 0);
    BOOST_CHECK_EQUAL(uf.find_opt(1), 1);
    BOOST_CHECK_THROW(uf.find_opt(2), std::out_of_range);

    BOOST_CHECK(!uf.join(0, 0));
    BOOST_CHECK(uf.join(0, 1));

    BOOST_CHECK_EQUAL(uf.find_opt(0), uf.find_opt(1));
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(size_two_const)
{
    union_find<int> uf(2);
    const auto& cuf = uf;

    BOOST_CHECK_THROW(cuf.find(-1), std::out_of_range);
    BOOST_CHECK_EQUAL(cuf.find(0), 0);
    BOOST_CHECK_EQUAL(cuf.find(1), 1);
    BOOST_CHECK_THROW(cuf.find(2), std::out_of_range);

    BOOST_CHECK(!uf.join(0, 0));
    BOOST_CHECK(uf.join(0, 1));

    BOOST_CHECK_EQUAL(cuf.find(0), cuf.find(1));
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(count_disjoint)
{
    union_find<int> uf(32);

    BOOST_CHECK_EQUAL(uf.count_disjoint(), uf.size());

    for (int i = 0; i < 32 - 8; ++i)
    {
        BOOST_CHECK_EQUAL(uf.count_disjoint(), uf.size() - i);
        BOOST_CHECK(uf.join(i, i + 8));
    }

    BOOST_CHECK_EQUAL(uf.count_disjoint(), 8);
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(count_singleton)
{
    union_find<int> uf(32);

    BOOST_CHECK_EQUAL(uf.count_singleton(), uf.size());

    for (int i = 0; i < 32 - 8; ++i)
    {
        BOOST_CHECK_EQUAL(uf.count_singleton(), uf.size() - i - (i <= 8 ? i : 8));
        BOOST_CHECK(uf.join(i, i + 8));
    }

    BOOST_CHECK_EQUAL(uf.count_singleton(), 0);
}

// =================================================================================================
BOOST_AUTO_TEST_SUITE_END()

// =================================================================================================
namespace
{
    class union_find_internals
        : public union_find<unsigned char>
    {
        public:
            using union_find::union_find;
            using union_find::is_root;
            using union_find::is_singleton;
            using union_find::subtree_size;
    };
}

// =================================================================================================
BOOST_AUTO_TEST_SUITE(internals)

// =================================================================================================
BOOST_AUTO_TEST_CASE(join_singletons)
{
    union_find_internals uf(5);

    BOOST_CHECK_EQUAL(uf.count_singleton(), 5);
    BOOST_CHECK_NE(uf.find(0), uf.find(4));
    BOOST_CHECK_EQUAL(uf.subtree_size(0), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(4), 1);

    BOOST_CHECK(uf.join(0, 4));
    BOOST_CHECK_EQUAL(uf.subtree_size(0) + uf.subtree_size(4), 1 + 2);

    BOOST_CHECK_EQUAL(uf.find_opt(0), uf.find_opt(4));
    BOOST_CHECK_EQUAL(uf.subtree_size(0) + uf.subtree_size(4), 1 + 2);
    BOOST_CHECK_EQUAL(uf.count_singleton(), 3);

}

// =================================================================================================
BOOST_AUTO_TEST_CASE(join_sets)
{
    union_find_internals uf(5);

    BOOST_CHECK_EQUAL(uf.count_singleton(), 5);

    BOOST_CHECK(uf.join(0, 1));
    BOOST_CHECK(uf.join(2, 3));

    BOOST_CHECK_EQUAL(uf.count_singleton(), 1);

    int root1 = uf.find(0);
    int child1 = root1 == 0 ? 1 : 0;
    int root2 = uf.find(2);
    int child2 = root2 == 2 ? 3 : 2;

    BOOST_CHECK_NE(uf.find(child1), uf.find(child2));

    BOOST_CHECK(uf.join(child1, child2));

    BOOST_CHECK_EQUAL(uf.find(child1), uf.find(root2));
    BOOST_CHECK_EQUAL(uf.find(root1), uf.find(child2));
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(path_compression_3_to_2)
{
    union_find_internals uf(5);

    BOOST_CHECK(uf.join(0, 1));
    BOOST_CHECK(uf.join(2, 3));

    int root1 = uf.find(0);
    int child1 = root1 == 0 ? 1 : 0;
    int root2 = uf.find(2);
    int child2 = root2 == 2 ? 3 : 2;

    BOOST_CHECK(uf.join(child1, child2));

    int newRoot = uf.find(root1);
    int oldRoot = root1 == newRoot ? root2 : root1;

    // longest path: newRoot <- oldRoot <- child (3 nodes)
    BOOST_CHECK_EQUAL(uf.subtree_size(newRoot), 4);
    BOOST_CHECK_EQUAL(uf.subtree_size(oldRoot), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(child1), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child2), 1);

    BOOST_CHECK_EQUAL(uf.find_opt(child1), uf.find_opt(child2));

    // longest path: newRoot <- all the others (2 nodes)
    BOOST_CHECK_EQUAL(uf.subtree_size(newRoot), 4);
    BOOST_CHECK_EQUAL(uf.subtree_size(oldRoot), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child1), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child2), 1);
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(path_compression_4_to_2)
{
    union_find_internals uf(8);

    BOOST_CHECK(uf.join(0, 1));
    BOOST_CHECK(uf.join(2, 3));

    int root1 = uf.find(0);
    int child1 = root1 == 0 ? 1 : 0;
    int root2 = uf.find(2);
    int child2 = root2 == 2 ? 3 : 2;

    BOOST_CHECK(uf.join(child1, child2));

    int root12 = uf.find(root1);
    int child12 = root12 == root1 ? root2 : root1;

    // longest path: root12 <- child12 <- childX (3 nodes)
    BOOST_CHECK_EQUAL(uf.subtree_size(root12), 4);
    BOOST_CHECK_EQUAL(uf.subtree_size(child12), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(root1), 4);
    BOOST_CHECK_EQUAL(uf.subtree_size(child1), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(root2), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(child2), 1);

    BOOST_CHECK(uf.join(4, 5));
    BOOST_CHECK(uf.join(6, 7));

    int root3 = uf.find(4);
    int child3 = root3 == 4 ? 5 : 4;
    int root4 = uf.find(6);
    int child4 = root4 == 6 ? 7 : 6;

    BOOST_CHECK(uf.join(child3, child4));

    int root34 = uf.find(root3);
    int child34 = root34 == root3 ? root4 : root3;

    // longest path: root34 <- child34 <- childX (3 nodes)
    BOOST_CHECK_EQUAL(uf.subtree_size(root34), 4);
    BOOST_CHECK_EQUAL(uf.subtree_size(child34), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(root3), 4);
    BOOST_CHECK_EQUAL(uf.subtree_size(child3), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(root4), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(child4), 1);

    BOOST_CHECK(uf.join(root12, root34));

    int root1234 = uf.find(root12);
    int child1234 = root1234 == root12 ? root34 : root12;

    // longest path: root1234 <- root34 <- root4 <- child4 (4 nodes)
    /*
      0       - root1234  = root12 = root1
     / \
    |   4     - child1234 = root34 = root3
    |\  |\
    | 2 | 6   - child12 = root2 | child34 = root4
    | | | |
    1 3 5 7   - child1 | child2 | child3 | child4

     */
    BOOST_CHECK_EQUAL(uf.subtree_size(root1234), 8);
    BOOST_CHECK_EQUAL(uf.subtree_size(child1234), 4);
    BOOST_CHECK_EQUAL(uf.subtree_size(root12), 8);
    BOOST_CHECK_EQUAL(uf.subtree_size(root34), 4);
    BOOST_CHECK_EQUAL(uf.subtree_size(child12), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(child34), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(root1), 8);
    BOOST_CHECK_EQUAL(uf.subtree_size(root2), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(root3), 4);
    BOOST_CHECK_EQUAL(uf.subtree_size(root4), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(child1), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child2), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child3), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child4), 1);

    BOOST_CHECK_EQUAL(uf.find_opt(child4), root1234);
    // child4 left root4/child34, both left child1234 and directly join root1234
    /*
        0       - root1234  = root12 = root1
      / |\\
     /  4 ||     - child1234 = root34 = root3
    /\  | ||
    | 2 | 6|   - child12 = root2 | child34 = root4
    | | |  |
    1 3 5  7   - child1 | child2 | child3 | child4

    */
    BOOST_CHECK_EQUAL(uf.subtree_size(root1234), 8);
    BOOST_CHECK_EQUAL(uf.subtree_size(child1234), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(root12), 8);
    BOOST_CHECK_EQUAL(uf.subtree_size(root34), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(child12), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(child34), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(root1), 8);
    BOOST_CHECK_EQUAL(uf.subtree_size(root2), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(root3), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(root4), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child1), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child2), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child3), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child4), 1);

    BOOST_CHECK_EQUAL(uf.find_opt(child2), root1234);
    // child2 left root2/chilid12 and directly joins root1234
    /*
        0       - root1234  = root12 = root1
      / |\\
     /| 4 ||     - child1234 = root34 = root3
    /|| | ||
    |2| | 6|   - child12 = root2 | child34 = root4
    | | |  |
    1 3 5  7   - child1 | child2 | child3 | child4

    */
    BOOST_CHECK_EQUAL(uf.subtree_size(root1234), 8);
    BOOST_CHECK_EQUAL(uf.subtree_size(child1234), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(root12), 8);
    BOOST_CHECK_EQUAL(uf.subtree_size(root34), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(child12), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child34), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(root1), 8);
    BOOST_CHECK_EQUAL(uf.subtree_size(root2), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(root3), 2);
    BOOST_CHECK_EQUAL(uf.subtree_size(root4), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child1), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child2), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child3), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child4), 1);

    BOOST_CHECK_EQUAL(uf.find_opt(child3), root1234);
    // child3 left child1234 and directly joins root1234
    /*
        0       - root1234  = root12 = root1
      //|\\
     /|4| ||     - child1234 = root34 = root3
    /|| | ||
    |2| | 6|   - child12 = root2 | child34 = root4
    | | |  |
    1 3 5  7   - child1 | child2 | child3 | child4

    */
    BOOST_CHECK_EQUAL(uf.subtree_size(root1234), 8);
    BOOST_CHECK_EQUAL(uf.subtree_size(child1234), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(root12), 8);
    BOOST_CHECK_EQUAL(uf.subtree_size(root34), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child12), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child34), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(root1), 8);
    BOOST_CHECK_EQUAL(uf.subtree_size(root2), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(root3), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(root4), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child1), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child2), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child3), 1);
    BOOST_CHECK_EQUAL(uf.subtree_size(child4), 1);
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(max_size)
{
    union_find_internals uf(255);

    BOOST_CHECK_EQUAL(uf.size(), 255);
    BOOST_CHECK_EQUAL(uf.max_value(), 254);
    BOOST_CHECK_EQUAL(uf.count_disjoint(), 255);
    BOOST_CHECK_EQUAL(uf.count_singleton(), 255);

    BOOST_CHECK(uf.join(0, 1));
    const auto root = uf.find(0);
    BOOST_CHECK_EQUAL(uf.count_disjoint(), 254);
    BOOST_CHECK_EQUAL(uf.count_singleton(), 253);

    BOOST_CHECK(uf.join(0, 2));
    BOOST_CHECK_EQUAL(uf.count_disjoint(), 253);
    BOOST_CHECK_EQUAL(uf.count_singleton(), 252);

    for (unsigned char i = 3; i < 254; ++i)
        uf.join(0, i);

    BOOST_CHECK_EQUAL(uf.count_disjoint(), 2);
    BOOST_CHECK_EQUAL(uf.count_singleton(), 1);

    BOOST_CHECK(uf.join(0, 254));
    BOOST_CHECK_EQUAL(uf.subtree_size(0), 255);
    BOOST_CHECK_EQUAL(uf.subtree_size(254), 1);
    BOOST_CHECK_EQUAL(uf.find(254), root);
    BOOST_CHECK_EQUAL(uf.count_disjoint(), 1);
    BOOST_CHECK_EQUAL(uf.count_singleton(), 0);
}

// =================================================================================================
BOOST_AUTO_TEST_SUITE_END()
