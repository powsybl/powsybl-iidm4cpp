/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DANGLINGLINE_HPP
#define POWSYBL_IIDM_DANGLINGLINE_HPP

#include <powsybl/iidm/Injection.hpp>

namespace powsybl {

namespace iidm {

class DanglingLine : public Injection {
public:
    virtual ~DanglingLine() = default;

protected:
    DanglingLine(const std::string& id, const std::string& name);
};

}

}

#endif  // POWSYBL_IIDM_DANGLINGLINE_HPP
