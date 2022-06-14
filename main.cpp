#include <iostream>

#include <torchaudio/csrc/sox/io.h>
#include <torchaudio/csrc/sox/effects.h>

#ifdef ENABLE_VISUALIZATION
#include <QString>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void showWindowWithPlot(const std::string &_title, const cv::Size _windowsize, const float *_data, const int _datalength, float _ymax, float _ymin, cv::Scalar _color)
{
    if(_datalength > 0 && _windowsize.area() > 0 && _data != NULL ) {

        cv::Mat _colorplot = cv::Mat::zeros(_windowsize, CV_8UC3);
        cv::rectangle(_colorplot,cv::Rect(0,0,_colorplot.cols,_colorplot.rows),cv::Scalar(20,20,20), -1);

        int _ticksX = 10;
        float _tickstepX = static_cast<float>(_windowsize.width)/ _ticksX ;
        for(int i = 1; i < _ticksX ; i++)
            cv::line(_colorplot, cv::Point2f(i*_tickstepX,0), cv::Point2f(i*_tickstepX,static_cast<float>(_colorplot.rows)), cv::Scalar(100,100,100), 1);

        int _ticksY = 10;
        float _tickstepY = static_cast<float>(_windowsize.height)/ _ticksY ;
        for(int i = 1; i < _ticksY ; i++) {
            cv::line(_colorplot, cv::Point2f(0,i*_tickstepY), cv::Point2f(static_cast<float>(_colorplot.cols),i*_tickstepY), cv::Scalar(100,100,100), 1);
            cv::putText(_colorplot, QString::number(_ymax - i * (_ymax-_ymin)/_ticksY,'f',1).toStdString(),
                        cv::Point2f(5, i*_tickstepY - 10), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(150,150,150), 1, cv::LINE_AA);
        }

        float invstepY = static_cast<float>(_ymax - _ymin) / _windowsize.height;
        float stepX = static_cast<float>(_windowsize.width) / (_datalength - 1);

        for(int i = 0; i < _datalength - 1; i++) {
            cv::line(_colorplot, cv::Point2f(i*stepX, _windowsize.height - static_cast<float>(_data[i] - _ymin)/invstepY),
                                 cv::Point2f((i+1)*stepX, _windowsize.height - static_cast<float>(_data[i+1] - _ymin)/invstepY),
                                 _color, 1, cv::LINE_AA);
        }
        cv::imshow(_title, _colorplot);
        cv::waitKey(0);
    }
}
#endif

using namespace std;

int main(int argc, char** argv)
{
    if(argc != 2) {
        std::cout << "Provide filename to read" << std::endl;
        return 1;
    }
    auto [a, b, c, d, s] = torchaudio::sox_io::get_info_file(argv[1],"wav");
    std::cout << "FILE META INFORMATION" << std::endl;
    std::cout << " - sample rate:         "<< a << std::endl;
    std::cout << " - samples per channel: " << b << std::endl;
    std::cout << " - channels:            " << c << std::endl;
    std::cout << " - bits per sample:     " << d << std::endl;
    std::cout << " - encoding:            " << s << std::endl;

    std::cout << "TORCH TENSOR AFTER FILE READING" << std::endl;
    std::tuple<torch::Tensor, int64_t> data = torchaudio::sox_io::load_audio_file(argv[1],0,-1,true,true,"wav");
    auto tensor = std::get<0>(data);
    std::cout << " - size: ";
    for(size_t i = 0; i < tensor.sizes().size(); ++i) {
        if(i != 0)
            std::cout << "x";
        std::cout << tensor.sizes()[i];
    }
    std::cout << std::endl;
    std::cout << " - frame rate: " << std::get<1>(data) << std::endl;

    std::cout << "RESAMPLING" << std::endl;
    torchaudio::sox_effects::initialize_sox_effects();
    std::vector<std::vector<std::string>> effects;
    effects.push_back({"rate", "8000"});
    data = torchaudio::sox_effects::apply_effects_tensor(tensor,std::get<1>(data),effects,true);
    tensor = std::get<0>(data);
    std::cout << " - size: ";
    for(size_t i = 0; i < tensor.sizes().size(); ++i) {
        if(i != 0)
            std::cout << "x";
        std::cout << tensor.sizes()[i];
    }
    std::cout << std::endl;
    std::cout << " - frame rate: " << std::get<1>(data) << std::endl;
    torchaudio::sox_effects::shutdown_sox_effects();

    // Let's normalize by max amplitude
    //tensor[0] = tensor[0] / torch::max(torch::abs(tensor[0]));

#ifdef ENABLE_VISUALIZATION
    size_t length = tensor.sizes()[1];
    float *audio = new float[length];
    for(size_t i = 0; i < length; ++i)
        audio[i] = tensor[0][i].item().toFloat();
    showWindowWithPlot("probe",cv::Size(1280,480),audio,length,1.0f,-1.0f,cv::Scalar(0,255,0));
#endif
    return 0;
}
