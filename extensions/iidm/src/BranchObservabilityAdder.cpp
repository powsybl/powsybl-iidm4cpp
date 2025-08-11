/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/BranchObservabilityAdder.hpp>

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/extensions/iidm/BranchObservability.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BranchObservabilityAdder::BranchObservabilityAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
    m_redundantP1.reset();
    m_redundantP2.reset();
    m_redundantQ1.reset();
    m_redundantQ2.reset();
}

std::unique_ptr<Extension> BranchObservabilityAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Branch>(extendable)) {
        auto extension = std::unique_ptr<BranchObservability>(new BranchObservability(dynamic_cast<Branch&>(extendable), m_observable));
        if (!std::isnan(m_standardDeviationP1)) {
            extension->setQualityP1(m_standardDeviationP1, m_redundantP1);
        }
        if (!std::isnan(m_standardDeviationP2)) {
            extension->setQualityP2(m_standardDeviationP2, m_redundantP2);
        }
        if (!std::isnan(m_standardDeviationQ1)) {
            extension->setQualityQ1(m_standardDeviationQ1, m_redundantQ1);
        }
        if (!std::isnan(m_standardDeviationQ2)) {
            extension->setQualityQ2(m_standardDeviationQ2, m_redundantQ2);
        }
        return extension;
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Branch>()));
}

BranchObservabilityAdder& BranchObservabilityAdder::withObservable(bool observable) {
    m_observable = observable;

    return *this;
}

BranchObservabilityAdder& BranchObservabilityAdder::withRedundantP1(bool redundant) {
    m_redundantP1 = redundant;

    return *this;
}

BranchObservabilityAdder& BranchObservabilityAdder::withRedundantP2(bool redundant) {
    m_redundantP2 = redundant;

    return *this;
}

BranchObservabilityAdder& BranchObservabilityAdder::withRedundantQ1(bool redundant) {
    m_redundantQ1 = redundant;

    return *this;
}

BranchObservabilityAdder& BranchObservabilityAdder::withRedundantQ2(bool redundant) {
    m_redundantQ2 = redundant;

    return *this;
}

BranchObservabilityAdder& BranchObservabilityAdder::withStandardDeviationP1(double standardDeviation) {
    m_standardDeviationP1 = standardDeviation;

    return *this;
}

BranchObservabilityAdder& BranchObservabilityAdder::withStandardDeviationP2(double standardDeviation) {
    m_standardDeviationP2 = standardDeviation;

    return *this;
}

BranchObservabilityAdder& BranchObservabilityAdder::withStandardDeviationQ1(double standardDeviation) {
    m_standardDeviationQ1 = standardDeviation;

    return *this;
}

BranchObservabilityAdder& BranchObservabilityAdder::withStandardDeviationQ2(double standardDeviation) {
    m_standardDeviationQ2 = standardDeviation;

    return *this;
}


}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
