CARDGAMEFILES = cardDeck.c cardPlayers.c

SHUFFLEFILE = shuffleMain.c $(CARDGAMEFILES)
SHUFFLEOUT = shuffle.out

POKERFILE = pokerMain.c $(CARDGAMEFILES)
POKEROUT = poker.out

buildPoker: $(POKERFILE)
	gcc $(POKERFILE) -std=c99 -o $(POKEROUT)

buildShuffle: $(SHUFFLEFILE)
	gcc $(SHUFFLEFILE) -o $(SHUFFLEOUT)

testPoker: $(POKEROUT)
#! should produce errors
	make buildPoker
	! ./poker.out fred linda
	! ./poker.out 5 linda
	! ./poker.out 5 10
	! ./poker.out 5 -3
	./poker.out 0 7
