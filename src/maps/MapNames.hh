#pragma once

#include "util/BLHashMap.hh"
#include "libshape/shapefil.h"
#include "util/FileUtil.hh"

/*
Represents the relationship between a name in a HashMap and
an entity within a geomap.
*/
struct MapEntry {
//    uint32_t nameOffset;  // starting byte of the name;
//    uint8_t len;          // length of the name (< 256 bytes)
    uint8_t entityType;   // type of entity within the map
    uint8_t type;         // feature type of name (could be used to choose font)
    uint32_t offset;      // if entityType=REGION_CONTAINER, then offset into
                          // regionContainers, ...
    // uint32_t displayNameOffset; // possible offset to local name without postal abbr.
    // for now, we know all US postal abbrs are size 2
};

struct NamedMapEntry {
  MapEntry countyEntry;
  MapEntry stateEntry;
  char countyName[24];
  char stateName[21];
};

 enum class EntityType { REGION_CONTAINER, REGION, SEGMENT, POINT };

enum class MapFeatureType {
  LOCAL,
  STATE,
  COUNTRY,
  CITY,
  ROAD,
  TRAIN,
  RIVER
};

const uint8_t FEAT_LOCAL = (uint8_t)MapFeatureType::LOCAL;
const uint8_t FEAT_STATE = (uint8_t)MapFeatureType::STATE;
const uint8_t ENT_COUNTY = (uint8_t)EntityType::REGION;
const uint8_t ENT_STATE = (uint8_t)EntityType::REGION_CONTAINER;