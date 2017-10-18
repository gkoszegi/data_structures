#include <union_find.hpp>
#include "testing.hpp"

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
