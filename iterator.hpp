/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 20:35:21 by jisokang          #+#    #+#             */
/*   Updated: 2022/09/27 21:24:03 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATOR_HPP
# define ITERATOR_HPP

# include <cstddef>				//FOR ptrdiff_t (https://en.cppreference.com/w/cpp/types/ptrdiff_t)
# include "iterator_traits.hpp"

namespace ft
{
	//24.3 primitives:
	//template<class Iterator> struct iterator_traits;
	//template<class T> struct iterator_traits<T*>;

														//ptrdiff_t는 또 뭐시당가 -> 찾아보자
	template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&> struct iterator
	{
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
		typedef Category	iterator_category;
	};

	//tag는 머시당가?
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag: public input_iterator_tag {};
	struct bidirectional_iterator_tag: public forward_iterator_tag {};
	struct random_access_iterator_tag: public bidirectional_iterator_tag {};

	//24.3.4 iterator operations:
	template <class InputIterator, class Distance>
		void advance(InputIterator& i, Distance n);
	template <class InputIterator>
		typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last);

	//24.4 predefined iterator:
	template <class Iterator> class reverse_iterator;

	template <class Iterator>
		bool operator==
		(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y
		);
	template <class Iterator>
		bool operator<
		(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y
		);
	template <class Iterator>
		bool operator!=
		(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y
		);
	template <class Iterator>
		bool operator>
		(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y
		);
	template <class Iterator>
		bool operator>=
		(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y
		);
	template <class Iterator>
		bool operator<=
		(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y
		);

	template <class Iterator>
		typename reverse_iterator<Iterator>::difference_type operator-
		(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y
		);
	template <class Iterator>
		reverse_iterator<Iterator>
			operator+
			(
				typename reverse_iterator<Iterator>::difference_type n,
				const reverse_iterator<Iterator>& y
			);

	//template <class Container> class back_insert_iterator;
	//template <class Container>
	//	back_insert_iterator<Container> back_insert(Container& x);

	//template <class Container> class front_insert_iterator;
	//template <class Container>
	//	front_insert_iterator<Container> front_insert(Container& x);

	//template <class Container> class insert_iterator;
	//template <class Container, class Iterator>
	//	insert_iterator<Container> inserter(Container& x, Iterator i);

	//24.5 stream iterators:
	//template <class T, class charT = char, class traits = char_traits<charT>, class Distance = ptrdiff_t>
	//														//char_traits는 뭔데 하이라이트가 안될까?
	//class istream_iterator;
	//template <class T, class charT, class traits, class Distance>
	//	bool operator==
	//	(
	//		const istream_iterator<T, charT, traits, Distance>& x,
	//		const istream_iterator<T, charT, traits, Distance>& y
	//	);
	//template <class T, class charT, class traits, class Distance>
	//	bool operator!=
	//	(
	//		const istream_iterator<T, charT, traits, Distance>& x,
	//		const istream_iterator<T, charT, traits, Distance>& y
	//	);
	//template <class T, class charT = char, class traits = char_traits<charT> >
	//	class ostream_iterator;
	//template <class charT, class traits = char_traits<charT> >
	//	class istreambuf_iterator;
	//template <class charT, class traits>
	//	bool operator==
	//	(
	//		const istreambuf_iterator<charT, traits>& a,
	//		const istreambuf_iterator<charT, traits>& b
	//	);
	//template <class charT, class traits>
	//	bool operator!=
	//	(
	//		const istreambuf_iterator<charT, traits>& a,
	//		const istreambuf_iterator<charT, traits>& b
	//	);
	//template <class charT, class traits = char_traits<charT> >
	//	class ostreambuf_iterator;
}

#endif
