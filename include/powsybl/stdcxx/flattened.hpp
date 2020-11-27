/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_FLATTENED_HPP
#define POWSYBL_STDCXX_FLATTENED_HPP

#include <boost/iterator_adaptors.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>

namespace stdcxx {

/**
 * The FlatteningIterator is used to iterate over a range of ranges:
 * - the OuterIterator is a forward iterator for the outer range
 * - the InnerIterator is a forward iterator for the inner ranges.
 *
 * This iterator SHOULD not be used directly. Use the pipe operator instead.
 */
template <typename Iterator>
class FlatteningIterator :
    public boost::iterator_adaptor<
        FlatteningIterator<Iterator>,
        Iterator,
        typename Iterator::value_type::iterator::value_type,
        boost::forward_traversal_tag,
        typename Iterator::value_type::iterator::value_type> {
public:
    using OuterIterator = Iterator;
    using InnerIterator = typename Iterator::value_type::iterator;
    using Value = typename Iterator::value_type::iterator::value_type;

    using base = boost::iterator_adaptor<FlatteningIterator<Iterator>, Iterator, Value, boost::forward_traversal_tag, Value>;

public:
    FlatteningIterator() = default;

    FlatteningIterator(Iterator it) :
        base(it),
        m_outer_end(it),
        m_inner_begin(),
        m_inner_end() {
    }

    FlatteningIterator(Iterator begin, Iterator end) :
        base(begin),
        m_outer_end(end) {

        if (begin != end) {
            m_inner_begin = (*begin).begin(),
            m_inner_end = (*begin).end();
            skipEmptyRanges();
        }
    }

private:
    void increment() {
        if (this->base_reference() == m_outer_end) {
            return;
        }

        ++m_inner_begin;
        skipEmptyRanges();
    }

    Value dereference() const {
        return *m_inner_begin;
    }

    void skipEmptyRanges() {
        while ((this->base_reference() != m_outer_end) && (m_inner_begin == m_inner_end)) {
            ++this->base_reference();
            if (this->base_reference() != m_outer_end) {
                m_inner_begin = (*this->base_reference()).begin();
                m_inner_end = (*this->base_reference()).end();
            }
        }
    }

    friend class boost::iterator_core_access;

private:
    OuterIterator m_outer_end;
    InnerIterator m_inner_begin;
    InnerIterator m_inner_end;
};

struct FlattenForwarder {};

namespace {

const FlattenForwarder flattened;

}  // namespace

template <typename SinglePassRange>
using FlattenRange = boost::iterator_range<FlatteningIterator<typename boost::range_iterator<SinglePassRange>::type>>;

template <typename SinglePassRange>
FlattenRange<SinglePassRange> operator|(SinglePassRange& range, FlattenForwarder /* unused */) {
    using Iterator = typename boost::range_iterator<SinglePassRange>::type;

    return boost::make_iterator_range(FlatteningIterator<Iterator>(boost::begin(range), boost::end(range)), FlatteningIterator<Iterator>(boost::end(range)));
}

template <typename SinglePassRange>
FlattenRange<SinglePassRange> operator|(const SinglePassRange& range, FlattenForwarder /* unused */) {
    using Iterator = typename boost::range_iterator<const SinglePassRange>::type;

    return boost::make_iterator_range(FlatteningIterator<Iterator>(boost::begin(range), boost::end(range)), FlatteningIterator<Iterator>(boost::end(range)));
}

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_FLATTENED_HPP
