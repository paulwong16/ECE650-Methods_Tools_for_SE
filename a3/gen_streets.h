//
// Created by Zhijie Wang on 2020/10/26.
//

#ifndef ECE650_A3_GEN_STREETS_H
#define ECE650_A3_GEN_STREETS_H

#include <vector>
#include <set>
#include <string>
#include <map>

// Struct for a coordinate
struct coordinate {
    int x;
    int y;
};


// Struct for a line segment
struct line_segment {
    coordinate start;
    coordinate end;
};


class gen_streets {
private:
    int street_number;
    int line_segment_number;
    int wait_time;
    int coordinate_range;
    std::set<std::string> street_names;
    std::map<std::string, std::vector<line_segment>> line_segments;
    std::vector<line_segment> total_line_segments;
    static int generate_random_number(int lower_bound, int upper_bound);
    static int generate_random_number_test(int lower_bound, int upper_bound);
    coordinate generate_coordinates();
    bool check_intersect(const std::string& street_name, line_segment seg);
    bool check_overlap(line_segment seg);
    static double cross_product(double a_x, double a_y, double b_x, double b_y);
    static bool check_point_on_seg(double x, double y, line_segment seg);
    static std::string generate_random_string();

public:
    explicit gen_streets(int s = 10, int n = 5, int l = 5, int c = 20);
    void generate_street_names();
    void generate_line_segments();
    std::vector<std::string> generate_output();
    std::vector<std::string> generate_remove_messages();
    int get_wait_time();
};


#endif //ECE650_A3_GEN_STREETS_H
