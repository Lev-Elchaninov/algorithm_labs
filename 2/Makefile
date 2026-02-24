CC = gcc
CFLAGS = -Wall -Wextra -fPIC -Iinclude -O2 -g
#fpic - эт позиционно независимый код (для того, чтобы он мог быть выполнен ваще из любого места в памяти) (для динам библ)
#-Iinclude - путь к заголовочному файлу
LDFLAGS = -lreadline
LIB_DIR = lib#для библиотек директория
BIN_DIR = bin#для испольняемого файла директория (app)
LIB_NAME = libstack#имя библиотеки
SRCS = src/app/input.c src/app/main.c#файлы самой моей программы
OBJS = $(SRCS:.c=.o)#объектные файлы
HEADER = include/library.h#местоположение хедера

IMPL ?= VECTOR  #реализация по умолчанию
LIB_TYPE ?= STATIC #библиотека по умолчанию
IMPL_UPPER = $(strip $(subst vector,VECTOR,$(subst list,LIST,$(IMPL))))#перевод в верхний регистр
LIB_TYPE_UPPER = $(strip $(subst static,STATIC,$(subst dynamic,DYNAMIC,$(LIB_TYPE))))

ifeq ($(IMPL_UPPER),VECTOR)
    IMPL_SRC = src/lib/task_vector.c
else ifeq ($(IMPL_UPPER),LIST)
    IMPL_SRC = src/lib/task_list.c
else
    $(error Invalid IMPLICATION. Use VECTOR or LIST)
endif


all: prepare lib $(BIN_DIR)/app#подгот+библ+приложуха

prepare:
	@mkdir -p $(LIB_DIR) $(BIN_DIR)
#создали директории lib и bin, @подавляет вывод команды в терминал -p создает все недостающие директории

# Правило для сборки статической библиотеки
$(LIB_DIR)/$(LIB_NAME).a: $(IMPL_SRC) $(HEADER)
	$(CC) $(CFLAGS) -c $(IMPL_SRC) -o $(LIB_DIR)/stack_impl.o #компиляция в .o файл
	ar rcs $@ $(LIB_DIR)/stack_impl.o #создание архива .a
#rcs r - replace, добавляет файл в архив
#c - создает архив если он не существует
#s - создает или обновляет индекс символов в архиве (это позволяет линковщику быстрее находить символы в библиотеке)

# Правило для сборки динамической библиотеки
$(LIB_DIR)/$(LIB_NAME).so: $(IMPL_SRC) $(HEADER)
	$(CC) $(CFLAGS) -shared $(IMPL_SRC) -o $@ -Wl,-rpath,$(LIB_DIR)
#shared - флаг для динамической библиотеки
#-Wl... - задает путь для поиска библиотеки во время выполнения
#Wl - передает опции непосредственно линковщику
#rpath указывает - где искать динам библиотеку


# Правило для сборки приложения
$(BIN_DIR)/app: $(OBJS)
ifeq ($(LIB_TYPE_UPPER),STATIC)
	$(CC) $^ -L$(LIB_DIR) -lstack -o $@ $(LDFLAGS)
else ifeq ($(LIB_TYPE_UPPER),DYNAMIC)
	$(CC) $^ -L$(LIB_DIR) -lstack -o $@ $(LDFLAGS) -Wl,-rpath,$(LIB_DIR)
else
	$(error Invalid LIB_TYPE. Use STATIC or DYNAMIC)
endif
#-L$(LIB_DIR) - путь к библиотекам, -lstack - ищем имя библиотеки
# Общие правила для объектных файлов
%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@


# Правило для сборки библиотеки (вызывает статическую или динамическую сборку)
lib:
ifeq ($(LIB_TYPE_UPPER),STATIC)
	$(MAKE) $(LIB_DIR)/$(LIB_NAME).a
else ifeq ($(LIB_TYPE_UPPER),DYNAMIC)
	$(MAKE) $(LIB_DIR)/$(LIB_NAME).so
else
	$(error Invalid LIB_TYPE. Use STATIC or DYNAMIC)
endif

clean:
	rm -rf $(LIB_DIR) $(BIN_DIR) *.o

.PHONY: all prepare clean lib




