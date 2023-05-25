#include "check_polygon.hpp"
// #include <opencv2/opencv.hpp>

// Get current time.
int time_now(){
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

// Check if license plate box is in drawn zone.
std::tuple<std::unordered_map<int, cv::Mat>,
    std::vector<std::vector<int>>,
    std::unordered_map<int, int>>
    check_box(cv::Mat frame,
              int cam_id,
              std::vector<std::vector<cv::Point>> spots,
              std::unordered_map<int, std::tuple<std::vector<cv::Mat>, std::vector<std::vector<std::vector<int>>>>> all_coordinates,
              std::vector<std::vector<int>> plate_zone){
    
    std::unordered_map<int, cv::Mat> spot_dict; // NEW PLATES TO READ
    std::unordered_map<int, int> car_ind_dict;
    
    for(auto pair: all_coordinates){
        std::vector<cv::Mat> imgs = std::get<0>(pair.second);
        std::vector<std::vector<std::vector<int>>> boxes = std::get<1>(pair.second);
        int car_ind = pair.first;
        
        for(int j=0; j<boxes.size(); j++){
            for(int i=0; i<spots.size(); i++){
                bool plate_in_spot = checkInside(spots[i], boxes[j]);
                if(plate_in_spot && (plate_zone[cam_id][i] == -1 || plate_zone[cam_id][i] != car_ind)){
                    plate_zone[cam_id][i] = car_ind;
                    spot_dict[i] = imgs[j];
                    car_ind_dict[i] = car_ind;
                }
                // Draw the zone 
                // if(plate_zone[cam_id][i].size() != 0){
                //     cv::Point p1(plate_zone[cam_id][i][0][0], plate_zone[cam_id][i][0][1]);
                //     cv::Point p2(plate_zone[cam_id][i][1][0], plate_zone[cam_id][i][1][1]);
                //     cv::rectangle(frame, p1, p2, cv::Scalar(0,255,255), 2);
                // }
            }
        }
    }
    return {spot_dict, plate_zone, car_ind_dict};
}

// Splits string with given delimeter (the same as python split()).
std::vector<std::string> split (std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

// Get current date and time.
const std::string currentDateTime() {
    time_t     now = time(0);
    tm* tstruct;
    char       buf[80];
    tstruct = localtime(&now);
    // std::cout << tstructtm_hour<<std::endl;
    std::string time_string = "";
    strftime(buf, sizeof(buf), "%Y-%m-%d", tstruct);
    time_string += buf;
    time_string += " ";
    if (tstruct->tm_hour < 10){
        time_string += "0";
    }
    time_string += std::to_string(tstruct->tm_hour);
    time_string += ":";
    if (tstruct->tm_min < 10){
        time_string += "0";
    }
    time_string += std::to_string(tstruct->tm_min);
    time_string += ":";
    if (tstruct->tm_sec < 10){
        time_string += "0";
    }
    time_string += std::to_string(tstruct->tm_sec);
    
    // std::cout <<<std::endl;
    return time_string;
}

// Create "blank" to fill with data to send to the server.
std::unordered_map<std::string, std::string> prepare_sending_data(std::string mac_address, std::string camera_id,
    std::string spot_id, std::string license_number, std::string confidence, std::string dtime){
    std::unordered_map<std::string, std::string> data;
    data["mac_address"] = mac_address;
    data["camera_id"] = camera_id;
    data["spot_id"] = spot_id;
    data["license_number"] = license_number;
    data["confidence"] = confidence;
    data["time"] = dtime;

    return data;
}

// Function to parse a line in the /proc/meminfo file
void parse_meminfo_line(const std::string& line, std::string& name, unsigned long long& value)
{
    std::size_t pos = line.find(':');
    if (pos != std::string::npos) {
        name = line.substr(0, pos);
        value = std::stoull(line.substr(pos + 1));
    }
}

// Function to get the RAM and SWAP usage information
void get_ram_usage_info(unsigned long long& total_ram_kb, unsigned long long& free_ram_kb, unsigned long long& buffers_kb,
                        unsigned long long& cached_kb, std::vector<unsigned long long>& pid_list, std::vector<unsigned long long>& rss_list)
{
    // Read the contents of /proc/meminfo
    std::ifstream meminfo("/proc/meminfo");
    if (meminfo.is_open()) {
        std::string line;
        while (std::getline(meminfo, line)) {
            std::string name;
            unsigned long long value;
            parse_meminfo_line(line, name, value);
            if (name == "MemTotal") {
                total_ram_kb = value;
            }
            else if (name == "MemFree") {
                free_ram_kb = value;
            }
            else if (name == "Buffers") {
                buffers_kb = value;
            }
            else if (name == "Cached") {
                cached_kb = value;
            }
        }
    }
    else {
        std::cerr << "Failed to open /proc/meminfo" << std::endl;
        return;
    }
    meminfo.close();
    std::ifstream meminfo2("/proc/meminfo");
    std::string line;
    int total_swap_kb = 0, free_swap_kb = 0, used_swap_kb = 0;
    while (std::getline(meminfo2, line)) {
        if (line.find("SwapTotal") == 0) {
            total_swap_kb = std::stoi(line.substr(line.find(":") + 2)) / 1024;
        } else if (line.find("SwapFree") == 0) {
            free_swap_kb = std::stoi(line.substr(line.find(":") + 2)) / 1024;
        }
    }
    used_swap_kb = total_swap_kb - free_swap_kb;
    double used_swap_percent = (static_cast<double>(used_swap_kb) / total_swap_kb) * 100;

    


    // Read the RSS values of all running processes
    std::ifstream proc_list("/proc/self/task");
    if (proc_list.is_open()) {
        std::string line;
        while (std::getline(proc_list, line)) {
            unsigned long long pid = std::stoull(line);
            std::string status_file_path = "/proc/" + line + "/status";
            std::ifstream status(status_file_path);
            if (status.is_open()) {
                std::string name;
                unsigned long long value;
                while (std::getline(status, line)) {
                    parse_meminfo_line(line, name, value);
                    if (name == "VmRSS") {
                        pid_list.push_back(pid);
                        rss_list.push_back(value);
                        break;
                    }
                }
                status.close();
            }
            else {
                std::cerr << "Failed to open " << status_file_path << std::endl;
            }
        }
        proc_list.close();
    }
    else {
        std::cerr << "Failed to open /proc/self/task" << std::endl;
    }
    

    // Print the RAM and SWAP usage information
    double total_ram_mb = static_cast<double>(total_ram_kb) / 1024;
    double free_ram_mb = static_cast<double>(free_ram_kb + buffers_kb + cached_kb) / 1024;
    double used_ram_mb = total_ram_mb - free_ram_mb;
    double used_ram_percent = (used_ram_mb / total_ram_mb) * 100;    
    std::cout << "Total RAM: " << total_ram_mb << " MB" << std::endl;
    std::cout << "Used RAM: " << used_ram_mb << " MB (" << used_ram_percent << "%)" << std::endl;
    std::cout << "Free RAM: " << free_ram_mb << " MB" << std::endl;
    std::cout << "Swap usage: " << used_swap_kb << " MB (" << used_swap_percent << "%)" << std::endl;

    std::cout<<"--------------------------------------------------\n";

}

//Prints RAM and SWAP memory usage
void call_ram_info(){
    unsigned long long total_ram_kb, free_ram_kb, buffers_kb, cached_kb;
    std::vector<unsigned long long> pid_list, rss_list;

    get_ram_usage_info(total_ram_kb, free_ram_kb, buffers_kb, cached_kb, pid_list, rss_list);

}

//Checks if Cameras are available
std::string cameraChecker(std::vector<std::string> names, std::vector<std::string> links){
    for(int i=0; i<names.size(); i++){
        cv::VideoCapture cap(links[i]);
        if(!cap.isOpened()){
            return names[i];
        }
        cap.release();
    }
    return "True";
}

// Convert cv::Mat to QImage for UI visualization 
QImage MatToQImage(cv::Mat const& mat)
{
    // Convert the color format of the image.
    cv::Mat mat_bgr;
    if (mat.channels() == 1)
    {
        cv::cvtColor(mat, mat_bgr, cv::COLOR_GRAY2BGR); 
    }
    else if (mat.channels() == 3)
    {
        cv::cvtColor(mat, mat_bgr, cv::COLOR_BGR2RGB);
    }
    else if (mat.channels() == 4)
    {
        cv::cvtColor(mat, mat_bgr, cv::COLOR_BGRA2RGBA);
    }
    else
    {
        throw std::runtime_error("Unsupported color format!");
    }

    // Create a QImage from the OpenCV Mat.
    QImage qimage(mat_bgr.data, mat_bgr.cols, mat_bgr.rows, mat_bgr.step, QImage::Format_RGB888);

    // Return the QImage.
    return qimage.copy();
}

// Checks if license plate is in Russian format
bool checkRusFormat(std::string plate){
    std::vector<int> plateFormat = {};
    // Formats
    std::vector<std::vector<int>> RusFormats = {{1, 0, 0, 0, 1, 1, 0, 0}, // 0-Digit, 1-letter
                                                {1, 0, 0, 0, 1, 1, 0, 0, 0},
                                                {1, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 1, 1, 0, 0, 0},
                                                {0, 0, 0, 0, 1, 1, 0, 0},
                                                {1, 1, 0, 0, 0, 0, 0},
                                                {1, 1, 0, 0, 0, 0, 0, 0},
                                                {1, 1, 0, 0, 0, 1, 0, 0},
                                                {1, 1, 0, 0, 0, 1, 0, 0, 0},
                                                {1, 1, 1, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 1, 1, 0, 0},
                                                {{0, 0, 0, 0, 1, 1, 0, 0, 0}}};
    for(int i=0; i<plate.size(); i++){
    if(isalpha(plate[i])){
        plateFormat.push_back(1);
    }
    else if(isdigit(plate[i])){
        plateFormat.push_back(0);
        
    }
    }
    for(std::vector<int> formats: RusFormats){
        if(plateFormat == formats){
            return true;
        }
    }
    return false;
}

// Convert latin letters to Russian if checkRusFormat is true
std::string LatinToRus(std::string LatinPlate){
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::string rusString = "";
    wchar_t WideChar;
    for(int j=0; j<LatinPlate.size(); j++){
        if(isalpha(LatinPlate[j])){
            if(LatinPlate[j] == 'A'){
                WideChar = L'А';
                std::string toString = converter.to_bytes(WideChar);
                rusString += toString;
            }
            if(LatinPlate[j] == 'B'){
                WideChar = L'В';
                std::string toString = converter.to_bytes(WideChar);
                rusString += toString;
            }
            if(LatinPlate[j] == 'E'){
                WideChar = L'Е';
                std::string toString = converter.to_bytes(WideChar);
                rusString += toString;
            }
            if(LatinPlate[j] == 'K'){
                WideChar = L'К';
                std::string toString = converter.to_bytes(WideChar);
                rusString += toString;
            }
            if(LatinPlate[j] == 'M'){
                WideChar = L'М';
                std::string toString = converter.to_bytes(WideChar);
                rusString += toString;
            }
            if(LatinPlate[j] == 'H'){
                WideChar = L'Н';
                std::string toString = converter.to_bytes(WideChar);
                rusString += toString;
            }
            if(LatinPlate[j] == 'P'){
                WideChar = L'Р';
                std::string toString = converter.to_bytes(WideChar);
                rusString += toString;
            }
            if(LatinPlate[j] == 'C'){
                WideChar = L'С';
                std::string toString = converter.to_bytes(WideChar);
                rusString += toString;
            }
            if(LatinPlate[j] == 'O'){
                WideChar = L'О';
                std::string toString = converter.to_bytes(WideChar);
                rusString += toString;
            }
            if(LatinPlate[j] == 'T'){
                WideChar = L'Т';
                std::string toString = converter.to_bytes(WideChar);
                rusString += toString;
            }if(LatinPlate[j] == 'Y'){
                WideChar = L'У';
                std::string toString = converter.to_bytes(WideChar);
                rusString += toString;
            }if(LatinPlate[j] == 'X'){
                WideChar = L'Х';
                std::string toString = converter.to_bytes(WideChar);
                rusString += toString;
            }
        }
        else{
            rusString += LatinPlate[j];
        }
    }
    return rusString;
}

// Russian plate processing
std::string RusPlateProcess(std::string Plate){
    bool isRus = checkRusFormat(Plate);
    std::cout<<"Is Russian: "<<isRus<<std::endl;
    if(isRus){
        std::string rusPlate = LatinToRus(Plate);
        return rusPlate;
    }
    return Plate;

}
