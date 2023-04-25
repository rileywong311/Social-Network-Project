##### PART 1 #####

#output: social_network.cpp network.cpp user.cpp
#	g++ social_network.cpp network.cpp user.cpp -o output




##### PART 2: PARTIAL COMPILATION #####

#output: social_network.o network.o user.o
#	g++ social_network.o network.o user.o -o output

#social_network.o: social_network.cpp
#	g++ -c social_network.cpp -o social_network.o

#network.o: network.cpp
#	 g++ -c network.cpp -o network.o

#user.o: user.cpp
#	 g++ -c user.cpp -o user.o

#clean: *.o
#	rm *.o
#	rm output




##### PART 3: GENERALIZING #####

#output: social_network.o network.o user.o
#	g++ $^ -o $@

#social_network.o: social_network.cpp
#	g++ -c $< -o $@

#network.o: network.cpp
#	 g++ -c $< -o $@

#user.o: user.cpp
#	 g++ -c $< -o $@

#clean:
#	rm *.o
#	rm output




##### PART 4: VARIABLES #####

COMP=g++
FLAGS=-Wall -g

output: social_network.o network.o user.o
	$(COMP) $(FLAGS) $^ -o $@

%.o: %.cpp
	$(COMP) $(FLAGS) -c $< -o $@

clean:
	rm *.o
	rm output