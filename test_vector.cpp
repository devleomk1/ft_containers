/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_vector.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 17:36:35 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/14 19:08:23 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <vector>
#include <iostream>
#include "color.hpp"
#include "vector.hpp"

void print_vector(std::vector<int> vec)
{
	std::vector<int>::iterator	it = vec.begin();
	int	i = 0;
	std::cout << "vec.size()     : " << vec.size() << "\n";
	std::cout << "vec.capacity() : " << vec.capacity() << "\n";
	while (it != vec.end())
	{
		std::cout << i << "- [" << *it << "]" << std::endl;
		i++;
		it++;
	}

}

int main()
{

	ft::vector<int>	vec;

	for (size_t i = 1; i <= 100; i++)
		vec.push_back(i);
	std::cout << "[1] [2] ... [100] [0] ... \n";
	std::cout << " vec." YELLOW "size" RESET "()\t\t: " << vec.size() << "\n";
	std::cout << " vec." YELLOW "capacity" RESET "()\t\t: " << vec.capacity() << "\n";
	std::cout << "*vec." YELLOW "begin" RESET "()\t\t: " << *vec.begin() << "\n";
	std::cout << "*vec." YELLOW "end" RESET "()\t\t: " << *vec.end() << "\n";
	std::cout << "&vec." YELLOW "begin" RESET "()\t\t: " << &(*vec.begin()) << "\n";
	std::cout << "&vec." YELLOW "rend" RESET "()\t\t: " << &(*vec.rend()) << "\n";
	std::cout << "&vec." YELLOW "end" RESET "()\t\t: " << &(*vec.end()) << "\n";
	std::cout << " vec." YELLOW "front" RESET "()\t\t: " << vec.front() << "\n";
	std::cout << " vec." YELLOW "back" RESET "()\t\t: " << vec.back() << "\n";
	std::cout << " vec." YELLOW "at" RESET "(3)\t\t: " << vec.at(3) << "\n";
	//std::cout << " vec[999]\t\t\t: " << vec[9999999999999999] << "\n";
	//vec.reserve(200);
	//std::cout << " vec." YELLOW "reserve" RESET "(200)\t: capacity() = " << vec.capacity() << "\n";

	//std::vector<int>::iterator	it;
	//vec.insert(vec.at(4), 3, 300);

	//std::vector<int>	insert_vec;
	//insert_vec.push_back(-1);
	//insert_vec.push_back(-2);
	//insert_vec.push_back(-3);
	//insert_vec.push_back(-4);
	//insert_vec.push_back(-5);

	return (0);
}
