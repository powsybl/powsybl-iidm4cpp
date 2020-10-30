/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/NetworkVariant.hpp>

#include <powsybl/iidm/ConnectedComponentsManager.hpp>
#include <powsybl/iidm/SynchronousComponentsManager.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace network {

VariantImpl::VariantImpl(Network& network) :
    m_connectedComponentsManager(stdcxx::make_unique<ConnectedComponentsManager>(network)),
    m_synchronousComponentsManager(stdcxx::make_unique<SynchronousComponentsManager>(network)) {
}

VariantImpl::~VariantImpl() noexcept = default;

std::unique_ptr<VariantImpl> VariantImpl::copy() const {
    return stdcxx::make_unique<VariantImpl>(m_connectedComponentsManager->getNetwork());
}

const ConnectedComponentsManager& VariantImpl::getConnectedComponentsManager() const {
    return *m_connectedComponentsManager;
}

ConnectedComponentsManager& VariantImpl::getConnectedComponentsManager() {
    return *m_connectedComponentsManager;
}

const SynchronousComponentsManager& VariantImpl::getSynchronousComponentsManager() const {
    return *m_synchronousComponentsManager;
}

SynchronousComponentsManager& VariantImpl::getSynchronousComponentsManager() {
    return *m_synchronousComponentsManager;
}

void VariantImpl::setVariantManagerHolder(VariantManagerHolder& variantManagerHolder) {
    m_connectedComponentsManager->setNetworkRef(static_cast<Network&>(variantManagerHolder));
    m_synchronousComponentsManager->setNetworkRef(static_cast<Network&>(variantManagerHolder));
}

}  // namespace network

}  // namespace iidm

}  // namespace powsybl
