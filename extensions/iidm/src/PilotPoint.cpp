/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/PilotPoint.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

PilotPoint::PilotPoint(const std::vector<std::string>& ids, double targetV) {
    if(ids.empty()) {
        throw PowsyblException("Empty busbar section of bus ID list");
    }
    for(const auto& id : ids) {
        if(id.empty()) {
            throw PowsyblException("Empty busbar section or bus ID");
        }
    }
    if(std::isnan(targetV)){
        throw PowsyblException("Invalid target voltage");
    }
    m_busbarSectionsOrBusesIds = ids;
    m_targetV = targetV; 
}

const std::vector<std::string>& PilotPoint::getBusbarSectionsOrBusesIds() const {
    return m_busbarSectionsOrBusesIds;
}

double PilotPoint::getTargetV() const {
    return m_targetV;
}

void PilotPoint::setTargetV(double targetV) {
    m_targetV = targetV;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl