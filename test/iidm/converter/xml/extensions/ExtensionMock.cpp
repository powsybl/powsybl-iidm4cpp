/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <memory>
#include <vector>

#include <boost/dll/alias.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

#include "LoadBarXmlSerializer.hpp"
#include "LoadFooXmlSerializer.hpp"
#include "LoadMockExtXmlSerializer.hpp"
#include "LoadQuxXmlSerializer.hpp"
#include "TerminalMockXmlSerializer.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

std::vector<std::unique_ptr<ExtensionProvider>> create() {
    std::vector<std::unique_ptr<ExtensionProvider>> serializers;
    serializers.emplace_back(stdcxx::make_unique<LoadBarXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<LoadFooXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<LoadMockExtXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<LoadQuxXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<TerminalMockXmlSerializer>());

    return serializers;
}

// NOLINTNEXTLINE BOOST_DLL_ALIAS uses reinterpret_cast
BOOST_DLL_ALIAS(
    create,
    createSerializers
)

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
