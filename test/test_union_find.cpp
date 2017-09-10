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
