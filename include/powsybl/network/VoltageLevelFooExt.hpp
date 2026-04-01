/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORK_VOLTAGELEVELFOOEXT_HPP
#define POWSYBL_IIDM_NETWORK_VOLTAGELEVELFOOEXT_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class VoltageLevel;

}  // namespace iidm

namespace network {

class VoltageLevelFooExt : public iidm::Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    explicit VoltageLevelFooExt(iidm::VoltageLevel& vl);

    ~VoltageLevelFooExt() override = default;

private:  // Extension
    void assertExtendable(const stdcxx::Reference<iidm::Extendable>& extendable) const override;
};

}  // namespace network

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORK_VOLTAGELEVELFOOEXT_HPP
