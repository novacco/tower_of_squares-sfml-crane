#include "crane.hpp"

int main(int argc, char** argv)
{
	int width = 1400;
	int height = 870;
	string title = "Symulator dzwig tylko kwadraty";

	Crane simulation(width, height, title);
	simulation.run();
	system("PAUSE");

	return 0;
}