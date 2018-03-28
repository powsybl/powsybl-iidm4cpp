/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSVIEW_HPP
#define POWSYBL_IIDM_BUSVIEW_HPP

#include <string>

namespace powsybl {

namespace iidm {

class Bus;

class BusView {
public:
    virtual ~BusView() = default;

    virtual Bus& getBus(const std::string& busId) const = 0;
};

}

}

#endif  // POWSYBL_IIDM_BUSVIEW_HPP
