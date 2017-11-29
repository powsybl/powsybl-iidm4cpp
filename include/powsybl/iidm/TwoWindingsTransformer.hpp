/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TWOWINDINGSTRANSFORMER_HPP
#define POWSYBL_IIDM_TWOWINDINGSTRANSFORMER_HPP

#include <powsybl/iidm/Branch.hpp>

namespace powsybl {

namespace iidm {

class TwoWindingsTransformer : public Branch {
public:
    virtual ~TwoWindingsTransformer() = default;

protected:
    TwoWindingsTransformer(const std::string& id, const std::string& name);
};

}

}

#endif  // POWSYBL_IIDM_TWOWINDINGSTRANSFORMER_HPP
