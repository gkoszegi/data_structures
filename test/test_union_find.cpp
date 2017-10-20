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
    union_find<int> uf(128);

    BOOST_CHECK_EQUAL(uf.count_disjoint(), uf.size());

    for (int i = 0; i < 120; ++i)
    {
        BOOST_CHECK_EQUAL(uf.count_disjoint(), uf.size() - i);
        BOOST_CHECK(uf.join(i, i + 8));
    }

    BOOST_CHECK_EQUAL(uf.count_disjoint(), 8);
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(count_singleton)
{
    union_find<int> uf(128);

    BOOST_CHECK_EQUAL(uf.count_singleton(), uf.size());

    for (int i = 0; i < 120; ++i)
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
        : public union_find<int>
    {
        public:
            using union_find::union_find;
            using union_find::is_root;
            using union_find::is_singleton;
            using union_find::num_inferiors;
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
    BOOST_CHECK_EQUAL(uf.num_inferiors(0), 1);
    BOOST_CHECK_EQUAL(uf.num_inferiors(4), 1);

    BOOST_CHECK(uf.join(0, 4));
    BOOST_CHECK_EQUAL(uf.num_inferiors(0) + uf.num_inferiors(4), 1 + 2);

    BOOST_CHECK_EQUAL(uf.find_opt(0), uf.find_opt(4));
    BOOST_CHECK_EQUAL(uf.num_inferiors(0) + uf.num_inferiors(4), 1 + 2);
    BOOST_CHECK_EQUAL(uf.count_singleton(), 3);

}

// =================================================================================================
BOOST_AUTO_TEST_CASE(join_sets)
{
    union_find_internals uf(5);

    BOOST_CHECK_EQUAL(uf.count_singleton(), 5);

    BOOST_CHECK(uf.join(0, 1));
    BOOST_CHECK(uf.join(2, 3));

    int root1 = uf.find(0);
    int child1 = root1 == 0 ? 1 : 0;
    int root2 = uf.find(2);
    int child2 = root2 == 2 ? 3 : 2;

    BOOST_CHECK_EQUAL(uf.num_inferiors(root1), 2);
    BOOST_CHECK_EQUAL(uf.num_inferiors(root2), 2);

    BOOST_CHECK_EQUAL(uf.count_singleton(), 1);

    BOOST_CHECK(uf.join(child1, child2));

    int newRoot = uf.find(root1);
    int oldRoot = root1 == newRoot ? root2 : root1;

    BOOST_CHECK_EQUAL(uf.num_inferiors(newRoot), 4);
    BOOST_CHECK_EQUAL(uf.num_inferiors(oldRoot), 2);
    BOOST_CHECK_EQUAL(uf.num_inferiors(child1), 1);
    BOOST_CHECK_EQUAL(uf.num_inferiors(child2), 1);
}

// =================================================================================================
BOOST_AUTO_TEST_SUITE_END()
