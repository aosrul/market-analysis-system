#ifndef NEURALNETWORK_GLOBAL_H
#define NEURALNETWORK_GLOBAL_H

#include <QtPlugin>

class NNPlugin : public QObject
{
public:
    virtual ~NNPlugin() {}
    virtual QString getType() = 0;
    virtual QString getName() = 0;
    virtual QString getVersion() = 0;

    virtual void setConfigName(const QString name) = 0;
    virtual void prepareDataSet() = 0;
    virtual void prepareDataSet(const qint32 rows, const qint32 columns) = 0;
    virtual void prepareDataSet(const std::vector<std::vector<double> > *data,
                                const qint32 rows, const qint32 columns) = 0;
    virtual void setNameRows(const QStringList names) = 0;
    virtual void setTargets(const std::vector<bool> targets) = 0;
    virtual void addRow(const std::vector<double> *row) = 0;
    virtual void addData(const std::vector<std::vector<double> > *data) = 0;
    virtual void setDivideData(const qint32 div[3]) = 0;
    virtual void setTrainingMethod(const QString method) = 0;
    virtual void setNeuralNetwork() = 0;
    virtual void setNeuralNetwork(const qint32 layers, const std::vector &sizes) = 0;
    virtual void func() = 0;
    virtual void func() = 0;
    virtual void func() = 0;
    virtual void func() = 0;
    virtual void func() = 0;

signals:
    virtual void message(QString text) = 0;
};

//QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(NNPlugin, "Terentew Aleksey/Market-Analysis-System/NNPlugin/1.0")
//QT_END_NAMESPACE

#endif // NEURALNETWORK_GLOBAL_H
