#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "include/presenter.h"
#include "include/settingsform.h"
#include "include/kitconfigform.h"
#include <QList>

#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>

#define MAX_TAB 10

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    struct KitTabWidget {
        QString         name;
        bool            changed;
        QWidget         *kitTab;
        QLabel          *nameKitName;
        QLabel          *serverName;
        QLabel          *pathToMt4Name;
        QListView       *inputListView;
        QListView       *outputListView;
        QLabel          *inputSize;
        QLabel          *outputSize;
        QProgressBar    *progressBar;
        QVBoxLayout     *vLayoutTab;
        QGroupBox       *hGBoxKitName;
        QHBoxLayout     *hLayoutName;
        QLabel          *nameKitLabel;
        QGroupBox       *hGBoxPathMt4;
        QHBoxLayout     *hLayoutPath;
        QLabel          *serverLabel;
        QLabel          *pathToMt4Label;
        QHBoxLayout     *hLayoutConf;
        QGroupBox       *vGBoxInput;
        QVBoxLayout     *vLayoutInput;
        QHBoxLayout     *hLayoutInputSize;
        QLabel          *inputLabel;
        QVBoxLayout     *vLayoutSymbol;
        QSpacerItem     *verticalSpacer_2;
        QLabel          *arrowLabel;
        QSpacerItem     *verticalSpacer_3;
        QGroupBox       *vGBoxOutput;
        QVBoxLayout     *vLayoutOutput;
        QHBoxLayout     *hLayoutOutputSize;
        QLabel          *outputLabel;
        QVBoxLayout     *vLayoutButtons;
        QPushButton     *configurationButton;
        QPushButton     *trainingButton;
        QPushButton     *workButton;
        QPushButton     *stopButton;
        QPushButton     *deleteButton;
        QSpacerItem     *verticalSpacer;
        QPlainTextEdit  *consoleTextEdit;
    };
    Ui::MainWindow *ui;
    Presenter *presenter;
    SettingsForm *settings;
    KitConfigForm *kitConfig;

    QVector<KitTabWidget *> tabList;
    qint32 currentTab = 0;
    qint32 countTabs = 0;

signals:
    void addNewKit(QString);
    void openKit(QString);
    void saveKit(QString);
    void deleteKit(QString);
    void closedKit(QString);
    void renamedKit(QString, QString);
    void runTrainingKit(QString);
    void runWorkKit(QString);
    void stopWorkKit(QString);

public slots:
    void errorMessage(QString text);

private slots:
    void addNew();
    void open();
    void save();
    void closeTab();
    void openSettings();
    void openKitConfig();
    void runTraining();
    void runWork();
    void stopWork();
    void delete_Kit();
    void openHelp();
    void openAbout();

    bool openTab(const qint32 idx, const QString name);
    void closeTab(const qint32 idx);
    void selectTab(const qint32 idx);
    void setTabName(const qint32 idx, const QString name);
    void setConnections();

    void addTabToUi(const qint32 idx);
    void addTabConnections(const qint32 idx);
    void deleteTabFromUi(const qint32 idx);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
