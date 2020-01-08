/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_GENERATORSHORTCIRCUITS_HPP
#define POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_GENERATORSHORTCIRCUITS_HPP

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/extensions/shortcircuits/AbstractShortCircuits.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace shortcircuits {

class GeneratorShortCircuits : public AbstractShortCircuits {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    GeneratorShortCircuits(Generator& generator, double transientReactance, double stepUpTransformerReactance);

    ~GeneratorShortCircuits() noexcept override = default;

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;
};

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_GENERATORSHORTCIRCUITS_HPP

