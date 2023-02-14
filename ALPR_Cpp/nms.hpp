#include <vector>
#include <tuple>
#include <torch/torch.h>
#include <opencv2/opencv.hpp>

torch::Tensor hard_nms(torch::Tensor box_scores, int top_k=-1, int candidate_size=200){
    torch::Tensor scores = box_scores.index({torch::indexing::Slice(0, torch::indexing::None, 1), -1});
    std::vector<int> picked;
    torch::Tensor _, indexes;
    std::tie(_, indexes) = torch::sort(scores, 0,true);

    indexes = indexes.index({torch::indexing::Slice(0, candidate_size, 1)});
    int current = indexes[0].item<int>();
    picked.push_back(current);

    return box_scores.index({picked[0], torch::indexing::Slice()});
}