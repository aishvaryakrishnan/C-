
#include <iostream>

template <typename T>
class SharedObject
{
public:
    SharedObject() = delete;
    SharedObject(const T &copy_obj) : s_obj(copy_obj), s_ref_cnt(1) { }
    SharedObject(const SharedObject<T> &shared_obj) = delete;
    
    template <typename U>
    friend class SmartSharedPointer;
private:

T s_obj;
uint32_t s_ref_cnt;
};
 
template <typename T>
class SmartSharedPointer
{
public:
  SmartSharedPointer() = delete;

  explicit SmartSharedPointer(T copy_obj) : s_obj_ptr(new SharedObject<T>(copy_obj)) { }

  explicit SmartSharedPointer(const SmartSharedPointer &copy_ptr)
  {
    s_obj_ptr = copy_ptr.s_obj_ptr; 
    ++(s_obj_ptr->s_ref_cnt);
  }
    
  ~SmartSharedPointer()
  {
    cleanup();
  }

  T& operator*() const
  {
    return s_obj_ptr->s_obj;
  }

  T* get() const
  {
    return &s_obj_ptr->s_obj;
  }

  uint32_t get_cnt() const
  {
    return s_obj_ptr->s_ref_cnt;
  }

private:
  void cleanup()
  {
    --s_obj_ptr->s_ref_cnt;
    
    if (s_obj_ptr->s_ref_cnt == 0)
    {
      delete s_obj_ptr;
    }
  }
  
  SharedObject<T> *s_obj_ptr;
};

template <typename T>
inline std::ostream& operator<<(std::ostream &os, const SmartSharedPointer<T> &s_obj_ptr)
{
    os << "address= " << s_obj_ptr.get() << "; ref_cnt= " << s_obj_ptr.get_cnt() << std::endl;

    return os;
}

int main()
{ 
  SmartSharedPointer<int> s_ptr(5);

  {
  SmartSharedPointer<int> new_ptr(s_ptr);

  std::cout << "first obj " << s_ptr << std::endl;
  std::cout << "second obj " << new_ptr << std::endl;
  }

  std::cout << "second obj is now out of scope" << std:: endl;
  std::cout << "first obj " << s_ptr << std::endl;
}
