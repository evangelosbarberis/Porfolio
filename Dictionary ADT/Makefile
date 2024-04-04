#------------------------------------------------------------------------------
# Evangelos Barberis
# ebarberi
# CSE 101 Winter 2024 PA8
# Makefile
#  Makefile for CSE 101 Programming Assignment 8
#
#  make                     makes WordFrequency and Order
#  make DictionaryTest      makes DictionaryTest
#  make clean               removes all binaries
#  make WordFrequencyCheck  runs WordFrequency under valgrind on Gutenberg
#  make DictionaryCheck     runs DictionaryTest under valgrind
#------------------------------------------------------------------------------

WORDFREQ_MAIN  = WordFrequency
ORDER_MAIN     = Order
ADT            = Dictionary
WORDFREQ_SRC   = $(WORDFREQ_MAIN).cpp
ORDER_SRC      = $(ORDER_MAIN).cpp
ADT_SRC        = $(ADT).cpp
WORDFREQ_OBJ   = $(WORDFREQ_MAIN).o
ORDER_OBJ      = $(ORDER_MAIN).o
ADT_OBJ        = $(ADT).o
ADT_HEADER     = $(ADT).h
COMPILE        = g++ -std=c++17 -Wall -c
LINK           = g++ -std=c++17 -Wall -o
REMOVE         = rm -f
MEMCHECK       = valgrind --leak-check=full

$(WORDFREQ_MAIN): $(WORDFREQ_OBJ) $(ADT_OBJ)
	$(LINK) $(WORDFREQ_MAIN) $(WORDFREQ_OBJ) $(ADT_OBJ)

$(ORDER_MAIN): $(ORDER_OBJ) $(ADT_OBJ)
	$(LINK) $(ORDER_MAIN) $(ORDER_OBJ) $(ADT_OBJ)

$(WORDFREQ_OBJ): $(WORDFREQ_SRC) $(ADT_HEADER)
	$(COMPILE) $(WORDFREQ_SRC)

$(ORDER_OBJ): $(ORDER_SRC) $(ADT_HEADER)
	$(COMPILE) $(ORDER_SRC)

$(ADT_OBJ): $(ADT_SRC) $(ADT_HEADER)
	$(COMPILE) $(ADT_SRC)

clean:
	$(REMOVE) $(WORDFREQ_MAIN) $(ORDER_MAIN) $(WORDFREQ_OBJ) $(ORDER_OBJ) $(ADT_OBJ)

$(WORDFREQ_MAIN)Check: $(WORDFREQ_MAIN)
	$(MEMCHECK) $(WORDFREQ_MAIN) Gutenberg myGutenberg-out

$(ADT)Check: $(ADT_TEST)
	$(MEMCHECK) $(ADT_TEST)
