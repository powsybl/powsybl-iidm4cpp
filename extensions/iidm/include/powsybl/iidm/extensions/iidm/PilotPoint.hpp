/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_PILOTPOINT_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_PILOTPOINT_HPP

#include <string>
#include <vector>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class PilotPoint {
public:
    PilotPoint(const std::vector<std::string>& ids, double targetV);

    PilotPoint() = delete;
    PilotPoint(const PilotPoint& coord) = default;
    PilotPoint(PilotPoint&&) noexcept = default;
    ~PilotPoint() noexcept = default;
    PilotPoint& operator=(const PilotPoint&) = default;
    PilotPoint& operator=(PilotPoint&&) noexcept = default;

    const std::vector<std::string>& getBusbarSectionsOrBusesIds() const;

    double getTargetV() const;

    void setTargetV(double targetV);


private:
    std::vector<std::string> m_busbarSectionsOrBusesIds;

    double m_targetV;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_PILOTPOINT_HPP
