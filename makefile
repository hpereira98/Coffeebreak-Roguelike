CFLAGS=-Wall -Wextra -pedantic -O2
OBJS=$(patsubst %.c,%.o ,$(wildcard *.c))
EXEC=Rogue
$(EXEC)	:	$(OBJS)
			$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)
			$ sudo cp $(EXEC) /usr/lib/cgi-bin
			sudo mkdir -p /var/www/html/estado
			sudo touch /var/www/html/estado/highscores
			sudo chmod a+rw /var/www/html/estado
			sudo chmod a+rw /var/www/html/estado/highscores
			sudo cp player.png /var/www/html
			sudo cp inimigo1.png /var/www/html
			sudo cp inimigo2.png /var/www/html
			sudo cp inimigo3.png /var/www/html
			sudo cp inimigo4.png /var/www/html
			sudo cp porta.png /var/www/html
			sudo cp obstaculo1.png /var/www/html
			sudo cp obstaculo2.png /var/www/html
			sudo cp obstaculo3.png /var/www/html
			sudo cp fundo.png /var/www/html
			sudo cp chao.png /var/www/html
			sudo cp gameover.jpg /var/www/html
			sudo cp mariomorto.png /var/www/html
			sudo cp mario.ttf /var/www/html
			sudo cp vida1.png /var/www/html
			sudo cp vida0.png /var/www/html
			sudo cp normal.png /var/www/html
			sudo cp powerup.png /var/www/html
			sudo cp ranged.png /var/www/html
			sudo cp destruir.png /var/www/html
			sudo cp voar.png /var/www/html
			sudo cp mariovoar.png /var/www/html
			sudo cp mariodestruir.png /var/www/html
			sudo cp marioranged.png /var/www/html
			sudo cp mariobomb.png /var/www/html
			sudo cp bomba.png /var/www/html
			sudo cp bomb.png /var/www/html
			sudo cp range.jpg /var/www/html
			sudo cp rangeoff.jpg /var/www/html
			sudo cp range1.jpg /var/www/html
			sudo cp range1off.jpg /var/www/html
			sudo cp ok.png /var/www/html
			sudo cp plus.png /var/www/html
			sudo cp cursor1.cur /var/www/html
			sudo cp cursor2.cur /var/www/html
			sudo cp menu1.png /var/www/html
			sudo cp menu2.png /var/www/html
			sudo cp fogo.png /var/www/html
			sudo cp boom.png /var/www/html
			sudo cp explosao.png /var/www/html
			sudo cp 1up.png /var/www/html
			sudo cp level.png /var/www/html
			sudo cp close.png /var/www/html



limpar:
	rm $(EXEC) *.o

doc:
	doxygen -g
	doxygen