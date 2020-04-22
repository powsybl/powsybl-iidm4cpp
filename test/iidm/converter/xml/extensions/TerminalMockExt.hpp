/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_TERMINALMOCKEXT_HPP
#define POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_TERMINALMOCKEXT_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Extendable;
class Load;
class Terminal;

namespace converter {

namespace xml {

namespace extensions {

class TerminalMockExt : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    explicit TerminalMockExt(Load& load);

    ~TerminalMockExt() override = default;

    const Terminal& getTerminal() const;

    TerminalMockExt& setTerminal(const Terminal& terminal);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    stdcxx::CReference<Terminal> m_terminal;
};

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
#endif  // POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_TERMINALMOCKEXT_HPP
