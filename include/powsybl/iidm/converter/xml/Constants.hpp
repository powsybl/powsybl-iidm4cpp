/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_CONSTANTS_HPP
#define POWSYBL_IIDM_CONVERTER_XML_CONSTANTS_HPP

#include <string>

#include <boost/optional.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

static const char* const ANONYMISED = "iidm.export.xml.anonymised";
static const char* const EXTENSIONS_LIST = "iidm.export.xml.extensions";
static const char* const INDENT = "iidm.export.xml.indent";
static const char* const ONLY_MAIN_CC = "iidm.export.xml.only-main-cc";
static const char* const THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND = "iidm.export.xml.throw-exception-if-extension-not-found";
static const char* const TOPOLOGY_LEVEL = "iidm.export.xml.topology-level";
static const char* const VERSION = "iidm.export.xml.version";
static const char* const WITH_BRANCH_STATE_VARIABLES = "iidm.export.xml.with-branch-state-variables";

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_CONSTANTS_HPP

