/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_CVG_HVDCOPERATORACTIVEPOWERRANGE_HPP
#define POWSYBL_IIDM_EXTENSIONS_CVG_HVDCOPERATORACTIVEPOWERRANGE_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class HvdcConverterStation;
class HvdcLine;

namespace extensions {

namespace cvg {

class HvdcOperatorActivePowerRange : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    HvdcOperatorActivePowerRange(HvdcLine& hvdcLine, double oprFromCS1toCS2, double oprFromCS2toCS1);

    double getOprFromCS1toCS2() const;

    double getOprFromCS2toCS1() const;

    HvdcOperatorActivePowerRange& setOprFromCS1toCS2(double oprFromCS1toCS2);

    HvdcOperatorActivePowerRange& setOprFromCS2toCS1(double oprFromCS2toCS1);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    double checkOPR(double opr, const HvdcConverterStation& from, const HvdcConverterStation& to);

private:
    /**
     * Operator active power range from the converter station 1 to the converter station 2 in MW.
     */
    double m_oprFromCS1toCS2;

    /**
     * Operator active power range from the converter station 2 to the converter station 1 in MW.
     */
    double m_oprFromCS2toCS1;
};

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_CVG_HVDCOPERATORACTIVEPOWERRANGE_HPP

