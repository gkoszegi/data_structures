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

    BOOST_CHECK_THROW(uf.find(-1), std::out_of_range);
    BOOST_CHECK_EQUAL(uf.find(0), 0);
    BOOST_CHECK_EQUAL(uf.find(1), 1);
    BOOST_CHECK_THROW(uf.find(2), std::out_of_range);

    BOOST_CHECK(!uf.join(0, 0));
    BOOST_CHECK(uf.join(0, 1));

    BOOST_CHECK_EQUAL(uf.find(0), uf.find(1));
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(count_disjoint)
{
    union_find<int> uf(128);

    for (int i = 0; i < 120; ++i)
    {
        BOOST_CHECK_EQUAL(uf.count_disjoint(), uf.size() - i);
        BOOST_CHECK(uf.join(i, i + 8));
    }

    BOOST_CHECK_EQUAL(uf.count_disjoint(), 8);
}
