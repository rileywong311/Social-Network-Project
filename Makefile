COMP=g++

output: social_network.o network.o user.o post.o directmessage.o
	$(COMP) $(FLAGS) $^ -o $@

%.o: %.cpp
	$(COMP) $(FLAGS) -c $< -o $@

clean:
	rm *.o
	rm output