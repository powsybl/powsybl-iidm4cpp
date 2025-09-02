/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LINE_HPP
#define POWSYBL_IIDM_LINE_HPP

#include <powsybl/iidm/AbstractConnectableBranch.hpp>
#include <powsybl/iidm/LineCharacteristics.hpp>

namespace powsybl {

namespace iidm {

class Line : public AbstractConnectableBranch {
public:  // Identifiable
    const IdentifiableType& getType() const override;

public:
    Line(const std::string& id, const std::string& name, bool fictitious, double r, double x, double g1, double b1, double g2, double b2);

    ~Line() noexcept override = default;

    double getB1() const;

    double getB2() const;

    double getG1() const;

    double getG2() const;

    double getR() const;

    double getX() const;

    Line& setB1(double b1);

    Line& setB2(double b2);

    Line& setG1(double g1);

    Line& setG2(double g2);

    Line& setR(double r);

    Line& setX(double x);

protected:
    Line(const std::string& id, const std::string& name, bool fictitious);

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    LineCharacteristics m_lineCharacteristics;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LINE_HPP
