/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bidirectional_iterator.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 16:48:13 by jisokang          #+#    #+#             */
/*   Updated: 2022/11/03 23:34:34 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIDIRECTIONAL_ITERATOR_HPP
# define BIDIRECTIONAL_ITERATOR_HPP

# include "iterator.hpp"
# include "utility.hpp"

namespace ft
{
	template <typename T, typename Compare, typename node>
	class bidirectional_iterator
	{
		public:
			typedef Compare																			key_compare;
			typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::value_type			value_type;
			typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::iterator_category	iterator_category;
			typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::difference_type		difference_type;
			typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::pointer				pointer;
			typedef typename ft::iterator<std::bidirectional_iterator_tag, T>::reference			reference;

			bidirectional_iterator(node* Node = NULL, node*  lastNode = NULL, const key_compare& comp = key_compare())
			: _node(Node), _last_node(lastNode), _comp(comp) {
			}

			bidirectional_iterator(const bidirectional_iterator<value_type, Compare, node>& other)
			{
				_node = other.getNode();
				_last_node = other.getLastNode();
				_comp = other.getCompare();
			}

			~bidirectional_iterator() {}

			bidirectional_iterator& operator=(const bidirectional_iterator<value_type, Compare, node>& other)
			{
				if (this != &other)
				{
					_node = other._node;
					_last_node = other._last_node;
					_comp = other._comp;
				}
				return (*this);
			}

			node* getNode() const               { return _node; }
			node* getLastNode() const           { return _last_node; }
			key_compare getCompare() const      { return _comp; }
			reference operator*() const         { return (_node->value); }
			pointer operator->() const          { return (&_node->value); }

			bidirectional_iterator& operator++()
			{
				if (_node->right)
				{
					_node = _node->right;
					while (_node->left)
						_node = _node->left;
				}
				else
				{
					node* tmp;
					do
					{
						tmp = _node;
						_node = _node->parent;
					} while (_node && tmp == _node->right);
				}
				return *this;
			}

			bidirectional_iterator operator++(int)
			{
				bidirectional_iterator tem(*this);
				operator++();
				return tem;
			}

			bidirectional_iterator& operator--()
			{
				if (_node->left)
				{
					_node = _node->left;
					while (_node && _node->right)
						_node = _node->right;
				}
				else
				{
					node* tmp;
					do
					{
						tmp = _node;
						_node = _node->parent;
					} while (_node && tmp == _node->left);
				}
				return *this;
			}

			bidirectional_iterator operator--(int)
			{
	  			bidirectional_iterator tem(*this);
				operator--();
				return tem;
			}

			bool operator==(const bidirectional_iterator& it) const   { return (it._node == _node); }
			bool operator!=(const bidirectional_iterator& it) const   { return (it._node != _node); }


			operator bidirectional_iterator<const value_type, Compare, node> () const
			{ return (bidirectional_iterator<const value_type, Compare, node>(_node, _last_node, _comp)); }

		private:
			node*       _node;
			node*       _last_node;
			key_compare _comp;

	};

	template <typename T, typename Compare, typename node>
	bool operator==(ft::bidirectional_iterator<const T, Compare, node>& lhs,
		ft::bidirectional_iterator<T, Compare, node>& rhs)
	{
		if (lhs.getNode() == NULL && rhs.getNode() == NULL)
			return false;
		return (lhs.getNode() == rhs.getNode());
	}

	template <typename T, typename Compare, typename node>
	bool operator==(ft::bidirectional_iterator<T, Compare, node>& lhs,
		ft::bidirectional_iterator<const T, Compare, node>& rhs)
	{
		if (lhs.getNode() == NULL && rhs.getNode() == NULL)
			return false;
		return (lhs.getNode() == rhs.getNode());
	}

	template <typename T, typename Compare, typename node>
	bool operator!=(ft::bidirectional_iterator<const T, Compare, node>& lhs,
		ft::bidirectional_iterator<T, Compare, node>& rhs)
	{
		if (lhs.getNode() == NULL && rhs.getNode() == NULL)
			return true;
		return (lhs.getNode() != rhs.getNode());
	}

	template <typename T, typename Compare, typename node>
	bool operator!=(ft::bidirectional_iterator<T, Compare, node>& lhs,
		ft::bidirectional_iterator<const T, Compare, node>& rhs)
	{
		if (lhs.getNode() == NULL && rhs.getNode() == NULL)
			return true;
		return (lhs.getNode() != rhs.getNode());
	}


}

#endif
