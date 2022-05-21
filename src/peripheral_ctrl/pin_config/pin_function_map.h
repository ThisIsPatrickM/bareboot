#pragma once

#include "peripheral_ctrl/aborting_error.h"
#include "enums.h"

namespace RODOS::pin_config {

struct PinMapping {
    pin_config::VA41620_PIN pin;
    pin_config::PIN_FUNCTION function;
};

struct NssPinFunction {
    pin_config::PIN_FUNCTION function;
    SPI_NSS_INDEX nssIndex;
};

struct NssPinMapping {
    pin_config::VA41620_PIN pin;
    NssPinFunction function;
};

template <size_t size, typename MapType = PinMapping>
struct PinMap {
    MapType m_table[size];

    constexpr PinMap(const MapType (&mappings)[size])
    : m_table{}
    {
        for(size_t i = 0; i<size; i++){
            m_table[i] = mappings[i];
        }
    }
};

template<size_t size>
using NssPinMap = PinMap<size, NssPinMapping>;

template<typename MapType = PinMapping>
struct GenericPinMapReference {
    const size_t m_mapSize;
    const MapType* const m_mapTable;

    constexpr decltype(MapType::function) getFunction(pin_config::VA41620_PIN pin) const{
        bool foundPin {false};
        for(size_t i { 0 }; i < m_mapSize; i++) {
            if(m_mapTable[i].pin == pin) {
                foundPin = true;
                return m_mapTable[i].function;
            }
        }
        if(!foundPin){
            //gcc compiler bug
            //shouldn't need to be in this if statement but doesn't compile otherwise
            abortingError("invalid pin configuration");
        }
        //never happens because either the pin was found or abortingerror is called
        return {};
    }

    template<size_t size>
    constexpr GenericPinMapReference(const PinMap<size, MapType>& map)
    : m_mapSize{size}
    , m_mapTable{map.m_table}
    {}
};

template<size_t len, typename MapType = PinMapping>
struct PinMapCollection {
    const GenericPinMapReference<MapType> m_pinMaps[len];

    constexpr decltype(MapType::function) getFunction(size_t idx, pin_config::VA41620_PIN pin) const{
        if(idx<len){
            return m_pinMaps[idx].getFunction(pin);
        }
        abortingError("invalid PIN configuration");
    }

    template<size_t... sizes, typename = std::enable_if_t<sizeof...(sizes)==len, void>>
    constexpr PinMapCollection(const PinMap<sizes, MapType>&... pinMaps)
    : m_pinMaps { {pinMaps}... }
    {}
};

}
