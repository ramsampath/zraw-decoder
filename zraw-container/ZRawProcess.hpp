#pragma once

#include <IConsoleOutput.hpp>
#include <ZrawProcessingModel.hpp>
#include <ZrawConverter.hpp>

class ZRawProcess
{
public:
    ZRawProcess(
        ZrawProcessingModel& model, IConsoleOutput& console, IProgressBar& progbar 
    ) :  _model(model), _converter(_model, console, progbar )
    {
        _model.EventValidityUpdate += MakeDelegate(this, &ZRawProcess::OnModelValidityUpdate);
        _model.EventMovContainerLogUpdate += MakeDelegate(this, &ZRawProcess::OnMovContainerLogUpdate);
        _converter.EventConversionFinished += MakeDelegate(this, &ZRawProcess::OnConversionProcessFinish);
    }

    ~ZRawProcess()
    {
        _converter.EventConversionFinished -= MakeDelegate(this, &ZRawProcess::OnConversionProcessFinish);
        _model.EventMovContainerLogUpdate -= MakeDelegate(this, &ZRawProcess::OnMovContainerLogUpdate);
        _model.EventValidityUpdate -= MakeDelegate(this, &ZRawProcess::OnModelValidityUpdate);
    }

    void Process()
    {
        _converter.StartProcess();
    }
protected:

    void OnOutputPathSelection(std::string path)
    {
        _model.OutputFolderPath_set(path);
    }


    void OnModelValidityUpdate(bool isValid, std::string descriprion)
    {
   
    }

    void OnMovContainerLogUpdate(std::string log)
    {
    }



    void OnConversionProcessFinish()
    {       
        // Force model validity update
        OnModelValidityUpdate(_model.IsValid(), _model.ValidityDescriprion());
    }

    ZrawProcessingModel& _model;
    ZrawConverter _converter;

    std::string _inputFilePath;
    std::string _outputFolderPath;
};
