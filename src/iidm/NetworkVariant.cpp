/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/NetworkVariant.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace network {

VariantImpl::VariantImpl(Network& network) :
    m_network(network),
    m_connectedComponentsManager(network),
    m_synchronousComponentsManager(network) {
}

std::unique_ptr<VariantImpl> VariantImpl::copy() const {
    return stdcxx::make_unique<VariantImpl>(m_network);
}

const ConnectedComponentsManager& VariantImpl::getConnectedComponentsManager() const {
    return m_connectedComponentsManager;
}

ConnectedComponentsManager& VariantImpl::getConnectedComponentsManager() {
    return m_connectedComponentsManager;
}

const SynchronousComponentsManager& VariantImpl::getSynchronousComponentsManager() const {
    return m_synchronousComponentsManager;
}

SynchronousComponentsManager& VariantImpl::getSynchronousComponentsManager() {
    return m_synchronousComponentsManager;
}

}  // namespace network

}  // namespace iidm

}  // namespace powsybl
