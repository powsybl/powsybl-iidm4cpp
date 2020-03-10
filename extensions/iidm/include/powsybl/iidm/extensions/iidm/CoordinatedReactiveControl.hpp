/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_COORDINATEDREACTIVECONTROL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_COORDINATEDREACTIVECONTROL_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/extensions/iidm/IidmExport.hpp>

namespace powsybl {

namespace iidm {

class Generator;

namespace extensions {

namespace iidm {

class EXT_IIDM_EXPORT CoordinatedReactiveControl : public Extension {
public:  // Extension
    const std::string &getName() const override;

    const std::type_index &getType() const override;

public:
    CoordinatedReactiveControl(Generator &generator, double qPercent);

    ~CoordinatedReactiveControl() noexcept override = default;

    double getQPercent() const;

    CoordinatedReactiveControl &setQPercent(double qPercent);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable> &extendable) const override;

private:
    static double checkQPercent(double qPercent);

private:
    double m_qPercent;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_COORDINATEDREACTIVECONTROL_HPP

