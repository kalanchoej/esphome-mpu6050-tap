#include "mpu6050_custom.h"

namespace esphome {
namespace custom_component {

void register_custom_components() {
  auto my_mpu6050 = new MPU6050Custom();

  // Get YAML configuration fields
  int interrupt_pin = get_optional<int>("interrupt_pin").value_or(26);
  std::string sensitivity = get_optional<std::string>("sensitivity").value_or("medium");
  std::string duration = get_optional<std::string>("duration").value_or("short");

  my_mpu6050->set_interrupt_pin(interrupt_pin);
  my_mpu6050->set_motion_sensitivity(sensitivity);
  my_mpu6050->set_motion_duration(duration);

  App.register_component(my_mpu6050);
}

}  // namespace custom_component
}  // namespace esphome
