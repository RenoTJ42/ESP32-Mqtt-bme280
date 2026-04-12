# ESP32 MQTT BME280 IoT System

An embedded IoT project using ESP32 and ESP-IDF to publish environmental sensor data using MQTT.

## Features

- ESP32 firmware written in C
- BME280 sensor interfacing using I2C
- MQTT communication
- Node-RED dashboard visualization

## Hardware

- ESP32 DevKit
- BME280 sensor module

## Software Stack

- ESP-IDF
- FreeRTOS
- MQTT
- Node-RED

## Project Architecture

ESP32 → MQTT Broker → Node-RED Dashboard

## Current Progress

Phase 1
- ESP-IDF environment setup
- LED blink firmware

Phase 2
- I2C initialization
- I2C device scanner

## Folder Structure

esp32-mqtt-bme280
│
├── firmware
│   └── esp32
│
├── hardware
│
├── docs
│
└── README.md
