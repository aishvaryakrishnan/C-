#include <iostream>

class RefCnt
{
public:
  RefCnt(uint32_t s_ref_cnt, uint32_t w_ref_cnt) : s_ref_cnt(s_ref_cnt), w_ref_cnt(w_ref_cnt) { }
    
  template <typename U>
  friend class SmartSharedPointer;
    
  template <typename U>
  friend class WeakPointer;
    
private:
  uint32_t s_ref_cnt;
  uint32_t w_ref_cnt;
};

template <typename T>
class SmartSharedPointer
{
public:
  SmartSharedPointer() = delete;
  explicit SmartSharedPointer(T *ptr) : s_ptr(ptr), ref_cnt(new RefCnt(1, 0)) { }

  explicit SmartSharedPointer(const SmartSharedPointer &shared_obj)
  {
    s_ptr = shared_obj.s_ptr;
    ref_cnt = shared_obj.ref_cnt;
    ++(ref_cnt->s_ref_cnt);
  }
    
  ~SmartSharedPointer()
  {
    cleanup();
  }

  T& operator*() const
  {
    // we don't need to check if s_ptr is nullptr, that is, if it's destroyed
    // because in such a case the smart pointer itself
    // would be destructed
    return *s_ptr;
  }

  T* get() const
  {
    return s_ptr;
  }

  uint32_t get_cnt() const
  {
    return ref_cnt->s_ref_cnt;
  }

  template <typename U>
  friend class WeakPointer;
private:
  void cleanup()
  {
    --(ref_cnt->s_ref_cnt);
    
    if (ref_cnt->s_ref_cnt == 0)
    {
      delete s_ptr;
    
      if (ref_cnt->w_ref_cnt == 0)
      {    
          delete ref_cnt;
      }
    }
  }
  
  T *s_ptr;
  RefCnt *ref_cnt;
};

template <typename T>
inline std::ostream& operator<<(std::ostream &os, const SmartSharedPointer<T> &s_ptr)
{
  try
  {
    os << "value = " << *s_ptr;
  }
  catch (std::exception e)
  {
    os << "no value since the shared object was destroyed";
  }

  os << "; address= " << s_ptr.get() << "; ref_cnt= " << s_ptr.get_cnt() << std::endl;

  return os;
}

template <typename T>
class WeakPointer
{
public:
    WeakPointer() = delete;
    WeakPointer(const SmartSharedPointer<T> &s_obj_ptr) : w_ptr(s_obj_ptr.s_ptr), ref_cnt(s_obj_ptr.ref_cnt) 
    {
      ++(ref_cnt->w_ref_cnt); 
    }
    
    WeakPointer(const WeakPointer<T> &w_obj_ptr) : w_ptr(w_obj_ptr.w_ptr), ref_cnt(w_obj_ptr.ref_cnt)
    {
      ++(ref_cnt->w_ref_cnt);
    }
    
    ~WeakPointer()
    {
      cleanup(); 
    } 
    
    T* get() const noexcept
    {
      if(!is_obj_alive())
      {
	return nullptr;
      }
      
      return w_ptr;       
    }
    
    T& operator*() const
    {
      if(!is_obj_alive())
      {
	throw std::runtime_error("Object was destroyed");
      }

      return *w_ptr;
    
    }
    
    
    uint32_t get_cnt() const
    {
      return ref_cnt->w_ref_cnt; 
    }
    
    bool is_obj_alive() const
    {
      return ref_cnt->s_ref_cnt > 0;
    }
        
private:

  void cleanup()
  {
    --(ref_cnt->w_ref_cnt);
        
    if(ref_cnt->w_ref_cnt == 0 && ref_cnt->s_ref_cnt == 0)
    {
      // never delete w_ptr because shared pointer should be deleting it.
      delete ref_cnt;
    }
  }
  T *w_ptr;
  RefCnt *ref_cnt;
};

template <typename T>
inline std::ostream& operator<<(std::ostream &os, const WeakPointer<T> &w_ptr)
{
  try
  {
    auto value = *w_ptr;
    os << "value= " << value << ";";
  }
  catch (std::exception e)
  {
    os << "no value since the shared object was destroyed;";
  }
  
  os << "address= " << w_ptr.get() << "; ref_cnt= " << w_ptr.get_cnt() << std::endl;

    return os;
}


WeakPointer<int> check_weak_ptr()
{
  SmartSharedPointer<int> s_ptr(new int(5));

  *s_ptr = 6;
 
  WeakPointer<int> w_ptr(s_ptr);
   
  {
  SmartSharedPointer<int> new_ptr(s_ptr);

  std::cout << "first obj " << s_ptr << std::endl;
  std::cout << "second obj " << new_ptr << std::endl;
  std::cout << "first weak obj " << w_ptr << std::endl;
  }

   
  std::cout << "second obj is now out of scope" << std:: endl;
  std::cout << "first obj " << s_ptr << std::endl;
  std::cout << "first weak obj " << w_ptr << std::endl;

    
 return w_ptr;
    
}

int main()
{ 
 
  auto w_ptr = check_weak_ptr();
  
  
  if (!w_ptr.is_obj_alive())
  {
    std::cout<<"shared objects are out of scope" << std::endl;
  }
  
  
  std::cout << "weak obj " << w_ptr << std::endl;
}
