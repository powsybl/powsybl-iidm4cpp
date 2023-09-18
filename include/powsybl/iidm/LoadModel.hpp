/**
 * Copyright (c) 2023, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOADMODEL_HPP
#define POWSYBL_IIDM_LOADMODEL_HPP

#include <powsybl/iidm/LoadModelType.hpp>

namespace powsybl {

namespace iidm {

class Load;

class LoadModel {
public:
    LoadModel() = default;

    virtual ~LoadModel() noexcept = default;

private:
    virtual const LoadModelType& getType() const = 0;

    friend class Load;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOADMODEL_HPP
