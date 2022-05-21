#pragma once
#include "pin_function_map.h"

namespace RODOS::pin_config {

template <char port, uint8_t pin>
static constexpr VA41620_PIN P { static_cast<int8_t>((port - 'A') * 16 + pin) };

constexpr pin_config::PIN_FUNCTION FUN0 = pin_config::PIN_FUNCTION::FUN0;
constexpr pin_config::PIN_FUNCTION FUN1 = pin_config::PIN_FUNCTION::FUN1;
constexpr pin_config::PIN_FUNCTION FUN2 = pin_config::PIN_FUNCTION::FUN2;
constexpr pin_config::PIN_FUNCTION FUN3 = pin_config::PIN_FUNCTION::FUN3;

constexpr pin_config::SPI_NSS_INDEX SS0 = pin_config::SPI_NSS_INDEX::SS0;
constexpr pin_config::SPI_NSS_INDEX SS1 = pin_config::SPI_NSS_INDEX::SS1;
constexpr pin_config::SPI_NSS_INDEX SS2 = pin_config::SPI_NSS_INDEX::SS2;
constexpr pin_config::SPI_NSS_INDEX SS3 = pin_config::SPI_NSS_INDEX::SS3;
constexpr pin_config::SPI_NSS_INDEX SS4 = pin_config::SPI_NSS_INDEX::SS4;
constexpr pin_config::SPI_NSS_INDEX SS5 = pin_config::SPI_NSS_INDEX::SS5;
constexpr pin_config::SPI_NSS_INDEX SS6 = pin_config::SPI_NSS_INDEX::SS6;
constexpr pin_config::SPI_NSS_INDEX SS7 = pin_config::SPI_NSS_INDEX::SS7;

constexpr PinMap UART0_PIN_MAP{{
    {P<'A', 0>, FUN3},
    {P<'A', 1>, FUN3},
    {P<'A', 2>, FUN3},
    {P<'A', 3>, FUN3},

    {P<'C', 2>, FUN2},
    {P<'C', 3>, FUN2},
    {P<'C', 4>, FUN2},
    {P<'C', 5>, FUN2},

    {P<'E', 0>, FUN3},
    {P<'E', 1>, FUN3},
    {P<'E', 2>, FUN3},
    {P<'E', 3>, FUN3},

    {P<'F', 14>, FUN1},
    {P<'F', 15>, FUN1},
    {P<'G', 0>, FUN1},
    {P<'G', 1>, FUN1},
}};

constexpr PinMap UART1_PIN_MAP{{
    {P<'B', 12>, FUN3},
    {P<'B', 13>, FUN3},
    {P<'B', 14>, FUN3},
    {P<'B', 15>, FUN3},

    {P<'D', 9>, FUN3},
    {P<'D', 10>, FUN3},
    {P<'D', 11>, FUN3},
    {P<'D', 12>, FUN3},

    {P<'F', 10>, FUN1},
    {P<'F', 11>, FUN1},
    {P<'F', 12>, FUN1},
    {P<'F', 13>, FUN1},
}};

constexpr PinMap UART2_PIN_MAP{{
    {P<'C', 12>, FUN2},
    {P<'C', 13>, FUN2},
    {P<'C', 14>, FUN2},
    {P<'C', 15>, FUN2},

    {P<'F', 6>, FUN1},
    {P<'F', 7>, FUN1},
    {P<'F', 8>, FUN1},
    {P<'F', 9>, FUN1},
}};

constexpr PinMapCollection<3> UART_PIN_MAPS {
    UART0_PIN_MAP,
    UART1_PIN_MAP,
    UART2_PIN_MAP
};



constexpr PinMap SPI0_PIN_MAP {{
    {P<'B', 15>, FUN1},
    {P<'C',  0>, FUN1},
    {P<'C',  1>, FUN1},
}};

constexpr PinMap SPI1_PIN_MAP {{
    {P<'B',  8>, FUN3},
    {P<'B',  9>, FUN3},
    {P<'B', 10>, FUN3},

    {P<'C',  9>, FUN2},
    {P<'C', 10>, FUN2},
    {P<'C', 11>, FUN2},

    {P<'E', 13>, FUN2},
    {P<'E', 14>, FUN2},
    {P<'E', 15>, FUN2},

    {P<'F',  3>, FUN1},
    {P<'F',  4>, FUN1},
    {P<'F',  5>, FUN1},

    {P<'G',  2>, FUN2},
    {P<'G',  3>, FUN2},
    {P<'G',  4>, FUN2},
}};

constexpr PinMap SPI2_PIN_MAP {{
    {P<'A',  5>, FUN2},
    {P<'A',  6>, FUN2},
    {P<'A',  7>, FUN2},

    {P<'F',  5>, FUN2},
    {P<'F',  6>, FUN2},
    {P<'F',  7>, FUN2},
}};

constexpr PinMapCollection<3> SPI_PIN_MAPS {
    SPI0_PIN_MAP,
    SPI1_PIN_MAP,
    SPI2_PIN_MAP,
};


constexpr NssPinMap<4> SPI0_NSS_MAP {{
    {P<'B', 11>, {FUN1, SS3}},
    {P<'B', 12>, {FUN1, SS2}},
    {P<'B', 13>, {FUN1, SS1}},
    {P<'B', 14>, {FUN1, SS0}},
}};

constexpr NssPinMap<23> SPI1_NSS_MAP {{
    {P<'B',  0>, {FUN3, SS7}},
    {P<'B',  1>, {FUN3, SS6}},
    {P<'B',  2>, {FUN3, SS5}},
    {P<'B',  3>, {FUN3, SS4}},
    {P<'B',  4>, {FUN3, SS3}},
    {P<'B',  5>, {FUN3, SS2}},
    {P<'B',  6>, {FUN3, SS1}},
    {P<'B',  7>, {FUN3, SS0}},

    {P<'C',  7>, {FUN2, SS1}},
    {P<'C',  8>, {FUN2, SS0}},

    {P<'E',  5>, {FUN3, SS7}},
    {P<'E',  6>, {FUN3, SS6}},
    {P<'E',  7>, {FUN3, SS5}},
    {P<'E',  8>, {FUN2, SS4}},
    {P<'E',  9>, {FUN2, SS3}},
    {P<'E', 10>, {FUN2, SS2}},
    {P<'E', 11>, {FUN2, SS1}},
    {P<'E', 12>, {FUN2, SS0}},

    {P<'F',  2>, {FUN1, SS0}},

    {P<'G',  2>, {FUN2, SS0}},

    {P<'G',  4>, {FUN1, SS3}},
    {P<'G',  5>, {FUN1, SS2}},
    {P<'G',  6>, {FUN1, SS1}},
}};

constexpr NssPinMap<12> SPI2_NSS_MAP {{
    {P<'A',  0>, {FUN2, SS4}},
    {P<'A',  1>, {FUN2, SS3}},
    {P<'A',  2>, {FUN2, SS2}},
    {P<'A',  3>, {FUN2, SS1}},
    {P<'A',  4>, {FUN2, SS0}},

    {P<'A',  8>, {FUN2, SS6}},
    {P<'A',  9>, {FUN2, SS5}},

    {P<'F',  0>, {FUN2, SS4}},
    {P<'F',  1>, {FUN2, SS3}},
    {P<'F',  2>, {FUN2, SS2}},
    {P<'F',  3>, {FUN2, SS1}},
    {P<'F',  4>, {FUN2, SS0}},
}};

constexpr PinMapCollection<3, NssPinMapping> SPI_NSS_MAPS {
    SPI0_NSS_MAP,
    SPI1_NSS_MAP,
    SPI2_NSS_MAP,
};

} // namespace RODOS::pinmap
