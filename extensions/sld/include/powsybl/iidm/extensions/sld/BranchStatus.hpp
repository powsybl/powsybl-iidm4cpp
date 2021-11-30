/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SLD_BRANCHSTATUS_HPP
#define POWSYBL_IIDM_EXTENSIONS_SLD_BRANCHSTATUS_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Connectable;

namespace extensions {

namespace sld {

class BranchStatus : public Extension {
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

    BranchStatus& setStatus(const Status& status);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    BranchStatus(Connectable& connectable, const Status& status);

    friend class BranchStatusAdder;

private:
    Status m_status;
};

std::ostream& operator<<(std::ostream& stream, const BranchStatus::Status& status);

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SLD_BRANCHSTATUS_HPP
