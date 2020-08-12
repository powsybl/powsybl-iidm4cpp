/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LINE_HPP
#define POWSYBL_IIDM_LINE_HPP

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/LineCharacteristics.hpp>

namespace powsybl {

namespace iidm {

class Line : public Branch {
public:
    Line(const std::string& id, const std::string& name, bool fictitious, double r, double x, double g1, double b1, double g2, double b2);

    ~Line() noexcept override = default;

    virtual double getB1() const;

    virtual double getB2() const;

    virtual double getG1() const;

    virtual double getG2() const;

    virtual double getR() const;

    virtual double getX() const;

    virtual bool isTieLine() const;

    virtual Line& setB1(double b1);

    virtual Line& setB2(double b2);

    virtual Line& setG1(double g1);

    virtual Line& setG2(double g2);

    virtual Line& setR(double r);

    virtual Line& setX(double x);

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
