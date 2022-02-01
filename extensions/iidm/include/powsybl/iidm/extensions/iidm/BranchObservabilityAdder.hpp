/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_BRANCHOBSERVABILITYADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_BRANCHOBSERVABILITYADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class BranchObservabilityAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit BranchObservabilityAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    BranchObservabilityAdder(const BranchObservabilityAdder&) = default;

    /**
     * Move constructor
     */
    BranchObservabilityAdder(BranchObservabilityAdder&&) noexcept = default;

    /**
     * Destructor
     */
    ~BranchObservabilityAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    BranchObservabilityAdder& operator=(const BranchObservabilityAdder&) = delete;

    /**
     * Move assignment operator
     */
    BranchObservabilityAdder& operator=(BranchObservabilityAdder&&) = delete;

    BranchObservabilityAdder& withObservable(bool observable);

    BranchObservabilityAdder& withRedundantP1(bool redundant);

    BranchObservabilityAdder& withRedundantP2(bool redundant);

    BranchObservabilityAdder& withRedundantQ1(bool redundant);

    BranchObservabilityAdder& withRedundantQ2(bool redundant);

    BranchObservabilityAdder& withStandardDeviationP1(double standardDeviation);

    BranchObservabilityAdder& withStandardDeviationP2(double standardDeviation);

    BranchObservabilityAdder& withStandardDeviationQ1(double standardDeviation);

    BranchObservabilityAdder& withStandardDeviationQ2(double standardDeviation);

private:  // ExtensionAdder
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    bool m_observable = false;

    double m_standardDeviationP1 = stdcxx::nan();

    double m_standardDeviationP2 = stdcxx::nan();

    double m_standardDeviationQ1 = stdcxx::nan();

    double m_standardDeviationQ2 = stdcxx::nan();

    bool m_redundantP1 = false;

    bool m_redundantP2 = false;

    bool m_redundantQ1 = false;

    bool m_redundantQ2 = false;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_BRANCHOBSERVABILITYADDER_HPP
