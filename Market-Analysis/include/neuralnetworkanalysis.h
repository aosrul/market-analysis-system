#ifndef NEURALNETWORKANALYSIS_H
#define NEURALNETWORKANALYSIS_H

#include <QObject>
#include "include/settingsstruct.h"
#include "include/imt4reader.h"
#include "interfaces_global.h"

class NeuralNetworkAnalysis : public QObject
{
    Q_OBJECT
public:
    explicit NeuralNetworkAnalysis(QObject *parent = 0);
    ~NeuralNetworkAnalysis();

private:
    ConfigMT4               *config;
    Settings                *settings;
    NN_PluginInterface      *neuralPlugin = 0;
    bool                    stopSygnal = false;
    qint32                  columnsDS;
    qint32                  rowsDS;
    qint64                  firstEntryTime;
    qint64                  lastEntryTime;

public slots:
    void setConfigKit(ConfigMT4 *cfg);
    void setSettings(Settings *sett);
    void runTraining(void);
    void runPrediction(void);
    void stop(void);

private slots:
    void doTraining(void);
    void doForcasting(void);
    bool loadNNPlugin(void);

    void prepareDataSet(FileType historyType);
    void prepareVariablesInfo(void);
    void prepareInstances(void);
    void prepareNeuralNetwork(void);
    void preparePerformanceFunc(void);
    void runTrainingNeuralNetwork(void);
    void saveResults(void);
    void runWorkingProcess(void);

    void loadHistoryFiles(QMap<QString, IMt4Reader *> &readers,
                          QMap<QString, qint32> &iters,
                          FileType historyType);
    void loadDataToPlugin(const QMap<QString, IMt4Reader *> &readers,
                  QMap<QString, qint32> &iters );
    void getFirstEntryTime(const QMap<QString, IMt4Reader *> &readers,
                           qint64 &first, qint64 &last);
    double getDoubleTimeSymbol(const QString &symbol, const qint64 &timeCurrentIter);

signals:
    void workedOut(void);
    void progress(qint32);
    void message(QString);
    void pause(qint32);
};

#endif // NEURALNETWORKANALYSIS_H
