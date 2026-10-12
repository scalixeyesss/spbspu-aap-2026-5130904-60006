#include <iostream>

int main() 
{
	int value = 0;
	int total_count = 0;
	int max = 0;
	int min = 0;
	int count_max = 0;
	int count_min = 0;

	while (true) 
	{
		if (!(std::cin >> value))
       		{
            		std::cerr << "Invalid input!\n";
            		return 1;
        	}

        	if (value == 0)
        	{
            		break;
        	}

        	total_count += 1;

        	// [CNT-MAX]
        	if (value) 
        	{
            		if (total_count == 1 || value > max)
            		{
                		max = value;
                		count_max = 1;
            		}
            		else if (value == max) 
            		{
                		count_max += 1;
            		}
        	}

        	//[CNT-MIN]
        	if (total_count == 1 || value < min)
        	{
            		min = value;
            		count_min = 1;
        	}
        	else if (value == min)
        	{
           		count_min += 1;
        	}

        	if (total_count == 0) 
        	{
            		std::cerr << "Sequence is small!\n";
            		return 2;
        	}

	}
    	std::cout << count_max << '\n';
    	std::cout << count_min << '\n';
    	return 0;
}
