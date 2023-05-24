#include <iostream>

template <typename T>
class SmartSharedPointer
{
public:
  SmartSharedPointer() : s_ptr(nullptr), s_ref_cnt(new uint32_t(0)) { }

  explicit SmartSharedPointer(T *ptr) : s_ptr(ptr), s_ref_cnt(new uint32_t(1)) { }

  explicit SmartSharedPointer(const SmartSharedPointer &shared_obj)
  {
    s_ptr = shared_obj.s_ptr;
    s_ref_cnt = shared_obj.s_ref_cnt;

    ++(*s_ref_cnt);
  }
    

  ~SmartSharedPointer()
  {
    cleanup<T>();
  }

  T& operator*() const
  {
    return *s_ptr;
  }

  T* get() const
  {
    return s_ptr;
  }

  uint32_t* get_cnt() const
  {
    return s_ref_cnt;
  }

private:

  template <typename U>
  void cleanup()
  {
    --(*s_ref_cnt);
    
    if (*s_ref_cnt == 0)
    {
      delete s_ptr;
      delete s_ref_cnt;
    }
    
  }

  template<>
  void cleanup<int[]>()
  {
    --(*s_ref_cnt);

    if (*s_ref_cnt == 0)
    {
      delete [] s_ptr;
      delete s_ref_cnt;
    }
  }
  
  template<>
  void cleanup<const char*>()
  {
    --(*s_ref_cnt);
    
    if (*s_ref_cnt == 0)
    {
      // const char* is not dynamically allocated on the heap, therefore need not be deleted/freed.
      delete s_ref_cnt;
    }
  }
  
  T *s_ptr;
  uint32_t *s_ref_cnt;
};

template <typename T>
inline std::ostream& operator<<(std::ostream &os, const SmartSharedPointer<T> &s_ptr)
{
  os << "value = " << *s_ptr << "; address= " << s_ptr.get() << "; ref_cnt= " << *s_ptr.get_cnt() << std::endl;

  return os;
}

int main()
{ 
  SmartSharedPointer<int> s_ptr(new int(5));

  
  {
  SmartSharedPointer<int> new_ptr(s_ptr);

  std::cout << "first obj " << s_ptr << std::endl;
  std::cout << "second obj " << new_ptr << std::endl;
  }

  std::cout << "second obj is now out of scope" << std:: endl;
  std::cout << "first obj " << s_ptr << std::endl;


  std::cout << "enter some text to store in a const char* shared object" << std::endl;
  std::string *text = new std::string();
  std::getline(std::cin, *text);

  const char *c_str = text->c_str();
  
  SmartSharedPointer<const char*> c_ptr(&c_str);

  std::cout <<"char obj " << c_ptr << std::endl;
}
