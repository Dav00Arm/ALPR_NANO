
// OCR model parts. Some of this functions are not used.
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

torch::Tensor linalg_norm(torch::Tensor *ptr_tensor, int dim = 0, bool keepdim = false) {
    return torch::sqrt(torch::sum(torch::pow(*ptr_tensor, 2), {dim}, keepdim));
}

class LocalizationNetwork
{
    public:
        int F, I_channel_num;
        torch::nn::Sequential conv=nullptr;
        torch::nn::Sequential localization_fc1=nullptr;
        torch::nn::Linear localization_fc2=nullptr;
        LocalizationNetwork(int F=20,int I_channel_num=1)
        {
            F = F;
            I_channel_num = I_channel_num;
        
        torch::nn::Sequential conv = torch::nn::Sequential(
                                    torch::nn::Conv2d(create_conv_options(I_channel_num,64,3,1,1)),
                                    torch::nn::BatchNorm2d(torch::nn::BatchNorm2d(64)),
                                    torch::nn::MaxPool2d(create_max_options(2,2)),
                                    torch::nn::Conv2d(create_conv_options(64,128,3,1,1)),
                                    torch::nn::BatchNorm2d(torch::nn::BatchNorm2d(64)),
                                    torch::nn::MaxPool2d(create_max_options(2,2)),
                                    torch::nn::Conv2d(create_conv_options(128,256,3,1,1)),
                                    torch::nn::BatchNorm2d(torch::nn::BatchNorm2dOptions(256)),
                                    torch::nn::MaxPool2d(create_max_options(2,2)),
                                    torch::nn::Conv2d(create_conv_options(256,512,3,1,1)),
                                    torch::nn::BatchNorm2d(torch::nn::BatchNorm2dOptions(512)),
                                    torch::nn::AdaptiveAvgPool2d(torch::nn::AdaptiveAvgPool2dOptions(1)));
        torch::nn::Sequential localization_fc1 = torch::nn::Sequential( 
                                            torch::nn::Linear(torch::nn::LinearOptions(512,256)),
                                            torch::nn::ReLU(torch::nn::ReLUOptions(true)));
        torch::nn::Linear localization_fc2 = torch::nn::Linear(torch::nn::LinearOptions(256,F*2));
        
        // torch::nn::Linear localization_fc2 = torch::nn::Linear(torch::nn::LinearOptions(256,F*2));
        // torch::OrderedDict<std::string,torch::Tensor> nmd_params = localization_fc2->named_parameters(false);
        // torch::Tensor* w = nmd_params.find("weight");
        // *w = *w.fill_(0);
        // torch::nn::fill(*w,0.0);
        
        }
        torch::Tensor forward(torch::Tensor batch_I)
        
        {
            int batch_size = batch_I.size(0);
            torch::Tensor features = conv->forward(batch_I);
            // features = features.view({batch_size,-1});
            torch::Tensor batch_C_prime = localization_fc2->forward(localization_fc1->forward(features).view(-1));
            
            return batch_C_prime;
        }
};

class GridGenerator: public torch::nn::Module{

    public:
        // int F;
        std::vector<int> I_r_size;
        int F=20;
        int I_r_height = 32;
        int I_r_width = 100;
        double eps = 1e-6;
        GridGenerator(int F=20,std::vector<int> inf={32,100})
        {
            F = F;
            I_r_height= inf[0];
            I_r_width = inf[1];
        }

        // int I_r_height = 32;
        // int I_r_width = I_r_size[1];
        
        torch::Tensor C = _build_C(F);
        torch::Tensor P = _build_P(I_r_width,I_r_height);
        torch::Tensor inv_delta_C = torch::Tensor(_build_inv_delta_C(F,C));
        torch::Tensor P_hat = torch::Tensor(_build_P_hat(F,C,P));
        
        torch::Tensor _build_C(int F)
        {
            std::cout<< "ajdhcijudn"<<std::endl;
            torch::Tensor ctrl_pts_x = torch::linspace(-1.0,1.0,int(F/2));
            torch::Tensor ctrl_pts_y_top = -1 * torch::ones(int(F / 2));
            torch::Tensor ctrl_pts_y_bottom = torch::ones(int(F / 2));
            torch::Tensor ctrl_pts_top = torch::stack({ctrl_pts_x,ctrl_pts_y_top},1);
            torch::Tensor ctrl_pts_bottom = torch::stack({ctrl_pts_x,ctrl_pts_y_bottom},1);
            torch::Tensor C = torch::concat({ctrl_pts_top,ctrl_pts_bottom},0);
            return C;
        }
        
        torch::Tensor _build_P(int w,int h)
        {
            torch::Tensor I_r_gtrid_x = (torch::arange(-w,w,2) + 1.0) / w;
            torch::Tensor I_r_gtrid_y = (torch::arange(-h,h,2) + 1.0) / h;
            torch::Tensor P = torch::stack(
                                {torch::meshgrid({I_r_gtrid_x,I_r_gtrid_y})},2);
            return P.reshape({-1,2});

        }
        
        torch::Tensor _build_inv_delta_C(int F,torch::Tensor C)
        {
            torch::Tensor hat_C = torch::zeros({F,F});
            std::cout << C.sizes()<<std::endl;
            for(int i=0;i<F;i++)
            {
                for(int j=i;j<F;j++)
                {
                    // std::cout << C.index({i,torch::indexing::Slice(0,torch::indexing::None)}).sizes()<<std::endl;
                    torch::Tensor b = C.index({i,torch::indexing::Slice(0,torch::indexing::None)}) - C.index({j,torch::indexing::Slice(0,torch::indexing::None)});
                    // std::cout << b<<std::endl;
                    torch::Tensor norm = linalg_norm(b);
                    hat_C.index({i,j}) = norm;
                    hat_C.index({j,i}) = norm;
                }
            }
            hat_C = hat_C.fill_diagonal_(1);
            hat_C = (hat_C * hat_C)* torch::log(hat_C);
            torch::Tensor delta_C = torch::concat(\
                                {torch::concat({torch::ones ({F,1}),C,hat_C},1),
                                torch::concat({torch::zeros({2,3}),C.transpose(0,1)},1),
                                torch::concat({torch::zeros({1,3}),torch::ones({1,F})},1)
                                },0);
            torch::Tensor inv_delta_C = torch::linalg::inv(delta_C);
            return inv_delta_C;
        }
        torch::Tensor _build_P_hat(int F,torch::Tensor C, torch::Tensor P)
        {
            int n = P.sizes()[0];
            
            torch::Tensor P_expand = P.view({P.sizes()[0],1,P.sizes()[1]});
            std::cout<< "AA"<<std::endl;
            torch::Tensor P_tile = torch::tile(P_expand,{1,F,1});
            torch::Tensor C_tile = C.view({1,C.sizes()[0],C.sizes()[1]});
            torch::Tensor P_diff = P_tile - C_tile;
            // torch::Tensor rbf_norm = torch::linalg::norm(P_diff,"2",2,false,torch::kFloat16);
            torch::Tensor rbf_norm = torch::linalg::vector_norm(P_diff, 2, 2, false, c10::nullopt);
            torch::Tensor rbf = torch::multiply(torch::square(rbf_norm),torch::log(rbf_norm + eps));
            torch::Tensor P_hat = torch::concat({torch::ones({n,1}),P,rbf},1);
            return P_hat;
        }

        torch::Tensor build_P_prime(torch::Tensor batch_C_prime)
        {
            int batch_size = batch_C_prime.sizes()[0];
            torch::Tensor batch_inv_delta_C = inv_delta_C.repeat({batch_size,1,1});
            torch::Tensor batch_P_hat = P_hat.repeat({batch_size,1,1});
            torch::Tensor batch_C_prime_with_zeros = torch::cat({batch_C_prime,torch::zeros({batch_size,3,2})},1);
            torch::Tensor batch_T = torch::bmm(batch_inv_delta_C,batch_C_prime_with_zeros);
            torch::Tensor batch_P_prime = torch::bmm(batch_P_hat,batch_T);
            return batch_P_prime;

        }
    };

class TPS_SpatialTransformerNetwork{
    public:
        int F,I_channel_num;
        std::vector<int> I_size,I_r_size;
        LocalizationNetwork locnet;
        GridGenerator gridgen;
        TPS_SpatialTransformerNetwork(int F=20, std::vector<int> I_size={32,100},std::vector<int> I_r_size={32,100},int I_channel_num=1)
        {
            F = F;
            F = F;
            I_size = I_size;
            I_r_size = I_r_size;
            I_channel_num = I_channel_num;
            std::cout << "BBB"<<std::endl;
            LocalizationNetwork locnet(F,I_channel_num);
            
            GridGenerator gridgen(F,I_r_size);
        }
        torch::Tensor forward(torch::Tensor batch_I)
        {
            
            torch::Tensor batch_C_prime=  this->locnet.forward(batch_I);
            // torch::Tensor batch_C_prime = torch::ones({5,10});
            torch::Tensor build_P_prime = this->gridgen.build_P_prime(batch_C_prime);
            torch::Tensor build_P_prime_reshape = build_P_prime.reshape({build_P_prime.size(0),I_r_size[0],I_r_size[1],2});
            torch::Tensor batch_I_r = torch::nn::functional::grid_sample(batch_I,build_P_prime_reshape,\
                                                                torch::nn::functional::GridSampleFuncOptions().mode(torch::kBilinear).\
                                                                padding_mode(torch::kBorder).align_corners(true));
            return batch_I_r;
        }

};