/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_CONSTANTS_HPP
#define POWSYBL_IIDM_CONVERTER_CONSTANTS_HPP

namespace powsybl {

namespace iidm {

namespace converter {

static const char* const ACCEPTABLE_DURATION = "acceptableDuration";
static const char* const ACTIVE_POWER_SETPOINT = "activePowerSetpoint";
static const char* const ALPHA = "alpha";
static const char* const ANGLE = "angle";
static const char* const B = "b";
static const char* const B_MIN = "bMin";
static const char* const B_MAX = "bMax";
static const char* const B_PER_SECTION = "bPerSection";
static const char* const B1 = "b1";
static const char* const B1_ = "b1_";
static const char* const B2 = "b2";
static const char* const B2_ = "b2_";
static const char* const BATTERY = "battery";
static const char* const BUS = "bus";
static const char* const BUS1 = "bus1";
static const char* const BUS2 = "bus2";
static const char* const BUS_BREAKER_TOPOLOGY = "busBreakerTopology";
static const char* const BUSBAR_SECTION = "busbarSection";
static const char* const CASE_DATE = "caseDate";
static const char* const CONNECTABLE_BUS = "connectableBus";
static const char* const CONNECTABLE_BUS1 = "connectableBus1";
static const char* const CONNECTABLE_BUS2 = "connectableBus2";
static const char* const CONVERTER_STATION1 = "converterStation1";
static const char* const CONVERTER_STATION2 = "converterStation2";
static const char* const CONVERTERS_MODE = "convertersMode";
static const char* const COUNTRY = "country";
static const char* const CURRENT_LIMITS = "currentLimits";
static const char* const CURRENT_LIMITS1 = "currentLimits1";
static const char* const CURRENT_LIMITS2 = "currentLimits2";
static const char* const CURRENT_LIMITS3 = "currentLimits3";
static const char* const CURRENT_SECTION_COUNT = "currentSectionCount";
static const char* const DANGLING_LINE = "danglingLine";
static const char* const ENERGY_SOURCE = "energySource";
static const char* const FICTITIOUS = "fictitious";
static const char* const FORECAST_DISTANCE = "forecastDistance";
static const char* const G = "g";
static const char* const G1 = "g1";
static const char* const G1_ = "g1_";
static const char* const G2 = "g2";
static const char* const G2_ = "g2_";
static const char* const GENERATOR = "generator";
static const char* const GEOGRAPHICAL_TAGS = "geographicalTags";
static const char* const HIGH_VOLTAGE_LIMIT = "highVoltageLimit";
static const char* const HVDC_LINE = "hvdcLine";
static const char* const ID = "id";
static const char* const ID_ = "id_";
static const char* const IIDM_PREFIX = "iidm";
static const char* const IIDM_URI = "http://www.itesla_project.eu/schema/iidm/1_0";
static const char* const INTERNAL_CONNECTION = "internalConnection";
static const char* const KIND = "kind";
static const char* const LCC_CONVERTER_STATION = "lccConverterStation";
static const char* const LINE = "line";
static const char* const LOAD = "load";
static const char* const LOAD_TAP_CHANGING_CAPABILITIES = "loadTapChangingCapabilities";
static const char* const LOAD_TYPE = "loadType";
static const char* const LOSS_FACTOR = "lossFactor";
static const char* const LOW_TAP_POSITION = "lowTapPosition";
static const char* const LOW_VOLTAGE_LIMIT = "lowVoltageLimit";
static const char* const MAX_P = "maxP";
static const char* const MAX_Q = "maxQ";
static const char* const MAXIMUM_SECTION_COUNT = "maximumSectionCount";
static const char* const MIN_MAX_REACTIVE_LIMITS = "minMaxReactiveLimits";
static const char* const MIN_P = "minP";
static const char* const MIN_Q = "minQ";
static const char* const NAME = "name";
static const char* const NAME_ = "name_";
static const char* const NETWORK = "network";
static const char* const NODE = "node";
static const char* const NODE_BREAKER_TOPOLOGY = "nodeBreakerTopology";
static const char* const NODE_COUNT = "nodeCount";
static const char* const NODE1 = "node1";
static const char* const NODE2 = "node2";
static const char* const NOMINAL_V = "nominalV";
static const char* const OPEN = "open";
static const char* const P = "p";
static const char* const P0 = "p0";
static const char* const PERMANENT_LIMIT = "permanentLimit";
static const char* const PHASE_TAP_CHANGER = "phaseTapChanger";
static const char* const POINT = "point";
static const char* const POWER_FACTOR = "powerFactor";
static const char* const PROPERTY = "property";
static const char* const Q = "q";
static const char* const Q0 = "q0";
static const char* const R = "r";
static const char* const R_ = "r_";
static const char* const R1 = "r1";
static const char* const R2 = "r2";
static const char* const R3 = "r3";
static const char* const RATED_S = "ratedS";
static const char* const RATED_U1 = "ratedU1";
static const char* const RATED_U2 = "ratedU2";
static const char* const RATED_U3 = "ratedU3";
static const char* const RATIO_TAP_CHANGER = "ratioTapChanger";
static const char* const RATIO_TAP_CHANGER2 = "ratioTapChanger2";
static const char* const RATIO_TAP_CHANGER3 = "ratioTapChanger3";
static const char* const REACTIVE_CAPABILITY_CURVE = "reactiveCapabilityCurve";
static const char* const REACTIVE_POWER_SET_POINT = "reactivePowerSetPoint";
static const char* const REACTIVE_POWER_SETPOINT = "reactivePowerSetpoint";
static const char* const REGULATING = "regulating";
static const char* const REGULATION_MODE = "regulationMode";
static const char* const REGULATING_TERMINAL = "regulatingTerminal";
static const char* const REGULATION_VALUE = "regulationValue";
static const char* const RETAINED = "retained";
static const char* const RHO = "rho";
static const char* const SHUNT = "shunt";
static const char* const SIDE = "side";
static const char* const SOURCE_FORMAT = "sourceFormat";
static const char* const STATIC_VAR_COMPENSATOR = "staticVarCompensator";
static const char* const STEP = "step";
static const char* const SUBSTATION = "substation";
static const char* const SWITCH = "switch";
static const char* const TAP_POSITION = "tapPosition";
static const char* const TARGET_DEADBAND = "targetDeadband";
static const char* const TARGET_P = "targetP";
static const char* const TARGET_Q = "targetQ";
static const char* const TARGET_V = "targetV";
static const char* const TEMPORARY_LIMIT = "temporaryLimit";
static const char* const TERMINAL_REF = "terminalRef";
static const char* const THREE_WINDINGS_TRANSFORMER = "threeWindingsTransformer";
static const char* const TIE_LINE = "tieLine";
static const char* const TOPOLOGY_KIND = "topologyKind";
static const char* const TSO = "tso";
static const char* const TWO_WINDINGS_TRANSFORMER = "twoWindingsTransformer";
static const char* const UCTE_XNODE_CODE = "ucteXnodeCode";
static const char* const V = "v";
static const char* const VALUE = "value";
static const char* const VOLTAGE_LEVEL = "voltageLevel";
static const char* const VOLTAGE_LEVEL_ID = "voltageLevelId";
static const char* const VOLTAGE_LEVEL_ID1 = "voltageLevelId1";
static const char* const VOLTAGE_LEVEL_ID2 = "voltageLevelId2";
static const char* const VOLTAGE_REGULATOR_ON = "voltageRegulatorOn";
static const char* const VOLTAGE_SET_POINT = "voltageSetPoint";
static const char* const VOLTAGE_SETPOINT = "voltageSetpoint";
static const char* const VSC_CONVERTER_STATION = "vscConverterStation";
static const char* const X = "x";
static const char* const X_ = "x_";
static const char* const X1 = "x1";
static const char* const X2 = "x2";
static const char* const X3 = "x3";
static const char* const XNODE_P_ = "xnodeP_";
static const char* const XNODE_Q_ = "xnodeQ_";

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_CONSTANTS_HPP

