
// ThreadSafeObject.h 
//  ThreadSafeObject by David Maisonave (609-345-1007) (www.axter.com)

/*Description:ThreadSafeObject is a thread safe wrapper class, which uses windows API 
Critical-Section logic. It can turn any object into a thread safe object.
ThreadSafeObject destructor will automatically destroy the object,so the object can 
be create via new on ThreadSafeObject constructor.This insures no other process has 
ownership of the object.

Example Usage

ThreadSafeObject<vector<int> > MyThreadSafeVectorInt(new vector<int>);
MyThreadSafeVectorInt.GetLockedObject()->push_back(123);
cout << MyThreadSafeVectorInt.GetLockedObject()->at(0) << endl;

//An instance of RefLock can be create to lock the object for the duration of a function.
Example Usage

ThreadSafeObject<string> MyThreadSafeString(new string);
string SomeFunction()
{
	ThreadSafeObject<string>::RefLock MyLockedString = MyThreadSafeString.GetLockedObject();
	MyLockedString->operator=("Hello World");
	return MyLockedString->substr(6);
}

//The destructor for RefLock automatically unlocks the object, so an explicit unlock call
//is not required.

*/


#ifndef THREADSAFEOBJECT_H_HEADER_GUARD_
#define THREADSAFEOBJECT_H_HEADER_GUARD_

template<class T>class ThreadSafeObject
{
public:     
	ThreadSafeObject(T* type)
		:m_type(type)
	{
		InitializeCriticalSection(&m_cs);
	}
	~ThreadSafeObject()
	{
		EnterCriticalSection (&m_cs);
		delete m_type;
		m_type=NULL;
		LeaveCriticalSection(&m_cs);
		DeleteCriticalSection(&m_cs);     
	}     

	class RefLock
	{     
		friend class ThreadSafeObject;

	public:
		RefLock()
			:m_type(NULL), m_cs(NULL)
		{
		}

		RefLock(T* type, CRITICAL_SECTION *cs)
			:m_type(type), m_cs(cs)
		{ 
			EnterCriticalSection (m_cs);
		}
		RefLock(const RefLock& Src)
			:m_type(Src.m_type), m_cs(Src.m_cs)
		{ 
			EnterCriticalSection (m_cs);
		}          

		~RefLock()
		{
			if (m_cs)
				LeaveCriticalSection(m_cs);
		}

		void UnLock()
		{
			if (m_cs)
				LeaveCriticalSection(m_cs);
			m_cs = NULL;
		}

		inline T* operator->() const{return m_type;}
		inline T& operator*() const{return *m_type;}

		T* m_type;
	private:          
		CRITICAL_SECTION *m_cs;     
	};     

	RefLock GetLockedObject()
	{
		return RefLock(m_type, &m_cs);
	}
	const RefLock GetConstLockedObject()const
	{
		return RefLock(m_type, &m_cs);
	}

	BOOL TryGetLockedObject(RefLock &ARefLock)
	{
		if (::TryEnterCriticalSection(&m_cs))
		{
			ARefLock.m_type = m_type;
			ARefLock.m_cs = &m_cs;
			return TRUE;
		}
		else
			return FALSE;
	}

private:     
	T* m_type;     
	CRITICAL_SECTION m_cs;     
	ThreadSafeObject(const ThreadSafeObject&);     
	ThreadSafeObject& operator=(const ThreadSafeObject&);

};

#endif //!THREADSAFEOBJECT_H_HEADER_GUARD_
