/* -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: t; tab-width: 4 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2004-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *   leon.sha@sun.com
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef __GC__
#define __GC__

#if defined(MMGC_PORTING_API)
	// When the porting API is in use, we exclude
	// stdlib functions or use them through
	// #defines for portability to platforms
	// that don't have them.
	#include "portapi_mmgc.h"
#else

#define MMGC_GCENTER(_gc)  MMgc::GCAutoExit __mmgc_auto_exit(_gc);

#if defined(MMGC_IA32) && defined(_MSC_VER)

#define MMGC_GET_STACK_EXTENTS(_gc, _stack, _size)						\
	jmp_buf __mmgc_env;													\
	setjmp(__mmgc_env);													\
	__asm { mov _stack,esp };											\
	GCAssertMsg(_gc->GetStackEnter() != 0, "Missing MMGC_GCENTER macro"); \
	_size = (uint32_t)(_gc->GetStackEnter() - (uintptr_t)_stack);

#else

#define MMGC_GET_STACK_EXTENTS(_gc, _stack, _size)						\
	jmp_buf __mmgc_env;													\
	setjmp(__mmgc_env);													\
	_stack = &__mmgc_env;												\
	GCAssertMsg(_gc->GetStackEnter() != 0, "Missing MMGC_GCENTER macro"); \
	_size = (uint32_t)(_gc->GetStackEnter() - (uintptr_t)_stack);

#endif

#ifdef MMGC_THREADSAFE
#define MMGC_ASSERT_GC_LOCK(gc)  GCAssert((gc)->m_lock.IsHeld() || (gc)->destroying)
#define MMGC_ASSERT_EXCLUSIVE_GC(gc) \
    GCAssert(((gc)->m_gcRunning \
			  && (gc)->m_exclusiveGCThread == GCThread::GetCurrentThread()) \
			 || (gc)->destroying)
#else
#define MMGC_ASSERT_GC_LOCK(gc)      ((void) 0)
#define MMGC_ASSERT_EXCLUSIVE_GC(gc) ((void) 0)
#endif

#endif // MMGC_PORTING_API

// Enable our own alloca() replacement that always allocates in the heap, this is good on
// systems with limited memory or limited stack
#define AVMPLUS_PARAM_ALLOCA_CUTOFF		4000	// Don't make real alloca() blow the stack; this limit is heuristic
#define AVMPLUS_PARAM_ALLOCA_DEFSIZE	1000	// Default number of bytes in a stack segment for heap-based alloca()
#ifdef AVMPLUS_HEAP_ALLOCA
#  define VMPI_alloca(core, autoptr, nbytes)	core->gc->allocaPush(nbytes, autoptr)
#  define VMPI_alloca_gc(gc, autoptr, nbytes)	gc->allocaPush(nbytes, autoptr)
#else
#  define VMPI_alloca(core, autoptr, nbytes)  (nbytes > AVMPLUS_PARAM_ALLOCA_CUTOFF ? core->gc->allocaPush(nbytes, autoptr) : alloca(nbytes))
#  define VMPI_alloca_gc(gc, autoptr, nbytes)  (nbytes > AVMPLUS_PARAM_ALLOCA_CUTOFF ? gc->allocaPush(nbytes, autoptr) : alloca(nbytes))
#endif

namespace avmplus
{
	class AvmCore;
}

namespace MMgc
{
	class GCAutoExit
	{
	public:
		GCAutoExit(GC *_gc);
		~GCAutoExit();
	private:
		GC* gc;
	};

	/**
	 * Conservative collector unit of work
	 */
	class GCWorkItem
	{
	public:
		GCWorkItem() : ptr(NULL), _size(0) { }
		inline GCWorkItem(const void *p, uint32_t s, bool isGCItem);

		uint32_t GetSize() const { return _size & ~1; }
		uint32_t IsGCItem() const { return _size & 1; }

		// If a WI is a GC item, `ptr` is the UserPointer; it must not
		// be the RealPointer nor an interior pointer
		const void *ptr;

		// The low bit of _size stores whether this is a GC item.
		// Always access this through `GetSize` and `IsGCItem`
		uint32_t _size;
	};

	/**
	 * GCRoot is root in the reachability graph, it contains a pointer a size 
	 * and will be searched for things.  
	 */
	class MMGC_API GCRoot
	{
		friend class GC;
		GCRoot();
		void init(GC*gc, const void *object, size_t size);
	public:
		/** subclassing constructor */
		GCRoot(GC *gc);
		/** general constructor */
		GCRoot(GC *gc, const void *object, size_t size);
		virtual ~GCRoot();

		// override new and delete so we can know the objects extents (via FixedMalloc::Size())
    void *operator new(size_t size)
    {
			return GCHeap::GetGCHeap()->GetFixedMalloc()->Alloc(size);
    }
        
		void operator delete (void *object)
		{
			GCHeap::GetGCHeap()->GetFixedMalloc()->Free(object);
		}

		const void *Get() const { return object; }
		void Set(const void *object, size_t size);

		GC *GetGC() const { return gc; }
		/** if your object goes away after the GC is deleted this can be useful */
		void Destroy();
	
	private:
		GC * gc;

		/** @access Requires(gc->m_rootListLock) */
		GCRoot *next;
		/** @access Requires(gc->m_rootListLock) */
		GCRoot *prev;
		const void *object;
		size_t size;

		GCWorkItem GetWorkItem() const { return GCWorkItem(object, (uint32_t)size, false); }
	};

	/**
	 * GCCallback is an interface that allows the application to get
	 * callbacks at interesting GC points.
	 */
	class MMGC_API GCCallback
	{
		friend class GC;
		friend class ZCT;
	public:
		GCCallback(GC *gc);
		virtual ~GCCallback();
		
		GC *GetGC() const { return gc; }
		/** if your object goes away after the GC is deleted this can be useful */
		void Destroy();

		/**
		 * This method is invoked after all marking and before any
		 * sweeping, useful for bookkeeping based on whether things
		 * got marked
		 *
		 * @access Requires(gc->exclusiveGC)
		 */
		virtual void presweep() {}

		/**
		 * This method is invoked after all sweeping
		 *
		 * @access Requires(gc->exclusiveGC)
		 */
		virtual void postsweep() {}

		/**
		 * This method is called whenever the collector decides to expand the heap
		 */
		virtual void heapgrew() {}

		// called before a ZCT reap begins
		virtual void prereap() {}

		// called after a ZCT reap completes
		virtual void postreap() {}

		/**
		 * This callback is the first thing a collecting thread does once it
		 * acquires exclusiveGC status.  When this is called, no other threads
		 * are running in requests and the calling thread holds gc->m_lock.
		 *
		 * Although this makes the most sense in an MMGC_THREADSAFE build, the
		 * callback is nonetheless called at the same point during collection
		 * even in non-MMGC_THREADSAFE builds.  This is so programs that build
		 * in both thread-safe and non-thread-safe configurations will see the
		 * same behavior in both.  The same idea applies to
		 * enterExclusiveGCNoLock and leaveExclusiveGC.
		 *
		 * @warning
		 *     Because the system calls this callback while holding the
		 *     GC-wide lock, implementations must keep it simple and not block
		 *     or call back into the GC, to avoid deadlock.
		 *
		 * @access Requires(gc->exclusiveGC && gc->m_lock)
		 */
		virtual void enterExclusiveGC() {}

		/**
		 * This callback happens after enterExclusiveGC.  The only difference
		 * is that this is called after the GC has released gc->m_lock.
		 *
		 * See notes and warning at GCCallback::enterExclusiveGC().
		 *
		 * @access Requires(gc->exclusiveGC && !gc->m_lock)
		 */
		virtual void enterExclusiveGCNoLock() {}

		/**
		 * This callback is the last thing a collecting thread does before it
		 * relinquishes exclusiveGC status, allowing other threads to run in
		 * requests.
		 *
		 * In an MMGC_THREADSAFE build, this is called while the GC is holding
		 * the GC-wide lock.  See warning at GCCallback::enterExclusiveGC().
		 *
		 * @access Requires(gc->exclusiveGC && gc->m_lock)
		 */
		virtual void leaveExclusiveGC() {}

		/**
		 * This method is called before an RC object is reaped
		 */
		virtual void prereap(void* /*rcobj*/) {}
		virtual void log(const char* /*str*/) {}
		virtual void oom(MemoryStatus) {}

	private:
		GC *gc;
		GCCallback *nextCB;
		GCCallback *prevCB;
	};

	#ifdef MMGC_64BIT
	#define HIDDENPTRMASK (uintptr_t(0x1L)<<63)
	#else
	#define HIDDENPTRMASK (uintptr_t(0x1L)<<31)
	#endif

	template <class T>
	class GCHiddenPointer
	{
	public:
		GCHiddenPointer(T obj=NULL) { set(obj); }
		operator T() const { return (T) (val^HIDDENPTRMASK);	 }
		T operator=(T tNew) 
		{ 
			set(tNew); 
			return (T)this; 
		}
		T operator->() const { return (T) (val^HIDDENPTRMASK); }

	private:
		// private to prevent its use and someone adding it, GCC creates
		// WriteBarrier's on the stack with it
		GCHiddenPointer(const GCHiddenPointer<T>& toCopy) { GCAssert(false); }
		
		void set(T obj) 
		{
			uintptr_t p = (uintptr_t)obj;
			val = p ^ HIDDENPTRMASK;
		}
		uintptr_t val;
	};

	/**
	 * The Zero Count Table used by DRC.
	 */
	class MMGC_API ZCT
	{
		friend class GC;
		// how many items there have to be to trigger a Reap
		static const int ZCT_REAP_THRESHOLD;

		// size of table in pages
		static const int ZCT_START_SIZE;
	public:
		ZCT();
		~ZCT();
		void Add(RCObject *obj);
		void Remove(RCObject *obj);
		void Reap();
	private:
		// for MMGC_GET_STACK_EXTENTS
		uintptr_t StackTop;

		GC *gc;
		void SetGC(GC*);

		// in pages
		int zctSize;

		// the zero count table
		RCObject **zct;

		// index to the end
		RCObject **zctNext;

		// freelist of open slots
		RCObject **zctFreelist;

		// during a reap where we are
		int zctIndex;

		// during a reap becomes zctNext
		int nextPinnedIndex;

		int count;
		int zctReapThreshold;

		// are we reaping the zct?
		bool reaping;

		void PinStackObjects(const void *start, size_t len);
		bool IsZCTFreelist(RCObject **obj)
		{
			return obj >= zct && obj < (RCObject**)(zct+zctSize/sizeof(RCObject*));
		}
	};

	/**
	 * This is a general-purpose garbage collector used by the Flash Player.
	 * Application code must implement the GCRoot interface to mark all
	 * reachable data.  Unreachable data is automatically destroyed.
	 * Objects may optionally be finalized by subclassing the GCObject
	 * interface.
	 *
	 * This garbage collector is intended to be modular, such that
	 * it can be used in other products or replaced with a different
	 * collector in the future.
	 *
	 * Memory allocation and garbage collection strategy:
	 * Mark and sweep garbage collector using fixed size classes
	 * implemented by the GCAlloc class.  Memory blocks are obtained
	 * from the OS via the GCHeap heap manager class.
	 *
	 * When an allocation fails because a suitable memory block is
	 * not available, the garbage collector decides either to garbage
	 * collect to free up space, or to expand the heap.  The heuristic
	 * used to make the decision to collect or expand is taken from the
	 * Boehm-Demers-Weiser (BDW) garbage collector and memory allocator.
	 * The BDW algorithm is (pseudo-code):
	 *
	 *    if (allocs since collect >= heap size / FSD)
	 *      collect
	 *    else
	 *      expand(request size + heap size / FSD)
	 *
	 * The FSD is the "Free Space Divisor."  For the first cut, I'm trying
	 * 4.  TODO: Try various FSD values against real Player workloads
	 * to find the optimum value.
	 *
	 */
	class MMGC_API GC
	{
		friend class GCRoot;
		friend class GCCallback;
		friend class GCAlloc;
		friend class GCLargeAlloc;
		friend class RCObject;
		friend class GCInterval;
		friend class ZCT;
	public:

		/**
		 * If you need context vars use this!
		 */
		enum
		{
			GCV_COREPLAYER,
			GCV_AVMCORE,
			GCV_COUNT
		};
		void *GetGCContextVariable(int var) const { return m_contextVars[var]; }
		void SetGCContextVariable(int var, void *val) { m_contextVars[var] = val; }
		
		avmplus::AvmCore *core() const { return (avmplus::AvmCore*)GetGCContextVariable(GCV_AVMCORE); }

		/**
		 * greedy is a debugging flag.  When set, every allocation
		 * will cause a garbage collection.  This makes code run
		 * abysmally slow, but can be useful for detecting mark
		 * bugs.
		 *
		 * The GC reads this flag only when holding the GC lock.  It is best
		 * to set it as soon as the GC is created.
		 *
		 * @access Requires(m_lock)
		 */
		bool greedy;

		/**
		 * nogc is a debugging flag.  When set, garbage collection
		 * never happens.
		 *
		 * @access Requires(m_lock)
		 */
		bool nogc;

		/**
	      * findUnmarkedPointers is a debugging flag, only 
		  */
		bool findUnmarkedPointers;

		/**
		* turns on code that does a trace before reaping zero count object and asserting on
		* any objects that get marked, debug builds only
		*/
		bool validateDefRef;		
		bool keepDRCHistory;

		/**
		 * Expand, don't collect, until we reach this threshold. Units are
		 * pages, not KB, just like GCHeap::GetTotalHeapSize().
		 *
		 * In an MMGC_THREADSAFE build, the GC reads this configuration value
		 * only when holding the GC lock.  Set it during
		 * initialization, before the GC is visible to multiple threads.
		 *
		 * @access Requires(m_lock)
		 */
		size_t collectThreshold;

		bool dontAddToZCTDuringCollection;
		bool incrementalValidation;

#ifdef _DEBUG
		bool incrementalValidationPedantic;
#endif

		/**
		 * Configuration flag enabling incremental collection.
		 *
		 * In an MMGC_THREADSAFE build, the GC reads this flag only when
		 * holding the GC lock.  Set it during initialization.
		 *
		 * @access Requires(m_lock)
		 */
		bool incremental;

		// -- Interface
		GC(GCHeap *heap);
		~GC();
		
		/**
		 * Causes an immediate garbage collection.
		 *
		 * In an MMGC_THREADSAFE build, the caller must not be inside a
		 * request.  If the caller is inside a request, call
		 * CollectFromRequest() instead.
		 *
		 * @access Requires(!m_lock && !request)
		 */
		void Collect();

		/**
		 * Perform some GC-related work if needed.  Call this during
		 * application down time.
		 *
		 * In incremental mode, this may result in a call to
		 * StartIncrementalMark() or IncrementalMark(), which may in turn push
		 * the current GC cycle to completion.  In non-incremental mode, this
		 * heuristically decides whether to do a full Collect().
		 *
		 * @param callerHasActiveRequest
		 *     Must be true iff the calling thread is already in a request.
		 */
		void MaybeGC(bool callerHasActiveRequest=false);

		/**
		* flags to be passed as second argument to alloc
		*/
		enum AllocFlags
		{
			kZero=1,
			kContainsPointers=2,
			kFinalize=4,
			kRCObject=8
		};

		enum PageType
		{
			kNonGC = 0,
			kGCAllocPage = 1,
			kGCLargeAllocPageRest = 2,
			kGCLargeAllocPageFirst = 3
		};

		/**
		 * Main interface for allocating memory.  Default flags is no
		 * finalization, contains pointers is set and zero is set.
		 *
		 * Do not call this from a finalizer.
		 *
		 * @access Requires(request)
		 */
		void *Alloc(size_t size, int flags=0);

#ifdef MMGC_RCROOT_SUPPORT
		
	private:
		class RCRootSegment : public GCRoot
		{
		public:
			RCRootSegment(GC* gc, void* mem, size_t size);
			void*			mem;
			size_t			size;
			RCRootSegment*	prev;
			RCRootSegment*	next;
		};
		
		RCRootSegment* rcRootSegments;
		
	public:
		
		/**
		 * Allocate memory that will be scanned for pointers to GC memory
		 * and from which pointers to RC objects will pin those objects.
		 */
		void* AllocRCRoot(size_t size);
		
		/**
		 * Free memory allocated with AllocRCRoot.
		 */
		void FreeRCRoot(void* mem);
		
#endif // MMGC_RCROOT_SUPPORT

		/** @access Requires(request && m_lock) */
		void *AllocAlreadyLocked(size_t size, int flags=0);

		
		/**
		 * overflow checking way to call Alloc for a # of n size'd items,
		 * all instance of Alloc(num*sizeof(thing)) should be replaced with:
		 * Calloc(num, sizeof(thing))
		 *
		 * Do not call this from a finalizer.
		 *
		 * @access Requires(request)
		 */
		void *Calloc(size_t num, size_t elsize, int flags=0);

		/**
		 * One can free a GC allocated pointer, this will throw an assertion
		 * if called during the Sweep phase (ie via a finalizer) it can only be
		 * used outside the scope of a collection
		 *
		 * @access Requires(request)
		 */
		void Free(const void *ptr);

		/**
		 * return the size of a piece of memory, may be bigger than what was asked for
		 *
		 * @access Requires(request || exclusiveGC)
		 */
		static size_t Size(const void *ptr)
		{
			GCAssert(GetGC(ptr)->IsGCMemory(ptr));			
			size_t size = GCLargeAlloc::GetBlockHeader(ptr)->usableSize;
			size -= DebugSize();
			return size;

		}

		/**
		 * Tracers should employ GetMark and SetMark to
		 * set the mark bits during the mark pass.
		 *
		 * @access Requires(request || exclusiveGC)
		 */
		static int GetMark(const void *item);

		static int SetMark(const void *item)
		{
			GCAssert(item != NULL);
#ifdef MMGC_MEMORY_INFO
			GC *gc = GetGC(item);	
			item = GetRealPointer(item);
			GCAssert(gc->IsPointerToGCPage(item));
#endif 			
			if (GCLargeAlloc::IsLargeBlock(item)) {
				return GCLargeAlloc::SetMark(item);
			} else {
				return GCAlloc::SetMark(item);
			}
		}
		
		void SetQueued(const void *item)
		{
#ifdef MMGC_MEMORY_INFO
			item = GetRealPointer(item);
			GCAssert(IsPointerToGCPage(item));
#endif 			
			if (GCLargeAlloc::IsLargeBlock(item)) {
				GCLargeAlloc::SetQueued(item);
			} else {
				GCAlloc::SetQueued(item);
			}
		}

		static void ClearFinalized(const void *item)
		{
#ifdef MMGC_MEMORY_INFO
			GC *gc = GetGC(item);	
			item = GetRealPointer(item);
			GCAssert(gc->IsPointerToGCPage(item));
#endif 			
			if (GCLargeAlloc::IsLargeBlock(item)) {
				GCLargeAlloc::ClearFinalized(item);
			} else {
				GCAlloc::ClearFinalized(item);
			}
		}

		static void SetFinalize(const void *item)
		{
#ifdef MMGC_MEMORY_INFO
			GC *gc = GetGC(item);	
			item = GetRealPointer(item);
			GCAssert(gc->IsPointerToGCPage(item));
#endif 			
			if (GCLargeAlloc::IsLargeBlock(item)) {
				GCLargeAlloc::SetFinalize(item);
			} else {
				GCAlloc::SetFinalize(item);
			}
		}

		static int IsFinalized(const void *item)
		{
#ifdef MMGC_MEMORY_INFO
			GC *gc = GetGC(item);	
			item = GetRealPointer(item);
			GCAssert(gc->IsPointerToGCPage(item));
#endif 			
			if (GCLargeAlloc::IsLargeBlock(item)) {
				return GCLargeAlloc::IsFinalized(item);
			} else {
				return GCAlloc::IsFinalized(item);
			}
		}

		static int HasWeakRef(const void *item)
		{
#ifdef MMGC_MEMORY_INFO
			GC *gc = GetGC(item);	
			item = GetRealPointer(item);
			GCAssert(gc->IsPointerToGCPage(item));
#endif 			
			if (GCLargeAlloc::IsLargeBlock(item)) {
				return GCLargeAlloc::HasWeakRef(item);
			} else {
				return GCAlloc::HasWeakRef(item);
			}
		}

		/**
		 * Utility function: Returns the GC object
		 * associated with an allocated object
		 */
		static GC* GetGC(const void *item)
		{
			GC **gc = (GC**) ((uintptr_t)item&~0xfff);
			return *gc;
		}

		/**
		 * Used by sub-allocators to obtain memory.
		 *
		 * @access Requires(m_lock)
		 */
		void* AllocBlock(int size, int pageType, bool zero=true);

		/** @access Requires((request && m_lock) || exclusiveGC) */
		void FreeBlock(void *ptr, uint32_t size);

		GCHeap *GetGCHeap() const { return heap; }

		void ReapZCT() { zct.Reap(); }
		bool Reaping() { return zct.reaping; }
#ifdef _DEBUG
		void RCObjectZeroCheck(RCObject *);
#endif

		/**
		 * debugging tool
	     */
		bool IsPointerToGCPage(const void *item);

		/**
		 * Do as much marking as possible in time milliseconds
		 */
		void IncrementalMark();

		/**
		 * Are we currently marking
		 */
		inline bool IncrementalMarking() const { return marking; }

		/**
		 * A magical write barrier that finds the container's address and the
		 * GC, just make sure @a address is a pointer to a GC page. Only used
		 * by WB smart pointers.
		 *
		 * @access Requires(request)
		 */
		static void WriteBarrier(const void *address, const void *value);

		/** @access Requires(request) */
		static void WriteBarrierNoSub(const void *address, const void *value);

		/** @access Requires(request) */
		void writeBarrier(const void *container, const void *address, const void *value)
		{
			GCAssert(!container || IsPointerToGCPage(container));
			GCAssert(((uintptr_t)address & 3) == 0);

			if (container) {
				GCAssert(address >= container);
				GCAssert(address < (char*)container + Size(container));
				WriteBarrierNoSubstitute(container, value);
			}
			WriteBarrierWrite(address, value);
		}

		/**
		 * optimized version with no RC checks or pointer masking
		 *
		 * @access Requires(request)
		 */
		void writeBarrierRC(const void *container, const void *address, const void *value);

		/**
		 * Write barrier when the value could be a pointer with anything in the lower 3 bits
		 * FIXME: maybe assert that the lower 3 bits are either zero or a pointer type signature,
		 * this would require the application to tell us what bit patterns are pointers.
		 *
		 * @access Requires(request)
		 */
		__forceinline void WriteBarrierNoSubstitute(const void *container, const void *value)
		{
			WriteBarrierTrap(container, (const void*)((uintptr_t)value&~7));
		}
			
		/**
		 * AVM+ write barrier, valuePtr is known to be pointer and the caller
		 * does the write.
		 *
		 * @access Requires(request)
		 */
		__forceinline void WriteBarrierTrap(const void *container, const void *valuePtr)
		{
			GCAssert(IsPointerToGCPage(container));
			GCAssert(((uintptr_t)valuePtr&7) == 0);
			GCAssert(IsPointerToGCPage(container));
			if(marking && valuePtr && GetMark(container) && IsWhite(valuePtr))
			{
				TrapWrite(container, valuePtr);
			}
		}

		void ConservativeWriteBarrierNoSubstitute(const void *address, const void *value)
		{
			if(IsPointerToGCPage(address))
				WriteBarrierNoSubstitute(FindBeginning(address), value);
		}

		void MemoryStatusChange(MemoryStatus from, MemoryStatus to)
		{
			pendingStatusChange = true;
			statusFrom = from;
			statusTo = to;
		}

		void Poke();

	private:
		__forceinline void WriteBarrierWrite(const void *address, const void *value);
		__forceinline void WriteBarrierWriteRC(const void *address, const void *value);

	public:

		/** @access Requires(request || exclusiveGC) */
		bool ContainsPointers(const void *item);

		/** @access Requires(request) */
		void *FindBeginning(const void *gcItem)
		{
			GCAssert(gcItem != NULL);
			GCAssert(GetPageMapValue((uintptr_t)gcItem) != 0);
			void *realItem = NULL;
			if((uintptr_t)gcItem < memStart || (uintptr_t)gcItem >= memEnd)
				return NULL;
			int bits = GetPageMapValue((uintptr_t)gcItem);
			switch(bits)
			{
			case kGCAllocPage:
				realItem = GCAlloc::FindBeginning(gcItem);
				break;
			case kGCLargeAllocPageFirst:
				realItem = GCLargeAlloc::FindBeginning(gcItem);
				break;
			case kGCLargeAllocPageRest:
				while(bits == kGCLargeAllocPageRest)
				{
					gcItem = (void*) ((uintptr_t)gcItem - GCHeap::kBlockSize);
					bits = GetPageMapValue((uintptr_t)gcItem);
				}
				realItem = GCLargeAlloc::FindBeginning(gcItem);
				break;
			default:
				return NULL;
			}		
#ifdef MMGC_MEMORY_INFO
			realItem = GetUserPointer(realItem);
#endif
			return realItem;
		}

		bool IsRCObject(const void *);

		/**
		 * True during Sweep phase.  Application code can use this to
		 * determine if it's being called (directly or indirectly) from a
		 * finalizer.
		 *
		 * @see IsGCRunning()
		 * @access Requires(request || exclusiveGC)
		 */
		bool Collecting() const
		{
			return collecting;
		}

		/** @access Requires(request || exclusiveGC) */
		bool IsGCMemory (const void *);

		/** @access Requires(request || exclusiveGC) */
		bool IsQueued(const void *item);

		static double duration(uint64_t start) 
		{
			return (double(VMPI_getPerformanceCounter() - start) * 1000) / VMPI_getPerformanceFrequency();
		}

#ifndef MMGC_THREADSAFE
		void DisableThreadCheck() { disableThreadCheck = true; }
#endif

		/** GC initialization time, in ticks.  Used for logging. */
		const uint64_t t0;

		// a tick is the unit of VMPI_getPerformanceFrequency()
		static uint64_t ticksToMicros(uint64_t ticks) 
		{ 
			return (ticks*1000000)/VMPI_getPerformanceFrequency();
		}


		static uint64_t ticksToMillis(uint64_t ticks) 
		{ 
			return (ticks*1000)/VMPI_getPerformanceFrequency();
		}

		/**
		 * Total number of bytes of pointer-containing memory scanned by this
		 * GC.  Used to measure marking rate, which is
		 * <code>bytesMarked/ticksToMillis(markTicks)</code>.
		 *
		 * @access ReadWrite(request, exclusiveGC)
		 */
		uint64_t bytesMarked;

		/**
		 * Total time spent doing incremental marking, in ticks.  See
		 * bytesMarked.
		 *
		 * @access ReadWrite(request, exclusiveGC)
		 */
		uint64_t markTicks;

		// calls to mark item
		uint32_t lastStartMarkIncrementCount;
		uint32_t markIncrements;

		/**
		 * Number of calls to MarkItem().
		 * @access ReadWrite(request, exclusiveGC)
		 */
		uint32_t marks;

		/**
		 * Number of calls to Sweep().
		 * @access ReadWrite(request, exclusiveGC)
		 */
        uint32_t sweeps;

		/**
		 * Number of calls to MarkItem() during the current (or most recent)
		 * IncrementalMark().
		 *
		 * @access ReadWrite(request, exclusiveGC)
		 */
		uint32_t numObjects;

		/**
		 * Number of bytes scanned in MarkItem() during the current (or most
		 * recent) IncrementalMark().
		 *
		 * @access ReadWrite(request, exclusiveGC)
		 */
		size_t objSize;

		/**
		 * Time of the latest FinishIncrementalMark() call, in ticks.
		 *
		 * @access ReadWrite(request, exclusiveGC)
		 */
		uint64_t sweepStart;

		/**
		 * True if we emptied the work queue during the most recent
		 * incremental mark.  This means the next mark will force the GC cycle
		 * through to completion.
		 *
		 * @access ReadWrite(request, exclusiveGC)
		 */
		bool hitZeroObjects;

		// called at some apropos moment from the mututor, ideally at a point
		// where all possible GC references will be below the current stack pointer
		// (ie in memory we can safely zero).  This will return right away if
		// called more than once between collections so it can safely be called
		// a lot without impacting performance
		void CleanStack(bool force=false);

		bool Destroying() { return destroying; }

		/** @access Requires(request) */
		static GCWeakRef *GetWeakRef(const void *obj);
		
		// a WeakRef that always refers to null. useful if you need one.
		GCWeakRef* emptyWeakRef;
	
		/** @access Requires((request && m_lock) || exclusiveGC) */
		void ClearWeakRef(const void *obj);

		const void *GetStackTop() const { return (const void*) GetStackEnter(); }
		uintptr_t GetStackEnter() const { return stackEnter; }
		void SetStackEnter(void *enter);

		// for deciding a tree of things should be scanned from presweep
		void PushWorkItem(GCWorkItem &item) { PushWorkItem(m_incrementalWork, item); }

	private:

		void *heapAlloc(size_t size, bool expand=true, bool zero=true);
		void heapFree(void *ptr, size_t siz=0);

		void gclog(const char *format, ...);
		void log_mem(const char *name, size_t s, size_t comp );

		const static int kNumSizeClasses = 40;

		// FIXME: only used for FixedAlloc, GCAlloc sized dynamically
		const static int kPageUsableSpace = 3936;

		/** @access Requires(request && m_lock) */
		uint32_t *GetBits(int numBytes, int sizeClass);

		/** @access Requires((request && m_lock) || exclusiveGC) */
		void FreeBits(uint32_t *bits, int sizeClass)
		{
#ifdef _DEBUG
			for(int i=0, n=noPointersAllocs[sizeClass]->m_numBitmapBytes; i<n;i++) GCAssert(((uint8_t*)bits)[i] == 0);
#endif
			*(uint32_t**)bits = m_bitsFreelists[sizeClass];
			m_bitsFreelists[sizeClass] = bits;
		}

		/** @access Requires((request && m_lock) || exclusiveGC) */
		uint32_t *m_bitsFreelists[kNumSizeClasses];
		/** @access Requires((request && m_lock) || exclusiveGC) */
		uint32_t *m_bitsNext;

		/** @access Requires((request && m_lock) || exclusiveGC) */
		GCHashtable weakRefs;

		bool destroying;

		/**
		 * We track the heap size so if it expands due to fixed memory
		 * allocations we can trigger a mark/sweep. Otherwise we can have lots
		 * of small GC objects allocating tons of fixed memory, which results
		 * in huge heaps and possible out of memory situations.
		 *
		 * @access Requires(m_lock)
		 */
		size_t heapSizeAtLastAlloc;

		// we track the top and bottom of the stack for cleaning purposes.
		// the top tells us how far up the stack as been dirtied.
		// the bottom is also tracked so we can ensure we're on the same
		// stack that the GC responsible for cleaning.  necessary if multiple
		// threads use the GC.  only thread that creates the GC will have its stack
		// tracked and cleaned.
		bool stackCleaned;
		const void *rememberedStackTop;
		uintptr_t stackEnter;

#ifndef MMGC_THREADSAFE
		// for external which does thread safe multi-thread AS execution
		bool disableThreadCheck;
#endif

		GCRoot* emptyWeakRefRoot;

		/**
		 * True if incremental marking is on and some objects have been marked.
		 * This means write barriers are enabled.
		 *
		 * @access ReadWrite(request, exclusiveGC)
		 *
		 * The GC thread may read and write this flag.  Application threads in
		 * requests have read-only access.
		 */
		bool marking;
		GCStack<GCWorkItem> m_incrementalWork;
		void StartIncrementalMark();
		void FinishIncrementalMark();

		/** @access Requires(request || exclusiveGC) */
		int IsWhite(const void *item);
		
		/** @access ReadWrite(request, exclusiveGC) */
		uint64_t lastMarkTicks;
		/** @access ReadWrite(request, exclusiveGC) */
		uint64_t lastSweepTicks;

		const static int16_t kSizeClasses[kNumSizeClasses];		
		const static uint8_t kSizeClassIndex[246];

		void *m_contextVars[GCV_COUNT];

		// bitmap for what pages are in use, 2 bits for every page
		// 0 - not in use
		// 1 - used by GCAlloc
		// 3 - used by GCLargeAlloc

		/** @access Requires(pageMapLock) */
		uintptr_t memStart;
		/** @access Requires(pageMapLock) */
		uintptr_t memEnd;

		/** @access Requires(m_lock) */
		size_t totalGCPages;

		/**
		 * The bitmap for what pages are in use.  Any access to either the
		 * pageMap pointer or the bitmap requires pageMapLock.
		 *
		 * (Note: A better synchronization scheme might be to use atomic
		 * operations to read and write the pageMap pointer, writing it only
		 * from within m_lock; and then using atomic read and write
		 * operations--on Intel x86, these are just ordinary reads and
		 * writes--to access the bitmap, with writes again only from within
		 * m_lock.  This would require reallocating the pageMap more often,
		 * but at least write barriers wouldn't have to acquire the spinlock.)
		 *
		 * @access Requires(pageMapLock)
		 */
		unsigned char *pageMap;

		/**
		 * This spinlock covers memStart, memEnd, and the contents of pageMap.
		 */
#ifdef MMGC_THREADSAFE
		mutable vmpi_spin_lock_t pageMapLock;
#endif

		inline int GetPageMapValue(uintptr_t addr) const
		{
#ifdef MMGC_THREADSAFE
			GCAcquireSpinlock lock(pageMapLock);
#endif
			return GetPageMapValueAlreadyLocked(addr);
		}

		/** @access Requires(pageMapLock) */
		int GetPageMapValueAlreadyLocked(uintptr_t addr) const;

		/**
		 * Set the pageMap bits for the given address.  Those bits must be
		 * zero beforehand.
		 *
		 * @access Requires(pageMapLock)
		 */
		void SetPageMapValue(uintptr_t addr, int val);

		/**
		 * Zero out the pageMap bits for the given address.
		 *
		 * @access Requires(pageMapLock)
		 */
		void ClearPageMapValue(uintptr_t addr);

		void MarkGCPages(void *item, uint32_t numpages, int val);
		void UnmarkGCPages(void *item, uint32_t numpages);
		
		/**
		 * Mark a region of memory, this will search all memory pointed to recursively
		 * and mark any GC Objects it finds
		 */
		void ConservativeMarkRegion(const void *base, size_t bytes);

		GCAlloc *containsPointersAllocs[kNumSizeClasses];
		GCAlloc *containsPointersRCAllocs[kNumSizeClasses];
		GCAlloc *noPointersAllocs[kNumSizeClasses];
		GCLargeAlloc *largeAlloc;
		GCHeap *heap;
		
		/** @access Requires(m_lock) */
		void* AllocBlockIncremental(int size, bool zero=true);

		/** @access Requires(m_lock) */
		void* AllocBlockNonIncremental(int size, bool zero=true);

	protected:
		/**
		 * Collect in a thread-safe, recursion-preventing way, with
		 * callbacks.
		 *
		 * Both parameters are ignored in non-MMGC_THREADSAFE builds.  In an
		 * MMGC_THREADSAFE build, callerHoldsLock must be true iff the calling
		 * thread already holds m_lock, and callerHasActiveRequest must be
		 * true iff the calling thread is already in an active request.
		 */
		void CollectWithBookkeeping(bool callerHoldsLock,
									bool callerHasActiveRequest);

	private:
		/**
		 * Just collect.
		 *
		 * @access Requires(exclusiveGC)
		 */
		void CollectImpl();

#ifdef _DEBUG
		public:
#endif
		/** @access Requires(exclusiveGC) */
		void ClearMarks();
#ifdef _DEBUG
		private:
#endif


#ifdef _DEBUG
		public:
		// sometimes useful for mutator to call this
		/** @access Requires(exclusiveGC) */
		void Trace(const void *stackStart=NULL, uint32_t stackSize=0);
		private:
#endif

		/** @access Requires(exclusiveGC) */
		void Finalize();
		/** @access Requires(exclusiveGC) */
		void Sweep(bool force=false);
		/** @access Requires(exclusiveGC) */
		void ForceSweep() { Sweep(true); }
		/** @access Requires(exclusiveGC) */
		void Mark(GCStack<GCWorkItem> &work);
		/** @access Requires(exclusiveGC) */
		void MarkQueueAndStack(GCStack<GCWorkItem> &work);
		/** @access Requires(exclusiveGC) */
		void MarkItem(GCStack<GCWorkItem> &work)
		{
			GCWorkItem workitem = work.Pop();
			MarkItem(workitem, work);
		}
		/** @access Requires(exclusiveGC) */
		void MarkItem(GCWorkItem &workitem, GCStack<GCWorkItem> &work);

		/**
		 * Write barrier slow path. Queues the white object.
		 *
		 * @access Requires(request)
		 */
		void TrapWrite(const void *black, const void *white);

		/**
		 * Number of pages allocated from the GCHeap since the start of the
		 * current GC cycle.
		 *
		 * @access Requires((request && m_lock) || exclusiveGC)
		 */
		unsigned int allocsSinceCollect;

		/**
		 * True during the sweep phase of collection.  Several things have to
		 * behave a little differently during this phase.  For example,
		 * GC::Free() does nothing during sweep phase; otherwise finalizers
		 * could be called twice.
		 *
		 * Also, Collect() uses this to protect itself from recursive calls
		 * (from badly behaved finalizers).
		 *
		 * @access ReadWrite(request, exclusiveGC)
		 */
		bool collecting;
 
		/** @access Requires((request && m_lock) || exclusiveGC) */ 
		bool finalizedValue;

		/** @access Requires(exclusiveGC) */
		void AddToSmallEmptyBlockList(GCAlloc::GCBlock *b)
		{
			b->next = smallEmptyPageList;
			smallEmptyPageList = b;
		}

		/**
		 * List of pages to be swept, built up in Finalize.
		 * @access Requires(exclusiveGC)
		 */
		GCAlloc::GCBlock *smallEmptyPageList;
		
		/** @access Requires(exclusiveGC) */
		void AddToLargeEmptyBlockList(GCLargeAlloc::LargeBlock *lb)
		{
			lb->next = largeEmptyPageList;
			largeEmptyPageList = lb;
		}

		/**
		 * List of pages to be swept, built up in Finalize.
		 * @access Requires(exclusiveGC)
		 */
		GCLargeAlloc::LargeBlock *largeEmptyPageList;
		
#ifdef MMGC_LOCKING
		vmpi_spin_lock_t m_rootListLock;
#endif

		/** @access Requires(m_rootListLock) */
		GCRoot *m_roots;
		void AddRoot(GCRoot *root);
		void RemoveRoot(GCRoot *root);
		
#ifdef MMGC_THREADSAFE
		vmpi_spin_lock_t m_callbackListLock;
#endif

		/**
		 * Points to the head of a linked list of callback objects.
		 *
		 * @access Requires(m_callbackListLock)
		 */
		GCCallback *m_callbacks;
		void AddCallback(GCCallback *cb);
		void RemoveCallback(GCCallback *cb);

		// Deferred ref counting implementation
		ZCT zct;
		void AddToZCT(RCObject *obj) { zct.Add(obj); }
		// public for one hack from Interval.cpp - no one else should call
		// this out of the GC
public:
		void RemoveFromZCT(RCObject *obj) { zct.Remove(obj); }
private:

		static const void *Pointer(const void *p) { return (const void*)(((uintptr_t)p)&~7); }

public:
		void DumpMemoryInfo();
private:

#ifndef MMGC_THREADSAFE
		void CheckThread();
#endif

		void PushWorkItem(GCStack<GCWorkItem> &stack, GCWorkItem item);

#ifdef _DEBUG		
		void CheckFreelist(GCAlloc *gca);
		void CheckFreelists();


		int m_gcLastStackTrace;

		/**
		 * Used by FindUnmarkedPointers.
		 *
		 * @access Requires(exclusiveGC)
		 */
		void UnmarkedScan(const void *mem, size_t size);

		/**
		 * Find unmarked pointers in the entire heap.
		 *
		 * @access Requires(exclusiveGC)
		 */
		void FindUnmarkedPointers();

		// methods for incremental verification

		/**
		 * Scan a region of memory for white pointers. Used by
		 * FindMissingWriteBarriers.
		 *
		 * @access Requires(exclusiveGC)
		 */
		void WhitePointerScan(const void *mem, size_t size);

		/**
		 * Scan all GC memory (skipping roots). If a GC object is black make sure
		 * it has no pointers to white objects.
		 *
		 * @access Requires(exclusiveGC)
		 */
		void FindMissingWriteBarriers();
#ifdef WIN32
		// store a handle to the thread that create the GC to ensure thread safety
		DWORD m_gcThread;
#endif
#endif

public:
#ifdef MMGC_MEMORY_INFO
		typedef void (*pDumpBackCallbackProc)(void* pContext, void *obj, const char *type );
		void DumpBackPointerChain(void *o, pDumpBackCallbackProc p = NULL, void *context = NULL);

		// debugging routine that records who marked who, can be used to
		// answer the question, how did I get marked?  also could be used to
		// find false positives by verifying the back pointer chain gets back
		// to a GC root
		static void WriteBackPointer(const void *item, const void *container, size_t itemSize);
#endif
#ifdef _DEBUG
		// Dump a list of objects that have pointers to the given location.
		void WhosPointingAtMe(void* me, int recurseDepth=0, int currentDepth=0);

		/**
		 * Used by WhosPointingAtMe.
		 * @access Requires(pageMapLock)
		 */
    	void ProbeForMatch(const void *mem, size_t size, uintptr_t value, int recurseDepth, int currentDepth);
#endif

		int DumpAlloc(GCAlloc*);
		size_t GetBytesInUse();
		size_t GetNumBlocks() { return totalGCPages; }


		bool pendingStatusChange;
		MemoryStatus statusFrom, statusTo;		

#ifdef MMGC_THREADSAFE
	public:
		/**
		 * True if marking or sweeping is happening.  In an MMGC_THREADSAFE
		 * build, this implies that no threads in requests are running right
		 * now.
		 *
		 * Contrast Collecting().
		 *
		 * @access Requires(request || exclusiveGC || m_lock)
		 */
		bool IsGCRunning() const { return m_gcRunning; }

	protected:
		/** @access Requires(m_lock) */
		void OnEnterRequestAlreadyLocked()
		{
			WaitForGCDone();
			m_requestCount++;
#ifdef _DEBUG
			GCThread::GetCurrentThread()->OnEnterRequest();
#endif
		}

		/** @access Requires(m_lock) */
		void OnLeaveRequestAlreadyLocked()
		{
			GCAssert(m_requestCount > 0);
			m_requestCount--;
			if (m_requestCount == 0)
				m_condNoRequests.NotifyAll();
#ifdef _DEBUG
			GCThread::GetCurrentThread()->OnLeaveRequest();
#endif
		}

	public:
		/**
		 * Call this when the number of active requests on a thread goes from
		 * zero to one.
		 */
		void OnEnterRequest()
		{
			GCAutoLock _lock(m_lock);
			OnEnterRequestAlreadyLocked();
		}

		/**
		 * Call this when the number of active requests on a thread goes from 
		 * one to zero.
		 */
		void OnLeaveRequest()
		{
			GCAutoLock _lock(m_lock);
			OnLeaveRequestAlreadyLocked();
		}

		/**
		 * Exactly like Collect(), except that the caller must be inside a
		 * request.
		 *
		 * @access Requires(request && !m_lock)
		 */
		void CollectFromRequest()
		{
			CollectWithBookkeeping(false, true);
		}

	protected:
		/**
		 * Wait for the current GC operation, if any, to finish.
		 *
		 * @access Requires(!request && m_lock)
		 */
		void WaitForGCDone()
		{
			GCAssert(m_exclusiveGCThread != GCThread::GetCurrentThread());
			while (m_exclusiveGCThread != NULL)
				m_condDone.Wait();
		}

		/**
		 * This lock protects m_exclusiveGCThread and m_requestCount.
		 * Policies built on m_lock and m_exclusiveGCThread govern how threads
		 * access the rest of MMgc (both data and code).
		 *
		 * These policies are documented with "\@access" comments, which is a
		 * kind of shorthand.  Here are a few illustrative examples:
		 *
		 * <code>\@access Requires(m_lock)</code> on a member variable or
		 * method means it must be used only by a thread that holds m_lock.
		 *
		 * <code>\@access Requires(exclusiveGC)</code> means the member is to
		 * be used only by the GC thread, and only when no threads are running
		 * in requests.  This applies to methods like Trace(), MarkItem(),
		 * Sweep(), and Finalize().
		 *
		 * <code>\@access Requires(request)</code> means the member must be
		 * used only by a thread that is in a request.  This applies to
		 * Alloc(), Calloc(), and Free().
		 *
		 * <code>\@access Requires(exclusiveGC || request)</code> requires
		 * that the caller be <em>either</em> in a request <em>or</em> that it
		 * be the GC thread, with no threads running in requests.  (Of course
		 * a thread can never be both.)  This is just like
		 * <code>Requires(request)</code> except that a member marked this way
		 * is also safe to use from finalizer code.  (Finalizers run with
		 * <code>exclusiveGC</code>, not in a request.)
		 *
		 * <code>\@access ReadWrite(request, exclusiveGC)</code> applies to
		 * several data members.  This denotes a read-write locking scheme.
		 * Any thread with <code>request || exclusiveGC</code> can safely
		 * <em>read</em> these data members, but only a thread with
		 * <code>exclusiveGC</code> can <em>write</em> to them.
		 *
		 * Other policies are occasionally used.  For example, the list of
		 * GCRoots has its own spinlock, so the head of the list and each of
		 * the links has <code>@access(m_rootListLock)</code>.
		 *
		 * XXX TODO - worry about less-than-one-word fields (just bools, I
		 * think) that are packed with other fields that have different access
		 * policies!
		 *
		 * Some data structures, like the list of GCRoots, are protected by
		 * separate spinlocks.  The rule to avoid deadlock is: do not try to
		 * acquire m_lock if you already hold any spinlock.
		 */
		mutable GCLock m_lock;

		/**
		 * The thread currently doing GC-related work (or waiting to do
		 * GC-related work); or NULL if there is no such thread.
		 *
		 * @access Requires(m_lock)
		 */
		GCThread *m_exclusiveGCThread;

		/**
		 * True if a thread is doing GC-related work, having already ensured
		 * that no threads are in active requests.
		 *
		 * (The complicated thread-safety policy here makes reads lock-free.
		 * It also supports checking the status even if the currently running
		 * code isn't in a request at all; Collect() needs that.)
		 *
		 * @access ReadWrite(request || exclusiveGC || m_lock, exclusiveGC && m_lock)
		 */
		bool m_gcRunning;

		/**
		 * This is notified whenever m_exclusiveGCThread becomes NULL.
		 *
		 * @access Requires(m_lock)
		 */
		GCCondition m_condDone;

		/**
		 * The number of threads currently in active requests.
		 *
		 * @access Requires(m_lock)
		 */
		int m_requestCount;

		/**
		 * This is notified whenever m_requestCount becomes zero.
		 *
		 * At most one thread is ever waiting on this condition at a time; if
		 * a thread is waiting on it, that thread is `m_exclusiveGCThread` and
		 * `m_gcRunning` is false.
		 *
		 * @access Requires(m_lock)
		 */
		GCCondition m_condNoRequests;
#endif

		/* A portable replacement for alloca().
		 *
		 * Memory is allocated from the heap and not from the stack.  It is freed in 
		 * one of two ways: If the function returns normally then an auto_ptr like
		 * mechanism frees the memory.  If the function leaves by throwing an exception
		 * (or if one of its callees throws an exception) then the exception
		 * handling mechanism in Exception.{h,cpp} frees the memory by releasing 
		 * everything that is still allocated that was allocated since the exception
		 * handler was erected.
		 *
		 * The auto_ptr mechanism, based on the class MMgc::GC::AllocaAutoPtr, cannot be
		 * circumvented, as allocaPush() takes a reference to such an object as an argument.
		 *
		 * Typical usage:
		 *
		 *    MMgc::GC::AllocaAutoPtr _ptr;                      // by convention prefixed by "_"
		 *    int* ptr = (int*)core->allocaPush(_ptr, nbytes);  // by convention same name, no "_"
		 *
		 * In practice the VMPI_alloca() macro, defined in avmbuild.h, should be used so that
		 * real alloca() can be used on platforms where that makes sense.
		 *
		 * Benchmarks suggest that the performance differences from using this mechanism
		 * instead of real alloca() are slight to nonexistent, and that the heap allocation
		 * sometimes provides a performance improvement.
		 */
	public:
		struct AllocaStackSegment
		{
			void* start;				// first address; also, the RCRoot pointer
			void* limit;				// address past data
			void* top;					// address past live if this segment is not the top
			AllocaStackSegment* prev;	// segments further from the top
		};
		
		void allocaInit();
		void allocaShutdown();
		void allocaPopToSlow(void* top);
		void* allocaPushSlow(size_t nbytes);
		void pushAllocaSegment(size_t nbytes);
		void popAllocaSegment();

		AllocaStackSegment* top_segment;// segment at the stack top
		void* stacktop;					// current first free word in top_segment
#ifdef _DEBUG
		size_t stackdepth;				// useful to have for debugging
#endif
		
	public:
		/* See documentation above */
		
		class AllocaAutoPtr
		{
			friend class GC;
		public:
			AllocaAutoPtr() : unwindPtr(NULL), gc(NULL) {}  // initialization of 'gc' to pacify gcc
			~AllocaAutoPtr() { if (unwindPtr) gc->allocaPopTo(unwindPtr); }
		private:
			GC* gc;
			void* unwindPtr;
		};
				
		inline void* allocaTop() 
		{
			return stacktop;
		}
		
		inline void allocaPopTo(void* top)
		{ 
			if (top >= top_segment->start && top <= top_segment->limit)
				stacktop = top;
			else
				allocaPopToSlow(top);
		}
	
		inline void* allocaPush(size_t nbytes, AllocaAutoPtr& x) 
		{
			GCAssert(x.unwindPtr == NULL);
			x.gc = this;
			x.unwindPtr = stacktop;
			nbytes = (nbytes + 7) & ~7;
			if ((char*)stacktop + nbytes <= top_segment->limit) {
				stacktop = (char*)stacktop + nbytes;
				return x.unwindPtr;
			}
			return allocaPushSlow(nbytes);
		}
	};

	// helper class to wipe out vtable pointer of members for DRC
	class Cleaner
	{
	public:
		Cleaner() {}
		// don't let myself move between objects
		Cleaner& operator=(const Cleaner& /*rhs*/) { return *this; }
		void set(const void * _v, size_t _size) { this->v = (int*)_v; this->size = _size; }
		~Cleaner() { 
			if(v) 
				VMPI_memset(v, 0, size);
			v = 0; 
			size = 0;
		}
		int *v;
		size_t size;
	};

	inline GCWorkItem::GCWorkItem(const void *p, uint32_t s, bool isGCItem)
		: ptr(p)
		, _size(s | uint32_t(isGCItem))
	{
#ifdef _DEBUG
		if (IsGCItem()) {
			GCAssert(GC::GetGC(p)->FindBeginning(p) == p);
		}
#endif
	}
}

#endif /* __GC__ */
