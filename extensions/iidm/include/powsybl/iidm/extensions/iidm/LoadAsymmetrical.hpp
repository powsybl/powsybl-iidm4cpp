/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_LOADASYMMETRICAL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_LOADASYMMETRICAL_HPP

#include <powsybl/iidm/Extension.hpp>

#include <powsybl/iidm/extensions/iidm/LoadConnectionType.hpp>

namespace powsybl {

namespace iidm {

class Load;

namespace extensions {

namespace iidm {

class LoadAsymmetrical : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

    LoadAsymmetrical(Load& load, LoadConnectionType connectionType, double deltaPa, double deltaQa, double deltaPb, double deltaQb, double deltaPc, double deltaQc);
    
    double getDeltaPa() const;
    double getDeltaQa() const;
    double getDeltaPb() const;
    double getDeltaQb() const;
    double getDeltaPc() const;
    double getDeltaQc() const;
    LoadConnectionType getConnectionType() const;

    LoadAsymmetrical& setDeltaPa(double deltaPa);
    LoadAsymmetrical& setDeltaQa(double deltaQa);
    LoadAsymmetrical& setDeltaPb(double deltaPb);
    LoadAsymmetrical& setDeltaQb(double deltaQb);
    LoadAsymmetrical& setDeltaPc(double deltaPc);
    LoadAsymmetrical& setDeltaQc(double deltaQc);
    LoadAsymmetrical& setConnectionType(LoadConnectionType connectionType);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

    LoadConnectionType m_connectionType;

    double m_deltaPa;
    double m_deltaQa;
    double m_deltaPb;
    double m_deltaQb;
    double m_deltaPc;
    double m_deltaQc;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_LOADASYMMETRICAL_HPP

