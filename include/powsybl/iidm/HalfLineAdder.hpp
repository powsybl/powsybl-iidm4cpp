/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_HALFLINEADDER_HPP
#define POWSYBL_IIDM_HALFLINEADDER_HPP

#include <functional>
#include <string>

#include <powsybl/iidm/Validable.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class TieLineAdder;

namespace tie_line {

class HalfLineAdder : public Validable {
public:  // Validable
    std::string getMessageHeader() const override;

public:
    HalfLineAdder(TieLineAdder& parent, unsigned long num);

    TieLineAdder& add();

    HalfLineAdder& setB1(double b1);

    HalfLineAdder& setB2(double b2);

    HalfLineAdder& setFictitious(bool fictitious);

    HalfLineAdder& setG1(double g1);

    HalfLineAdder& setG2(double g2);

    HalfLineAdder& setId(const std::string& id);

    HalfLineAdder& setName(const std::string& name);

    HalfLineAdder& setR(double r);

    HalfLineAdder& setX(double x);

    HalfLineAdder& setXnodeP(double xnodeP);

    HalfLineAdder& setXnodeQ(double xnodeQ);

private:
    friend class iidm::TieLineAdder;

private:
    TieLineAdder& m_parent;

    unsigned long m_num;

    std::string m_id;

    std::string m_name;

    bool m_fictitious = false;

    double m_xnodeP = stdcxx::nan();

    double m_xnodeQ = stdcxx::nan();

    double m_r = stdcxx::nan();

    double m_x = stdcxx::nan();

    double m_g1 = stdcxx::nan();

    double m_g2 = stdcxx::nan();

    double m_b1 = stdcxx::nan();

    double m_b2 = stdcxx::nan();
};

}  // namespace tie_line

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_HALFLINEADDER_HPP
