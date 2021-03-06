//+---------------------------------------------------------------------------+
//|                                                  MASi_MoneyManagement.mq4 |
//|                                         Copyright 2017, Terentyev Aleksey |
//|                                 https://www.mql5.com/ru/users/terentjew23 |
//+---------------------------------------------------------------------------+
#property copyright     "Copyright 2017, Terentyev Aleksey"
#property link          "https://www.mql5.com/ru/users/terentjew23"
#property description   "Indicator for a money management."
#property version       "1.2"
#property strict

#include                "MASh_Include.mqh"

//---------------------Indicators---------------------------------------------+
#property indicator_chart_window
#property indicator_buffers 2
#property indicator_plots   2
//--- plot ProtectLine
#property indicator_label1  "Protect Buy"
#property indicator_type1   DRAW_LINE
#property indicator_color1  clrDimGray
#property indicator_style1  STYLE_SOLID
#property indicator_width1  1
//--- plot ProtectLine
#property indicator_label2  "Protect Sell"
#property indicator_type2   DRAW_LINE
#property indicator_color2  clrDimGray
#property indicator_style2  STYLE_SOLID
#property indicator_width2  1
//--- indicator buffers
double      ProtectBuyBuffer[];
double      ProtectSellBuffer[];

//+---------------------------------------------------------------------------+
//--
input OptimizationType  OPTIMIZE = Medium;      // Optimization type
input bool              ON_COMMENT = true;      // Enable a risk info
input bool              ON_PROTECT = true;      // Enable a protect lines (stoploss)
input bool              ON_PROFIT = false;      // Enable a profit lines (takeprofit)
input int               RISK = 2;               // Risk per order
input int               CHANNEL = 1000;         // Channel 26 EMA for TP
input double            MFACTOR = 3;            // Factor a mean depression
//--
string          first, second, prefix;
string          currency;
double          leverage;
double          balance, freeMargin;
double          lotMin, lotMax, lotStep;
double          lotSize, lotPriceSecond, lotPriceCurr, lotPriceCurrStd;
double          bidPrice, lotOrderSize;
double          takeProfitPrice, stopLossPrice;
double          takeProfitSize, stopLossSize;
double          riskPriceAcc, riskPriceLot;

//+---------------------------------------------------------------------------+
int OnInit()
{
    tickCount = 0;
    SetIndexBuffer( 0, ProtectBuyBuffer );
    SetIndexBuffer( 1, ProtectSellBuffer );
    SetIndexShift( 0, 1 );
    SetIndexShift( 1, 1 );
    if( !ON_COMMENT && !ON_PROTECT && !ON_PROFIT ) {
        Print( "All parameters disabled." );
        return INIT_PARAMETERS_INCORRECT;
    }
    return INIT_SUCCEEDED;
}

//+---------------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
{
    if( OptimizedSkip(OPTIMIZE) ) {
        return rates_total - 1;
    }
    if( ON_PROTECT ) {
        int limit = rates_total - prev_calculated;
        if( prev_calculated > 0 ) {
            limit++;
        }
        if( limit >= iBars(Symbol(), Period()) - 10 )
            limit -= 10;
        for( int idx = limit; idx >= -1; idx-- ) {
            ProtectBuyBuffer[idx+1] = StopBuyMax( idx, MFACTOR, Symbol(), Period() );
            ProtectSellBuffer[idx+1] = StopSellMin( idx, MFACTOR, Symbol(), Period() );
        }
    }
    if( ON_COMMENT ) {
        InitVariables();
        Comment( "========= Счет ========="
                 "\nПлечо = ",  leverage,
                 "\nБаланс = ", balance, " ", currency,
                 " | Свободная маржа = ", freeMargin, " ", currency,
                 "\n======= Инструмент ======="
                 "\nРазмер 1 лота (базовая) = ",        lotSize, " ", first,
                 "\nРазмер 1 лота (котировка) = ",      lotPriceSecond, " ", second,
                 "\nСтоимость 1 лота (маржа) = ",       lotPriceCurr, " ", currency,
                 "\nПлечо = ",                          leverage,
                 "\nМинимальный/Шаг/Максимальный лот = ", lotMin, "/", lotStep, "/", lotMax,
                 "\n======= Условия сделки (Buy) ======="
                 "\nФиксация прибыли = ",               NormalizeDouble( takeProfitPrice, Digits() ),
                 "\nПредложение = ",                    bidPrice,
                 "\nФиксация убытка = ",                stopLossPrice,
                 "\nОтношение прибыль/убыток = ",       NormalizeDouble( (takeProfitPrice-stopLossPrice)-(bidPrice-stopLossPrice), Digits() ), "/",
                                                        NormalizeDouble( (takeProfitPrice-stopLossPrice)-(takeProfitPrice-bidPrice), Digits() ),
                 "\n========= Риски ========="
                 "\nРазмер риска = ",                   RISK, "%"
                 "\nСтоимость риска (баланс) = ",       riskPriceAcc, " ", currency,
                 "\nСтоимость риска (лот) = ",          riskPriceLot, " ", currency,
                 "\nМаксимальный размер лота = ",       lotOrderSize );  
    }
    return rates_total;
}

//+---------------------------------------------------------------------------+
void OnDeinit(const int reason)
{
    Comment( "" );
};

//+---------------------------------------------------------------------------+
void InitVariables()
{
    first           = StringSubstr( Symbol(), 0, 3 );       // первый символ,   например EUR
    second          = StringSubstr( Symbol(), 3, 3 );       // второй символ,   например USD
    prefix          = StringSubstr( Symbol(), 6 );          // префикс брокера
    currency        = AccountCurrency();                    // валюта депозита, например USD
    leverage        = AccountLeverage();                    // кредитное плечо, например 100
    
    balance         = AccountBalance();                     // Баланс
    //balance         = 250.0;                                // Баланс
    freeMargin      = AccountFreeMargin();                  // Свободныя маржа 
    
    lotMin          = MarketInfo( Symbol(), MODE_MINLOT );
    lotMax          = MarketInfo( Symbol(), MODE_MAXLOT );
    lotStep         = MarketInfo( Symbol(), MODE_LOTSTEP );
    
    bidPrice        = MarketInfo( Symbol(), MODE_BID );     // Котировка на покупку
    lotSize         = MarketInfo( Symbol(), MODE_LOTSIZE ); // Стоимость лота в базовой валюте
    lotPriceSecond  = lotSize * bidPrice;                   // Стоимость лота в валюте котировки
    lotPriceCurr    = MarketInfo( Symbol(), MODE_MARGINREQUIRED ); // Стоимость лота в валюте депозита
    
    takeProfitPrice = iMA( Symbol(), Period(), 26, 0, MODE_EMA, PRICE_CLOSE, 0 ) + CHANNEL * Point();
    stopLossPrice   = StopBuyMax( 0, MFACTOR, Symbol(), Period() );
    
    riskPriceAcc    = balance * 0.01 * RISK;                // Стоимость риска для баланса
    riskPriceLot    = NormalizeDouble( Convert(lotPriceSecond - lotSize * stopLossPrice, second, currency), 2 ); // Стоимость риска на 1 лот
    lotOrderSize    = riskPriceLot * lotMin >= riskPriceAcc ? lotMin : 
                        NormalizeDouble( riskPriceAcc / riskPriceLot, 2 );// Размер лота
};

