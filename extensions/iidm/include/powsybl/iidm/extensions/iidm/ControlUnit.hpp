/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLUNIT_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLUNIT_HPP

#include <string>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ControlUnit {
public:
    ControlUnit(const std::string& id, bool participate = true);

    ControlUnit() = delete;
    ControlUnit(const ControlUnit& coord) = default;
    ControlUnit(ControlUnit&&) noexcept = default;
    ~ControlUnit() noexcept = default;
    ControlUnit& operator=(const ControlUnit&) = default;
    ControlUnit& operator=(ControlUnit&&) noexcept = default;

    const std::string& getId() const;

    bool isParticipate() const;
    void setParticipate(bool participate);

private:

    std::string m_id;

    bool m_participate;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLUNIT_HPP
