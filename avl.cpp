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
