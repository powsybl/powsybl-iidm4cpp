/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CALCULATEDBUS_HPP
#define POWSYBL_IIDM_CALCULATEDBUS_HPP

#include <powsybl/iidm/Bus.hpp>

namespace powsybl {

namespace iidm {

class CalculatedBus : public Bus {
public:
    virtual ~CalculatedBus() = default;

    virtual void invalidate() = 0;

protected:
    CalculatedBus(const std::string& id);
};

}

}

#endif  // POWSYBL_IIDM_CALCULATEDBUS_HPP
