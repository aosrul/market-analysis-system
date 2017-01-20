#include "nn_opennn.h"

nnPlugin_OpenNN::nnPlugin_OpenNN()
{
    //srand((unsigned)time(NULL));
}

QString nnPlugin_OpenNN::getType()
{
    return "NeuralNetwork";
}

QString nnPlugin_OpenNN::getName()
{
    return "OpenNN";
}

QString nnPlugin_OpenNN::getVersion()
{
    return "1.0";
}

void nnPlugin_OpenNN::setConfigName(const QString name)
{

}

void nnPlugin_OpenNN::prepareDataSet()
{

}

void nnPlugin_OpenNN::prepareDataSet(const qint32 rows, const qint32 columns)
{

}

void nnPlugin_OpenNN::prepareDataSet(const std::vector<std::vector<double> > *data, const qint32 rows, const qint32 columns)
{

}

void nnPlugin_OpenNN::setNameRows(const QStringList names)
{

}

void nnPlugin_OpenNN::setTargets(const std::vector<bool> targets)
{

}

void nnPlugin_OpenNN::addRow(const std::vector<double> *row)
{

}

void nnPlugin_OpenNN::addData(const std::vector<std::vector<double> > *data)
{

}

void nnPlugin_OpenNN::setDivideData(const qint32 div[])
{

}

void nnPlugin_OpenNN::setTrainingMethod(const QString method)
{

}

void nnPlugin_OpenNN::setNeuralNetwork()
{

}

void nnPlugin_OpenNN::setNeuralNetwork(const qint32 layers, const std::vector<qint32> &sizes)
{

}
