/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_GROUND_HPP
#define POWSYBL_IIDM_GROUND_HPP

#include <powsybl/iidm/Injection.hpp>

namespace powsybl {

namespace iidm {

class Ground : public Injection {
public:  // Identifiable
    const IdentifiableType& getType() const override;

    void setFictitious(bool fictitious) override;


public:
    Ground(const std::string& id, const std::string& name);

    ~Ground() noexcept override = default;

private: // Identifiable
    const std::string& getTypeDescription() const override;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_GROUND_HPP
