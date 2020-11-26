/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SYNCHRONOUSCOMPONENT_HPP
#define POWSYBL_IIDM_SYNCHRONOUSCOMPONENT_HPP

#include <powsybl/iidm/Component.hpp>

namespace powsybl {

namespace iidm {

class SynchronousComponent : public Component {
public:
    SynchronousComponent(Network& network, unsigned long num, unsigned long size);

    ~SynchronousComponent() noexcept override = default;

private:  // Component
    bool checkBus(const Bus& bus) const override;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SYNCHRONOUSCOMPONENT_HPP
