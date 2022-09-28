/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 14:53:18 by jisokang          #+#    #+#             */
/*   Updated: 2022/09/28 15:09:38 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATOR_TRAITS_HPP
# define ITERATOR_TRAITS_HPP

# include "iterator.hpp"

namespace ft
{
	template<class InputIterator1, class InputIterator2>
		bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2);
	template<class InputIterator1, class InputIterator2, class BinaryPredicate>
		bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred);

	//25.3.8 Lexicographical comparison
	template<class InputIterator1, class InputIterator2>
		bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2);
	template<class InputIterator1, class InputIterator2, class Compare>
		bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp);

	/*	Returns: true if the sequence of elements defined by the range [first1, last1) is lexicographi- cally less than the sequence of elements defined by the range [first2, last2).
				 Returns false otherwise.
	*/
}

#endif
