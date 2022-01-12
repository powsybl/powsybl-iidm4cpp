/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_INJECTIONOBSERVABILITYADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_INJECTIONOBSERVABILITYADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class InjectionObservabilityAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit InjectionObservabilityAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    InjectionObservabilityAdder(const InjectionObservabilityAdder&) = default;

    /**
     * Move constructor
     */
    InjectionObservabilityAdder(InjectionObservabilityAdder&&) noexcept = default;

    /**
     * Destructor
     */
    ~InjectionObservabilityAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    InjectionObservabilityAdder& operator=(const InjectionObservabilityAdder&) = delete;

    /**
     * Move assignment operator
     */
    InjectionObservabilityAdder& operator=(InjectionObservabilityAdder&&) = delete;

    InjectionObservabilityAdder& withObservable(bool observable);

    InjectionObservabilityAdder& withRedundantP(bool redundant);

    InjectionObservabilityAdder& withRedundantQ(bool redundant);

    InjectionObservabilityAdder& withRedundantV(bool redundant);

    InjectionObservabilityAdder& withStandardDeviationP(double standardDeviation);

    InjectionObservabilityAdder& withStandardDeviationQ(double standardDeviation);

    InjectionObservabilityAdder& withStandardDeviationV(double standardDeviation);

private:  // ExtensionAdder
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    bool m_observable = false;

    stdcxx::optional<double> m_standardDeviationP;

    stdcxx::optional<double> m_standardDeviationQ;

    stdcxx::optional<double> m_standardDeviationV;

    stdcxx::optional<bool> m_redundantP;

    stdcxx::optional<bool> m_redundantQ;

    stdcxx::optional<bool> m_redundantV;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_INJECTIONOBSERVABILITYADDER_HPP
