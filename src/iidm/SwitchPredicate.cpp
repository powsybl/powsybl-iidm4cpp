/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/SwitchPredicate.hpp>

namespace powsybl {

namespace iidm {

SwitchPredicate::SwitchPredicate() {
}

const stdcxx::Predicate<Switch>& SwitchPredicate::IS_NONFICTIONAL_CLOSED_BREAKER() {
    static stdcxx::Predicate<Switch> predicate([](const Switch& switchObject) {
        return switchObject.getKind() == SwitchKind::BREAKER && !switchObject.isOpen() && !switchObject.isFictitious();
    });
    return predicate;
}
const stdcxx::Predicate<Switch>& SwitchPredicate::IS_NONFICTIONAL_BREAKER(){
    static stdcxx::Predicate<Switch> predicate([](const Switch& switchObject) {
        return switchObject.getKind() == SwitchKind::BREAKER && !switchObject.isFictitious();
    });
    return predicate;
}
const stdcxx::Predicate<Switch>& SwitchPredicate::IS_CLOSED_BREAKER() {
    static stdcxx::Predicate<Switch> predicate([](const Switch& switchObject) {
        return switchObject.getKind() == SwitchKind::BREAKER && !switchObject.isOpen();
    });
    return predicate;
}
const stdcxx::Predicate<Switch>& SwitchPredicate::IS_BREAKER_OR_DISCONNECTOR() {
    static stdcxx::Predicate<Switch> predicate([](const Switch& switchObject) {
        return switchObject.getKind() == SwitchKind::BREAKER || switchObject.getKind() == SwitchKind::DISCONNECTOR;
    });
    return predicate;
}
const stdcxx::Predicate<Switch>& SwitchPredicate::IS_OPEN_DISCONNECTOR() {
    static stdcxx::Predicate<Switch> predicate([](const Switch& switchObject) {
        return switchObject.getKind() == SwitchKind::DISCONNECTOR && switchObject.isOpen();
    });
    return predicate;
}
const stdcxx::Predicate<Switch>& SwitchPredicate::IS_BREAKER() {
    static stdcxx::Predicate<Switch> predicate([](const Switch& switchObject) {
        return switchObject.getKind() == SwitchKind::BREAKER;
    });
    return predicate;
}
const stdcxx::Predicate<Switch>& SwitchPredicate::IS_NONFICTIONAL() {
    static stdcxx::Predicate<Switch> predicate([](const Switch& switchObject) {
        return !switchObject.isFictitious();
    });
    return predicate;
}
const stdcxx::Predicate<Switch>& SwitchPredicate::IS_OPEN() {
    static stdcxx::Predicate<Switch> predicate([](const Switch& switchObject) {
        return switchObject.isOpen();
    });
    return predicate;
}

}  // namespace iidm

}  // namespace powsybl

