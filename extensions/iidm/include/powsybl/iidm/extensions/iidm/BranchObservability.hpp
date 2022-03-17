/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_BRANCHOBSERVABILITY_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_BRANCHOBSERVABILITY_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/extensions/iidm/Observability.hpp>
#include <powsybl/iidm/extensions/iidm/ObservabilityQuality.hpp>
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Branch;

namespace extensions {

namespace iidm {

class BranchObservability : public Extension, public Observability {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    ~BranchObservability() noexcept override = default;

    stdcxx::CReference<ObservabilityQuality> getQualityP1() const;

    stdcxx::Reference<ObservabilityQuality> getQualityP1();

    stdcxx::CReference<ObservabilityQuality> getQualityP2() const;

    stdcxx::Reference<ObservabilityQuality> getQualityP2();

    stdcxx::CReference<ObservabilityQuality> getQualityQ1() const;

    stdcxx::Reference<ObservabilityQuality> getQualityQ1();

    stdcxx::CReference<ObservabilityQuality> getQualityQ2() const;

    stdcxx::Reference<ObservabilityQuality> getQualityQ2();

    BranchObservability& setQualityP1(double standardDeviation, bool redundant = false);

    BranchObservability& setQualityP2(double standardDeviation, bool redundant = false);

    BranchObservability& setQualityQ1(double standardDeviation, bool redundant = false);

    BranchObservability& setQualityQ2(double standardDeviation, bool redundant = false);

protected:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    BranchObservability(Branch& branch, bool observable);

    friend class BranchObservabilityAdder;

private:
    stdcxx::optional<ObservabilityQuality> m_qualityP1;

    stdcxx::optional<ObservabilityQuality> m_qualityP2;

    stdcxx::optional<ObservabilityQuality> m_qualityQ1;

    stdcxx::optional<ObservabilityQuality> m_qualityQ2;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_BRANCHOBSERVABILITY_HPP
