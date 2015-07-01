// GameAutoPtr.h: declare for the CAutoPtr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEAUTOPTR_H__5708545C_E8F2_4155_AC66_4191BE0F150E__INCLUDED_)
#define AFX_GAMEAUTOPTR_H__5708545C_E8F2_4155_AC66_4191BE0F150E__INCLUDED_
#include "common.h"
#include "basetype.h"
//////////////////////////////////////////////////////////////////////
// 说明：简单智能指针，不支持COPY和=。该模板需要Release()函数支持
// 注意：包含该对象的类必须是个不能COYP和=的类。

template<TYPENAME T>
class CMyAutoPtr
{
public:
	CMyAutoPtr() : m_ptr(NULL)					{}
	CMyAutoPtr(T* ptr) : m_ptr(ptr)				{}		// may be null
	~CMyAutoPtr()								{ if(m_ptr) m_ptr->Release(); }
	CMyAutoPtr& operator=(T* ptr)				{ ASSERT(!m_ptr && ptr); m_ptr=ptr; return *this; }
	T*	pop()									{ T* p=m_ptr; m_ptr=NULL; return p; }
private: // can't use these
	CMyAutoPtr& operator=(const CMyAutoPtr& ptr);
	CMyAutoPtr(const CMyAutoPtr&);

public:
	operator T*()									{ return m_ptr; }
	POINT_NO_RELEASE<T>*	operator->()								{ ASSERT(m_ptr); return static_cast<POINT_NO_RELEASE<T>* >(m_ptr); }
	operator const T*()	 	 const					{ return m_ptr; }
	const POINT_NO_RELEASE<T>*	operator->() const					{ ASSERT(m_ptr); return static_cast<POINT_NO_RELEASE<T>* >(m_ptr); }

protected:
	T*			m_ptr;
};

template <class T>
class CScopePtr
{
public:
	CScopePtr(){m_ptr=NULL;}
	CScopePtr(T* ptr){m_ptr=ptr;}
	~CScopePtr()
	{
		SAFE_DELETE(m_ptr);
	}
	
	void operator=(T* ptr){m_ptr=ptr;}
	T* operator->(){return m_ptr;}
	T& operator*(){return *m_ptr;}
	T* operator&(){return m_ptr;}
	BOOL IsNull(){return m_ptr==NULL;}
private:
	T* m_ptr;
};

//////////////////////////////////////////////////////////////////////
// 说明：简单类JAVA对象，不支持COPY和=。该模板需要CreateNew()和Release()函数支持
// 注意：包含该对象的类必须是个不能COYP和=的类。

template<TYPENAME T>
class CJavaObj
{
public:
	CJavaObj() : m_ptr(NULL)					{}
	~CJavaObj()									{ if(m_ptr) m_ptr->Release(); }
private: // can't use these
	CJavaObj& operator=(const CJavaObj& ptr);
	CJavaObj(const CJavaObj&);

public:
	operator T*()									{ if(!m_ptr) m_ptr=T::CreateNew(); ASSERT(m_ptr); return m_ptr; }
	T*	operator->()								{ if(!m_ptr) m_ptr=T::CreateNew(); ASSERT(m_ptr); return m_ptr; }

protected:
	T*			m_ptr;
};

// Useful threading defines
#define OGRE_AUTO_MUTEX_NAME mutex
#if OGRE_THREAD_SUPPORT
	#define OGRE_AUTO_MUTEX mutable boost::recursive_mutex OGRE_AUTO_MUTEX_NAME;
	#define OGRE_LOCK_AUTO_MUTEX boost::recursive_mutex::scoped_lock ogreAutoMutexLock(OGRE_AUTO_MUTEX_NAME);
	#define OGRE_MUTEX(name) mutable boost::recursive_mutex name;
	#define OGRE_LOCK_MUTEX(name) boost::recursive_mutex::scoped_lock ogrenameLock(name);
	// like OGRE_AUTO_MUTEX but mutex held by pointer
	#define OGRE_AUTO_SHARED_MUTEX mutable boost::recursive_mutex *OGRE_AUTO_MUTEX_NAME;
	#define OGRE_LOCK_AUTO_SHARED_MUTEX assert(OGRE_AUTO_MUTEX_NAME); boost::recursive_mutex::scoped_lock ogreAutoMutexLock(*OGRE_AUTO_MUTEX_NAME);
	#define OGRE_NEW_AUTO_SHARED_MUTEX assert(!OGRE_AUTO_MUTEX_NAME); OGRE_AUTO_MUTEX_NAME = new boost::recursive_mutex();
	#define OGRE_DELETE_AUTO_SHARED_MUTEX assert(OGRE_AUTO_MUTEX_NAME); delete OGRE_AUTO_MUTEX_NAME;
	#define OGRE_COPY_AUTO_SHARED_MUTEX(from) assert(!OGRE_AUTO_MUTEX_NAME); OGRE_AUTO_MUTEX_NAME = from;
	#define OGRE_SET_AUTO_SHARED_MUTEX_NULL OGRE_AUTO_MUTEX_NAME = 0;
	#define OGRE_MUTEX_CONDITIONAL(mutex) if (mutex)
#else
	#define OGRE_AUTO_MUTEX
	#define OGRE_LOCK_AUTO_MUTEX
	#define OGRE_MUTEX(name)
	#define OGRE_LOCK_MUTEX(name)
	#define OGRE_AUTO_SHARED_MUTEX
	#define OGRE_LOCK_AUTO_SHARED_MUTEX
	#define OGRE_NEW_AUTO_SHARED_MUTEX
	#define OGRE_DELETE_AUTO_SHARED_MUTEX
	#define OGRE_COPY_AUTO_SHARED_MUTEX(from)
	#define OGRE_SET_AUTO_SHARED_MUTEX_NULL
	#define OGRE_MUTEX_CONDITIONAL(name)
#endif

template<class T>
class SharedPtr
{
protected:
	T* pRep;
	unsigned int* pUseCount;
public:
	OGRE_AUTO_SHARED_MUTEX // public to allow external locking
	/** Constructor, does not initialise the SharedPtr.
		@remarks
			<b>Dangerous!</b> You have to call bind() before using the SharedPtr.
	*/
	SharedPtr() : pRep(0), pUseCount(0)
    {
        OGRE_SET_AUTO_SHARED_MUTEX_NULL
    }
//explicit
	 SharedPtr(T* rep) : pRep(rep), pUseCount(new unsigned int(1))
	{
        OGRE_SET_AUTO_SHARED_MUTEX_NULL
		OGRE_NEW_AUTO_SHARED_MUTEX
	}
	SharedPtr(const SharedPtr& r)
        : pRep(0), pUseCount(0)
	{
		// lock & copy other mutex pointer
        
        OGRE_SET_AUTO_SHARED_MUTEX_NULL
        OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
        {
			OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			pRep = r.pRep;
			pUseCount = r.pUseCount; 
			// Handle zero pointer gracefully to manage STL containers
			if(pUseCount)
			{
				++(*pUseCount); 
			}
        }
	}
	SharedPtr& operator=(const SharedPtr& r) {
		if (pRep == r.pRep)
			return *this;
		release();
		// lock & copy other mutex pointer
        OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
        {
			OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			pRep = r.pRep;
			pUseCount = r.pUseCount;
			if (pUseCount)
			{
				++(*pUseCount);
			}
        }
		return *this;
	}
	virtual ~SharedPtr() {
        release();
	}


	inline T& operator*() const { assert(pRep); return *pRep; }
	inline T* operator->() const { assert(pRep); return pRep; }
	inline T* get() const { return pRep; }

	/** Binds rep to the SharedPtr.
		@remarks
			Assumes that the SharedPtr is uninitialised!
	*/
	void bind(T* rep) {
		assert(!pRep && !pUseCount);
        OGRE_NEW_AUTO_SHARED_MUTEX
		OGRE_LOCK_AUTO_SHARED_MUTEX
		pUseCount = new unsigned int(1);
		pRep = rep;
	}

	inline bool unique() const { OGRE_LOCK_AUTO_SHARED_MUTEX assert(pUseCount); return *pUseCount == 1; }
	inline unsigned int useCount() const { OGRE_LOCK_AUTO_SHARED_MUTEX assert(pUseCount); return *pUseCount; }
	inline unsigned int* useCountPointer() const { return pUseCount; }

	inline T* getPointer() const { return pRep; }

	inline bool isNull(void) const { return pRep == 0; }

    inline void setNull(void) { 
		if (pRep)
		{
			// can't scope lock mutex before release incase deleted
			release();
			pRep = 0;
			pUseCount = 0;
		}
    }
	operator bool()const
	{
		return isNull()==false;
	}
protected:
    inline void release(void)
    {
		bool destroyThis = false;

        /* If the mutex is not initialized to a non-zero value, then
           neither is pUseCount nor pRep.
         */

        OGRE_MUTEX_CONDITIONAL(OGRE_AUTO_MUTEX_NAME)
		{
			// lock own mutex in limited scope (must unlock before destroy)
			OGRE_LOCK_AUTO_SHARED_MUTEX
			if (pUseCount)
			{
				if (--(*pUseCount) == 0) 
				{
					destroyThis = true;
	            }
			}
        }
		if (destroyThis)
			destroy();

        OGRE_SET_AUTO_SHARED_MUTEX_NULL
    }
    virtual void destroy(void)
    {
        // IF YOU GET A CRASH HERE, YOU FORGOT TO FREE UP POINTERS
        // BEFORE SHUTTING OGRE DOWN
        // Use setNull() before shutdown or make sure your pointer goes
        // out of scope before OGRE shuts down to avoid this.
		delete pRep;
		pRep=NULL;
        delete pUseCount;
		pUseCount=NULL;
		OGRE_DELETE_AUTO_SHARED_MUTEX
    }
};

template<class T, class U> inline bool operator==(SharedPtr<T> const& a, SharedPtr<U> const& b)
{
	return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(SharedPtr<T> const& a, SharedPtr<U> const& b)
{
	return a.get() != b.get();
}
template<class T> inline bool operator!(SharedPtr<T> const& a)
{
	return a.get() ==NULL;
}


// 完善的智能指针模板...比AutoPtr提供更强大的功能, 是按照引用计数来清理的, 并且支持=号操作的浅拷贝
template<class T>
class TSmartPtr
{
	public:
		//	Constructors and destructor
		TSmartPtr (const TSmartPtr<T>& in_pRight )
			:	m_pRealPtr(in_pRight.m_pRealPtr), m_pRefCount(in_pRight.m_pRefCount)		{ this->AddRef(); }
		TSmartPtr ( T* in_pRight =0) : m_pRealPtr(in_pRight), m_pRefCount( new long ( 0 ) ) { this->AddRef(); }
		virtual ~TSmartPtr ( void )															{ this->RemoveRef(); }

		//	Assignment Operators
		TSmartPtr<T>& operator= (const TSmartPtr<T>& in_pRight );
		TSmartPtr<T>& operator= ( T* in_pRight );

		//	Operators
		T* operator-> ( void ) const;
		T& operator* ( void )const;

		//	Casting operators
		operator T* ( void )const;

		//	Comparison Operators
		bool operator== ( TSmartPtr<T>& in_oRight ) const;
		bool operator!= ( TSmartPtr<T>& in_oRight ) const;

	private:
		void	Destroy(void) { SAFE_RELEASE (m_pRealPtr); SAFE_DELETE (m_pRefCount); }
		void	AddRef(void);
		void	RemoveRef(void);
		bool	IsNull(void) const;	
		long	GetRefCount(void) const;

	private:
		T*		m_pRealPtr;
		long*	m_pRefCount;
};

template<class T>
TSmartPtr<T> &TSmartPtr<T>::operator= (const TSmartPtr<T>& in_pRight)
{
	if (*this != in_pRight)
	{
		this->RemoveRef();
		m_pRealPtr = in_pRight.m_pRealPtr;
		m_pRefCount = in_pRight.m_pRefCount;
		this->AddRef();
	}
	return *this;
}

template<class T>
TSmartPtr<T> &TSmartPtr<T>::operator= (T* in_pRight)
{
	this->RemoveRef();
	m_pRealPtr = in_pRight;
	m_pRefCount = new long ( 0 );
	this->AddRef();
	return *this;
}

template<class T>
inline T* TSmartPtr<T>::operator-> (void) const
{
	ASSERT (!IsNull());
	return m_pRealPtr;
}

template<class T>
inline T &TSmartPtr<T>::operator* (void) const
{
	ASSERT (!IsNull());
	return *m_pRealPtr;
}

template<class T>
inline TSmartPtr<T>::operator T* (void) const
{
	return m_pRealPtr;
}

template<class T>
inline bool TSmartPtr<T>::operator== (TSmartPtr<T>& in_pRight)const
{
	return m_pRealPtr == in_pRight.m_pRealPtr;
}

template<class T>
inline bool TSmartPtr<T>::operator!= (TSmartPtr<T>& in_pRight)const
{
	return m_pRealPtr != in_pRight.m_pRealPtr;
}

template<class T>
inline bool TSmartPtr<T>::IsNull(void) const
{
	return m_pRealPtr == NULL;
}

template<class T>
inline long TSmartPtr<T>::GetRefCount (void) const
{
	ASSERT (!IsNull());
	return *m_pRefCount;
}

template<class T>
void TSmartPtr<T>::AddRef (void)
{
//	if (!IsNull())
	{
		::InterlockedIncrement (m_pRefCount);
	}
}

template<class T>
void TSmartPtr<T>::RemoveRef (void)
{
//	if (!IsNull())
	{
		::InterlockedDecrement (m_pRefCount);
		if (*m_pRefCount == 0)
		{
			this->Destroy();
		}
		m_pRealPtr = NULL;
		m_pRefCount = NULL;
	}
}
	
	
	




#endif // !defined(AFX_GAMEAUTOPTR_H__5708545C_E8F2_4155_AC66_4191BE0F150E__INCLUDED_)
