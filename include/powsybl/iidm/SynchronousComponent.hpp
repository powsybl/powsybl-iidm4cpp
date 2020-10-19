/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SYNCHRONOUSCOMPONENT_HPP
#define POWSYBL_IIDM_SYNCHRONOUSCOMPONENT_HPP

#include <powsybl/iidm/AbstractSynchronousComponent.hpp>

namespace powsybl {

namespace iidm {

class SynchronousComponent : public AbstractSynchronousComponent {
public:  // Component
    const Network& getNetwork() const override;

    Network& getNetwork() override;

public:
    SynchronousComponent(unsigned long num, unsigned long size, Network& network);

    ~SynchronousComponent() noexcept override = default;

private:
    Network& m_network;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SynchronousComponent_HPP
