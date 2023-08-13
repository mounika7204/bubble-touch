#include "particle.hpp"

constexpr const float MIN_PARTICLE_SIZE = 2;
constexpr const float MAX_PARTICLE_SIZE = 5;

constexpr const float MIN_PARTICLE_SPEED = 5;
constexpr const float MAX_PARTICLE_SPEED = 50;

constexpr const float MIN_PARTICLE_LT = 0.5;
constexpr const float MAX_PARTICLE_LT = 1.2;

Particle spawn_random_particle_at_position(cv::RNG& rng, cv::Point pos, cv::Scalar color)
{
  float size = std::lerp( //
      MIN_PARTICLE_SIZE,
      MAX_PARTICLE_SIZE,
      rng.uniform((float)0, (float)1) //
  );

  float speed = std::lerp( //
      MIN_PARTICLE_SPEED,
      MAX_PARTICLE_SPEED,
      rng.uniform((float)0, (float)1) //
  );

  float angle = std::lerp(0, 2 * (float)M_PI, rng.uniform((float)0, (float)1));

  float dx = std::cos(angle) * speed;
  float dy = std::sin(angle) * speed;

  float lt = std::lerp( //
      MIN_PARTICLE_LT,
      MAX_PARTICLE_LT,
      rng.uniform((float)0, (float)1) //
  );

  return Particle {
    size,
    pos,
    cv::Point2f { dx, dy },
    color,
    lt,
  };
}
