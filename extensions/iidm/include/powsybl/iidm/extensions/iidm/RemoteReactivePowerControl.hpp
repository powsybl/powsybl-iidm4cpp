/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_REMOTEREACTIVEPOWERCONTROL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_REMOTEREACTIVEPOWERCONTROL_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Generator;
class Terminal;

namespace extensions {

namespace iidm {

class RemoteReactivePowerControlAdder;

class RemoteReactivePowerControl : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    ~RemoteReactivePowerControl() noexcept override = default;

    const Terminal& getRegulatingTerminal() const;

    Terminal& getRegulatingTerminal();

    double getTargetQ() const;

    bool isEnabled() const;

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    RemoteReactivePowerControl(Generator& generator, double targetQ, Terminal& terminal, bool enabled);

    friend RemoteReactivePowerControlAdder;

private:
    double m_targetQ;

    std::reference_wrapper<Terminal> m_regulatingTerminal;

    bool m_enabled;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_REMOTEREACTIVEPOWERCONTROL_HPP
