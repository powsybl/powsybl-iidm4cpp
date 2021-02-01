/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_HALFLINE_HPP
#define POWSYBL_IIDM_HALFLINE_HPP

#include <string>

#include <powsybl/iidm/LineCharacteristics.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class TieLine;
class TieLineAdder;

namespace tie_line {

class HalfLineAdder;

class HalfLine : public Validable {
public: // Validable
    std::string getMessageHeader() const override;

public:
    double getB1() const;

    double getB2() const;

    double getG1() const;

    double getG2() const;

    const std::string& getId() const;

    const std::string& getName() const;

    double getR() const;

    double getX() const;

    double getXnodeP() const;

    double getXnodeQ() const;

    bool isFictitious() const;

    HalfLine& setB1(double b1);

    HalfLine& setB2(double b2);

    HalfLine& setFictitious(bool fictitious);

    HalfLine& setG1(double g1);

    HalfLine& setG2(double g2);

    HalfLine& setR(double r);

    HalfLine& setX(double x);

    HalfLine& setXnodeP(double xnodeP);

    HalfLine& setXnodeQ(double xnodeQ);

private:
    HalfLine(const std::string& id, const std::string& name, bool fictitious, double xnodeP, double xnodeQ,
             double r, double x, double g1, double b1, double g2, double b2);

    HalfLine(HalfLine&& halfLine) noexcept;

    ~HalfLine() override = default;

    void setParent(TieLine& parent);

    friend class iidm::TieLine;

    friend class iidm::TieLineAdder;

    friend class HalfLineAdder;

private:
    stdcxx::Reference<TieLine> m_parent;

    std::string m_id;

    std::string m_name;

    LineCharacteristics m_lineCharacteristics;

    double m_xnodeP = stdcxx::nan();

    double m_xnodeQ = stdcxx::nan();

    bool m_fictitious = false;
};

}  // namespace tie_line

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_HALFLINE_HPP
