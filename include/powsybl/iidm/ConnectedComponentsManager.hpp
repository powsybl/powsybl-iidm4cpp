/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONNECTEDCOMPONENTSMANAGER_HPP
#define POWSYBL_IIDM_CONNECTEDCOMPONENTSMANAGER_HPP

#include <powsybl/iidm/AbstractConnectedComponentsManager.hpp>
#include <powsybl/iidm/ConnectedComponent.hpp>
#include <powsybl/iidm/NetworkVariant.hpp>
#include <powsybl/iidm/Ref.hpp>

namespace powsybl {

namespace iidm {

class ConnectedComponentsManager : public AbstractConnectedComponentsManager<ConnectedComponent> {
public:
    explicit ConnectedComponentsManager(Network& network);

protected:
    std::unique_ptr<ConnectedComponent> createComponent(unsigned long num, unsigned long size) override;

    const Network& getNetwork() const override;

    Network& getNetwork() override;

    void setComponentNumber(Bus& bus, const stdcxx::optional<unsigned long>& num) override;

private:
    void setNetworkRef(Network& network);

    friend class network::VariantImpl;

private:
    NetworkRef m_network;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONNECTEDCOMPONENTSMANAGER_HPP
