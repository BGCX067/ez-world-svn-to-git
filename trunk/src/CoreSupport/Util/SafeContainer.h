#ifndef CLIST_H
#define CLIST_H
#include <vector>
#include "blockallocator.h"
#include "ThreadSafeObject.h"

#define GetPtr(C)	C.GetSafePtr()->GetLockedObject()
namespace SafeContainer
{

	// 基于STL，线程安全的可管理内存的vector
	template <class Node> class cVector
	{
	public:
		cVector();
		virtual ~cVector(){ Destroy(); }
		ThreadSafeObject< std::vector<Node> > * GetSafePtr() { return m_pSafeLst;}
	private:
		BOOL Destroy();
	private:
		ThreadSafeObject< std::vector<Node> > * m_pSafeLst;
	};

	template <class Node>
	cVector< Node >::cVector()
	{
		m_pSafeLst = new ThreadSafeObject< std::vector<Node> >( new std::vector<Node>() );
	}

	template <class Node>
		BOOL cVector< Node>::Destroy()
	{
		if(m_pSafeLst)
			delete m_pSafeLst;
		m_pSafeLst = NULL;
		return TRUE;
	}

	// 基于STL，线程安全的可管理内存的list
	template <class Node> class cList
	{
	public:
		cList();
		virtual ~cList(){ Destroy(); }
		ThreadSafeObject< std::list<Node> > * GetSafePtr() { return m_pSafeLst;}
	private:
		BOOL Destroy();
	private:
		ThreadSafeObject< std::list<Node> > * m_pSafeLst;
	};

	template <class Node>
	cList<Node>::cList()
	{
		m_pSafeLst = new ThreadSafeObject< std::list<Node> >(new std::list<Node>());
	}

	template <class Node>
	BOOL cList<Node>::Destroy()
	{
		if(m_pSafeLst)
			delete m_pSafeLst;
		m_pSafeLst = NULL;
		return TRUE;
	}

	// 基于STL，线程安全的可管理内存的set
	template <class Node> class cSet
	{
	public:
		cSet();
		virtual ~cSet(){ Destroy(); }
		ThreadSafeObject< std::set<Node> > * GetSafePtr() { return m_pSafeSet;}
	private:
		BOOL Destroy();
	private:
		ThreadSafeObject< std::set<Node> > * m_pSafeSet;
	};

	template <class Node>
	cSet<Node>::cSet()
	{
		m_pSafeSet = new ThreadSafeObject< std::set<Node> >(new std::set<Node>());
	}

	template <class Node>
	BOOL cSet<Node>::Destroy()
	{
		if(m_pSafeSet)
			delete m_pSafeSet;
		m_pSafeSet = NULL;
		return TRUE;
	}

	// 基于STL，线程安全的可管理内存的multiset
	template <class Node> class cMultiSet
	{
	public:
		cMultiSet();
		virtual ~cMultiSet(){ Destroy(); }
		ThreadSafeObject< std::multiset<Node> > * GetSafePtr() { return m_pSafeSet;}
	private:
		BOOL Destroy();
	private:
		ThreadSafeObject< std::multiset<Node> > * m_pSafeSet;
	};

	template <class Node>
	cMultiSet<Node>::cMultiSet()
	{
		m_pSafeSet = new ThreadSafeObject< std::multiset<Node> >(new std::multiset<Node>());
	}

	template <class Node>
	BOOL cMultiSet<Node>::Destroy()
	{
		if(m_pSafeSet)
			delete m_pSafeSet;
		m_pSafeSet = NULL;
		return TRUE;
	}

	// 基于STL，线程安全的可管理内存的map
	template <class Key,class Type> class cMap
	{
	public:
		cMap();
		virtual ~cMap(){ Destroy(); }
		ThreadSafeObject< std::map<Key,Type> > * GetSafePtr() { return m_pSafeMap;}
	private:
		BOOL Destroy();
	private:
		ThreadSafeObject< std::map<Key,Type> > * m_pSafeMap;
	};

	template <class Key,class Type>
	cMap<Key,Type>::cMap()
	{
		m_pSafeMap = new ThreadSafeObject< std::map<Key,Type> >(new std::map<Key,Type>());
	}

	template <class Key,class Type>
	BOOL cMap<Key,Type>::Destroy()
	{
		if(m_pSafeMap)
			delete m_pSafeMap;
		m_pSafeMap = NULL;
		return TRUE;
	}

	// 基于STL，线程安全的可管理内存的multimap
	template <class Key,class Type> class cMultiMap
	{
	public:
		cMultiMap();
		virtual ~cMultiMap(){ Destroy(); }
		ThreadSafeObject< std::multimap<Key,Type> > * GetSafePtr() { return m_pSafeMap;}
	private:
		BOOL Destroy();
	private:
		ThreadSafeObject< std::multimap<Key,Type> > * m_pSafeMap;
	};

	template <class Key,class Type>
	cMultiMap<Key,Type>::cMultiMap()
	{
		m_pSafeMap = new ThreadSafeObject< std::multimap<Key,Type> >(new std::multimap<Key,Type>());
	}

	template <class Key,class Type>
	BOOL cMultiMap<Key,Type>::Destroy()
	{
		if(m_pSafeMap)
			delete m_pSafeMap;
		m_pSafeMap = NULL;
		return TRUE;
	}

};
#endif