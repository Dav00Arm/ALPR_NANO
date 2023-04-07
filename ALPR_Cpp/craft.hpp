#include "craft_onnx_utils.hpp"
#include <chrono>
using namespace std::chrono;

// This file's functions are for Craft (cropping lines) and refiner's part. 

// Normalize mean and variance of input image.
torch::Tensor normalizeMeanVariance(cv::Mat image){
    torch::Tensor tensor_image = torch::from_blob(image.data, { image.rows, image.cols, image.channels() }, at::kByte);
    tensor_image = tensor_image.permute({ 2,0,1 });
    tensor_image = tensor_image.to(torch::kFloat32);
    tensor_image.index({0}) -= 0.485*255.0;
    tensor_image.index({1}) -= 0.456*255.0;
    tensor_image.index({2}) -= 0.406*255.0;

    tensor_image.index({0}) /= 0.229*255.0;
    tensor_image.index({1}) /= 0.224*255.0;
    tensor_image.index({2}) /= 0.225*255.0;
    return tensor_image;
}

// Resize input image.
std::vector<float> resize_aspect_ratio(cv::Mat img){
    float height = img.rows, width = img.cols;
    float ratio_w, ratio_h;
    if (height == 0 or width == 0){
        ratio_h = 100.0;
        ratio_w = 100.0;
    }
    else{
        ratio_h = 100 / height;
        ratio_w = 100 / width;
    }
    std::vector<float> out = {ratio_h, ratio_w};

    return out;
}

// Call normalizeMeanVariance and resize_aspect_ratio
std::tuple<torch::Tensor, float, float> craft_preprocess(cv::Mat image){
    std::vector<float> ratios = resize_aspect_ratio(image);
    cv::Mat img_resized;
    cv::resize(image, img_resized, {100,100}, cv::INTER_AREA);

    float ratio_h = 1 / ratios[0];
    float ratio_w = 1 / ratios[1];
    
    torch::Tensor x = normalizeMeanVariance(img_resized);
    x = x.unsqueeze(0);

    return {x, ratio_h, ratio_w};
}

// A part of postprocessing 
std::vector<torch::Tensor> getDetBoxes_core(torch::Tensor textmap_, torch::Tensor linkmap_, float text_threshold, float link_threshold, float low_text){

    int img_h = textmap_.size(0);
    int img_w = textmap_.size(1); 

    // Thresholding
    
    torch::Tensor text_score = torch::ones({50,50}, torch::TensorOptions().dtype(torch::kFloat32));
    torch::Tensor link_score = torch::ones({50,50}, torch::TensorOptions().dtype(torch::kFloat32));
    // Can be optimized
    for(int i=0; i<50; i++){
        for(int j=0; j<50; j++){
            if(textmap_[i][j].item<float>() < low_text){
                text_score[i][j] = 0;
            }
            else if(textmap_[i][j].item<float>() >= low_text){
                text_score[i][j] = 1;
            }
            if(linkmap_[i][j].item<float>() < link_threshold){
                link_score[i][j] = 0;
            }
            else if(linkmap_[i][j].item<float>() >= link_threshold){
                link_score[i][j] = 1;
            }
        }
    }
    

    //Clipping
    
    // Can be optimized
    torch::Tensor to_clip = text_score + link_score, text_score_comb = torch::zeros({50,50}, torch::TensorOptions().dtype(torch::kFloat32));
    for(int i=0; i<50; i++){
        for(int j=0; j<50; j++){
            if(to_clip[i][j].item<float>() < 0){
                text_score_comb[i][j] = 0;
            }
            else if(to_clip[i][j].item<float>() > 1){
                text_score_comb[i][j] = 1;
            }      
            else{
                text_score_comb[i][j] = to_clip[i][j].item<float>();
            } 
        }
    }

     
    //1 CHANNEL TORCH::TENSOR TO CV::MAT     
    text_score_comb = text_score_comb.to(torch::kU8);
    cv::Mat text_score_comb_mat(50, 50, CV_8U);
    std::memcpy((void *) text_score_comb_mat.data, text_score_comb.data_ptr(), sizeof(torch::kU8) * text_score_comb.numel());
    
    cv::Mat labels, stats, centroids;
    cv::connectedComponentsWithStats(text_score_comb_mat, labels, stats, centroids, 4);
    torch::Tensor tensor_labels = torch::from_blob(labels.data, { 50, 50 }, torch::kInt32);
    tensor_labels = tensor_labels.to(torch::kInt32);

    std::vector<torch::Tensor> det;
    for(int k=1; k<stats.rows; k++){
        int size = stats.at<int>(k, cv::CC_STAT_AREA);

        if(size < 10){ continue; }

        if(torch::max(textmap_.index({tensor_labels==k})).item<float>() < text_threshold) { continue; } 

        torch::Tensor segmap = torch::zeros({textmap_.sizes()}, torch::TensorOptions().dtype(torch::kUInt8));
        segmap = segmap.index_put_({tensor_labels==k}, 255);
        segmap = segmap.index_put_({torch::logical_and(link_score==1, text_score==0)}, 0);
        
        int x = stats.at<int>(k, cv::CC_STAT_LEFT);
        int y = stats.at<int>(k, cv::CC_STAT_TOP); 
        int w = stats.at<int>(k, cv::CC_STAT_WIDTH);
        int h = stats.at<int>(k, cv::CC_STAT_HEIGHT);

        int niter = std::sqrt(size * std::min(w, h) / (w * h)) * 2;

        int sx = x - niter;
        int ex = x + w + niter + 1;
        int sy = y - niter;
        int ey = y + h + niter + 1;
     
        if(sx < 0){ sx=0; }
        if(sy < 0){ sy=0; }
        if(ex >=img_w){ ex=img_w; }
        if(ey >=img_h){ ey=img_h; }

        cv::Mat segmap_mat(segmap.size(0), segmap.size(1), CV_8U);
        std::memcpy((void *) segmap_mat.data, segmap.data_ptr(), sizeof(torch::kU8) * segmap.numel());
        segmap_mat = segmap_mat(cv::Range(sy, ey), cv::Range(sx, ex));

        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, {1 + niter, 1 + niter});
        cv::Mat dilated;
        cv::dilate(segmap_mat,dilated , kernel);
    
        torch::Tensor tensor_dilated = torch::from_blob(dilated.data, { dilated.rows, dilated.cols }, torch::kU8);
        tensor_dilated = tensor_dilated.to(torch::kInt32);

        segmap.index({torch::indexing::Slice(sy, ey), torch::indexing::Slice(sx, ex)}) = tensor_dilated;
        auto where = torch::where(segmap != 0);
        torch::Tensor where_tensor = torch::stack({where});
        torch::Tensor contours = torch::roll(where_tensor, 1, 0).to(torch::kU8).transpose(0,1).reshape({-1,2});
        
        cv::Mat contours_mat;
        for(int i=0; i<contours.size(0); i++){
            cv::Mat tmp(1, contours[i].size(0), CV_8U);
            std::memcpy((void *) tmp.data, contours[i].data_ptr(), sizeof(torch::kU8) * contours[i].numel());
            contours_mat.push_back(tmp);
        }
        
        contours_mat.convertTo(contours_mat, CV_32S);
        cv::RotatedRect rectangle = cv::minAreaRect(contours_mat);
        cv::Mat box;
        cv::boxPoints(rectangle, box);
        box.convertTo(box, CV_32F);

        torch::Tensor tensor_box = torch::from_blob(box.data, { box.rows, box.cols }, torch::kFloat32);
        int w_ = torch::linalg_norm(tensor_box[0] - tensor_box[1]).item<int>();
        int h_ = torch::linalg_norm(tensor_box[1] - tensor_box[2]).item<int>();

        float box_ratio = std::max({w_, h_}) / (std::min({w_, h_}) + 1e-5);

        //WARNING
        // if(std::abs(1 - box_ratio) <= 0.5){
        int l_ = torch::min(contours.index({torch::indexing::Slice(0, torch::indexing::None), 0})).item<int>();
        int r_ = torch::max(contours.index({torch::indexing::Slice(0, torch::indexing::None), 0})).item<int>();
        int t_ = torch::min(contours.index({torch::indexing::Slice(0, torch::indexing::None), 1})).item<int>();
        int b_ = torch::max(contours.index({torch::indexing::Slice(0, torch::indexing::None), 1})).item<int>();
        tensor_box = torch::zeros({4, 2});
        tensor_box[0][0] = l_, tensor_box[0][1] = t_, tensor_box[1][0] = r_, tensor_box[1][1] = t_;
        tensor_box[2][0] = r_, tensor_box[2][1] = b_, tensor_box[3][0] = l_, tensor_box[3][1] = b_;             
        // }
        int startidx = torch::argmin(torch::sum(tensor_box, 1)).item<int>();

        tensor_box = torch::roll(tensor_box, 4-startidx, 0).to(torch::kFloat32);

        det.push_back(tensor_box);
    }
    return det;
}

std::vector<torch::Tensor> getDetBoxes(torch::Tensor textmap_, torch::Tensor linkmap_, float text_threshold, float link_threshold, float low_text){
    std::vector<torch::Tensor> boxes = getDetBoxes_core(textmap_, linkmap_, text_threshold, link_threshold, low_text);
    return boxes;
}

std::vector<torch::Tensor> adjustResultCoordinates(std::vector<torch::Tensor> polys, float ratio_w, float ratio_h){
    if (polys.size()>0){
        for(int k=0; k<polys.size(); k++){
            polys[k][0][0] *= (ratio_w * 2);
            polys[k][1][0] *= (ratio_w * 2);
            polys[k][2][0] *= (ratio_w * 2);
            polys[k][3][0] *= (ratio_w * 2);

            polys[k][0][1] *= (ratio_h * 2);
            polys[k][1][1] *= (ratio_h * 2);
            polys[k][2][1] *= (ratio_h * 2);
            polys[k][3][1] *= (ratio_h * 2);
        }
    }


    return polys;
}


// Call getDetBoxes and adjust coordinates as they were normalized.
std::vector<torch::Tensor> get_boxes(torch::Tensor score_text, torch::Tensor score_link, float text_threshold, float link_threshold, float low_text, float ratio_w, float ratio_h){
    std::vector<torch::Tensor> boxes;
    boxes = getDetBoxes(score_text, score_link, text_threshold, link_threshold, low_text);

    boxes = adjustResultCoordinates(boxes, ratio_w, ratio_h);

    return boxes;
}


// Main craft inference function.
std::vector<torch::Tensor> predict_craft(cv::Mat image){

    //CRAFT

    torch::Tensor x;
    float ratio_h, ratio_w;
    std::tie(x, ratio_h, ratio_w) = craft_preprocess(image);

    x = x.contiguous();
    std::vector<float> inputCraft(x.data_ptr<float>(), x.data_ptr<float>() + x.numel());
    auto inputTensorCraft = Ort::Value::CreateTensor<float>(memory_info, inputCraft.data(), inputCraft.size(), inputShapeCraft.data(), inputShapeCraft.size());
    
    auto outputTensorCraft = craft_session.Run(Ort::RunOptions{nullptr},
                                                inputNamesCraft.data(),
                                                &inputTensorCraft,
                                                1,
                                                outputNamesCraft.data(),
                                                2
                                                );

    auto* yOutput = outputTensorCraft[0].GetTensorData<float>();
    std::vector<int64_t> outputShape_y = outputTensorCraft[0].GetTensorTypeAndShapeInfo().GetShape();
    size_t count_y = outputTensorCraft[0].GetTensorTypeAndShapeInfo().GetElementCount();
    std::vector<float> output_y(yOutput, yOutput + count_y);
    
    auto* featureOutput = outputTensorCraft[1].GetTensorData<float>();
    std::vector<int64_t> outputShape_feature = outputTensorCraft[1].GetTensorTypeAndShapeInfo().GetShape();
    size_t count_feature = outputTensorCraft[1].GetTensorTypeAndShapeInfo().GetElementCount();
    std::vector<float> output_feature(featureOutput, featureOutput + count_feature);

    auto y = torch::from_blob(output_y.data(), {1,50,50,2}).to(torch::kFloat32);
    auto feature = torch::from_blob(output_feature.data(), {1,32,50,50}).to(torch::kFloat32);

    //REFINER
    y = y.contiguous();
    feature = feature.contiguous();

    std::vector<float> inputRefinerY(y.data_ptr<float>(), y.data_ptr<float>() + y.numel());
    std::vector<float> inputRefinerFeature(feature.data_ptr<float>(), feature.data_ptr<float>() + feature.numel());

    std::vector<Ort::Value> inputRefiner;
    inputRefiner.emplace_back(Ort::Value::CreateTensor<float>(memory_info, inputRefinerY.data(), inputRefinerY.size(), inputShapeRefinerY.data(), inputShapeRefinerY.size()));
    inputRefiner.emplace_back(Ort::Value::CreateTensor<float>(memory_info, inputRefinerFeature.data(), inputRefinerFeature.size(), inputShapeRefinerFeature.data(), inputShapeRefinerFeature.size()));

    auto outputTensorRefiner = refinet_session.Run(Ort::RunOptions{nullptr},
                                            inputNamesRefiner.data(),
                                            inputRefiner.data(),
                                            2,
                                            outputNamesRefiner.data(),
                                            1
                                            );

    auto* refinerOutput = outputTensorRefiner[0].GetTensorData<float>();
    std::vector<int64_t> outputShape_refiner = outputTensorRefiner[0].GetTensorTypeAndShapeInfo().GetShape();
    size_t count_refiner = outputTensorRefiner[0].GetTensorTypeAndShapeInfo().GetElementCount();
    std::vector<float> output_refiner(refinerOutput, refinerOutput + count_refiner);
    torch::Tensor y_refiner = torch::from_blob(output_refiner.data(), {50,50,1}).to(torch::kFloat32);
    y_refiner = torch::from_blob(output_refiner.data(), {50,50,1}).to(torch::kFloat32);
    
    torch::Tensor score_text = y.index({0,torch::indexing::Slice(0, torch::indexing::None, 1), torch::indexing::Slice(0, torch::indexing::None, 1), 0});
    torch::Tensor score_link = y_refiner.index({torch::indexing::Slice(0, torch::indexing::None, 1), torch::indexing::Slice(0, torch::indexing::None, 1), 0});
    std::vector<torch::Tensor> boxes = get_boxes(score_text, score_link, 0.7, 0.4, 0.4, ratio_w, ratio_h);


    return boxes;
}

// Crop image with given coordinates.
cv::Mat cropping_image(cv::Mat img, torch::Tensor pts){

    int maximum_y = img.rows;
    int maximum_x = img.cols;

    cv::Mat pts_mat(pts.size(0), pts.size(1), CV_32F);
    pts_mat.at<float>(0, 0) = pts[0][0].item<float>();
    pts_mat.at<float>(1, 0) = pts[1][0].item<float>();
    pts_mat.at<float>(2, 0) = pts[2][0].item<float>();
    pts_mat.at<float>(3, 0) = pts[3][0].item<float>();

    pts_mat.at<float>(0, 1) = pts[0][1].item<float>();
    pts_mat.at<float>(1, 1) = pts[1][1].item<float>();
    pts_mat.at<float>(2, 1) = pts[2][1].item<float>();
    pts_mat.at<float>(3, 1) = pts[3][1].item<float>();

    cv::Rect rect = cv::boundingRect(pts_mat);
    int x = rect.x;
    int y = rect.y;
    int w = rect.width;
    int h = rect.height;

    int x_min = std::max({0, x});
    int x_max = std::min({x+w, maximum_x});
    int y_min = std::max({0, y});
    int y_max = std::min({y+h, maximum_y});

    cv::Mat cropped = img(cv::Range(y_min, y_max), cv::Range(x_min, x_max));

    cv::cvtColor(cropped,cropped,cv::COLOR_BGR2GRAY);
    return cropped; 
}

// Craft call function.
std::vector<std::vector<cv::Mat>> crop_lines(cv::Mat image){
    std::vector<std::vector<cv::Mat>> all_results;
    if(image.rows == 0){
        cv::Mat empty;
        all_results.push_back(empty);
    }
    else{
        std::vector<torch::Tensor> polys;
        polys = predict_craft(image);
        std::vector<cv::Mat> cropped;
        for(int poly=0; poly<polys.size(); poly++){
            cropped.push_back(cropping_image(image, polys[poly]));
        }
        all_results.push_back(cropped);
    }
    return all_results;
}