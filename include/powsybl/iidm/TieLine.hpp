/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TIELINE_HPP
#define POWSYBL_IIDM_TIELINE_HPP

#include <powsybl/iidm/Line.hpp>

namespace powsybl {

namespace iidm {

class TieLine : public Line {
public:
    ~TieLine() override = default;

protected:
    TieLine(const std::string& id, const std::string& name);
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TIELINE_HPP
