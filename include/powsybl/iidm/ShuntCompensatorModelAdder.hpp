/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATORMODELADDER_HPP
#define POWSYBL_IIDM_SHUNTCOMPENSATORMODELADDER_HPP

#include <memory>

namespace powsybl {

namespace iidm {

class ShuntCompensator;
class ShuntCompensatorAdder;
class ShuntCompensatorModel;

class ShuntCompensatorModelAdder {
public:
    ShuntCompensatorModelAdder() = default;

    virtual ~ShuntCompensatorModelAdder() noexcept = default;

    virtual ShuntCompensatorAdder& add() = 0;

private:
    virtual std::unique_ptr<ShuntCompensatorModel> build() const = 0;

    virtual unsigned long getMaximumSectionCount() const = 0;

    friend class ShuntCompensatorAdder;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATORMODELADDER_HPP
