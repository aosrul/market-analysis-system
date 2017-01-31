//+------------------------------------------------------------------+
//|                                                MAS_Assistant.mq4 |
//|                                 Copyright 2016, Terentew Aleksey |
//|                        https://www.mql5.com/ru/users/terentjew23 |
//+------------------------------------------------------------------+
#property copyright     "Copyright 2016, Terentew Aleksey"
#property link          "https://www.mql5.com/ru/users/terentjew23"
#property description   ""
#property version       "1.3.0-alpha"
#property strict
#include                <MAS_Include.mqh>
//---------------------Indicators------------------------------------+
#property indicator_chart_window
#property indicator_buffers 3
#property indicator_plots   3
//--- plot Forecast_High
#property indicator_label1  "Forecast_High"
#property indicator_type1   DRAW_LINE
#property indicator_color1  clrOrangeRed
#property indicator_style1  STYLE_DOT
#property indicator_width1  1
//--- plot Forecast_Low
#property indicator_label2  "Forecast_Low"
#property indicator_type2   DRAW_LINE
#property indicator_color2  clrFireBrick
#property indicator_style2  STYLE_DOT
#property indicator_width2  1
//--- plot Forecast_Close
#property indicator_label3  "Forecast_Close"
#property indicator_type3   DRAW_LINE
#property indicator_color3  clrMediumSpringGreen
#property indicator_style3  STYLE_DASH
#property indicator_width3  1
//#property indicator_style3  STYLE_DOT | STYLE_DASH
//--- indicator buffers
double  fHigh_Buffer[];
double  fLow_Buffer[];
double  fClose_Buffer[];

//-----------------Global variables----------------------------------+
const string Copyright = "Copyright 2016, Terentew Aleksey";
const char  csvChar = ';';
// File parameters
string      mainSavePath;
string      mainReadPath;
string      configFile;
string      saveFileName;
string      readFileName;
// Configuration
bool        onePeriod;
int         periods[3];



//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit()
{
    // Indicator buffers mapping
    SetIndexBuffer(0, fHigh_Buffer);
    SetIndexShift(0, 0);
    SetIndexBuffer( 1, fLow_Buffer);
    SetIndexShift(1, 0);
    SetIndexBuffer( 2, fClose_Buffer);
    SetIndexShift(2, 0);
    // Set File parameters
    configFile = "mas_mt4.conf";
    mainSavePath = "MAS_MarketData/";
    mainReadPath = "MAS_Prediction/";
    //saveFileName = StringConcatenate( mainSavePath, "/", _Symbol, "/", currentYM, "-", _Period );
    //readFileName = StringConcatenate( mainReadPath, "/", _Symbol, "/", currentYM, "-", _Period );
    // Set Configuration
    SetConfigs();
    // Set status MAS modules
    assistState = true;
    autotraderState = false;
    
    return(INIT_SUCCEEDED);
}



//+------------------------------------------------------------------+
//| Main function. Called with tick                                  |
//+------------------------------------------------------------------+
int OnCalculate( const int rates_total,
                     const int prev_calculated,
                     const datetime &time[],
                     const double &open[],
                     const double &high[],
                     const double &low[],
                     const double &close[],
                     const long &tick_volume[],
                     const long &volume[],
                     const int &spread[] )
{
    if( isReady ) {
        if( NewBar() ) {
            SaveHistoryFiles( inputList );
        }
        if( NewBar() || NewForecast() ) {
            ReadForecastFile( outputSymbol );
        }
        UiUpdate();
    } else {
        if( !configIsReaded ) {
            WriteConfigFile( configFile );
            ReadConfigFile( configFile );
        }
        if( ConfigKit ) {
            if( _Symbol == outputSymbol ) {
                isReady = true;
            } else {
                OpenNewWindow( outputSymbol );
                //CloseThisWindow();
            }
        }
    }
    return( rates_total );
}

