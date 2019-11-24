#include "shared.h"

double to_radians(double angle) {
  return PI/180.0f*angle;
}

double clamp_yaw(double yaw) {
  if(yaw > 180.0)
    yaw -= 360.0;
  else if(yaw < -180.0)
    yaw += 360.0;
  return yaw;
}

double clamp_pitch(double pitch) {
  if(pitch >= 90.0)
    pitch = 89.9;
  else if(pitch <= -90.0)
    pitch = -89.9;
  return pitch;
}
