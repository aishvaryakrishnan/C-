#include <iostream>

template <typename T>
class SmartSharedPointer
{
public:
  SmartSharedPointer() : s_ptr(nullptr), s_ref_cnt(0) { }

  explicit SmartSharedPointer(T *ptr) : s_ptr(ptr), s_ref_cnt(new uint32_t(1)) { }

  explicit SmartSharedPointer(const SmartSharedPointer &shared_obj)
  {
    s_ptr = shared_obj.s_ptr;
    s_ref_cnt = shared_obj.s_ref_cnt;

    if(s_ptr)
    {
      ++(*s_ref_cnt);
    }

  }
    

  ~SmartSharedPointer()
  {
    cleanup();
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
  void cleanup()
  {
    --(*s_ref_cnt);
    
    if (s_ref_cnt == 0)
    {
      delete s_ptr;
      delete s_ref_cnt;
    }
    
  }
  
  T *s_ptr;
  uint32_t *s_ref_cnt;
};

template <typename T>
inline std::ostream& operator<<(std::ostream &os, const SmartSharedPointer<T> &s_ptr)
{
    os << "address= " << s_ptr.get() << "; ref_cnt= " << *s_ptr.get_cnt() << std::endl;

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
}
