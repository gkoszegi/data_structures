#pragma once

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <vector>

template<typename T>
class union_find
{
    public:

        static_assert(std::is_integral<T>::value, "union_find<T>: T must be integral");

        using value_type = T;
        using size_type = std::make_unsigned_t<T>;

        union_find(value_type n)
            : mSets(n)
            , mSize(n)
        {
            std::iota(mSets.begin(), mSets.end(), 0);
            std::fill(mSize.begin(), mSize.end(), 1);
        }

        bool join(value_type v1, value_type v2)
        {
            auto r1 = find(v1);
            auto r2 = find(v2);

            if (r1 == r2)
                return false;

            if (mSize[r1] < mSize[r2])
                std::swap(r1, r2);

            merge_into_left(r1, r2);
            compress_path(v2, r1);

            return true;
        }

        value_type find(value_type v) const
        {
            if (static_cast<size_t>(v) >= mSets.size())
                throw std::out_of_range("union_find::find(): value out of range");

            while (v != mSets[v])
                v = mSets[v];

            return v;
        }

    protected:

        void merge_into_left(value_type r1, value_type r2)
        {
            mSets[r2] = r1;
            mSize[r1] += mSize[r2];
        }

        void compress_path(value_type val, value_type root)
        {
            while (val != root)
            {
                value_type parent = mSets[val];
                mSets[val] = root;
                mSize[parent] -= mSize[val];
                mSize[root] += mSize[val];
                val = parent;
            }
        }

        std::vector<value_type> mSets;
        std::vector<size_type> mSize;
};
