NAME		 =	coRSAir
CC 			 =	gcc -g
CFLAGS 		 =	-Wall -Wextra -w $(INCLUDES)
LDFLAGS 	 =	-L/usr/lib
LDLIBS 	 	 =	-lssl -lcrypto
OPENSSL_PATH = 	/usr/include/openssl
INCLUDES 	 = 	-I$(OPENSSL_PATH)
SRC 		 = 	coRSAir.c make_RSA.c decrypt_file.c RSA_mcd_attack.c
OBJ_DIR 	 = 	obj
DEBUG_FLAGS	 =	-g3 $(SANITIZE)
SANITIZE		=	-fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope

OBJS = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

all: $(NAME)

$(OBJ_DIR)/%.o:	%.c
		@mkdir -p $(@D)
		$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	${CC} ${CFLAGS} $(LDFLAGS) -o $@ $^ $(LDLIBS)

debug: LDFLAGS += $(DEBUG_FLAGS)
debug: LDFLAGS += $(SANITIZE)
debug: fclean $(NAME)

clean:
	rm -f *.o
	rm -f *.txt
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all re