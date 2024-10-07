CC = g++

TARGET = lexi
SRC_PATH = src
SUB_DIR = src

CFLAGS = -std=c++17 -Wall -Wextra -I $(SRC_PATH)

compile:
	emcc src/wasm/lexi.cpp -O3 -s WASM=1 -s SIDE_MODULE=1 -s -o testproject/static/wasm/lexi.wasm
	emcc src/wasm/lexi.cpp -o testproject/static/wasm/lexi.js -s MODULARIZE=1 -s EXPORT_NAME='createModule' -s EXPORTED_FUNCTIONS='["_add", "_initTokenizer"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["UTF8ToString", "lengthBytesUTF8"]'

serve:
	cd testproject && python3 manage.py runserver