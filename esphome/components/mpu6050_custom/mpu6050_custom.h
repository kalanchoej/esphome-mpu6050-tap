#include "esphome.h"
#include <Wire.h>
#include <map>
#include <string>

class MPU6050Custom : public Component {
 public:
  void set_interrupt_pin(int pin) { interrupt_pin_ = pin; }

  void set_motion_sensitivity(const std::string &level) {
    if (sensitivity_map_.count(level)) {
      mot_thr_ = sensitivity_map_[level];
      sensitivity_ = level;
    } else {
      ESP_LOGE("MPU6050", "Invalid sensitivity level: %s", level.c_str());
    }
  }

  void set_motion_duration(const std::string &length) {
    if (duration_map_.count(length)) {
      mot_dur_ = duration_map_[length];
      duration_ = length;
    } else {
      ESP_LOGE("MPU6050", "Invalid duration: %s", length.c_str());
    }
  }

  void setup() override {
    // Initialize MPU6050
    Wire.begin();
    write_register(0x6B, 0x00);  // Wake up MPU6050

    // Configure Motion Detection
    write_register(0x1C, 0x00);  // Accelerometer sensitivity ±2g
    write_register(0x1F, mot_thr_);  // Set MOT_THR (sensitivity)
    write_register(0x20, mot_dur_);  // Set MOT_DUR (duration)
    write_register(0x38, 0x40);  // Enable Motion Interrupt

    // Configure interrupt pin as input
    pinMode(interrupt_pin_, INPUT_PULLUP);

    ESP_LOGI("MPU6050", "Configured with sensitivity '%s' and duration '%s'.",
             sensitivity_.c_str(), duration_.c_str());
  }

  void loop() override {
    // No continuous loop actions needed; binary sensor handles interrupts
  }

 private:
  int interrupt_pin_;
  uint8_t mot_thr_;
  uint8_t mot_dur_;
  std::string sensitivity_;
  std::string duration_;

  std::map<std::string, uint8_t> sensitivity_map_ = {
      {"light", 0x10},    // Low sensitivity (more sensitive)
      {"medium", 0x20}, // Medium sensitivity
      {"strong", 0x40}};  // High sensitivity (less sensitive)

  std::map<std::string, uint8_t> duration_map_ = {
      {"short", 0x01},  // Short duration
      {"medium", 0x02}, // Medium duration
      {"long", 0x03}};  // Long duration

  void write_register(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(0x68);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
  }
};
