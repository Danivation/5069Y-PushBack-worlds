#pragma once
#include "danielib/danielib.hpp" // IWYU pragma: keep
#include "pros/imu.hpp" // IWYU pragma: keep
#include <cmath> // IWYU pragma: keep

/**
    * @brief Resets the pose based on distance sensors
    * 
    * @param beams distance sensor beams to consider when resetting pose
    * @param maxChange the max amount the pose can change in either direction for it to be valid
    */
void lemlibDistReset(std::initializer_list<danielib::Beam*> beams, float xChange = infinityf(), float yChange = infinityf());

class CustomImu : public pros::Imu {
public:
    double scale;
    CustomImu(int port, double scale = 1) : pros::Imu(port), scale(scale) {}

    double get_heading() const override {
        return fmod(pros::Imu::get_heading() * scale, 360.0);
    }

    double get_rotation() const override {
        return pros::Imu::get_rotation() * scale;
    }

    std::int32_t set_heading(const double target) const override {
        return pros::Imu::set_heading(target / scale);
    }

    std::int32_t set_rotation(const double target) const override {
        return pros::Imu::set_rotation(target / scale);
    }
};