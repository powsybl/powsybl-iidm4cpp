/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_NETWORK_FOURSUBSTATIONSNODEBREAKERFACTORY_HPP
#define POWSYBL_NETWORK_FOURSUBSTATIONSNODEBREAKERFACTORY_HPP

namespace powsybl {

namespace iidm {

class Network;

}  // namespace iidm

namespace network {

class FourSubstationsNodeBreakerFactory {
public:
    static iidm::Network create();

public:
    FourSubstationsNodeBreakerFactory() = delete;

    FourSubstationsNodeBreakerFactory(const FourSubstationsNodeBreakerFactory&) = delete;

    FourSubstationsNodeBreakerFactory(FourSubstationsNodeBreakerFactory&&) noexcept = delete;

    ~FourSubstationsNodeBreakerFactory() = delete;

    FourSubstationsNodeBreakerFactory& operator=(const FourSubstationsNodeBreakerFactory&) = delete;

    FourSubstationsNodeBreakerFactory& operator=(FourSubstationsNodeBreakerFactory&&) noexcept = delete;
};

}  // namespace network

}  // namespace powsybl

#endif  // POWSYBL_NETWORK_FOURSUBSTATIONSNODEBREAKERFACTORY_HPP
