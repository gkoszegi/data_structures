#include <indexed_heap.hpp>
#include "testing.hpp"

namespace
{
    class test_heap
        : public indexed_heap<unsigned short, int>
    {
        public:
            using indexed_heap::indexed_heap;

            bool check_heap() const
            {
                for (index_type i = 1; i < mHeap.size(); ++i)
                {
                    auto parent = (i - 1) / 2;
                    if (mHeap[i].prio < mHeap[parent].prio)
                        return false;
                    if (mIndex[mHeap[i].elem] != i)
                        return false;
                }
                return true;
            }

            bool check_index() const
            {
                for (index_type e = 0; e < mIndex.size(); ++e)
                {
                    if (mIndex[e] == invalidIndex)
                        continue;
                    if (mIndex[e] >= mHeap.size())
                        return false;
                    if (mHeap[mIndex[e]].elem != e)
                        return false;
                }
                return true;
            }
    };
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(zero_sized_heap)
{
    test_heap q(0);

    BOOST_CHECK(q.empty());
    BOOST_CHECK_EQUAL(q.size(), 0u);
    BOOST_CHECK_NO_THROW(q.pop());
    BOOST_CHECK_THROW(q.top(), std::out_of_range);
    BOOST_CHECK_THROW(q.top_priority(), std::out_of_range);
    BOOST_CHECK_THROW(q.push(0, 0), std::out_of_range);
    BOOST_CHECK_THROW(q.get_priority(0), std::out_of_range);
    BOOST_CHECK_THROW(q.change_priority(0, 0), std::out_of_range);
    BOOST_CHECK(q.check_index());
    BOOST_CHECK(q.check_heap());
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(size_one_heap)
{
    test_heap q(1);

    // empty checks
    BOOST_CHECK(q.empty());
    BOOST_CHECK_EQUAL(q.size(), 0);
    BOOST_CHECK_NO_THROW(q.pop());
    BOOST_CHECK(!q.change_priority(0, 6));
    BOOST_CHECK_THROW(q.top(), std::out_of_range);
    BOOST_CHECK_THROW(q.top_priority(), std::out_of_range);
    
    // cannot push 1 (only 0)
    BOOST_CHECK_THROW(q.push(1, 2), std::out_of_range);
    BOOST_CHECK(q.empty());

    // push the single possible element
    BOOST_CHECK(q.push(0, 1));
    BOOST_CHECK(!q.empty());
    BOOST_CHECK_EQUAL(q.size(), 1);
    BOOST_CHECK_EQUAL(q.top(), 0);
    BOOST_CHECK_EQUAL(q.top_priority(), 1);
    BOOST_CHECK_EQUAL(q.get_priority(0), 1);

    // repeated push
    BOOST_CHECK(!q.push(0, 3));
    BOOST_CHECK_EQUAL(q.size(), 1);
    BOOST_CHECK_EQUAL(q.top_priority(), 1);
    BOOST_CHECK_EQUAL(q.get_priority(0), 1);

    // reset the priority
    BOOST_CHECK_NO_THROW(q.change_priority(0, 6));
    BOOST_CHECK(!q.empty());
    BOOST_CHECK_EQUAL(q.top(), 0);
    BOOST_CHECK_EQUAL(q.top_priority(), 6);
    BOOST_CHECK_EQUAL(q.get_priority(0), 6);

    // check heap and index properies
    BOOST_CHECK(q.check_index());
    BOOST_CHECK(q.check_heap());

    // pop and check empty
    BOOST_CHECK_NO_THROW(q.pop());
    BOOST_CHECK(q.empty());
    BOOST_CHECK_EQUAL(q.size(), 0);
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(heap_property_push_pop)
{
    test_heap q(5);

    for (unsigned elem: {0,3,2,4,1})
    {
        BOOST_CHECK(q.push(elem, 10 + elem));
        BOOST_CHECK(q.check_heap());
        BOOST_CHECK(q.check_index());
    }

    BOOST_CHECK_EQUAL(q.size(), 5);

    for (unsigned elem: {0,1,2,3,4})
    {
        BOOST_CHECK_EQUAL(q.top(), elem);
        BOOST_CHECK_EQUAL(q.top_priority(), 10 + elem);
        BOOST_CHECK_NO_THROW(q.pop());
        BOOST_CHECK(q.check_heap());
        BOOST_CHECK(q.check_index());
    }

    BOOST_CHECK_EQUAL(q.size(), 0);
}

// =================================================================================================
BOOST_AUTO_TEST_CASE(heap_property_repriorize)
{
    test_heap q(5);

    for (unsigned elem: {0,3,2,4,1})
    {
        BOOST_CHECK(q.push(elem, 10 + elem));
        BOOST_CHECK(q.check_heap());
        BOOST_CHECK(q.check_index());
    }

    BOOST_CHECK_EQUAL(q.size(), 5);

    for (unsigned elem: {0,3,2,4,1})
    {
        BOOST_CHECK(q.change_priority(elem, 10 - elem));
        BOOST_CHECK(q.check_heap());
        BOOST_CHECK(q.check_index());
    }

    BOOST_CHECK_EQUAL(q.size(), 5);

    for (unsigned elem: {4,3,2,1,0})
    {
        BOOST_CHECK_EQUAL(q.top(), elem);
        BOOST_CHECK_EQUAL(q.top_priority(), 10 - elem);
        BOOST_CHECK_NO_THROW(q.pop());
        BOOST_CHECK(q.check_heap());
        BOOST_CHECK(q.check_index());
    }

    BOOST_CHECK_EQUAL(q.size(), 0);
}
