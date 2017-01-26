#ifndef INTERFACES_GLOBAL_H
#define INTERFACES_GLOBAL_H

#include <QtPlugin>

class NN_PluginInterface
{
public:
    virtual ~NN_PluginInterface() {}
    virtual QString getType() = 0;
    virtual QString getName() = 0;
    virtual QString getVersion() = 0;
    virtual void setConfigName(const QString name) = 0;
    virtual void prepareDataSet(const qint32 rows, const qint32 columns,
                                const std::vector<std::vector<double> > *data = 0) = 0;
    virtual void setRow(const qint32 idx, const std::vector<double> *row) = 0;
    virtual void setData(const std::vector<std::vector<double> > *data) = 0;
    virtual void setNameRows(const QStringList names) = 0;
    virtual void setTargets(const std::vector<bool> targets) = 0;
    virtual void setDivideData(const qint32 div[3]) = 0;
    virtual void setNeuralNetwork() = 0;
    virtual void setNeuralNetwork(const qint32 layers,
                                  const std::vector<qint32> &sizes) = 0;
    virtual void setTrainingMethod(const QString method) = 0;
//    virtual void func() = 0;
//    virtual void func() = 0;
//    virtual void func() = 0;
//    virtual void func() = 0;
//    virtual void func() = 0;

signals:
    void message(QString text);
};

class OSC_PluginInterface
{
public:
    virtual ~OSC_PluginInterface() {}
    virtual QString getType() = 0;
    virtual QString getName() = 0;
    virtual QString getVersion() = 0;
};

class PP_PluginInterface
{
public:
    virtual ~PP_PluginInterface() {}
    virtual QString getType() = 0;
    virtual QString getName() = 0;
    virtual QString getVersion() = 0;
};

QT_BEGIN_NAMESPACE

#define NN_PluginInterface_iid "Terentew_Aleksey.Market-Analysis-System.NN_PluginInterface.1.0"

Q_DECLARE_INTERFACE(NN_PluginInterface, NN_PluginInterface_iid)

#define OSC_PluginInterface_iid  "Terentew_Aleksey.Market-Analysis-System.OSC_PluginInterface.1.0"

Q_DECLARE_INTERFACE(OSC_PluginInterface, OSC_PluginInterface_iid)

#define PP_PluginInterface_iid "Terentew_Aleksey.Market-Analysis-System.PP_PluginInterface.1.0"

Q_DECLARE_INTERFACE(PP_PluginInterface, PP_PluginInterface_iid)

QT_END_NAMESPACE

#endif // INTERFACES_GLOBAL_H
