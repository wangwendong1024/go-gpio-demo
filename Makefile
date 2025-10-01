# Makefile for go-gpio-demo with DHT11 demo

# 目录
C_DIR := C
GO_CMD := go run main.go

# C 文件和库
C_SRC := $(C_DIR)/dht11_demo.c
C_OBJ := $(C_DIR)/dht11_demo.o
C_LIB := $(C_DIR)/libdht11_demo.so

# wiringPi 链接
LIBS := -lwiringPi

.PHONY: all clean run force

all: force $(C_LIB)

# 强制目标，确保重新编译
force:

# 编译共享库
$(C_LIB): $(C_OBJ)
	@echo "生成共享库 $@ ..."
	gcc -shared -o $@ $^ $(LIBS)

# 编译 C 文件
$(C_OBJ): $(C_SRC)
	@echo "编译 C 文件 $< ..."
	gcc -c -fPIC $< -o $@

# 运行 Go 程序
run: all
	@echo "运行 Go 程序..."
	@export LD_LIBRARY_PATH=$(C_DIR):$$LD_LIBRARY_PATH; \
	$(GO_CMD)

# 清理
clean:
	@echo "清理中..."
	rm -f $(C_OBJ) $(C_LIB)
