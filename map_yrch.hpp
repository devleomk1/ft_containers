#ifndef MAP_HPP
# define MAP_HPP

# include <iostream>
# include <memory>
# include <stdexcept>
# include <cstddef> //for ptrdiff_t size_t
//# include <iostream>
# include "utility.hpp" //for ft::pair
# include "algorithm.hpp"
# include "iterator.hpp"
# include "type_traits.hpp"
# include "bidirectional_iterator.hpp"


namespace ft
{
	template < class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map
	{
		private:
		    struct node
            {
                ft::pair<const Key, T>  value;
                int 					height;
				node* 					parent;
                node* 					left;
                node* 					right;
            };

		public:
			// Member types //
			typedef Key 																	key_type;
			typedef	T 																		mapped_type;
			typedef typename ft::pair<const Key, T> 										value_type;
			typedef size_t 																	size_type;
			typedef typename std::ptrdiff_t 												difference_type;
			typedef Compare 																key_compare;
			typedef Allocator 																allocator_type;
			typedef typename Allocator::template rebind<node>::other 						allocNode;
			typedef value_type& 															reference;
			typedef const value_type& 														const_reference;
			typedef typename allocator_type::pointer 										pointer;
			typedef typename allocator_type::const_pointer 									const_pointer;
			typedef ft::bidirectional_iterator<value_type, Compare, node>					iterator;
			typedef ft::bidirectional_iterator<const value_type, Compare, node>				const_iterator;
			typedef ft::reverse_iterator<iterator>											reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>									const_reverse_iterator;
			// Member classes //
			class value_compare
			{
				friend class map;
				public:
					typedef bool 		result_type;
					typedef value_type	first_argument_type;
					typedef value_type	second_argument_type;
					bool operator()(const value_type& lhs, const value_type& rhs) const { return _comp(lhs.first, rhs.first); }
				protected:
					key_compare _comp;
					value_compare(key_compare comp) : _comp(comp) {}
			};
			// Member functions //
			map(const Compare &comp = key_compare(), const Allocator &alloc = allocator_type());
			template< class InputIt >
			map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type* = nullptr);
			map(const map& other);
			~map();
			map &operator=( const map &other );
			allocator_type get_allocator() const 	{return _allocPair;}

			// // Element access //
			T& at(const Key& key);
			const T& at(const Key& key) const;
			T& operator[](const Key &key);
			// Iterators //
            iterator begin()                        { node *n = _top; while (n && n->left) n = n->left;  return iterator(n, _lastNode, _comp); }
			const_iterator begin() const            { node *n = _top; while (n && n->left) n = n->left;  return const_iterator(n, _lastNode, _comp); }
            iterator end()                          { return iterator(_lastNode, _lastNode, _comp); }
            const_iterator end() const              { return const_iterator(_lastNode, _lastNode, _comp); }
            reverse_iterator rbegin()               { return reverse_iterator(this->end()); }
            const_reverse_iterator rbegin() const   { return const_reverse_iterator(this->end()); }
            reverse_iterator rend()                 { return reverse_iterator(this->begin()); }
            const_reverse_iterator rend() const     { return const_reverse_iterator(this->begin()); }
			// Capacity //
            bool empty() const						{ return (_size == 0); }
            size_t size() const						{ return (_size); }
            size_t max_size() const 				{ return (_allocNode.max_size()); }
			// Modifiers //
			void clear();
			ft::pair<iterator, bool> insert(const value_type &value);
			iterator insert(iterator hint, const value_type& value);
			template< class InputIterator >
			void insert(InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = nullptr);
            void erase (iterator position);
            void erase (iterator first, iterator last);
            size_type erase (const key_type& key);
			void swap(map &other);
			// Lookup //
			size_type count(const Key &key);
			iterator find(const Key &key);
			const_iterator find(const Key &key) const;
			ft::pair<iterator, iterator> equal_range(const Key &key);
			ft::pair<const_iterator, const_iterator> equal_range(const Key &key) const;
			iterator lower_bound(const Key &key);
			const_iterator lower_bound(const Key &key) const;
			iterator upper_bound(const Key &key);
			const_iterator upper_bound(const Key &key) const;
			// Observers //
            key_compare key_comp() const        { return _comp; }
            value_compare value_comp() const    { return value_compare(_comp); }
			// tree print //
			void treePrint_int() {display_int(_top, 1); std::cout << "\n\n" << std::endl;};
			void treePrint_string() {display_string(_top, 1); std::cout << "\n\n" << std::endl;};

		private:
            node*                   _top;
			node*					_newNode;
			node*					_lastNode;
            size_type               _size;
            allocator_type          _allocPair;
            key_compare             _comp;
            allocNode    			_allocNode;

            void insertNode(node* node, const value_type& value)
            {
                _top = insert_node(node, value);
            }

            void deleteNode(node* node, const key_type& key)
			{
            	_top = delete_node(node, key);
            }

            int max(int a, int b) {
                if (a > b)
                    return a;
                return b;
            }

            int height(node* node) {
                if (node == NULL)
                    return 0;
                return node->height;
            }

            node* createNode(const value_type& value)
            {
                node* node = _allocNode.allocate(1);
                _allocPair.construct(&node->value, value);
                node->height = 1;
				node->parent = nullptr;
                node->left = nullptr;
                node->right = nullptr;
                return node;
            }

            node* insert_node(node* node, const value_type& value) {
				if (node == _lastNode)
				{
					_newNode = createNode(value);
					_newNode->right = _lastNode;
					_lastNode->parent = _newNode;
					return _newNode;
				}
                if (!node)
				{
					_newNode = createNode(value);
					return _newNode;
				}
                if  (node->value.first > value.first)
				{
                	node->left = insert_node(node->left,  value);
					node->left->parent = node;
				}
                else if (node->value.first < value.first)
				{
					node->right = insert_node(node->right, value);
					node->right->parent = node;
				}
                node->height = max(height(node->left), height(node->right)) + 1;
                return balance(node);
            }

            int getDifference(node* node) {
                if (!node)
					return 0;
                return (height(node->left) - height(node->right));
            }

            node* rotate_right(node* node)
            {
				struct node* _tem = node->left; // 오른쪽으로 돌리니까 node의 left를 오른쪽 위로 이동 후 tem 오른쪽에 기존 노드 붙인다
				_tem->parent = node->parent;
                node->left = _tem->right;
				if (_tem->right)
					_tem->right->parent = node;
                _tem->right = node; // 오른쪽에 기존 노드
				node->parent = _tem;
                node->height = max(height(node->left), height(node->right)) + 1;
                _tem->height = max(height(_tem->left), height(_tem->right)) + 1;
                return _tem;
            }

            node* rotate_left(node* node)
            {
				struct node* _tem = node->right; // 왼쪽으로 돌리니까 node의 right를 왼쪽위로 이동 후 tem 왼쪽에 기존 노드 붙인다
				_tem->parent = node->parent;
                node->right = _tem->left;
				if (_tem->left)
					_tem->left->parent = node;
                _tem->left = node; //  왼쪽에 기존 노드
				node->parent = _tem;
                node->height = max(height(node->left), height(node->right)) + 1;
                _tem->height = max(height(_tem->left), height(_tem->right)) + 1;
                return _tem;
            }

            node* balance(node* node)
            {
                if (getDifference(node) > 1)   //노드 n의 왼쪽 서브트리가 높아서 불균형 발생
                {
                    if (getDifference(node->left) < 0) // 노드 n의 왼쪽 자식의 오른쪽서브트리가 높은 경우
                        node->left = rotate_left(node->left); //LR-회전
                    node = rotate_right(node); //LL-회전
                }
                else if (getDifference(node) < -1) //노드 n의 오른쪽 서브트리가 높아서 불균형 발생
                {
                    if (getDifference(node->right) > 0)  //노드 n의 오른쪽자식의 왼쪽 서브트리가 높은 경우
                        node->right = rotate_right(node->right); //RL-회전
                    node = rotate_left(node); //RR-회전
                }
                return node;
            }

            node* delete_node(node* node, const key_type& key)
            {
                if (!node)
                    return nullptr;
                if (node->value.first > key) // 왼쪽 자식으로 이동
				{
					node->left = delete_node(node->left, key);
					if (node->left)
						node->left->parent = node;
				}
                else if (node->value.first < key) // 오른쪽 자식으로 이동
				{
					node->right = delete_node(node->right, key);
					if (node->right)
						node->right->parent = node;
				}
                else  // 삭제할 노드 발견
                {
                    if (node->left && (!node->right || node->right == _lastNode))
					{
						struct node* _target = node;
						node = _target->left;
						node->parent = _target->parent;
						if (_target->right == _lastNode)
						{
							node->right = _lastNode;
							_lastNode->parent = node;
						}
						deallocateNode(_target);
					}
                    else if (!node->left && node->right)
					{
						struct node* _target = node;
						node = _target->right;
						node->parent = _target->parent;
						deallocateNode(_target);
					}
					else if (node->left && node->right)
					{
						struct node* _target = node;    //  1  1
						node = minimum(_target->right); //중위 후속자를 찾아서 n이 참조하게 함
						node->parent= _target->parent;
						node->right = del_min(_target->right);
						if (node->right)
							node->right->parent = node;
						node->left = _target->left;
						node->left->parent = node;
						deallocateNode(_target);
					}
					else
					{
						deallocateNode(node);
						return nullptr;
					}
                }
                node->height = max(height(node->left), height(node->right)) + 1;
                return balance(node);
            }

			//key값이 있는지 체크
            node* checkNode(node* node, const key_type& key)
            {
                if (!node)
                    return (nullptr);
                if (node->value.first == key)
                    return (node);
                if (node->value.first > key && node->left )
                    return checkNode(node->left, key);
                else if (node->value.first < key && node->right )
                    return checkNode(node->right, key);
                return (nullptr);
            }

            node* minimum(node* node)
            {
                if (!node->left)
                    return node;
                return minimum(node->left);
            }

            node* maximum(node* node)
            {
                if (!node->right)
                    return node;
                return maximum(node->right);
            }

            node* del_min(node* node)
            {
                if (!node->left)
                    return node->right;
                node->left = del_min(node->left);
                node->height = max(height(node->left), height(node->right)) + 1;
                return balance(node);
            }

			void deallocateNode(node* node)
			{
                _allocPair.destroy(&node->value);
                _allocNode.deallocate(node, 1);
			}

			void display_string(node* node, int level) {
				if (node != NULL) {
					// 가장 우측 노드부터 방문
					display_string(node->right, level + 1);
					std::cout << std::endl;

					if (node == _top) {
						std::cout << "ROOT : ";
					}

					for (int i = 0;i < level && node != _top;i++) {
						std::cout << "     ";
					}
					std::cout << (node->value.first).c_str() << "(" <<  height(node) << ")";
					display_string(node->left, level + 1);
				}
			}

			void display_int(node* node, int level) {
				if (node != NULL) {
					// 가장 우측 노드부터 방문
					display_int(node->right, level + 1);
					std::cout << std::endl;

					if (node == _top) {
						std::cout << "ROOT : ";
					}

					for (int i = 0;i < level && node != _top;i++) {
						std::cout << "     ";
					}
					std::cout << node->value.first << "(" <<  height(node) << ")";
					display_int(node->left, level + 1);
				}
			}
	};

    // Member functions //
    template < class Key, class T, class Compare, class Allocator>
	map<Key, T, Compare, Allocator>::map(const Compare &comp, const Allocator &alloc) : _size(0), _allocPair(alloc), _comp(comp)
	{
		 _lastNode = createNode(value_type());
		 _lastNode->height = 0;
		 _top = _lastNode;
	}

    template < class Key, class T, class Compare, class Allocator>
    template< class InputIt >
	map<Key, T, Compare, Allocator>::map(InputIt first, InputIt last, const Compare& comp, const Allocator& alloc, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type*)
    : _size(0), _allocPair(alloc), _comp(comp)
    {
        _lastNode = createNode(value_type());
		_lastNode->height = 0;
		_top = _lastNode;
		while (first != last)
			insert(*first++);
    }

    template < class Key, class T, class Compare, class Allocator>
	map<Key, T, Compare, Allocator>::map(const map& other)
	: _size(0), _allocPair(other._allocPair), _comp(other._comp), _allocNode(other._allocNode)
    {
		_lastNode = createNode(value_type());
		_lastNode->height = 0;
		_top = _lastNode;
        for (const_iterator it = other.begin(); it != other.end(); ++it)
            insert(*it);
    }

    template < class Key, class T, class Compare, class Allocator>
	map<Key, T, Compare, Allocator>::~map(){ clear(); }

    template < class Key, class T, class Compare, class Allocator>
	map<Key, T, Compare, Allocator> &map<Key, T, Compare, Allocator>::operator=( const map &other )
    {
		if (&other == this)
			return (*this);
		this->clear();
		this->insert(other.begin(), other.end());
		return (*this);
    }

    // Element access //
    template < class Key, class T, class Compare, class Allocator>
    T& map<Key, T, Compare, Allocator>::at(const Key& key)
    {
        node* haveNode = checkNode(_top, key);
        if (haveNode)
            return (haveNode->value.second);
        throw (std::out_of_range("map::at:  key not found"));
    }

    template < class Key, class T, class Compare, class Allocator>
    const T& map<Key, T, Compare, Allocator>::at(const Key& key) const
    {
        node* haveNode = checkNode(_top, key);
        if (haveNode)
            return (haveNode->value.second);
        throw (std::out_of_range("map::at:  key not found"));
    }

    template < class Key, class T, class Compare, class Allocator>
    T& map<Key, T, Compare, Allocator>::operator[](const Key &key)
    {
        node* haveNode = checkNode(_top, key);
        if (haveNode)
            return haveNode->value.second;
        value_type value = ft::make_pair<key_type, mapped_type>(key, mapped_type());
		insertNode(_top, value);
        ++_size;
        return (_newNode->value.second);
    }

	// Modifiers //
	template < class Key, class T, class Compare, class Allocator>
	void map<Key, T, Compare, Allocator>::clear() { erase(begin(), end()); }

	template < class Key, class T, class Compare, class Allocator>
	ft::pair<typename map<Key, T, Compare, Allocator>::iterator, bool> map<Key, T, Compare, Allocator>::insert(const value_type &value)
	{
		node* haveNode = checkNode(_top, value.first);
		if (haveNode)
			return (ft::pair<iterator, bool>(iterator(haveNode, _lastNode, _comp), false));
		insertNode(_top, value);
		++_size;
		return (ft::pair<iterator, bool>(iterator(_newNode, _lastNode, _comp), true));
	}


	template < class Key, class T, class Compare, class Allocator>
	typename map<Key, T, Compare, Allocator>::iterator map<Key, T, Compare, Allocator>::insert(iterator hint, const value_type &value)
	{
		(void)hint;
		node* haveNode = checkNode(_top, value.first);
		if (haveNode)
			return (iterator(haveNode, _lastNode, _comp));
		insertNode(_top, value);
		++_size;
		return (iterator(_newNode, _lastNode, _comp));
	}

	template < class Key, class T, class Compare, class Allocator>
	template< class InputIterator >
	void map<Key, T, Compare, Allocator>::insert(InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type*)
	{
		while (first != last)
			insert(*first++);
	}

	template < class Key, class T, class Compare, class Allocator>
	void map<Key, T, Compare, Allocator>::erase (iterator position)
	{
		deleteNode(_top, position.getNode()->value.first);
		--_size;
	}

	template < class Key, class T, class Compare, class Allocator>
	void map<Key, T, Compare, Allocator>::erase (iterator first, iterator last)
	{
		while (first != last)
		{
			iterator tmp(first);
			++first;
			erase(tmp);
		}
	}

	template < class Key, class T, class Compare, class Allocator>
	size_t map<Key, T, Compare, Allocator>::erase (const key_type& key)
	{
		node* haveNode = checkNode(_top, key);
		if (!haveNode)
			return 0;
		deleteNode(_top, key);
		--_size;
		return 1;
	}

	template < class Key, class T, class Compare, class Allocator>
	void map<Key, T, Compare, Allocator>::swap(map &other)
	{
		node* tem_top = _top;
		node* tem_lastNode = _lastNode;
		size_type tem_size = _size;
        key_compare tem_comp = _comp;
        allocator_type tem_allocPair = _allocPair;
        std::allocator<node> tem_allocNode = _allocNode;

		_top = other._top;
		_lastNode = other._lastNode;
		_size = other._size;
		_comp = other._comp;
		_allocPair = other._allocPair;
		_allocNode = other._allocNode;

		other._top = tem_top;
		other._lastNode = tem_lastNode;
		other._size = tem_size;
		other._comp = tem_comp;
		other._allocPair = tem_allocPair;
		other._allocNode = tem_allocNode;
	}

	// Lookup //
	template < class Key, class T, class Compare, class Allocator>
	size_t map<Key, T, Compare, Allocator>::count(const Key &key)
	{
		node* tem = checkNode(_top, key);
		if (tem)
			return (1);
		else
			return (0);
	}

	template < class Key, class T, class Compare, class Allocator>
	typename map<Key, T, Compare, Allocator>::iterator map<Key, T, Compare, Allocator>::find(const Key &key)
	{
		node* tem = checkNode(_top, key);
		if (tem)
			return iterator(tem, _lastNode, _comp);
		return end();
	}
	template < class Key, class T, class Compare, class Allocator>
	typename map<Key, T, Compare, Allocator>::const_iterator map<Key, T, Compare, Allocator>::find(const Key &key) const
	{
		node* tmp = checkNode(_top, key);
		if (tmp)
			return const_iterator(tmp, _lastNode, _comp);
		return end();
	}

	template < class Key, class T, class Compare, class Allocator>
	ft::pair<typename map<Key, T, Compare, Allocator>::iterator, typename map<Key, T, Compare, Allocator>::iterator>
		map<Key, T, Compare, Allocator>::equal_range(const Key &key)
	{
		return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
	}

	template < class Key, class T, class Compare, class Allocator>
	ft::pair<typename map<Key, T, Compare, Allocator>::const_iterator, typename map<Key, T, Compare, Allocator>::const_iterator>
		map<Key, T, Compare, Allocator>::equal_range(const Key &key) const
	{
		return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
	}

	template < class Key, class T, class Compare, class Allocator>
	typename map<Key, T, Compare, Allocator>::iterator map<Key, T, Compare, Allocator>::lower_bound(const Key &key)
	{
		iterator it = begin();
		for (; it != end(); ++it)
			if (!_comp(it->first, key))
				break;
		return it;
	}

	template < class Key, class T, class Compare, class Allocator>
	typename map<Key, T, Compare, Allocator>::const_iterator map<Key, T, Compare, Allocator>::lower_bound(const Key &key) const
	{
		const_iterator it = begin();
		for (; it != end(); ++it)
			if (!_comp(it->first, key))
				break;
		return it;
	}

	template < class Key, class T, class Compare, class Allocator>
	typename map<Key, T, Compare, Allocator>::iterator map<Key, T, Compare, Allocator>::upper_bound(const Key &key)
	{
		iterator it = begin();
		for (; it != end(); ++it)
			if (_comp(key, it->first))
				break;
		return it;
	}

	template < class Key, class T, class Compare, class Allocator>
	typename map<Key, T, Compare, Allocator>::const_iterator map<Key, T, Compare, Allocator>::upper_bound(const Key &key) const
	{
		const_iterator it = begin();
		for (; it != end(); ++it)
			if (_comp(key, it->first))
				break;
		return it;
	}

	template <typename Key, typename T, typename Compare, class Allocator>
	bool operator==(const map<Key, T, Compare, Allocator> &lhs, const map<Key, T, Compare, Allocator> &rhs)
	{
		if (lhs.size() != rhs.size())
			return false;
		return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <typename Key, typename T, typename Compare, class Allocator>
	bool operator!=(const map<Key, T, Compare, Allocator> &lhs, const map<Key, T, Compare, Allocator> &rhs)
	{
		return !(lhs == rhs);
	}

	template <typename Key, typename T, typename Compare, class Allocator>
	bool operator<(const map<Key, T, Compare, Allocator> &lhs, const map<Key, T, Compare, Allocator> &rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <typename Key, typename T, typename Compare, class Allocator>
	bool operator>=(const map<Key, T, Compare, Allocator> &lhs, const map<Key, T, Compare, Allocator> &rhs)
	{
		return !(lhs < rhs);
	}

	template <typename Key, typename T, typename Compare, class Allocator>
	bool operator>(const map<Key, T, Compare, Allocator> &lhs, const map<Key, T, Compare, Allocator> &rhs)
	{
		return rhs < lhs;
	}

	template <typename Key, typename T, typename Compare, class Allocator>
	bool operator<=(const map<Key, T, Compare, Allocator> &lhs, const map<Key, T, Compare, Allocator> &rhs)
	{
		return !(lhs > rhs);
	}

}

#endif
