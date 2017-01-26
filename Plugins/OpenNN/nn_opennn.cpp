#include "nn_opennn.h"

nnPlugin_OpenNN::nnPlugin_OpenNN()
{
    srand( (unsigned)time( NULL ) );
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
    kitName = name;
}

void nnPlugin_OpenNN::prepareDataSet(const qint32 rows, const qint32 columns,
                                     const std::vector<std::vector<double> > *data)
{
    if( dataSet )
        delete dataSet;
    columnsDS = columns;
    rowsDS = rows;
    dataSet = new DataSet( rowsDS, columnsDS );
    if( data == 0 )
        return;
    for( qint32 i = 0; i < rowsDS; i++ ) {
        setRow( i, &(*data)[i] );
    }
}

void nnPlugin_OpenNN::setRow(const qint32 idx, const std::vector<double> *row)
{
    if( dataSet == 0 ) {
        message( tr("First, set the size of the data set.") );
        throw 41;
    }
    const Vector<double> *vecRow = static_cast<const Vector<double> *>( static_cast<const void *>(row) ); // ???
    dataSet->set_instance( idx, *vecRow );
}

void nnPlugin_OpenNN::setData(const std::vector<std::vector<double> > *data)
{
    if( dataSet == 0 ) {
        message( tr("First, set the size of the data set.") );
        throw 41;
    }
    if( rowsDS <= 1 || columnsDS <= 1 ) {
        message( tr("First, set the size of the data set.") );
        throw 41;
    }
    if( data->size() != rowsDS || (*data)[0].size() != columnsDS ) {
        message( tr("Dimensions new data does not match the size of the dataset.") );
        throw 42;
    }
    for( qint32 i = 0; i < rowsDS; i++ ) {
        setRow( i, &(*data)[i] );
    }
}

void nnPlugin_OpenNN::setNameRows(const QStringList names)
{
    if( dataSet == 0 ) {
        message( tr("First, set the size of the data set.") );
        throw 41;
    }
    Variables* variablesPtr = dataSet->get_variables_pointer();
    for( qint32 i = 0; i < columnsDS; i++ ) {
        variablesPtr->set_name( i, names.at(i).toStdString() );
    }
}

void nnPlugin_OpenNN::setTargets(const std::vector<bool> targets)
{
    if( dataSet == 0 ) {
        message( tr("First, set the size of the data set.") );
        throw 41;
    }
    Variables* variablesPtr = dataSet->get_variables_pointer();
    for( qint32 i = 0; i < columnsDS; i++ ) {
        if( targets[i] != true )
            variablesPtr->set_use( i, Variables::Input );
        else
            variablesPtr->set_use( i, Variables::Target );
    }
}

void nnPlugin_OpenNN::setDivideData(const qint32 div[])
{
    dataSet->get_instances_pointer()->split_sequential_indices( div[0] * 0.01,
                                                                div[1] * 0.01,
                                                                div[2] * 0.01 );
}

void nnPlugin_OpenNN::setNeuralNetwork()
{
    if( dataSet == 0 ) {
        message( tr("") );
        return;
    }
    if( neuralNetwork )
        delete neuralNetwork;
    const size_t inputsNumber = dataSet->
            get_variables_pointer()->count_inputs_number();
    const size_t outputsNumber = dataSet->
            get_variables_pointer()->count_targets_number();
    const size_t hiddenPerceptronsNumber = (inputsNumber - outputsNumber);
    neuralNetwork = new NeuralNetwork( inputsNumber,
                                       hiddenPerceptronsNumber,
                                       outputsNumber );
    neuralNetwork->get_inputs_pointer()->
            set_information( dataSet->
                             get_variables_pointer()->
                             arrange_inputs_information() );
    neuralNetwork->get_outputs_pointer()->
            set_information( dataSet->
                             get_variables_pointer()->
                             arrange_targets_information() );

    neuralNetwork->construct_scaling_layer();
    ScalingLayer* scalingLayerPointer = neuralNetwork->
                                        get_scaling_layer_pointer();
    scalingLayerPointer->set_statistics( dataSet->
                                         scale_inputs_mean_standard_deviation() );
    scalingLayerPointer->set_scaling_method( ScalingLayer::NoScaling );
    neuralNetwork->construct_unscaling_layer();
    UnscalingLayer* unscalingLayerPointer = neuralNetwork->
                                            get_unscaling_layer_pointer();
    unscalingLayerPointer->set_statistics( dataSet->
                                           scale_targets_mean_standard_deviation() );
    unscalingLayerPointer->set_unscaling_method( UnscalingLayer::NoUnscaling );

    performanceFunc = new PerformanceFunctional( neuralNetwork, dataSet );
    performanceFunc->set_regularization_type( PerformanceFunctional::
                                              NEURAL_PARAMETERS_NORM );
}

void nnPlugin_OpenNN::setNeuralNetwork(const qint32 layers, const std::vector<qint32> &sizes)
{
    if( dataSet == 0 ) {
        message( tr("") );
        throw;
    }
    if( neuralNetwork )
        delete neuralNetwork;
    const size_t inputsNumber = dataSet->
            get_variables_pointer()->count_inputs_number();
    const size_t outputsNumber = dataSet->
            get_variables_pointer()->count_targets_number();
    const size_t hiddenPerceptronsNumber = (inputsNumber - outputsNumber);
    neuralNetwork = new NeuralNetwork( inputsNumber,
                                       hiddenPerceptronsNumber,
                                       outputsNumber );
    neuralNetwork->get_inputs_pointer()->
            set_information( dataSet->
                             get_variables_pointer()->
                             arrange_inputs_information() );
    neuralNetwork->get_outputs_pointer()->
            set_information( dataSet->
                             get_variables_pointer()->
                             arrange_targets_information() );
    // scale
    neuralNetwork->construct_scaling_layer();
    ScalingLayer* scalingLayerPointer = neuralNetwork->
                                        get_scaling_layer_pointer();
    scalingLayerPointer->set_statistics( dataSet->
                                         scale_inputs_mean_standard_deviation() );
    scalingLayerPointer->set_scaling_method( ScalingLayer::NoScaling );
    neuralNetwork->construct_unscaling_layer();
    UnscalingLayer* unscalingLayerPointer = neuralNetwork->
                                            get_unscaling_layer_pointer();
    unscalingLayerPointer->set_statistics( dataSet->
                                           scale_targets_mean_standard_deviation() );
    unscalingLayerPointer->set_unscaling_method( UnscalingLayer::NoUnscaling );

    performanceFunc = new PerformanceFunctional( neuralNetwork, dataSet );
    performanceFunc->set_regularization_type( PerformanceFunctional::
                                              NEURAL_PARAMETERS_NORM );

}

void nnPlugin_OpenNN::setTrainingMethod(const QString method)
{
    Q_UNUSED(method);
}
