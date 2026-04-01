/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorShortCircuit.hpp>

#include <powsybl/iidm/Generator.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorShortCircuit::GeneratorShortCircuit(Generator& generator, double directSubtransX, double directTransX, double stepUpTransformerX) :
    AbstractShortCircuit(generator, directSubtransX, directTransX, stepUpTransformerX) {
}

void GeneratorShortCircuit::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Generator>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Generator>()));
    }
}

const std::string& GeneratorShortCircuit::getName() const {
    static std::string s_name = "generatorShortCircuit";
    return s_name;
}

const std::type_index& GeneratorShortCircuit::getType() const {
    static std::type_index s_type = typeid(GeneratorShortCircuit);
    return s_type;
}


}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
