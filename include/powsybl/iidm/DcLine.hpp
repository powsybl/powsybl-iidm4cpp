/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCLINE_HPP
#define POWSYBL_IIDM_DCLINE_HPP

#include <powsybl/iidm/DcConnectable.hpp>

namespace powsybl {

namespace iidm {

class DcLine : public DcConnectable {

public:
    DcLine(const std::string& id, const std::string& name, bool fictitious, double r);
    ~DcLine() noexcept override = default;

public:  // Identifiable
    const IdentifiableType& getType() const override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

public:
    const DcTerminal& getDcTerminal1() const;
    DcTerminal& getDcTerminal1();

    const DcTerminal& getDcTerminal2() const;
    DcTerminal& getDcTerminal2();

    const DcTerminal& getDcTerminal(const TwoSides& side) const;
    DcTerminal& getDcTerminal(const TwoSides& side);

    TwoSides getSide(const DcTerminal& terminal) const;

    double getR() const;
    DcLine& setR(double r);

private:
    double m_r;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCLINE_HPP
