#ifndef OPENNN_H
#define OPENNN_H

#if defined(OPENNN_PLUGIN)
#  define OPENNNSHARED_EXPORT Q_DECL_EXPORT
#else
#  define OPENNNSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "../global_headers/neuralnetwork_global.h"
// include opennn library
#include "../../../OpenNN/opennn/opennn.h"
using namespace OpenNN;

class OPENNNSHARED_EXPORT OpenNN : public NNPlugin
{
    Q_OBJECT Q_INTERFACES(NNPlugin)
public:
    OpenNN(QObject *parent = 0);
    ~OpenNN();
    virtual QString getName()       { return "OpenNN"; }
    virtual QString getVersion()    { return "1.0"; }


private:
    qint32                          rowsDS = 1;
    qint32                          columnsDS = 1;
    Matrix<double>                  *matrixDS = 0;
    DataSet                         *dataSet = 0;
    NeuralNetwork                   *neuralNetwork = 0;
    PerformanceFunctional           *performanceFunc = 0;
    TrainingStrategy                *trainingStrategy = 0;
};

#endif // OPENNN_H
