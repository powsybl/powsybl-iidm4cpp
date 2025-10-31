/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_GROUNDADDER_HPP
#define POWSYBL_IIDM_GROUNDADDER_HPP

#include <powsybl/iidm/InjectionAdder.hpp>

namespace powsybl {

namespace iidm {

class Ground;
class VoltageLevel;

class GroundAdder : public InjectionAdder<Ground, GroundAdder> {
public:
    ~GroundAdder() noexcept override = default;

    Ground& add() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit GroundAdder(VoltageLevel& voltageLevel);

    friend class VoltageLevel;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_GROUNDADDER_HPP
