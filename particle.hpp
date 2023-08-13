#pragma once

#include <opencv2/opencv.hpp>

struct Particle {
  double size;
  cv::Point pos;
  cv::Point2f velocity;
  cv::Scalar color;
  double lt;
};

Particle spawn_random_particle_at_position(cv::RNG& rng, cv::Point pos, cv::Scalar color);
