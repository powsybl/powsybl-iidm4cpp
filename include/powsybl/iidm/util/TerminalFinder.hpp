/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_TERMINALFINDER_HPP
#define POWSYBL_IIDM_UTIL_TERMINALFINDER_HPP

#include <functional>
#include <vector>

#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Terminal;

namespace util {

class TerminalFinder {
public:
    using Comparator = std::function<bool(const Terminal& t1, const Terminal& t2)>;

    using Predicate = std::function<bool(const Terminal&)>;

public:
    static TerminalFinder getDefault();

public:
    explicit TerminalFinder(const std::vector<Predicate>& predicates);

    explicit TerminalFinder(const Comparator& comparator);

    TerminalFinder(const TerminalFinder&) = delete;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::function is not marked noexcept
    TerminalFinder(TerminalFinder&&) = default;

    TerminalFinder& operator=(const TerminalFinder&) = delete;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::function is not marked noexcept
    TerminalFinder& operator=(TerminalFinder&&) = default;

    stdcxx::CReference<Terminal> find(const stdcxx::const_range<Terminal>& terminals) const;

    stdcxx::Reference<Terminal> find(const stdcxx::range<Terminal>& terminals) const;

private:
    static Comparator getComparator(const std::vector<Predicate>& predicates);

    static const std::vector<Predicate>& getDefaultRules();

private:
    Comparator m_comparator;
};

}  // namespace util

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_TERMINALFINDER_HPP
