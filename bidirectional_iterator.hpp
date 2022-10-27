/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bidirectional_iterator.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 16:48:13 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/27 19:47:39 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIDIRECTIONAL_ITERATOR_HPP
# define BIDIRECTIONAL_ITERATOR_HPP

# include "iterator.hpp"
# include "utility.hpp"

/**
 * A class or a built-in type X satisfies the requirements of
 * a bidirectional iterator if, in addition to satisfying the requirements
 * for forward iterators, the following expressions are valid as shown in Table 75:
 *
 */

namespace ft
{
	template <typename T, typename Compare, typename node>
	class bidirectional_iterator
	{
		public:
			typedef Compare																			key_compare;
			typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::difference_type		difference_type;
			typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::value_type			value_type;
			typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::pointer				pointer;
			typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::reference			reference;
			typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::iterator_category	iterator_category;

			bidirectional_iterator(node* nodes, node* last_node, const key_compare& comp = key_compare())
			: _node(NULL), _last_node(NULL), _comp(comp) {}
		private:
        	node*       _node;
            node*       _last_node;
            key_compare _comp;
	};

}

#endif
