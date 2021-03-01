/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/TerminalFinder.hpp>

#include <vector>

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace TerminalFinder {

using Predicate = std::function<bool(const Terminal&)>;

Comparator getComparator(const std::vector<Predicate>& predicates) {
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

const std::vector<TerminalFinder::Predicate>& getDefaultRules() {
    static std::vector<Predicate> s_rules = {
        [](const Terminal& t) { return stdcxx::isInstanceOf<BusbarSection>(t.getConnectable()); },
        [](const Terminal& t) { return stdcxx::isInstanceOf<Injection>(t.getConnectable()); },
        [](const Terminal& t) { return stdcxx::isInstanceOf<Branch>(t.getConnectable()); },
        [](const Terminal& t) { return stdcxx::isInstanceOf<ThreeWindingsTransformer>(t.getConnectable()); },
        [](const Terminal& t) { return stdcxx::isInstanceOf<HvdcConverterStation>(t.getConnectable()); }
    };
    return s_rules;
}

stdcxx::CReference<Terminal> find(const stdcxx::const_range<Terminal>& terminals) {
    return find(getComparator(getDefaultRules()), terminals);
}

stdcxx::Reference<Terminal> find(const stdcxx::range<Terminal>& terminals) {
    return find(getComparator(getDefaultRules()), terminals);
}

stdcxx::CReference<Terminal> find(const Comparator& comparator, const stdcxx::const_range<Terminal>& terminals) {
    auto it = std::max_element(terminals.begin(), terminals.end(), comparator);
    return it != terminals.end() ? stdcxx::cref(*it) : stdcxx::cref<Terminal>();
}

stdcxx::Reference<Terminal> find(const Comparator& comparator, const stdcxx::range<Terminal>& terminals) {
    auto it = std::max_element(terminals.begin(), terminals.end(), comparator);
    return it != terminals.end() ? stdcxx::ref(*it) : stdcxx::ref<Terminal>();
}

}  // namespace TerminalFinder

}  // namespace iidm

}  // namespace powsybl
