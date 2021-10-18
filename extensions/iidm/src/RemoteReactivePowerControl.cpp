/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/RemoteReactivePowerControl.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

RemoteReactivePowerControl::RemoteReactivePowerControl(Generator& generator, double targetQ, const stdcxx::Reference<Terminal>& terminal, bool enabled) :
    Extension(generator), m_targetQ(targetQ), m_regulatingTerminal(terminal), m_enabled(enabled) {
}

void RemoteReactivePowerControl::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Generator>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Generator>()));
    }
}

const std::string& RemoteReactivePowerControl::getName() const {
    static std::string s_name = "generatorRemoteReactivePowerControl";
    return s_name;
}

stdcxx::CReference<Terminal> RemoteReactivePowerControl::getRegulatingTerminal() const {
    return stdcxx::cref(m_regulatingTerminal);
}

stdcxx::Reference<Terminal> RemoteReactivePowerControl::getRegulatingTerminal() {
    return m_regulatingTerminal;
}

double RemoteReactivePowerControl::getTargetQ() const {
    return m_targetQ;
}

const std::type_index& RemoteReactivePowerControl::getType() const {
    static std::type_index s_type = typeid(RemoteReactivePowerControl);
    return s_type;
}

bool RemoteReactivePowerControl::isEnabled() const {
    return m_enabled;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
