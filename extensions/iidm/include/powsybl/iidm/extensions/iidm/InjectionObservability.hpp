/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_INJECTIONOBSERVABILITY_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_INJECTIONOBSERVABILITY_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/extensions/iidm/Observability.hpp>
#include <powsybl/iidm/extensions/iidm/ObservabilityQuality.hpp>
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Injection;

namespace extensions {

namespace iidm {

class InjectionObservability : public Extension, public Observability {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    ~InjectionObservability() noexcept override = default;

    stdcxx::CReference<ObservabilityQuality> getQualityP() const;

    stdcxx::Reference<ObservabilityQuality> getQualityP();

    stdcxx::CReference<ObservabilityQuality> getQualityQ() const;

    stdcxx::Reference<ObservabilityQuality> getQualityQ();

    stdcxx::CReference<ObservabilityQuality> getQualityV() const;

    stdcxx::Reference<ObservabilityQuality> getQualityV();

    InjectionObservability& setQualityP(double standardDeviation, const stdcxx::optional<bool>& redundant = stdcxx::optional<bool>());

    InjectionObservability& setQualityQ(double standardDeviation, const stdcxx::optional<bool>& redundant = stdcxx::optional<bool>());

    InjectionObservability& setQualityV(double standardDeviation, const stdcxx::optional<bool>& redundant = stdcxx::optional<bool>());

protected:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    InjectionObservability(Injection& injection, bool observable);

    friend class InjectionObservabilityAdder;

private:
    stdcxx::optional<ObservabilityQuality> m_qualityP;

    stdcxx::optional<ObservabilityQuality> m_qualityQ;

    stdcxx::optional<ObservabilityQuality> m_qualityV;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_INJECTIONOBSERVABILITY_HPP
