#ifndef NN_OPENNN_H
#define NN_OPENNN_H

#include "interfaces_global.h"
#include <QObject>
#include <QString>
// include opennn library
//#include "../../../OpenNN/opennn/opennn.h"
//using namespace OpenNN;

class nnPlugin_OpenNN : public QObject, public NN_PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Terentew_Aleksey.Market-Analysis-System.NN_PluginInterface.1.0" FILE "nn_opennn.json")
    Q_INTERFACES(NN_PluginInterface)

public:
    nnPlugin_OpenNN();
    QString getType() Q_DECL_OVERRIDE;
    QString getName() Q_DECL_OVERRIDE;
    QString getVersion() Q_DECL_OVERRIDE;

    void setConfigName(const QString name) Q_DECL_OVERRIDE;
    void prepareDataSet() Q_DECL_OVERRIDE;
    void prepareDataSet(const qint32 rows, const qint32 columns) Q_DECL_OVERRIDE;
    void prepareDataSet(const std::vector<std::vector<double> > *data,
                        const qint32 rows, const qint32 columns) Q_DECL_OVERRIDE;
    void setNameRows(const QStringList names) Q_DECL_OVERRIDE;
    void setTargets(const std::vector<bool> targets) Q_DECL_OVERRIDE;
    void addRow(const std::vector<double> *row) Q_DECL_OVERRIDE;
    void addData(const std::vector<std::vector<double> > *data) Q_DECL_OVERRIDE;
    void setDivideData(const qint32 div[3]) Q_DECL_OVERRIDE;
    void setTrainingMethod(const QString method) Q_DECL_OVERRIDE;
    void setNeuralNetwork() Q_DECL_OVERRIDE;
    void setNeuralNetwork(const qint32 layers,
                          const std::vector<qint32> &sizes) Q_DECL_OVERRIDE;

private:
    qint32          rowsDS = 1;
    qint32          columnsDS = 1;
//    Matrix<double>                  *matrixDS = 0;
//    DataSet                         *dataSet = 0;
//    NeuralNetwork                   *neuralNetwork = 0;
//    PerformanceFunctional           *performanceFunc = 0;
//    TrainingStrategy                *trainingStrategy = 0;
};

#endif // NN_OPENNN_H
