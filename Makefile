CC = g++

TARGET = lexi
SRC_PATH = src
SUB_DIR = src

CFLAGS = -std=c++17 -Wall -Wextra -I $(SRC_PATH)

compile:
	emcc src/wasm/lexi.cpp -O2 -s WASM=1 -s SIDE_MODULE=1 -s -o testproject/static/wasm/lexi.wasm
	emcc src/wasm/lexi.cpp -o testproject/static/wasm/lexi-wasm.js -s MODULARIZE=1 -s EXPORT_NAME='createModule' -s EXPORTED_FUNCTIONS='["_add", "_initTokenizer", "_testStr", "_malloc", "_free", "_addRule", "_tokenize", "_getTokenType", "_getStart", "_getEnd", "_getTokenType", "_getNext"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["stringToUTF8", "UTF8ToString", "lengthBytesUTF8", "cwrap"]' -s TOTAL_STACK=512mb

serve:
	cd testproject && python3 manage.py runserver

make-dist:
	cp testproject/static/wasm/lexi-wasm.js dist/
	cp testproject/static/wasm/lexi-wasm.wasm dist/
	cp src/js/lexi.js dist/