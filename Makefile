# 编译器
CC = clang

# 静态库名称
LIB_NAME = mylib.a

# 源文件目录
SRC_DIR = .
# 所有的源文件
SRC_FILES = $(wildcard $(SRC_DIR)/openflow/*.c)
MEMBAG_SRCS = $(wildcard membag/*.c)
INC_DIRS = $(SRC_DIR)/include
# 所有的目标对象文件
OBJ_FILES = $(SRC_FILES:.c=.o) $(MEMBAG_SRCS:.c=.o)

# 编译选项
CFLAGS = -Wall -Wextra -I $(INC_DIRS) -I $(SRC_DIR) -Wno-int-conversion

# 静态库命令
AR = ar
ARFLAGS = rcs

# 默认目标
all: $(LIB_NAME)

# 静态库的生成规则
$(LIB_NAME): $(OBJ_FILES)
	$(AR) $(ARFLAGS) $@ $^

# 源文件的编译规则
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理规则
clean:
	rm -f $(LIB_NAME) $(OBJ_FILES)

