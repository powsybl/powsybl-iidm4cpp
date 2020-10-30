/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORKVARIANT_HPP
#define POWSYBL_IIDM_NETWORKVARIANT_HPP

#include <powsybl/iidm/Variant.hpp>

namespace powsybl {

namespace iidm {

class ConnectedComponentsManager;
class Network;
class SynchronousComponentsManager;

namespace network {

class VariantImpl : public Variant<VariantImpl> {
public: // Variant
    std::unique_ptr<VariantImpl> copy() const override;

public:
    explicit VariantImpl(Network& network);

    ~VariantImpl() noexcept override = default;

    const ConnectedComponentsManager& getConnectedComponentsManager() const;

    ConnectedComponentsManager& getConnectedComponentsManager();

    const SynchronousComponentsManager& getSynchronousComponentsManager() const;

    SynchronousComponentsManager& getSynchronousComponentsManager();

    void setVariantManagerHolder(VariantManagerHolder& variantManagerHolder) override;

private:
    std::unique_ptr<ConnectedComponentsManager> m_connectedComponentsManager;

    std::unique_ptr<SynchronousComponentsManager> m_synchronousComponentsManager;
};

}  // namespace network

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORKVARIANT_HPP
