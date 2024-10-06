CC = g++

TARGET = lexi
SRC_PATH = src
SUB_DIR = src

CFLAGS = -std=c++17 -Wall -Wextra -I $(SRC_PATH)

compile:
	emcc src/wasm/lexi.hpp -O3 -s WASM=1 -o testproject/static/wasm/lexi.wasm

serve:
	cd testproject && python3 manage.py runserver