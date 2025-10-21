/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SLD_OPERATINGSTATUS_HPP
#define POWSYBL_IIDM_EXTENSIONS_SLD_OPERATINGSTATUS_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Identifiable;

namespace extensions {

namespace sld {

class OperatingStatus : public Extension {
public:
    enum class Status : unsigned char {
        IN_OPERATION,
        PLANNED_OUTAGE,
        FORCED_OUTAGE
    };

public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    const Status& getStatus() const;

    OperatingStatus& setStatus(const Status& status);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    OperatingStatus(Identifiable& identifiable, const Status& status);

    bool isAllowedIdentifiable(Identifiable& identifiable);

    friend class OperatingStatusAdder;

private:
    Status m_status;
};

std::ostream& operator<<(std::ostream& stream, const OperatingStatus::Status& status);

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SLD_OPERATINGSTATUS_HPP
