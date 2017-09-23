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
			sudo cp img/player.png /var/www/html
			sudo cp img/inimigo1.png /var/www/html
			sudo cp img/inimigo2.png /var/www/html
			sudo cp img/inimigo3.png /var/www/html
			sudo cp img/inimigo4.png /var/www/html
			sudo cp img/porta.png /var/www/html
			sudo cp img/obstaculo1.png /var/www/html
			sudo cp img/obstaculo2.png /var/www/html
			sudo cp img/obstaculo3.png /var/www/html
			sudo cp img/fundo.png /var/www/html
			sudo cp img/chao.png /var/www/html
			sudo cp img/gameover.jpg /var/www/html
			sudo cp img/mariomorto.png /var/www/html
			sudo cp img/mario.ttf /var/www/html
			sudo cp img/vida1.png /var/www/html
			sudo cp img/vida0.png /var/www/html
			sudo cp img/normal.png /var/www/html
			sudo cp img/powerup.png /var/www/html
			sudo cp img/ranged.png /var/www/html
			sudo cp img/destruir.png /var/www/html
			sudo cp img/voar.png /var/www/html
			sudo cp img/mariovoar.png /var/www/html
			sudo cp img/mariodestruir.png /var/www/html
			sudo cp img/marioranged.png /var/www/html
			sudo cp img/mariobomb.png /var/www/html
			sudo cp img/bomba.png /var/www/html
			sudo cp img/bomb.png /var/www/html
			sudo cp img/range.jpg /var/www/html
			sudo cp img/rangeoff.jpg /var/www/html
			sudo cp img/range1.jpg /var/www/html
			sudo cp img/range1off.jpg /var/www/html
			sudo cp img/ok.png /var/www/html
			sudo cp img/plus.png /var/www/html
			sudo cp img/cursor1.cur /var/www/html
			sudo cp img/cursor2.cur /var/www/html
			sudo cp img/menu1.png /var/www/html
			sudo cp img/menu2.png /var/www/html
			sudo cp img/fogo.png /var/www/html
			sudo cp img/boom.png /var/www/html
			sudo cp img/explosao.png /var/www/html
			sudo cp img/1up.png /var/www/html
			sudo cp img/level.png /var/www/html
			sudo cp img/close.png /var/www/html



limpar:
	rm $(EXEC) *.o

doc:
	doxygen -g
	doxygen