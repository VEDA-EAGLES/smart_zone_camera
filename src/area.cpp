#include "area.h"

void Area_Handler::init(){
    current_time=std::chrono::system_clock::now();
}

void Area_Handler::update(std::vector<Area> list,cv::Mat& frame) {
    area_list=list;
    prev_time=current_time;
    current_time=std::chrono::system_clock::now();
    for (Area a:area_list) {
        int id=a.area_id;
        cv::putText(frame, cv::format("%d : %s", id, a.area_name.c_str()), cv::Point(a.x, a.y - 5),
                    0, 0.6, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
        cv::rectangle(frame, cv::Rect(a.x,a.y,a.width,a.height), cv::Scalar(37 * id % 255, 17 * id % 255, 29 * id % 255), 2);
    }
}

bool Area_Handler::area_within(Area a2, int id) {
    for (Area a1:area_list) {
        if (a1.x<a2.x && a1.y<a2.y && a1.x+a1.width>a2.x+a2.width/2 && a1.y+a1.height>a2.y+a2.height/2){
            if(objects[a1.area_id].find(id) == objects[a1.area_id].end()){
                objects[a1.area_id][id].Tbegin=current_time;
                objects[a1.area_id][id].Tend=current_time;
            }
            else{
                objects[a1.area_id][id].Tend=current_time;
            }
            /*std::cout<<"object "<<id<<" in "<<a1.area_id<<std::endl;
            std::time_t start_time = std::chrono::system_clock::to_time_t(objects[a1.area_id][id].Tbegin);
            std::time_t end_time = std::chrono::system_clock::to_time_t(objects[a1.area_id][id].Tend);
            std::cout<<"start time: "<<std::ctime(&start_time)<<", end time: "<<std::ctime(&end_time)<<std::endl;*/
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
        for (const auto& obj : objects[a.area_id]) {
            if (obj.second.Tend >= current_time)
                count++;
        }
        result.emplace_back(a.area_id, count, prev_time, current_time);
    }
    return result;
}

std::vector<People_stay> Area_Handler::calc_timespent() {
    std::vector<People_stay> result;

    for (const auto& area_pair : objects) {
        int area_id = area_pair.first;
        auto& object_map = area_pair.second;

        std::vector<int> objects_to_remove;

        for (const auto& obj_pair : object_map) {
            int object_id = obj_pair.first;
            const auto& obj_info = obj_pair.second;

            // 객체가 1초 이상 사라졌는지 확인
            auto time_since_last_seen = std::chrono::duration_cast<std::chrono::seconds>(current_time - obj_info.Tend);
            if (time_since_last_seen.count() >= 1) {
                // 머문 시간 계산
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(obj_info.Tend - obj_info.Tbegin);

                // 머문 시간이 0보다 큰 경우에만 결과에 추가
                if (duration.count() > 0) {
                    result.emplace_back(area_id, duration.count(), obj_info.Tbegin, obj_info.Tend);
                    objects_to_remove.push_back(object_id);
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
                        std::chrono::duration_cast<std::chrono::seconds>(current_time - other_info.Tend).count() <= 5.0) {
                        path_counts[{from_area_id, to_area_id}]++;
                    }
                }
            }
        }
    }
    
    for (const auto& path : path_counts) {
        result.emplace_back(path.first.first, path.first.second, path.second, prev_time, current_time);
    }
    
    return result;
}