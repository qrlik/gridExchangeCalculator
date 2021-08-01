#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H
#include "QString"

const auto priceMaskStocks = QStringLiteral("00000.00");
constexpr int precisionStocks = 2;
const auto priceMaskCrypt = QStringLiteral("000000.00000000");
constexpr int precisionCrypto = 8;
const auto taxMask = QStringLiteral("0.000");
constexpr int precisionTax = 3; // in percents

#endif // GLOBALVARIABLES_H
