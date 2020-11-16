/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/TerminalFinder.hpp>

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace util {

TerminalFinder::TerminalFinder(const std::vector<Predicate>& predicates) :
    m_comparator(getComparator(predicates)) {
}

TerminalFinder::TerminalFinder(const Comparator& comparator) :
    m_comparator(comparator) {
}

stdcxx::CReference<Terminal> TerminalFinder::find(const stdcxx::const_range<Terminal>& terminals) const {
    auto it = std::max_element(terminals.begin(), terminals.end(), m_comparator);
    return it != terminals.end() ? stdcxx::cref(*it) : stdcxx::cref<Terminal>();
}

stdcxx::Reference<Terminal> TerminalFinder::find(const stdcxx::range<Terminal>& terminals) const {
    auto it = std::max_element(terminals.begin(), terminals.end(), m_comparator);
    return it != terminals.end() ? stdcxx::ref(*it) : stdcxx::ref<Terminal>();
}

TerminalFinder::Comparator TerminalFinder::getComparator(const std::vector<Predicate>& predicates) {
    return [&predicates](const Terminal& t1, const Terminal& t2) {
        if (stdcxx::areSame(t1, t2)) {
            return false;
        }
        for (const auto& predicate : predicates) {
            if (predicate(t1) || predicate(t2)) {
                return !predicate(t1) && predicate(t2);
            }
        }
        return false; // no predicate is verified by t1 or t2, the first is then considered better than the latter
    };
}

TerminalFinder TerminalFinder::getDefault() {
    return TerminalFinder(getDefaultRules());
}

const std::vector<TerminalFinder::Predicate>& TerminalFinder::getDefaultRules() {
    static std::vector<Predicate> s_rules = {
        [](const Terminal& t) { return stdcxx::isInstanceOf<BusbarSection>(t.getConnectable()); },
        [](const Terminal& t) { return stdcxx::isInstanceOf<Injection>(t.getConnectable()); },
        [](const Terminal& t) { return stdcxx::isInstanceOf<Branch>(t.getConnectable()); },
        [](const Terminal& t) { return stdcxx::isInstanceOf<ThreeWindingsTransformer>(t.getConnectable()); },
        [](const Terminal& t) { return stdcxx::isInstanceOf<HvdcConverterStation>(t.getConnectable()); }
    };
    return s_rules;
}

}  // namespace util

}  // namespace iidm

}  // namespace powsybl
