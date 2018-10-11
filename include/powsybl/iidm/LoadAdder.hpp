/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOADADDER_HPP
#define POWSYBL_IIDM_LOADADDER_HPP

#include <powsybl/iidm/InjectionAdder.hpp>
#include <powsybl/iidm/LoadType.hpp>

namespace powsybl {

namespace iidm {

class Load;
class Network;
class VoltageLevel;

class LoadAdder : public InjectionAdder<LoadAdder> {
public:
    ~LoadAdder() noexcept override = default;

    Load& add();

    LoadAdder& setLoadType(const LoadType& loadType);

    LoadAdder& setP0(double p0);

    LoadAdder& setQ0(double q0);

protected: // IdentifiableAdder
    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit LoadAdder(VoltageLevel& voltageLevel);

    friend class VoltageLevel;

private:
    VoltageLevel& m_voltageLevel;

    LoadType m_loadType;

    double m_p0;

    double m_q0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOADADDER_HPP
