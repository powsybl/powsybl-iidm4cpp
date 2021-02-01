/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_HVDCCONVERTERSTATION_HPP
#define POWSYBL_IIDM_HVDCCONVERTERSTATION_HPP

#include <powsybl/iidm/Injection.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class HvdcLine;

class HvdcConverterStation : public Injection {
public:
    enum class HvdcType {
        VSC,
        LCC
    };

public:
    ~HvdcConverterStation() noexcept override = default;

    stdcxx::CReference<HvdcLine> getHvdcLine() const;

    stdcxx::Reference<HvdcLine> getHvdcLine();

    virtual HvdcType getHvdcType() const = 0;

    double getLossFactor() const;

    virtual HvdcConverterStation& setLossFactor(double lossFactor);

protected:
    HvdcConverterStation(const std::string& id, const std::string& name, bool fictitious, double lossFactor);

private:
    void setHvdcLine(const stdcxx::Reference<HvdcLine>& hvdcLine);

    friend class HvdcLine;

private:
    double m_lossFactor;

    stdcxx::Reference<HvdcLine> m_hvdcLine;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_HVDCCONVERTERSTATION_HPP
