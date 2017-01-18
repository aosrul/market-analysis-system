#include "include/neuralnetworkanalysis.h"
#include "include/hstreader.h"
#include "include/csvreader.h"
#include "include/csvwriter.h"

#include <QDateTime>
#include <QApplication>

#include <QDebug>

NeuralNetworkAnalysis::NeuralNetworkAnalysis(QObject *parent) : QObject(parent),
    matrixDS(nullptr),
    dataSet(nullptr),
    neuralNetwork(nullptr),
    performanceFunc(nullptr),
    trainingStrategy(nullptr)
{
    srand((unsigned)time(NULL));
}

NeuralNetworkAnalysis::~NeuralNetworkAnalysis()
{
    if( trainingStrategy )
        delete trainingStrategy;
    if( performanceFunc )
        delete performanceFunc;
    if( neuralNetwork )
        delete neuralNetwork;
    if( dataSet )
        delete dataSet;
    if( matrixDS )
        delete matrixDS;
}

void NeuralNetworkAnalysis::setConfigKit(ConfigMT4 *cfg)
{
    config = cfg;
}

void NeuralNetworkAnalysis::runTraining()
{
    try {
        message( tr("Start training model - %1.")
                 .arg( QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") ) );
        progress( 0 );
        config->isRun = true;
        prepareDataSet( FileType::HST );
        progress( 22 );
        prepareVariablesInfo();
        progress( 27 );
        prepareInstances();
        prepareNeuralNetwork();
        preparePerformanceFunc();
        progress( 32 );
        runTrainingNeuralNetwork();
        progress( 98 );
        saveResultsTraining();
        progress( 100 );
        config->isRun = false;
        emit trained();
        message( tr("Training model done - %1.")
                 .arg( QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") ) );
    } catch(qint32 e) {
        message( tr("Training model error - %1.").arg( e ) );
        config->isRun = false;
        progress( 0 );
        return;
    }
}

void NeuralNetworkAnalysis::runPrediction()
{
    try {
        message( tr("Start work of prediction - %1.")
                 .arg( QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") ) );
        progress( 0 );
        config->isRun = true;
        if( !config->isReady ) {
            message( tr("Prediction work stoped. Model not ready.") );
            throw 30;
        } else if( !config->isTrained ) {
            message( tr("Prediction work stoped. Model not trained.") );
            throw 31;
        }
        prepareDataSet( FileType::CSV );
        progress( 18 );
        prepareVariablesInfo();
        progress( 24 );
        runWorkingProcess();
        progress( 100 );
        config->isRun = false;
        message( tr("Work stoped - %1.")
                 .arg( QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") ) );
    } catch(qint32 e) {
        message( tr("Work process error - %1.").arg( e ) );
        config->isRun = false;
        progress( 0 );
        return;
    }
}

void NeuralNetworkAnalysis::stop()
{
    config->isRun = false;
    if( !config->isTrained )
        message( tr("Training process aborted by user - %1.")
                 .arg( QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") ) );
    else
        message( tr("Work process aborted by user - %1.")
                 .arg( QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") ) );
}

bool NeuralNetworkAnalysis::loadTrainedModel()
{
    if( !config->isTrained )
        return false;
    if( !neuralNetwork )
        neuralNetwork = new NeuralNetwork();
    neuralNetwork->load( QString("%1/neuralNetwork.xml")
                         .arg( config->kitPath ).toStdString() );
    config->isReady = true;
    message( tr("The Neural Network model of %1 kit loaded.").arg( config->nameKit ) );
    return true;
}

void NeuralNetworkAnalysis::prepareDataSet(FileType historyType)
{
    message( tr("Loading data set...") );
    QMap<QString, IMt4Reader *> readers;
    QMap<QString, qint32> iters;
    matrixDS = new Matrix<double>;
//==========Load history data=============================
    loadHistoryFiles( readers, iters, historyType );
    getFirstEntryTime( readers, firstEntryTime, lastEntryTime );
    message( tr("The data set belongs to the interval of time:\n \
                 \t%1 (%2 sec.) - %3 (%4 sec.)")
                .arg( QDateTime::fromTime_t( firstEntryTime )
                      .toString("yyyy.MM.dd hh:mm:ss") )
                .arg( firstEntryTime )
                .arg( QDateTime::fromTime_t( lastEntryTime )
                      .toString("yyyy.MM.dd hh:mm:ss") )
                .arg( lastEntryTime ));
    qint32 minPeriod = *std::min_element( config->periods.begin(), config->periods.end() );
    columnsDS = config->sumInput() + config->sumOutput();
    rowsDS = ( lastEntryTime - firstEntryTime ) / ( 60 * minPeriod );
    matrixDS->set( rowsDS, columnsDS );
    loadDataToMatrixDS( readers, iters, *matrixDS );
//==========Save dataset & Clean readers================
    if( matrixDS->get_columns_number() != columnsDS )
        throw 22;                           // !err matrixDS->get_columns_number() != columnsDS
    if( matrixDS->get_rows_number() != rowsDS ) {
        message( tr("matrix=%1; rows=%2").arg( matrixDS->get_rows_number() ).arg( rowsDS ) );
        //throw 23;                           // !err matrixDS->get_rows_number() != rowsDS
    }
    dataSet = new DataSet( matrixDS->get_rows_number(), matrixDS->get_columns_number() );
    dataSet->set_data( *matrixDS );
    QMapIterator<QString, IMt4Reader *> i(readers);
    while( i.hasNext() ) {
        i.next();
        delete i.value();
    }
}

void NeuralNetworkAnalysis::prepareVariablesInfo()
{
    message( tr("Set variables information...") );
    Variables* variablesPtr = dataSet->get_variables_pointer();
    Vector<Variables::Item> varItems( columnsDS );
    qint32 idx = 0, idxH;
    qint32 sizeHist = config->recurrentModel ? 1 : config->depthHistory;
    qint32 sizePred = config->depthPrediction;
    qint32 sizeTsIn = !config->readVolume ? 4 : 5;
    qint32 sizeTsOut = 3; // !config->readVolume ? 3 : 4;
    foreach( QString symbol, config->input ) {
        if( !config->isTimeSymbol(symbol) ) {
            for( idxH = 0; idxH < sizeHist; idxH++ ) {
                for( qint32 i = 0; i < sizeTsIn; i++ ) {
                    varItems[idx].name = symbol.toStdString();
                    switch( i ) {
                    case 0: varItems[idx].units = "Open"; break;
                    case 1: varItems[idx].units = "High"; break;
                    case 2: varItems[idx].units = "Low"; break;
                    case 3: varItems[idx].units = "Close"; break;
                    case 4: varItems[idx].units = "Volume";
                    default: ;
                    }
                    varItems[idx].use = Variables::Input;
                    idx += 1;
                }
            }
        } else {
            varItems[idx].name = symbol.toStdString();
            varItems[idx].units = "Time";
            varItems[idx].use = Variables::Input;
            idx += 1;
        }
    }
    foreach( QString symbol, config->output ) {
        if( !config->isTimeSymbol(symbol) ) {
            for( idxH = 0; idxH < sizePred; idxH++ ) {
                for( qint32 i = 0; i < sizeTsOut; i++ ) {
                    varItems[idx].name = symbol.toStdString();
                    switch( i ) {
                    case 0: varItems[idx].units = "High"; break;
                    case 1: varItems[idx].units = "Low"; break;
                    case 2: varItems[idx].units = "Close"; break;
                    default: ;
                    }
                    varItems[idx].use = Variables::Target;
                    idx += 1;
                }
            }
        } else {
            varItems[idx].name = symbol.toStdString();
            varItems[idx].units = "Time";
            varItems[idx].use = Variables::Target;
            idx += 1;
        }
    }
    if( idx > columnsDS )
        throw 24;
    variablesPtr->set_items( varItems );
}

void NeuralNetworkAnalysis::prepareInstances()
{
    message( tr("Prepare instances...") );
    Instances* instances_pointer = dataSet->get_instances_pointer();
    instances_pointer->split_sequential_indices( config->divideInstances[0] * 0.01,
                                                 config->divideInstances[1] * 0.01,
                                                 config->divideInstances[2] * 0.01 );
}

void NeuralNetworkAnalysis::prepareNeuralNetwork()
{
    message( tr("Create neural network...") );
    if( !loadTrainedModel() ) {
        Vector<size_t> layers;
        layers.push_back( config->sumInput() );
        qint32 idx = 0;
        while( config->layersSize[idx] > 0 && idx < config->layersCount ) {
            layers.push_back( config->layersSize[idx] );
            idx += 1;
        }
        layers.push_back( config->sumOutput() );
        if( neuralNetwork && !config->isTrained )
            delete neuralNetwork;
        neuralNetwork = new NeuralNetwork( layers );
    }
    Inputs* inputsPtr = neuralNetwork->get_inputs_pointer();
    inputsPtr->set_information( dataSet->get_variables_pointer()->
                                arrange_inputs_information() );
    Outputs* outputsPtr = neuralNetwork->get_outputs_pointer();
    outputsPtr->set_information( dataSet->get_variables_pointer()->
                                 arrange_targets_information() );
    neuralNetwork->construct_scaling_layer();
    ScalingLayer* scalingLayerPtr = neuralNetwork->get_scaling_layer_pointer();
    scalingLayerPtr->set_statistics( dataSet->scale_inputs_mean_standard_deviation() );
    scalingLayerPtr->set_scaling_method( ScalingLayer::NoScaling );
    neuralNetwork->construct_unscaling_layer();
    UnscalingLayer* unscalingLayerPtr = neuralNetwork->get_unscaling_layer_pointer();
    unscalingLayerPtr->set_statistics( dataSet->scale_targets_mean_standard_deviation() );
    unscalingLayerPtr->set_unscaling_method( UnscalingLayer::NoUnscaling );
}

void NeuralNetworkAnalysis::preparePerformanceFunc()
{
    message( tr("Set performance functional...") );
    performanceFunc = new PerformanceFunctional( neuralNetwork, dataSet);
    performanceFunc->set_regularization_type( PerformanceFunctional::NEURAL_PARAMETERS_NORM ); //need?
    message( tr("Set training strategy...") );
    trainingStrategy = new TrainingStrategy( performanceFunc );
    QuasiNewtonMethod* quasiNewtonMethodPtr = trainingStrategy->get_quasi_Newton_method_pointer();
    quasiNewtonMethodPtr->set_maximum_iterations_number(1000);
    quasiNewtonMethodPtr->set_display_period(10);
    quasiNewtonMethodPtr->set_minimum_performance_increase(1.0e-6);
    quasiNewtonMethodPtr->set_reserve_performance_history(true);
}

void NeuralNetworkAnalysis::runTrainingNeuralNetwork()
{
    message( tr("Results training strategy...") );
    TrainingStrategy::Results trainingStrategyResults = trainingStrategy->perform_training();
    message( tr("Testing analysis.") );
    TestingAnalysis testingAnalysis( neuralNetwork, dataSet );
    TestingAnalysis::LinearRegressionResults linearRegressionResults =
            testingAnalysis.perform_linear_regression_analysis();
    neuralNetwork->get_scaling_layer_pointer()->
            set_scaling_method( ScalingLayer::MeanStandardDeviation );
    neuralNetwork->get_unscaling_layer_pointer()->
            set_unscaling_method( UnscalingLayer::MeanStandardDeviation );
    message( tr("Model training done!") );
    config->isTrained = true;
    trainingStrategy->save( QString("%1/trainingStrategy.xml").arg( config->kitPath ).toStdString() );
    trainingStrategyResults.save( QString("%1/tsResults.dat").arg( config->kitPath ).toStdString() );
    linearRegressionResults.save( QString("%1/linearRegressionResults.dat")
                                  .arg( config->kitPath ).toStdString() );
}

void NeuralNetworkAnalysis::saveResultsTraining()
{
    message( tr("Save results.") );
    dataSet->save( QString("%1/dataSet.xml").arg( config->kitPath ).toStdString() );
    neuralNetwork->save( QString("%1/neuralNetwork.xml").arg( config->kitPath ).toStdString() );
    neuralNetwork->save_expression( QString("%1/nnExpression.txt").arg( config->kitPath ).toStdString() );
    performanceFunc->save( QString("%1/performanceFunctional.xml").arg( config->kitPath ).toStdString() );
}

void NeuralNetworkAnalysis::runWorkingProcess()
{
    qint64 timeCurrentIter = firstEntryTime;
    qint32 minPeriod = *std::min_element( config->periods.begin(), config->periods.end() );
    QMap<QString, CsvWriter *> predictionWriters;
    foreach( QString symbol, config->output ) {
        if( config->isTimeSymbol(symbol) )
            continue;
        QString symbolClear;
        qint32 period = 1440;
        foreach( QString item, config->symbols )
            if( symbol.contains( item ) ) symbolClear = item;
        foreach( qint32 item, config->periods )
            if( symbol.contains( QString::number( item ) ) ) period = item;
        predictionWriters[symbol] = new CsvWriter( QString("%1%2%3.csv")
                                                   .arg( config->mt4Path )
                                                   .arg( config->predictionPath )
                                                   .arg( symbol ) );
        HeaderWr *header = predictionWriters[symbol]->getHeader();
        header->Symbol = symbolClear;
        header->Period = period;
        header->Digits = 5;
        header->Depth = config->depthPrediction;
    }
    Matrix<double> inputMatrix( dataSet->arrange_input_data() );
    message( tr("Calculate...") );
    Matrix<double> outputMatrix( neuralNetwork->calculate_output_data( inputMatrix ) );
    if( config->sumOutput() != outputMatrix.get_columns_number() )
        throw 32;
    for( qint32 i = 0; i < outputMatrix.get_rows_number(); i++ ) {
        timeCurrentIter += ( 60 * minPeriod );
        while( QDateTime::fromTime_t(timeCurrentIter).date().dayOfWeek() >= 6 )
            timeCurrentIter += ( 60 * minPeriod );
        foreach( QString symbol, config->output ) {
            std::vector<Forecast *> *forecast = predictionWriters[symbol]->
                                                getForecastVector();
            Forecast *newFLine = new Forecast;
            newFLine->Time = timeCurrentIter;
            newFLine->High[0] = outputMatrix.arrange_row( i )[0];
            newFLine->Low[0] = outputMatrix.arrange_row( i )[1];
            newFLine->Close[0] = outputMatrix.arrange_row( i )[2];
            forecast->push_back( newFLine );
            predictionWriters[symbol]->setSize( i );
        }
    }
    foreach( QString symbol, config->output )
        predictionWriters[symbol]->writeFile();
    // clean prediction writers
    QMapIterator<QString, CsvWriter *> i(predictionWriters);
    while( i.hasNext() ) {
        i.next();
        delete i.value();
    }
    message( tr("Prediction write done.") );
}

void NeuralNetworkAnalysis::loadHistoryFiles(QMap<QString, IMt4Reader *> &readers,
                                             QMap<QString, qint32> &iters,
                                             FileType historyType)
{
    foreach( QString symbol, config->input ) {
        if( config->isTimeSymbol(symbol) )
            continue; // is time
        iters[symbol] = 0;
        if( historyType == FileType::HST )
            readers[symbol] = new HstReader( QString("%1%2%3.hst")
                                             .arg( config->mt4Path )
                                             .arg( config->historyPath )
                                             .arg( symbol ) );
        else
            readers[symbol] = new CsvReader( QString("%1%2%3.csv")
                                             .arg( config->mt4Path )
                                             .arg( config->newHistoryPath )
                                             .arg( symbol ) );
        if( readers[symbol]->readFile() ) {
            message( tr("History file \"%1\" succeful loaded.")
                     .arg( readers[symbol]->getFileName() ) );
        } else {
            message( tr("History file \"%1\" cannot be loaded.")
                     .arg( readers[symbol]->getFileName() ) );
            throw 20;
        }
    }
    progress( 5 );
}

void NeuralNetworkAnalysis::loadDataToMatrixDS(const QMap<QString, IMt4Reader *> &readers,
                                               QMap<QString, qint32> &iters,
                                               Matrix<double> &matrixDS)
{
    qint32 i, idx, idxSym;
    qint32 sizeHist = config->recurrentModel ? 1 : config->depthHistory;
    qint32 sizePred = config->depthPrediction;
    qint32 iterPeriod = 60 * *std::min_element( config->periods.begin(), config->periods.end() );
    qint64 timeCurrentIter = firstEntryTime;
    for( i = 0; timeCurrentIter < (lastEntryTime - iterPeriod * sizePred); timeCurrentIter += iterPeriod ) {
        if( QDateTime::fromTime_t( timeCurrentIter ).date().dayOfWeek() == 6 ||
                QDateTime::fromTime_t( timeCurrentIter ).date().dayOfWeek() == 7 )
            continue;
        Vector<double> newRow;
        foreach( QString symbol, config->input ) {
            if( readers.contains(symbol) ) { //timeseries
                for( idx = 0; idx < sizeHist; idx++ ) {
                    idxSym = (iters[symbol] - idx) > 0 ? iters[symbol] - idx : 0;
                    newRow.push_back( (*readers[symbol]->getHistory())[idxSym]->Open );
                    newRow.push_back( (*readers[symbol]->getHistory())[idxSym]->High );
                    newRow.push_back( (*readers[symbol]->getHistory())[idxSym]->Low );
                    newRow.push_back( (*readers[symbol]->getHistory())[idxSym]->Close );
                    if( config->readVolume )
                        newRow.push_back( static_cast<double>((*readers[symbol]->getHistory())[idxSym]->Volume) );
                }
                if( (*readers[symbol]->getHistory())[iters[symbol]]->Time <= timeCurrentIter )
                    iters[symbol]++;
            } else if( config->isTimeSymbol(symbol) ) {
                newRow.push_back( getDoubleTimeSymbol( symbol, timeCurrentIter ) );
            } else {
                throw 21;                       // !err symbol not be find
            }
        }
        foreach( QString symbol, config->output ) {
            if( readers.contains(symbol) ) { //timeseries
                for( idx = 0; idx < sizePred; idx++ ) {
                    idxSym = iters[symbol] + idx; //?
                    newRow.push_back( (*readers[symbol]->getHistory())[idxSym]->High );
                    newRow.push_back( (*readers[symbol]->getHistory())[idxSym]->Low );
                    newRow.push_back( (*readers[symbol]->getHistory())[idxSym]->Close );
                }
            } else if( config->isTimeSymbol(symbol) ) {
                newRow.push_back( getDoubleTimeSymbol( symbol, timeCurrentIter ) );
            } else {
                throw 21;                       // !err symbol not be find
            }
        }
        if( i < matrixDS.get_rows_number() )
            matrixDS.insert_row( i, newRow);
        else
            matrixDS.append_row( newRow );
        i++;
        progress( static_cast<qint32>( (lastEntryTime - timeCurrentIter) / (lastEntryTime - firstEntryTime) * 16 + 5) );
    }
}

void NeuralNetworkAnalysis::getFirstEntryTime(const QMap<QString, IMt4Reader *> &readers,
                                              qint64 &first, qint64 &last)
{
    first = std::numeric_limits<qint64>::max();
    last = 0;
    QMapIterator<QString, IMt4Reader *> i(readers);
    while( i.hasNext() ) {
        i.next();
        if( i.value()->getHistorySize() > 0 ) {
            if( (*i.value()->getHistory())[0]->Time < first )
                first = (*i.value()->getHistory())[0]->Time;
            if( (*i.value()->getHistory())[i.value()->getHistorySize()-1]->Time > last )
                last = (*i.value()->getHistory())[i.value()->getHistorySize()-1]->Time;
        }
    }
}

double NeuralNetworkAnalysis::getDoubleTimeSymbol(const QString &symbol,
                                                  const qint64 &timeCurrentIter)
{
    if( symbol == "YEAR" )
        return static_cast<double>(QDateTime::fromTime_t( timeCurrentIter ).date().year());
    else if( symbol == "MONTH" )
        return static_cast<double>(QDateTime::fromTime_t( timeCurrentIter ).date().month());
    else if( symbol == "DAY" )
        return static_cast<double>(QDateTime::fromTime_t( timeCurrentIter ).date().day());
    else if( symbol == "YEARDAY" )
        return static_cast<double>(QDateTime::fromTime_t( timeCurrentIter ).date().dayOfYear());
    else if( symbol == "HOUR" )
        return static_cast<double>(QDateTime::fromTime_t( timeCurrentIter ).time().hour());
    else if( symbol == "MINUTE" )
        return static_cast<double>(QDateTime::fromTime_t( timeCurrentIter ).time().minute());
    else if( symbol == "WEEKDAY" )
        return static_cast<double>(QDateTime::fromTime_t( timeCurrentIter ).date().dayOfWeek());
    else throw 25;                      // !err not timeSymbol
    return -1.00;
}

