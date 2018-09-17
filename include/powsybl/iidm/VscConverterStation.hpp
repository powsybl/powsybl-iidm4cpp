/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VSCCONVERTERSTATION_HPP
#define POWSYBL_IIDM_VSCCONVERTERSTATION_HPP

#include <powsybl/iidm/HvdcConverterStation.hpp>

namespace powsybl {

namespace iidm {

class VscConverterStation : public HvdcConverterStation {
public:
    ~VscConverterStation() noexcept override = default;

protected:
    VscConverterStation(const std::string& id, const std::string& name);
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VSCCONVERTERSTATION_HPP
