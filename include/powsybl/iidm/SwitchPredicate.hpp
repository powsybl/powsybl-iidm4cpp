/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SWITCHPREDICATE_HPP
#define POWSYBL_IIDM_SWITCHPREDICATE_HPP

#include <powsybl/iidm/Switch.hpp>
#include <powsybl/stdcxx/Predicate.hpp>

namespace powsybl {

namespace iidm {

class SwitchPredicate {
public:
    static const stdcxx::Predicate<Switch>& IS_NONFICTIONAL_CLOSED_BREAKER();
    static const stdcxx::Predicate<Switch>& IS_NONFICTIONAL_BREAKER();
    static const stdcxx::Predicate<Switch>& IS_CLOSED_BREAKER();
    static const stdcxx::Predicate<Switch>& IS_BREAKER_OR_DISCONNECTOR();
    static const stdcxx::Predicate<Switch>& IS_OPEN_DISCONNECTOR();
    static const stdcxx::Predicate<Switch>& IS_BREAKER();
    static const stdcxx::Predicate<Switch>& IS_NONFICTIONAL();
    static const stdcxx::Predicate<Switch>& IS_OPEN();
    static const stdcxx::Predicate<Switch>& TRUE();

private:
    SwitchPredicate();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SWITCHPREDICATE_HPP
