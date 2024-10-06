CC = g++

TARGET = lexi
SRC_PATH = src
SUB_DIR = src

CFLAGS = -std=c++17 -Wall -Wextra -I $(SRC_PATH)

compile:
	emcc src/wasm/lexi.cpp -Os -s WASM=1 -s SIDE_MODULE=1 -o testproject/static/wasm/lexi.wasm

serve:
	cd testproject && python3 manage.py runserver