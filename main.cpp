#include "Program.h"

int main(int argc, char* args[]) 
{
	Program program1;
	while (!program1.is_ending)
	{
		program1.handle_events();
		program1.display();
	}
	return 0;
}