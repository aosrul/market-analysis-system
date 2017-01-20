#include "include/settingsstruct.h"
#include <QApplication>
#include <QDir>
#include <QPluginLoader>

Settings::Settings()
{
    QString mDir = qApp->applicationDirPath();
    pluginsPath = mDir + "/Plugins";
    loadPlugins();
}

Settings::~Settings()
{
    foreach( NN_PluginInterface *obj, nnPlugins )
        delete obj;
    foreach( OSC_PluginInterface *obj, oscPlugins )
        delete obj;
    foreach( PP_PluginInterface *obj, ppPlugins )
        delete obj;
}

void Settings::loadPlugins()
{
#ifdef Q_OS_WIN32
    QStringList filter;
    filter << "*.dll";
#endif
#ifdef Q_OS_LINUX
    QStringList filter;
    filter << "*.so";
#endif
    QDir pluginsDir( pluginsPath );
    foreach( QString file, pluginsDir.entryList( filter, QDir::Files ) ) {
        QPluginLoader loader( pluginsDir.absoluteFilePath(file) );
        QObject *plugin = loader.instance();
        if( plugin ) {
            sortPlugin( plugin );
        }
    }
}

void Settings::sortPlugin(QObject *obj)
{
    NN_PluginInterface *nn_plugin = qobject_cast<NN_PluginInterface *>(obj);
    if( nn_plugin ) {
        nnPlugins.append( nn_plugin );
    }
    OSC_PluginInterface *osc_plugin = qobject_cast<OSC_PluginInterface *>(obj);
    if( osc_plugin ) {
        oscPlugins.append( osc_plugin );
    }
    PP_PluginInterface *pp_plugin = qobject_cast<PP_PluginInterface *>(obj);
    if( pp_plugin ) {
        ppPlugins.append( pp_plugin );
    }
}

//===============ConfigMT4================

void ConfigMT4::rename(const QString newName) {
    if( nameKit != newName ) {
        renamePath( newName );
        nameKit = newName;
    }
}

void ConfigMT4::removePath(QString path) {
    QDir dir( path );
    QStringList lstDirs = dir.entryList( QDir::Dirs|QDir::AllDirs|QDir::NoDotAndDotDot );
    QStringList lstFiles = dir.entryList(QDir::Files);
    foreach( QString entry, lstFiles ) {
        QString entryAbsPath = dir.absolutePath() + "/" + entry;
        QFile::setPermissions(entryAbsPath, QFile::ReadOwner | QFile::WriteOwner);
        QFile::remove(entryAbsPath);
    }
    foreach (QString entry, lstDirs) {
        QString entryAbsPath = dir.absolutePath() + "/" + entry;
        removePath( entryAbsPath );
    }
    QDir().rmdir( dir.absolutePath() );
}

bool ConfigMT4::isTimeSymbol(QString symbol) {
    foreach( auto time, symbolsOfTime )
        if( symbol == time )
            return true;
    return false;
}

void ConfigMT4::updateServerParameters() {
    setServer();
    setSymbols();
}

qint32 ConfigMT4::sumInput()
{
    qint32 res = 0;
    foreach( QString in, input ) {
        if( isTimeSymbol( in ) ) {
            res += 1;
        } else {
            res += recurrentModel ? 4 : 4 * depthHistory;
            if( readVolume )
                res += recurrentModel ? 1 : depthHistory;
        }
    }
    return res;
}

qint32 ConfigMT4::sumOutput()
{
    qint32 res = 0;
    foreach( QString out, output ) {
        if( isTimeSymbol( out ) ) {
            res += 1;
        } else {
            res += 3 * depthPrediction;
        }
    }
    return res;
}

void ConfigMT4::setPath() {
    QString mDir = qApp->applicationDirPath();
    mDir += "/Market Kits/";
    mDir += nameKit;
    if( !QDir().exists(mDir) )
        QDir().mkdir( mDir );
    kitPath = mDir;
}

void ConfigMT4::renamePath(const QString newName) {
    QString mDir2 = qApp->applicationDirPath();
    mDir2 += "/Market Kits/";
    mDir2 += newName;
    if( !QDir().exists(kitPath) )
        QDir().rename( kitPath, mDir2 );
    removePath( kitPath );
    kitPath = mDir2;
}

void ConfigMT4::setServer() {
    if( mt4Path.size() <= 5 )
        return;
    QDir path( QString("%1%2").arg( mt4Path, "/history" ) );
    QStringList files = path.entryList( QDir::Dirs );
    files.removeOne( "default" );
    files.removeOne( "deleted" );
    files.removeOne( "mailbox" );
    files.removeOne( "signals" );
    files.removeOne( "symbolsets" );
    files.removeOne( "." );
    files.removeOne( ".." );
    servers = files;
    if( server == "" )
        if( !server.isEmpty() )
            server = servers.first();
}

void ConfigMT4::setSymbols() {
    //symbols.clear();
    // read mas_mt4.conf
}

void ConfigMT4::setSymbolsOfTime() {
    symbolsOfTime.append("YEAR");
    symbolsOfTime.append("MONTH");
    symbolsOfTime.append("DAY");
    symbolsOfTime.append("YEARDAY");
    symbolsOfTime.append("HOUR");
    symbolsOfTime.append("MINUTE");
    symbolsOfTime.append("WEEKDAY");
}

void ConfigMT4::setTrainingMethods() {
    trainingMethods.append( "Random Search" );
    trainingMethods.append( "Gradient Descent" );
    trainingMethods.append( "Newton Method" );
    trainingMethods.append( "Conjugate Gradient" );
    trainingMethods.append( "Quasi Newton Method" );
    trainingMethods.append( "Evolutionary Algorithm" );
    trainingMethods.append( "Levenberg Marquardt Algorithm" );
    trainingMethod = "Quasi Newton Method";
}
