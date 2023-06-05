NAME	 =	coRSAir
CC 		 =	gcc -g
CFLAGS 	 =	-Wall -Wextra -w #-Werror
LDFLAGS  =	-L/usr/lib
LDLIBS 	 =	-lssl -lcrypto
OPENSSL_PATH = /usr/include/openssl
INCLUDES = -I$(OPENSSL_PATH)

SRC = coRSAir.c make_RSA.c decrypt_file.c RSA_mcd_attack.c
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

all: $(NAME)

$(OBJ_DIR)/%.o:	%.c Makefile
		@mkdir -p $(@D)
		$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	${CC} ${CFLAGS} $(INCLUDES) $(LDFLAGS) -o ${NAME} ${OBJS} $(LDLIBS)

clean:
	rm -f *.o
	rm -f *.txt
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all re