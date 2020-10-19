/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONNECTEDCOMPONENT_HPP
#define POWSYBL_IIDM_CONNECTEDCOMPONENT_HPP

#include <powsybl/iidm/AbstractConnectedComponent.hpp>

namespace powsybl {

namespace iidm {

class ConnectedComponent : public AbstractConnectedComponent {
public:  // Component
    const Network& getNetwork() const override;

    Network& getNetwork() override;

public:
    ConnectedComponent(unsigned long num, unsigned long size, Network& network);

    ~ConnectedComponent() noexcept override = default;

private:
    Network& m_network;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONNECTEDCOMPONENT_HPP
