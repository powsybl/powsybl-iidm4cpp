/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "TerminalMockExt.hpp"

#include <powsybl/iidm/Load.hpp>
#include <powsybl/stdcxx/demangle.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

TerminalMockExt::TerminalMockExt(Load& load) :
    Extension(load),
    m_terminal(load.getTerminal()) {
}

void TerminalMockExt::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Load>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Load>()));
    }
}

const std::string& TerminalMockExt::getName() const {
    static std::string s_name = "terminalMock";
    return s_name;
}

const std::type_index& TerminalMockExt::getType() const {
    static std::type_index s_type = typeid(TerminalMockExt);
    return s_type;
}

const Terminal& TerminalMockExt::getTerminal() const {
    return m_terminal.get();
}

TerminalMockExt& TerminalMockExt::setTerminal(const Terminal& terminal) {
    m_terminal = stdcxx::cref(terminal);
    return *this;
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
