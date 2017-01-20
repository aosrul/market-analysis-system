#ifndef SETTINGSSTRUCT_H
#define SETTINGSSTRUCT_H

#include <QObject>
#include <QDateTime>
#include "include/interfaces_global.h"

struct Settings {
    Settings();
    ~Settings();
    qint32          maxOpenTabs = 5;
    QStringList     savedKits;
    QStringList     session;
    QString         pluginsPath;
    QList<NN_PluginInterface *>
                    nnPlugins;
    QList<OSC_PluginInterface *>
                    oscPlugins;
    QList<PP_PluginInterface *>
                    ppPlugins;
    qint32          winPosX;
    qint32          winPosY;
    qint32          winSizeX;
    qint32          winSizeY;
//===========Functions==============================
private:
    void loadPlugins();
    void sortPlugin(QObject *obj);
};

struct ConfigMT4 {
    ConfigMT4(QString name) : nameKit( name ) {
        setPath(); setTrainingMethods(); setSymbolsOfTime();
        nnPluginName = "OpenNN";
    }
    ~ConfigMT4() { }
    QString         nameKit;
    QString         kitPath;
    QString         mt4Path = "C:/";
    QString         server;
    qint32          mt4Account = 0;
    QStringList     servers;
    QStringList     symbols;
    QStringList     symbolsOfTime;
    QStringList     trainingMethods;
    QString         nnPluginName;
    QString         historyPath = "/history/"; // default ?
    const QString   configFile = "/MQL4/Files/mas.conf";
    const QString   newHistoryPath = "/MQL4/Files/MAS_MarketData/";
    const QString   predictionPath = "/MQL4/Files/MAS_Prediction/";
//== Model parameters
    QList<qint32>   periods;
    QStringList     input;
    QStringList     output;
    bool            recurrentModel = false;
    bool            readVolume = false;
    qint32          depthHistory = 1;
    qint32          depthPrediction = 1;
    qint32          layersCount = 1;
    QList<qint32>   layersSize = { 9, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    QString         trainingMethod;
    QList<qint32>   divideInstances = { 60, 20, 20 };
    QDateTime       lastTraining;
    qint32          progress = 0;
    bool            isLoaded = false;
    bool            isReady = false;
    bool            isTrained = false;
    bool            isRun = false;
//===========Functions==============================
    void rename(const QString newName);
    void removePath(QString path);
    bool isTimeSymbol(QString symbol);
    void updateServerParameters(void);
    qint32 sumInput(void);
    qint32 sumOutput(void);
private:
    void setPath(void);
    void renamePath(const QString newName);
    void setServer(void);
    void setSymbols(void);
    void setSymbolsOfTime(void);
    void setTrainingMethods(void);
};

#endif // SETTINGSSTRUCT_H
