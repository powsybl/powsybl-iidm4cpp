/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/InjectionObservabilityAdder.hpp>

#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/extensions/iidm/InjectionObservability.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

InjectionObservabilityAdder::InjectionObservabilityAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {

}

std::unique_ptr<Extension> InjectionObservabilityAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Injection>(extendable)) {
        auto extension = std::unique_ptr<InjectionObservability>(new InjectionObservability(dynamic_cast<Injection&>(extendable), m_observable));
        if (!std::isnan(m_standardDeviationP)) {
            extension->setQualityP(m_standardDeviationP, m_redundantP);
        }
        if (!std::isnan(m_standardDeviationQ)) {
            extension->setQualityQ(m_standardDeviationQ, m_redundantQ);
        }
        if (!std::isnan(m_standardDeviationV)) {
            extension->setQualityV(m_standardDeviationV, m_redundantV);
        }
        return extension;
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Injection>()));
}

InjectionObservabilityAdder& InjectionObservabilityAdder::withObservable(bool observable) {
    m_observable = observable;

    return *this;
}

InjectionObservabilityAdder& InjectionObservabilityAdder::withRedundantP(bool redundant) {
    m_redundantP = redundant;

    return *this;
}

InjectionObservabilityAdder& InjectionObservabilityAdder::withRedundantQ(bool redundant) {
    m_redundantQ = redundant;

    return *this;
}

InjectionObservabilityAdder& InjectionObservabilityAdder::withRedundantV(bool redundant) {
    m_redundantV = redundant;

    return *this;
}

InjectionObservabilityAdder& InjectionObservabilityAdder::withStandardDeviationP(double standardDeviation) {
    m_standardDeviationP = standardDeviation;

    return *this;
}

InjectionObservabilityAdder& InjectionObservabilityAdder::withStandardDeviationQ(double standardDeviation) {
    m_standardDeviationQ = standardDeviation;

    return *this;
}

InjectionObservabilityAdder& InjectionObservabilityAdder::withStandardDeviationV(double standardDeviation) {
    m_standardDeviationV = standardDeviation;

    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
