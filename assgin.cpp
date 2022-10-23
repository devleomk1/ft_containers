#include "vector.hpp"
#include <iostream>
#include <deque>

#define TESTED_NAMESPACE ft

#define TESTED_TYPE int

template <typename T>
void	printSize(std::deque<T> const &deq, bool print_content = 1)
{
	std::cout << "size: " << deq.size() << std::endl;
	std::cout << "max_size: " << deq.max_size() << std::endl;
	if (print_content)
	{
		typename std::deque<T>::const_iterator it = deq.begin();
		typename std::deque<T>::const_iterator ite = deq.end();
		std::cout << std::endl << "Content is:" << std::endl;
		for (; it != ite; ++it)
			std::cout << "- " << *it << std::endl;
	}
	std::cout << "###############################################" << std::endl;
}

int		main(void)
{
	TESTED_NAMESPACE::vector<TESTED_TYPE> vct(7);
	TESTED_NAMESPACE::vector<TESTED_TYPE> vct_two(4);
	TESTED_NAMESPACE::vector<TESTED_TYPE> vct_three;
	TESTED_NAMESPACE::vector<TESTED_TYPE> vct_four;

	for (unsigned long int i = 0; i < vct.size(); ++i)
		vct[i] = (vct.size() - i) * 3;
	for (unsigned long int i = 0; i < vct_two.size(); ++i)
		vct_two[i] = (vct_two.size() - i) * 5;
	//printSize(vct);
	//printSize(vct_two);

	std::cout << "PASS1" << std::endl;

	vct_three.assign(vct.begin(), vct.end());
	std::cout << "PASS2" << std::endl;
	vct.assign(vct_two.begin(), vct_two.end());
	vct_two.assign(2, 42);
	vct_four.assign(4, 21);

	std::cout << "\t### After assign(): ###" << std::endl;

	//printSize(vct);
	//printSize(vct_two);
	//printSize(vct_three);
	//printSize(vct_four);

	vct_four.assign(6, 84);
	//printSize(vct_four);

	std::cout << "\t### assign() on enough capacity and low size: ###" << std::endl;

	vct.assign(5, 53);
	vct_two.assign(vct_three.begin(), vct_three.begin() + 3);

	//printSize(vct);
	//printSize(vct_two);

	return (0);
}
