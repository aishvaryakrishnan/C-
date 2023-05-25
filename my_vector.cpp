#include <iostream>
#include <exception>

template <typename T>
class MyVector
{
public:
  // default c'tor
  MyVector() : size(0), current_size(0)
  {
    items = nullptr;
  }

  // parameterized c'tor specifying initial size
  MyVector(const size_t size) : size(size), current_size(0)
  {
    items = new T[size];
  }
  
  // copy c'tor
  MyVector(MyVector& v) : size(v.size), current_size(v.current_size)
  {
    items = new T[size];
    
    for (int i = 0; i < current_size; ++i)
      { 
	items[i] = v.items[i];
      }
  }
  
  // move c'tor, invoked when we use std::move
  MyVector(MyVector&& v) : items(v.items),  size(v.size), current_size(v.current_size)
  { 
    v.items = nullptr;
    v.size = 0;
    v.current_size = 0;
  }

  ~MyVector()
  {
    delete[] items;
    current_size = 0;
    size = 0;
  }
  
  // copy assignment operator
  MyVector& operator=(MyVector& v) 
  {
    items = new T[size];
    size = v.size;
    current_size = v.current_size;
    
    for (int i = 0; i < current_size; ++i)
      { 
	items[i] = v.items[i];
      }

    return *this;
  }

  // move assignment operator invoked when we use std::move
  MyVector& operator=(MyVector&& v) 
  {
    items = v.items;
    size = v.size;
    current_size = v.current_size;
    
    v.items = nullptr;
    v.size = 0;
    v.current_size = 0;

    return *this;
  }

  T& operator[](const int idx)
  {
    if (idx > current_size)
    {
      throw std::runtime_error("index out of bounds");
    }
    return items[idx];
  }

  // different iterator impl
  const T* cbegin() const
  {
    return items;
  }

  const T* cend() const
  { 
    return items + current_size;
  }

  T* begin()
  {
    return items;
  }

  T* end()
  {
    return items + current_size;
  }

  const T* begin() const
  {
    return cbegin();
  }

  const T* end() const
  {
    return cend();
  }

  void push_back(const T& item)
  {
    if (current_size == size)
      {
	resize(size * 2);
      }
    items[current_size++] = item;
  }

  void resize(const size_t new_size)
  {
    if (new_size > size)
      {
	T *new_items = new T[new_size];

	for (int i = 0; i < size; ++i)
	  {
	new_items[i] = items[i];
	  }

	items = std::move(new_items);
      }
    else
      {
	current_size = new_size;
      }
     size = new_size;
  }

  size_t capacity() const
  {
    return size;
  }

  size_t length() const
  {
    return current_size;
  }

  bool empty() const
  {
    return current_size == 0;
  }
  
  // easy print
  void print(const bool inc_stats = false) const
  {
    if (inc_stats)
      {
	print_stats();
      }
    
    for (auto i = 0; i < current_size; ++i)
      {
	std::cout << items[i] << std::endl;
      }
  }

  void print_stats() const
  {
    std::cout << "===========================================" << std::endl;
    std::cout << "capacity:" << capacity() << std::endl;
    std::cout << "length:" << length() << std::endl;
    std::cout << "empty:" << std::boolalpha << empty() << std::endl;
    std::cout << "===========================================" << std::endl;
  }
  
private:
  size_t size;
  T *items;
  size_t current_size;
};

template <typename T>
inline std::ostream& operator<<(std::ostream &os, MyVector<T> v)
{
  v.print();

  return os;
}

int main()
{
  MyVector<int> vector(5);

  std::cout << "vector:" << std::endl;
  vector.print(true);
  
  MyVector<int> copy_v(std::move(vector));
  copy_v.push_back(1);

  std::cout << "After move constructor was used" << std::endl;
  std::cout << "vector:" << std::endl;
  vector.print(true);
  std::cout << "copy v:" << std::endl;
  copy_v.print(true);
  
  MyVector<int> assign_v;

  assign_v = std::move(copy_v);
  for (int i = 2; i < 6; ++i)
    {
      assign_v.push_back(i);
    }

  std::cout << "After move assignment operator was used" << std::endl;
  std::cout << "vector:" << std::endl;
  vector.print();
  std::cout << "copy v:" << std::endl;
  copy_v.print(true);
  std::cout << "assign v:" << std::endl;
  assign_v.print(true);


  assign_v.resize(3);
  std::cout << "After downsizing" << std::endl;
  std::cout << "assign v:" << std::endl;
  assign_v.print(true);
}
