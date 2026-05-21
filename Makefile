# Simple shortcuts — run from project root:  make run
BUILD_DIR := build

.PHONY: run build config

config:
	cmake -S . -B $(BUILD_DIR)

build: config
	cmake --build $(BUILD_DIR) --target retro_poker -j

run: build
	./$(BUILD_DIR)/retro_poker
