#include <torch/torch.h>
#include <map>
#include <string>

// This file's content is for OCR. Whole code is copied so you can pass this).

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
