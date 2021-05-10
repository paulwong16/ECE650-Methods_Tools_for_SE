//
// Created by Zhijie Wang on 2020/10/26.
//

#include "gen_streets.h"
#include <fstream>
#include <iostream>
#include <random>
#include <thread>
#include <chrono>

/*!
 * Generate a random number in [lower_bound, upper_bound]
 * @param lower_bound
 * @param upper_bound
 * @return a random int number
 */
int gen_streets::generate_random_number(int lower_bound, int upper_bound) {
    int up = upper_bound - lower_bound;
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }

    unsigned int num = 42;
    urandom.read((char *)&num, sizeof(int));
    unsigned int rand_num = (unsigned int)num % (up + 1);

    urandom.close();

    return rand_num + lower_bound;
}

/*!
 * Constructor of rgen with parameters
 * @param s number of street upper bound
 * @param n line_segments upper bound
 * @param l waiting time upper bound
 * @param c coordinate range
 */
gen_streets::gen_streets(int s, int n, int l, int c) {
    street_number = generate_random_number(2, s);
    line_segment_number = n;
    wait_time = generate_random_number(5, l);
    coordinate_range = c;
}

/*!
 * Generate a random number in [lower_bound, upper_bound] (used random lib for testing on windows)
 * @param lower_bound
 * @param upper_bound
 * @return a random int number
 */
int gen_streets::generate_random_number_test(int lower_bound, int upper_bound) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<> dist(lower_bound, upper_bound);
    return dist(rd);
}

/*!
 * Generate street names
 */
void gen_streets::generate_street_names() {
    for (int i = 0; i < street_number; i ++) {
        int count = 0;
        std::string street_name = generate_random_string();
        while (street_names.find(street_name) != street_names.end()) {
            street_name = generate_random_string();
            count += 1;
            if (count >= 25) {
                std::cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << std::endl;
                exit(1);
            }
        }
        street_names.insert(street_name);
    }
}

/*!
 * Generate random coordinate.
 * @return a coordinate
 */
coordinate gen_streets::generate_coordinates() {
    int x = generate_random_number(-coordinate_range, coordinate_range);
    int y = generate_random_number(-coordinate_range, coordinate_range);
    coordinate coord = {x, y};
    return coord;
}

/*!
 * Generate all line segments
 */
void gen_streets::generate_line_segments() {
    for (const auto& street : street_names) {
        if (line_segments[street].empty()) {
            int count = 0;
            line_segment segment = {generate_coordinates(), generate_coordinates()};
            while ((segment.start.x == segment.end.x) && (segment.start.y == segment.end.y)) {
                count ++;
                if (count >= 25) {
                    std::cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << std::endl;
                    exit(1);
                }
            }
            line_segments[street].push_back(segment);
        }
        int number_of_segments_this_line = generate_random_number(1, line_segment_number);
        for (int i = 1; i < number_of_segments_this_line; i ++) {
            coordinate last_coordinate = line_segments[street].back().end;
            line_segment segment = {last_coordinate, generate_coordinates()};
            int count = 0;
            while (((segment.start.x == segment.end.x) && (segment.start.y == segment.end.y)) || check_intersect(street, segment) || check_overlap(segment)) {
                segment = {last_coordinate, generate_coordinates()};
                count ++;
                if (count >= 25) {
                    std::cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << std::endl;
                    exit(1);
                }
            }
            line_segments[street].push_back(segment);
        }
        for (auto seg : line_segments[street]) {
            total_line_segments.push_back(seg);
        }
    }
}

/*!
 * Check if a line_segment is intersect with any other line segments in this street.
 * @param street_name street to add line_segment
 * @param seg new line_segment
 * @return boolean
 */
bool gen_streets::check_intersect(const std::string& street_name, line_segment seg) {
    std::vector<line_segment> current_street_segments = line_segments[street_name];
    if (check_point_on_seg(seg.end.x, seg.end.y, current_street_segments.back())) {
        return true;
    }
    for (size_t i = 0; i < current_street_segments.size() - 1; i ++) {
        auto this_seg = current_street_segments[i];
        coordinate x_diff = {this_seg.start.x - this_seg.end.x, seg.start.x - seg.end.x};
        coordinate y_diff = {this_seg.start.y - this_seg.end.y, seg.start.y - seg.end.y};
        double product = cross_product(x_diff.x, x_diff.y, y_diff.x, y_diff.y);
        double d_0 = cross_product(this_seg.start.x, this_seg.start.y, this_seg.end.x, this_seg.end.y);
        double d_1 = cross_product(seg.start.x, seg.start.y, seg.end.x, seg.end.y);
        if (product != 0) {
            double x = cross_product(d_0, d_1, x_diff.x, x_diff.y) / product;
            double y = cross_product(d_0, d_1, y_diff.x, y_diff.y) / product;
            if ((check_point_on_seg(x, y, this_seg)) && check_point_on_seg(x, y, this_seg)) {
                return true;
            }
        }
        else if (check_point_on_seg(seg.end.x, seg.end.y, this_seg)) {
            return true;
        }
    }
    return false;
}

/*!
 * Check if a line_segment is overlapped any other line segments
 * @param seg new line_segment
 * @return a boolean
 */
bool gen_streets::check_overlap(line_segment seg) {
    for (auto this_seg : total_line_segments) {
        coordinate x_diff = {this_seg.start.x - this_seg.end.x, seg.start.x - seg.end.x};
        coordinate y_diff = {this_seg.start.y - this_seg.end.y, seg.start.y - seg.end.y};
        double product = cross_product(x_diff.x, x_diff.y, y_diff.x, y_diff.y);
        double d_0 = cross_product(this_seg.start.x, this_seg.start.y, this_seg.end.x, this_seg.end.y);
        double d_1 = cross_product(seg.start.x, seg.start.y, seg.end.x, seg.end.y);
        if ((product == 0) && (d_0 == d_1)) {
            if (check_point_on_seg(seg.start.x, seg.start.y, this_seg) || (check_point_on_seg(seg.end.x, seg.end.y, this_seg))) {
                return true;
            }
        }
    }
    return false;
}

/*!
 * Get cross product of two vectors.
 * @param a_x
 * @param a_y
 * @param b_x
 * @param b_y
 * @return a double num
 */
double gen_streets::cross_product(double a_x, double a_y, double b_x, double b_y) {
    return a_x * b_y - a_y * b_x;
}

/*!
 * Check if a point is on a line_seg
 * @param x
 * @param y
 * @param seg
 * @return a boolean
 */
bool gen_streets::check_point_on_seg(double x, double y, line_segment seg) {
    int max_x = std::max(seg.start.x, seg.end.x);
    int min_x = std::min(seg.start.x, seg.end.x);
    int max_y = std::max(seg.start.y, seg.end.y);
    int min_y = std::min(seg.start.y, seg.end.y);
    return ((max_x >= x) && (min_x <= x) && (max_y >= y) && (min_y <= y));
}

/*!
 * Generate output control commands
 * @return a string
 */
std::vector<std::string> gen_streets::generate_output() {
    std::vector<std::string> output_string;
    for (const auto& street_name : street_names) {
        std::string command = "add ";
        command += '\"' + street_name + '\"' + ' ';
        for (auto seg : line_segments[street_name]) {
            command += '(' + std::to_string(seg.start.x) + ',' + std::to_string(seg.start.y) + ')' + ' ';
        }
        command += '(' + std::to_string(line_segments[street_name].back().end.x) + ',' + std::to_string(line_segments[street_name].back().end.y) + ')';
        output_string.push_back(command);
    }
    return output_string;
}

/*!
 * Generate 'rm' command
 * @return a string
 */
std::vector<std::string> gen_streets::generate_remove_messages() {
    std::vector<std::string> msgs;
    for (const auto& street_name : street_names) {
        std::string command = "rm ";
        command += '\"' + street_name + '\"' + ' ';
        msgs.push_back(command);
    }
    return msgs;
}

/*!
 * Generate a random string
 * @return a string
 */
std::string gen_streets::generate_random_string() {
    std::string str;
    int str_length = generate_random_number(10, 15);
    for (int i = 0; i < str_length; i ++) {
        str += char(generate_random_number(97,  97 + 25));
    }
    return str;
}

/*!
 * Get the waiting time for this round
 * @return int
 */
int gen_streets::get_wait_time() {
    return wait_time;
}


int main (int argc, char **argv) {
    int s = 10;
    int n = 5;
    int l = 5;
    int c = 20;
    for (int i = 1; i <=argc - 1; i+=2) {
        if (argv[i][1] == 's') {
            s = std::atoi(argv[i + 1]);
        }
        else if (argv[i][1] == 'n') {
            n = std::atoi(argv[i + 1]);
        }
        else if (argv[i][1] == 'l') {
            l = std::atoi(argv[i + 1]);
        }
        else if (argv[i][1] == 'c') {
            c = std::atoi(argv[i+1]);
        }
        else {
            std::cerr << "Error: Unknown command" << std::endl;
            exit(1);
        }
    }
    if (s < 2) {
        std::cerr << "Error: s should be greater or equal to 2" << std::endl;
        exit(1);
    }
    if (n < 1) {
        std::cerr << "Error: n should be greater or equal to 1" << std::endl;
        exit(1);
    }
    if (l < 5) {
        std::cerr << "Error: l should be greater or equal to 5" << std::endl;
        exit(1);
    }
    if (c < 1) {
        std::cerr << "Error: c should be greater or equal to 1" << std::endl;
        exit(1);
    }
    while (true) {
        if (std::cin.eof()) {
            break;
        }
        gen_streets generator(s, n, l, c);
        generator.generate_street_names();
        generator.generate_line_segments();
        std::vector<std::string> output_string = generator.generate_output();
        for (const auto& str : output_string) {
            std::cout << str << std::endl;
        }
        std::cout << "gg" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(generator.get_wait_time()));
        std::vector<std::string> remove_msgs = generator.generate_remove_messages();
        for (const auto& str : remove_msgs) {
            std::cout << str << std::endl;
        }
    }
}