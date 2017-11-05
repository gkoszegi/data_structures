#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

template<typename elem_type, typename prio_type>
class indexed_heap
{
    public:
        static_assert(std::is_integral<elem_type>::value, "indexed_heap: elem_type must be integral");
        static_assert(std::is_unsigned<elem_type>::value, "indexed_heap: elem_type must be unsigned");
        static_assert(std::is_integral<prio_type>::value, "indexed_heap: prio_type must be integral");
        using index_type = std::make_unsigned_t<elem_type>;

    protected:
        const index_type invalidIndex = std::numeric_limits<index_type>::max();

        struct item_type
        {
            prio_type prio;
            elem_type elem;

            item_type() {}
            item_type(prio_type p, elem_type e)
                : prio(p), elem(e)
            {}

            bool operator< (const item_type& other) const
            { return prio < other.prio; }

            bool operator> (const item_type& other) const
            { return prio > other.prio; }
        };

    public:
        indexed_heap(elem_type itemCount = 0)
            : mIndex(itemCount, invalidIndex)
        {
            mHeap.reserve(itemCount);
        }

        auto size() const
        { return mHeap.size(); }

        bool empty() const
        { return mHeap.empty(); }

        elem_type top() const
        { return mHeap.at(0).elem; }

        prio_type top_priority() const
        { return mHeap.at(0).prio; }

        void pop()
        {
            if (empty())
                return;

            elem_type e = mHeap.back().elem;
            mIndex[e] = 0; // last will be moved to root
            mIndex[mHeap.front().elem] = invalidIndex; // to be removed
            mHeap.front() = mHeap.back(); // move to root
            mHeap.resize(mHeap.size() - 1); // remove last moved from
            bubble_down(e, 0); // restore heap property
        }

        bool push(const elem_type elem, const prio_type priority)
        {
            auto& idx = mIndex.at(elem);
            if (idx != invalidIndex)
                return false;
            idx = mHeap.size();
            mHeap.emplace_back(priority, elem);
            bubble_up(elem, mIndex[elem]);
            return true;
        }

        prio_type get_priority(const elem_type elem) const
        {
            return mHeap.at(mIndex.at(elem)).prio;
        }

        bool change_priority(const elem_type elem, const prio_type priority)
        {
            const auto idx = mIndex.at(elem);
            if (idx == invalidIndex)
                return false;

            auto& onHeap = mHeap[idx];
            if (priority == onHeap.prio)
                return true;

            auto restoreHeap = priority < onHeap.prio ?
                &indexed_heap::bubble_up : &indexed_heap::bubble_down;
            onHeap.prio = priority;

            (this->*restoreHeap)(elem, idx);
            return true;
        }

        void set_priority(const elem_type elem, const prio_type priority)
        {
            if (!change_priority(elem, priority))
                push(elem, priority);
        }

    protected:

        void bubble_up(elem_type elem, index_type elemIdx)
        {
            index_type parentIdx = (elemIdx - 1) / 2;

            while (elemIdx > 0 && mHeap[elemIdx].prio < mHeap[parentIdx].prio)
            {
                elem_type parent = mHeap[parentIdx].elem;
                std::swap(mHeap[elemIdx], mHeap[parentIdx]);
                std::swap(mIndex[elem], mIndex[parent]);

                elemIdx = parentIdx;
                parentIdx = (elemIdx - 1) / 2;
            }
        }

        void bubble_down(elem_type elem, index_type elemIdx)
        {
            index_type childIdx = 2 * elemIdx + 1; // first child

            while (childIdx < mHeap.size())
            {
                if (static_cast<size_t>(childIdx) + 1 < mHeap.size() &&
                        mHeap[childIdx + 1].prio < mHeap[childIdx].prio)
                {
                    ++childIdx;
                }

                if (mHeap[elemIdx].prio < mHeap[childIdx].prio)
                    return;

                elem_type child = mHeap[childIdx].elem;
                std::swap(mHeap[elemIdx], mHeap[childIdx]);
                std::swap(mIndex[elem], mIndex[child]);

                elemIdx = childIdx;
                childIdx = 2 * elemIdx + 1;
            }
        }

        std::vector<item_type> mHeap; // min-heap of priorized elements
        std::vector<index_type> mIndex; // index in heap by element
};
