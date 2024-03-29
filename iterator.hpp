/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 20:35:21 by jisokang          #+#    #+#             */
/*   Updated: 2022/11/01 00:35:03 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATOR_HPP
# define ITERATOR_HPP

# include <cstddef>				// ptrdiff_t (https://en.cppreference.com/w/cpp/types/ptrdiff_t)
# include "iterator_traits.hpp"

namespace ft
{
	template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
		typedef Category	iterator_category;
	};

template <class Iterator>
	class reverse_iterator : public
		iterator<typename iterator_traits<Iterator>::iterator_category,
				 typename iterator_traits<Iterator>::value_type,
				 typename iterator_traits<Iterator>::difference_type,
				 typename iterator_traits<Iterator>::pointer,
				 typename iterator_traits<Iterator>::reference> {
	public:
		typedef Iterator											iterator_type;
		typedef typename iterator_traits<Iterator>::difference_type	difference_type;
		typedef typename iterator_traits<Iterator>::reference		reference;
		typedef typename iterator_traits<Iterator>::pointer			pointer;

	protected:
		Iterator current;

	public:
		reverse_iterator()
		{};
		explicit reverse_iterator(Iterator x)
		: current(x) {};
		template <class U> reverse_iterator(const reverse_iterator<U>& u)
		: current(u.base()) {};

		iterator_type	base() const{
			return(current);
		};
		reference	operator*() const{
			Iterator tmp = current;
			return *--tmp;
		};
		pointer		operator->() const{
			return &(operator*());
		};

		reverse_iterator&	operator++(){
			--current;
			return (*this);
		};
		reverse_iterator	operator++(int){
			reverse_iterator tmp = *this;
			--current;
			return (tmp);
		};
		reverse_iterator&	operator--(){
			++current;
			return (*this);
		};
		reverse_iterator	operator--(int){
			reverse_iterator tmp = *this;
			++current;
			return (tmp);
		};

		reverse_iterator	operator+ (typename reverse_iterator<Iterator>::difference_type n) const{
			return (reverse_iterator(current - n));
		};
		reverse_iterator&	operator+=(typename reverse_iterator<Iterator>::difference_type n){
			current -= n;
			return (*this);
		};
		reverse_iterator	operator- (typename reverse_iterator<Iterator>::difference_type n) const{
			return (reverse_iterator(current + n));
		};
		reverse_iterator&	operator-=(typename reverse_iterator<Iterator>::difference_type n){
			current += n;
			return (*this);
		};
		reference operator[](typename reverse_iterator<Iterator>::difference_type n) const{
			return (current[-n-1]);
		};
	};

	// 🚨
	// Q1. template <class Iterator> Header에는 이렇게 써있는데 왜 Class 2개 받는거임???
	// Q2. current로 받아도 되는거 아닌가? 왜 base()로 받지? -> protected라 접근 못할 수 있음
	template <class IteratorL, class IteratorR>
		bool operator==(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y)
			{
				return (x.base() == y.base());
			};
	template <class IteratorL, class IteratorR>
		bool operator<(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y)
			{
				return (x.base() > y.base());
			};
	template <class IteratorL, class IteratorR>
		bool operator!=(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y)
			{
				return (x.base() != y.base());
			};
	template <class IteratorL, class IteratorR>
		bool operator>(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y)
			{
				return (x.base() < y.base());
			};
	template <class IteratorL, class IteratorR>
		bool operator>=(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y)
			{
				return (x.base() <= y.base());
			};
	template <class IteratorL, class IteratorR>
		bool operator<=(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y)
			{
				return (x.base() >= y.base());
			};
	template <class IteratorL, class IteratorR>
		typename reverse_iterator<IteratorL>::difference_type operator-(	//왜 difference_type으로 가져올까?
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y)
			{
				return (y.base() - x.base());
			};
	template <class Iterator>
		reverse_iterator<Iterator> operator+(
			typename reverse_iterator<Iterator>::difference_type n,
			const reverse_iterator<Iterator>& x)
			{
				return reverse_iterator<Iterator>(x.base() - n);
			};

}

#endif
