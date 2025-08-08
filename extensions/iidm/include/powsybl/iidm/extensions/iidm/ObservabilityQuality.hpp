/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_OBSERVABILITYQUALITY_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_OBSERVABILITYQUALITY_HPP

#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ObservabilityQuality {
public:
    ObservabilityQuality(double standardDeviation);
    ObservabilityQuality(double standardDeviation, bool redundant);

    ~ObservabilityQuality() noexcept = default;

    double getStandardDeviation() const;

    bool isRedundant() const;
    stdcxx::optional<bool> getRedundant() const;

    ObservabilityQuality& setRedundant(bool redundant);

    ObservabilityQuality& setStandardDeviation(double standardDeviation);

private:
    double m_standardDeviation;

    stdcxx::optional<bool> m_redundant;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_OBSERVABILITYQUALITY_HPP
