/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DanglingLineFilter.hpp>

#include <powsybl/iidm/DanglingLine.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

DanglingLineFilter::DanglingLineFilter(std::function<bool(const DanglingLine&)> pred) noexcept :
    m_predicate(pred) {
}

const std::function<bool(const DanglingLine&)>& DanglingLineFilter::getPredicate() const {
    return m_predicate;
}

const DanglingLineFilter& DanglingLineFilter::ALL() { 
    static DanglingLineFilter ALL([](const DanglingLine& /*dl*/){
        return true;
    });
    return ALL;
}

const DanglingLineFilter& DanglingLineFilter::PAIRED() { 
    static DanglingLineFilter PAIRED([](const DanglingLine& dl){
        return dl.isPaired();
    });
    return PAIRED;
}

const DanglingLineFilter& DanglingLineFilter::UNPAIRED() { 
    static DanglingLineFilter UNPAIRED([](const DanglingLine& dl){
        return !dl.isPaired();
    });
    return UNPAIRED;
}

}  // namespace iidm

}  // namespace powsybl
