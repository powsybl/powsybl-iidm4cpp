/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/shortcircuits/GeneratorShortCircuits.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace shortcircuits {

GeneratorShortCircuits::GeneratorShortCircuits(Generator& generator, double transientReactance, double stepUpTransformerReactance) :
    AbstractShortCircuits(generator, transientReactance, stepUpTransformerReactance) {

}

void GeneratorShortCircuits::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Generator>(extendable.get())) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Generator>()));
    }
}

const std::string& GeneratorShortCircuits::getName() const {
    static std::string s_name = "generatorShortCircuits";
    return s_name;
}

const std::type_index& GeneratorShortCircuits::getType() const {
    static std::type_index s_type = typeid(GeneratorShortCircuits);
    return s_type;
}

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

