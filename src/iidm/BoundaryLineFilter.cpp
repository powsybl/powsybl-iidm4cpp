/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/BoundaryLineFilter.hpp>

#include <powsybl/iidm/BoundaryLine.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

BoundaryLineFilter::BoundaryLineFilter(std::function<bool(const BoundaryLine&)> pred) noexcept :
    m_predicate(pred) {
}

const std::function<bool(const BoundaryLine&)>& BoundaryLineFilter::getPredicate() const {
    return m_predicate;
}

const BoundaryLineFilter& BoundaryLineFilter::ALL() { 
    static BoundaryLineFilter ALL([](const BoundaryLine& /*bl*/){
        return true;
    });
    return ALL;
}

const BoundaryLineFilter& BoundaryLineFilter::PAIRED() { 
    static BoundaryLineFilter PAIRED([](const BoundaryLine& bl){
        return bl.isPaired();
    });
    return PAIRED;
}

const BoundaryLineFilter& BoundaryLineFilter::UNPAIRED() { 
    static BoundaryLineFilter UNPAIRED([](const BoundaryLine& bl){
        return !bl.isPaired();
    });
    return UNPAIRED;
}

}  // namespace iidm

}  // namespace powsybl
