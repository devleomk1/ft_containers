/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 14:53:18 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/31 07:54:02 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

namespace ft
{
	/**
	 * @brief Compares the elements in the range [first1,last1) with those in the range beginning at first2, and returns true if all of the elements in both ranges match.
	 *
	 * @tparam InputIterator1
	 * @tparam InputIterator2
	 * @param first1
	 * @param last1
	 * @param first2
	 * @return true
	 * @return false
	 * @link (https://cplusplus.com/reference/algorithm/equal/)
	 */
	template<class InputIterator1, class InputIterator2>
		bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2){
			while (first1 != last1)
			{
				if (!(*first1 == *first2))
				{
					return ( false );
				}
				first1++;
				first2++;
			}
			return ( true );
		};

	template<class InputIterator1, class InputIterator2, class BinaryPredicate>
		bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred){
			while (first1 != last1)
			{
				if (!pred(*first1,*first2))
				{
					return false;
				}
				++first1;
				++first2;
			}
			return true;
		};

	//25.3.8 Lexicographical comparison
	/**
	 * @brief 사전식 비교
	 *
	 * @tparam InputIterator1
	 * @tparam InputIterator2
	 * @param first1
	 * @param last1
	 * @param first2
	 * @param last2
	 * @return true
	 * @return false
	 * @link (https://cplusplus.com/reference/algorithm/lexicographical_compare/)
	 */
	template<class InputIterator1, class InputIterator2>
		bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2){
			while (first1!=last1)
			{
				if (first2 == last2 || *first2 < *first1 )
					return false;
				else if (*first1 < *first2)
					return true;
				++first1;
				++first2;
			}
			return (first2!=last2);
		};
	template<class InputIterator1, class InputIterator2, class Compare>
		bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp){
			while (first1!=last1)
			{
				if (first2==last2 || comp(*first2, *first1))
					return false;
				else if (comp(*first1, *first2))
					return true;
				++first1;
				++first2;
			}
			return (first2!=last2);
		};

	/*	Returns: true if the sequence of elements defined by the range [first1, last1) is lexicographi- cally less than the sequence of elements defined by the range [first2, last2).
				 Returns false otherwise.
	*/
}

#endif
