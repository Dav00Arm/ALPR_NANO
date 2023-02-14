#include <torch/torch.h>
#include <map>
#include <string>
torch::nn::Conv2dOptions
create_conv_options(int64_t in_planes, int64_t out_planes, int64_t kerner_size,
                    int64_t stride = 1, int64_t padding = 0, int64_t groups = 1,
                    int64_t dilation = 1, bool bias = false)
{
    torch::nn::Conv2dOptions conv_options =
        torch::nn::Conv2dOptions(in_planes, out_planes, kerner_size)
            .stride(stride)
            .padding(padding)
            .bias(bias)
            .groups(groups)
            .dilation(dilation);

    return conv_options;
}
torch::nn::MaxPool2dOptions create_max_options(int64_t kerner_size,
                                               int64_t stride = 1, int64_t padding = 0, int64_t dilation = 1)
{
    torch::nn::MaxPool2dOptions conv_options =
        torch::nn::MaxPool2dOptions(kerner_size)
            .stride(stride)
            .padding(padding)
            .dilation(dilation);

    return conv_options;
}

// class BasicBlock{
//     int expansion = 1;
//     int stride;
//     torch::nn::Conv2d conv1 = nullptr;
//     torch::nn::Conv2d conv2 = nullptr;
//     torch::nn::BatchNorm2d bn1 = nullptr;
//     torch::nn::BatchNorm2d bn2 = nullptr;
//     torch::nn::ReLU relu = torch::nn::ReLU(true);
//     torch::nn::Sequential downsample=nullptr;

//     BasicBlock(int inplanes,int planes,int stride=1,torch::nn::Sequential downsample=nullptr){
//         conv1 = _conv3x3(inplanes,planes);
//         conv2 = _conv3x3(inplanes,planes);
//         bn1 = torch::nn::BatchNorm2d(torch::nn::BatchNorm2dOptions(inplanes));
//         bn2 = torch::nn::BatchNorm2d(torch::nn::BatchNorm2dOptions(inplanes));
//         relu = torch::nn::ReLU(true);
//         downsample=downsample;

//     }

//     torch::nn::Conv2d _conv3x3(int in_planes,int out_planes,int stride=1){
//         return torch::nn::Conv2d(create_conv_options(in_planes,out_planes,3,stride,1));
//     }

//     torch::Tensor forward(torch::Tensor x){
//         torch::Tensor residual = x;
//         torch::Tensor out = this->conv1->forward(x);
//         out = this->bn1->forward(out);
//         out = this->relu->forward(out);

//         out = this->conv2->forward(out);
//         out = this->bn2->forward(out);

//         residual = this->downsample->forward(x);
//         out += residual;
//         out = this->relu->forward(out);
//         return out;
//     }

// };

// class ResNet : torch::nn::Module{
//     std::vector<int> output_channel_block;
//     int inplanes;
//     torch::nn::Conv2d conv0_1 = nullptr;
//     torch::nn::Conv2d conv0_2,conv1 = nullptr;
//     torch::nn::Conv2d conv2 = nullptr;
//     torch::nn::Conv2d conv3 = nullptr;
//     torch::nn::Conv2d conv4_1 = nullptr;
//     torch::nn::Conv2d conv4_2 = nullptr;
//     torch::nn::BatchNorm2d bn0_1 = nullptr;
//     torch::nn::BatchNorm2d bn0_2 = nullptr;
//     torch::nn::BatchNorm2d bn1 = nullptr;
//     torch::nn::BatchNorm2d bn2 = nullptr;
//     torch::nn::BatchNorm2d bn3 = nullptr;
//     torch::nn::BatchNorm2d bn4_1 = nullptr;
//     torch::nn::BatchNorm2d bn4_2 = nullptr;
//     torch::nn::ReLU relu = torch::nn::ReLU(true);

// };

class AttnLabelConverter
{
    std::vector<std::string> characters = {"[GO]","[s]"};

public:
    AttnLabelConverter(std::string characters)
    {
        for(int i=0;i<characters.length();i++)
        {
            std::string s;
            s.push_back(characters[i]);
            this->characters.push_back(s);
        }
        // this->characters += characters;
        std::cout << this->characters.size()<<std::endl;
        std::map<char, int> dict;
    }
    std::vector<std::string> decode(torch::Tensor text_index, torch::Tensor length)
    {
        std::vector<std::string> texts;
        for (int i = 0; i < length.sizes()[0]; i++)
        {
            torch::Tensor txt = text_index[i];
            std::string text = "";
            for (int j = 0; j < txt.sizes()[0]; j++)
            {
                // int a =  txt[j].item<int>();
                text += characters[txt[j].item<int>()];
            }
            texts.push_back(text);
        }
        return texts;
    }
};
