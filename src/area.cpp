#include "area.h"

void Area_Handler::insertArea(std::string name, std::string color, int cameraId, int id, int x, int y, int width, int height)
{
    area_list.push_back(Area(x,y,width,height,id,cameraId,name,color));
}

void Area_Handler::deleteArea(int areaId)
{
    if (areaId == -1) {
        area_list.clear();
    } else {
        for (auto it = area_list.begin(); it != area_list.end(); it++) {  
            if (it->areaId == areaId) {
                it = area_list.erase(it);  
                break;
            }
        }
    }
    
}

void Area_Handler::init(){
    current_time=std::chrono::system_clock::now();
}

void Area_Handler::update(cv::Mat& frame) {
    prev_time=current_time;
    current_time=std::chrono::system_clock::now();
    for (Area a:area_list) {
        int r = std::stoi(a.color.substr(1, 2), nullptr, 16);
        int g = std::stoi(a.color.substr(3, 2), nullptr, 16);
        int b = std::stoi(a.color.substr(5, 2), nullptr, 16);
        cv::putText(frame, std::to_string(a.areaId) + " : " + a.areaName, cv::Point(a.x, a.y - 5),
                    0, 0.6, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
        cv::rectangle(frame, cv::Rect(a.x,a.y,a.width,a.height), cv::Scalar(b, g, r), 2);
    }
}

bool Area_Handler::area_within(Area a2, int id) {
    for (Area a1:area_list) {
        if (a1.x<a2.x && a1.y<a2.y && a1.x+a1.width>a2.x+a2.width/2 && a1.y+a1.height>a2.y+a2.height/2){
            if(objects[a1.areaId].find(id) == objects[a1.areaId].end()){
                objects[a1.areaId][id].Tbegin=current_time;
                objects[a1.areaId][id].Tend=current_time;
            }
            else{
                objects[a1.areaId][id].Tend=current_time;
            }
            
            return true;
        }
    }
    return false;
}

void Area_Handler::draw_area(cv::Mat& frame, Area a, int id) {
    int idx=id+static_cast<int>(area_list.size());
    cv::putText(frame, cv::format("%d", id), cv::Point(a.x, a.y - 5), 0, 0.6, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    cv::rectangle(frame, cv::Rect(a.x,a.y,a.width,a.height), cv::Scalar(37 * idx % 255, 17 * idx % 255, 29 * idx % 255), 2);
}

std::vector<People_count> Area_Handler::calc_peoplecount() {
    std::vector<People_count> result;
    for (const Area& a : area_list) {
        int count = 0;
        for (const auto& obj : objects[a.areaId]) {
            if (obj.second.Tend >= current_time)
                count++;
        }
        result.emplace_back(a.areaId, count, prev_time, current_time);
        /*std::cout << "Area ID = " << a.areaId
                    << ", people count = " << count
                    << ", Begin Time = " << prev_time.time_since_epoch().count()
                    << ", End Time = " << current_time.time_since_epoch().count()
                    << std::endl;*/
    }
    return result;
}

std::vector<People_stay> Area_Handler::calc_timespent() {
    std::vector<People_stay> result;

    for (const auto& area_pair : objects) {
        int area_id = area_pair.first;
        auto& object_map = area_pair.second;

        for (const auto& obj_pair : object_map) {
            int object_id = obj_pair.first;
            const auto& obj_info = obj_pair.second;

            auto time_since_last_seen = std::chrono::duration_cast<std::chrono::seconds>(current_time - obj_info.Tend);

            if (time_since_last_seen.count() < elapsed_time) {
                auto it = std::find(lost_objects.begin(), lost_objects.end(), object_id);
                if (it != lost_objects.end()) {
                    lost_objects.erase(it);
                }
            }

            if (time_since_last_seen.count() >= 1) {
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(obj_info.Tend - obj_info.Tbegin);

                if (duration.count() > 0 && std::find(lost_objects.begin(), lost_objects.end(), object_id) == lost_objects.end()) {
                    result.emplace_back(area_id, duration.count(), obj_info.Tbegin, obj_info.Tend);
                    lost_objects.push_back(object_id);
                    /*std::cout << "Added object: Area ID = " << area_id
                              << ", Object ID = " << object_id
                              << ", Duration = " << duration.count() << " seconds"
                              << ", Begin Time = " << obj_info.Tbegin.time_since_epoch().count()
                              << ", End Time = " << obj_info.Tend.time_since_epoch().count()
                              << std::endl;*/
                }
            }
        }
    }

    return result;
}

std::vector<People_move> Area_Handler::calc_path() {
    std::vector<People_move> result;
    std::map<std::pair<int, int>, int> path_counts;
    
    for (const auto& area_entry : objects) {
        int from_area_id = area_entry.first;
        
        for (const auto& object_entry : area_entry.second) {
            int object_id = object_entry.first;
            const ObjectInfo& info = object_entry.second;
            
            for (const auto& other_area_entry : objects) {
                int to_area_id = other_area_entry.first;
                
                if (from_area_id == to_area_id) continue;
                
                auto it = other_area_entry.second.find(object_id);
                if (it != other_area_entry.second.end()) {
                    const ObjectInfo& other_info = it->second;
                    
                    if (other_info.Tbegin > info.Tend && 
                        std::chrono::duration_cast<std::chrono::seconds>(current_time - other_info.Tend).count() <= elapsed_time) {
                        path_counts[{from_area_id, to_area_id}]++;
                    }
                }
            }
        }
    }

    std::chrono::duration<double> elapsed_system_time(elapsed_time);
    for (const auto& path : path_counts) {
        result.emplace_back(path.first.first, path.first.second, path.second, std::chrono::time_point_cast<std::chrono::system_clock::duration>(current_time - elapsed_system_time), current_time);
        /*std::cout << "from_area_id = " << path.first.first
                    << ", to_area_id = " << path.first.second
                    << ", count = " << path.second
                    << ", Begin Time = " << prev_time.time_since_epoch().count()
                    << ", End Time = " << current_time.time_since_epoch().count()
                    << std::endl;*/
    }
    
    return result;
}