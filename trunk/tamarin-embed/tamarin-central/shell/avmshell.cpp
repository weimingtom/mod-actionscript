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

#include <iostream>
#include <stdlib.h>
using namespace std;


#include "avmshell.h"

#if defined(DARWIN) || (defined(AVMPLUS_UNIX) && !defined(SOLARIS))
#include <sys/signal.h>
#include <unistd.h>
#endif

/*

#if defined(SOLARIS)
#include <signal.h>
#include <unistd.h>
#include <ucontext.h>
extern "C" greg_t _getsp(void);
#endif

#ifdef __SUNPRO_CC
#define PRIVATE __hidden
#else
#define PRIVATE
#endif

#ifdef WIN32
#pragma warning(disable: 4201)

#include <Mmsystem.h>
#ifndef UNDER_CE
#include "dbghelp.h"
bool P4Available();
#endif
#elif defined AVMPLUS_UNIX
bool P4Available();
#endif

#ifdef UNDER_CE
	#define STRTOL10(x,y,z) wcstol((x),(y),(z))
#else
	#define STRTOL10(x,y,z) strtol((x),(y),(z))
#endif

#if defined(AVM_SHELL_PLATFORM_HOOKS)
    void AVMShellDidEndTest();
    void AVMShellDidTimeout();
    OutputStream *AVMShellNewConsoleStream(MMgc::GC *gc);
    int AVMShellPlatformMain(int argc, char **argv);
    void AVMShellPlatformTeardown();
    bool AVMShellShouldExitOnException(Exception *exception);
    void AVMShellWillBeginTest(const char *filename);
#endif

static MMgc::FixedMalloc* fm = NULL;

#ifndef OVERRIDE_GLOBAL_NEW
// Custom new and delete operators
// User-defined operator new.

PRIVATE void *operator new(size_t size)
{
	// 10.5 calls new before main
	if (!fm)
	{
		MMgc::GCHeap::Init();
		MMgc::FixedMalloc::Init();

		fm = MMgc::FixedMalloc::GetInstance();
	}

    return fm->Alloc(size);
}

PRIVATE void *operator new[](size_t size)
{
	// 10.5 calls new before main
	if (!fm)
	{
		MMgc::GCHeap::Init();
		MMgc::FixedMalloc::Init();

		fm = MMgc::FixedMalloc::GetInstance();
	}

    return fm->Alloc(size);
}

// User-defined operator delete.
#ifdef _MAC
	// CW9 wants the C++ official prototype, which means we must have an empty exceptions list for throw.
	// (The fact exceptions aren't on doesn't matter.) - mds, 02/05/04
	void operator delete( void *p) throw()
#else
PRIVATE	void operator delete( void *p)
#endif
	{
		if (fm)
			fm->Free(p);
	}

#ifdef _MAC
    // CW9 wants the C++ official prototype, which means we must have an empty exceptions list for throw.
    // (The fact exceptions aren't on doesn't matter.) - mds, 02/05/04
    void operator delete[]( void *p) throw()
#else
PRIVATE void operator delete[]( void *p )
#endif
    {
		if (fm)
			fm->Free(p);
    }
#endif // OVERRIDE_GLOBAL_NEW

*/

namespace avmplus {
	namespace NativeID {
		using namespace avmshell;
		#include "shell_toplevel.cpp"
	}
}

namespace avmshell
{
	const int kScriptTimeout = 15;
	const int kScriptGracePeriod = 5;

	Shell *shell = NULL;
	bool show_error = false;

	#ifdef WIN32
	void Shell::computeStackBase()
	{
#ifdef AVMPLUS_AMD64
		const int kStackMargin = 262144;
#elif defined(UNDER_CE)
		const int kStackMargin = 16384;
#else
		const int kStackMargin = 131072;
#endif

		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);

		int dummy;
		uintptr_t sp = (uintptr_t)(&dummy);
		sp &= ~uintptr_t(sysinfo.dwPageSize-1);

		MEMORY_BASIC_INFORMATION buf;
		if (VirtualQuery((void*)sp, &buf, sizeof(buf)) == sizeof(buf)) {
			minstack = (uintptr)buf.AllocationBase + kStackMargin;
		}
	}

	void CALLBACK TimeoutProc(UINT /*uTimerID*/,
							  UINT /*uMsg*/,
							  DWORD_PTR dwUser,
							  DWORD_PTR /*dw1*/,
							  DWORD_PTR /*dw2*/)
	{
		AvmCore *core = (AvmCore*)dwUser;
		core->interrupted = true;
	}
	#else
	void Shell::computeStackBase()
	{
		// A hard limit here is always wrong on every system.
		// https://bugzilla.mozilla.org/show_bug.cgi?id=456054

		const int kMaxAvmPlusStack = 4096*1024;  // changed to 4MB for testing purposes, used to be 512KB
		uintptr_t sp;
		#ifdef AVMPLUS_PPC
		asm("mr %0,r1" : "=r" (sp));
        #elif defined(AVMPLUS_ARM)
		asm("mov %0,sp" : "=r" (sp));
		#elif defined SOLARIS
		sp = _getsp();
		#else
		#ifdef AVMPLUS_64BIT
		asm("mov %%rsp,%0" : "=r" (sp));
		#else
		asm("movl %%esp,%0" : "=r" (sp));
		#endif
		#endif
		minstack = sp-kMaxAvmPlusStack;
	}

	void alarmProc(int /*signum*/)
	{
		shell->interrupted = true;
	}
	#endif

	ShellToplevel::ShellToplevel(VTable* vtable, ScriptObject* delegate)
		: Toplevel(vtable, delegate)
	{
		shellClasses = (ClassClosure**) core()->GetGC()->Calloc(avmplus::NativeID::shell_toplevel_abc_class_count, sizeof(ClassClosure*), MMgc::GC::kZero | MMgc::GC::kContainsPointers);
	}




#ifdef UNDER_CE
#define strcmp(_str, _conststr)		_tcscmp(_str, _T(_conststr))
#define strrchr(_str, _constchr)	_tcsrchr(_str, _T(_constchr))
#define strlen(_str)				_tcslen(_str)
#define strcpy(_str, _conststr)		_tcscpy(_str, _conststr)
#endif

	void Shell::stackOverflow(MethodEnv *env)
	{
		if (inStackOverflow)
		{
			// Already handling a stack overflow, so do not
			// re-enter handler.
			return;
		}

		// Temporarily disable stack overflow checks
		// so that we can construct an exception object.
		// There should be plenty of margin before the
		// actual stack bottom to do this.
		inStackOverflow = true;

		Toplevel *toplevel = env->vtable->toplevel;

		Stringp errorMessage = getErrorMessage(kStackOverflowError);
		Atom args[2] = { nullObjectAtom, errorMessage->atom() };
		Atom errorAtom = toplevel->errorClass()->construct(1, args);
		Exception *exception = new (GetGC()) Exception(errorAtom
                                                  #ifdef DEBUGGER
												  ,this
                                                  #endif
		);

		// Restore stack overflow checks
		inStackOverflow = false;

		// Throw the stack overflow exception
		throwException(exception);
	}

	void Shell::interrupt(MethodEnv *env)
	{
		interrupted = false;

		Toplevel *toplevel = env->vtable->toplevel;

		if (gracePeriod) {
			// This script has already had its chance; it violated
			// the grace period.
			// Throw an exception it cannot catch.
			Stringp errorMessage = getErrorMessage(kScriptTerminatedError);
			Atom args[2] = { nullObjectAtom, errorMessage->atom() };
			Atom errorAtom = toplevel->errorClass()->construct(1, args);
			Exception *exception = new (GetGC()) Exception(errorAtom
													  #ifdef DEBUGGER
													  ,this
													  #endif
			                                          );
			exception->flags |= Exception::EXIT_EXCEPTION;
			throwException(exception);
		}

		// Give the script an additional grace period to
		// clean up, and throw an exception.
		gracePeriod = true;

		#ifdef WIN32
		timeSetEvent(kScriptGracePeriod*1000,
					 kScriptGracePeriod*1000,
					 (LPTIMECALLBACK)TimeoutProc,
					 (DWORD_PTR)this,
					 TIME_ONESHOT);
		#else
		#ifndef AVMPLUS_ARM // TODO AVMPLUS_ARM
		alarm(kScriptGracePeriod);
		#endif
		#endif

        #if defined(AVM_SHELL_PLATFORM_HOOKS)
		    AVMShellDidTimeout();
		#endif

		toplevel->throwError(kScriptTimeoutError);
	}

	void Shell::initShellPool()
	{
		shellPool = AVM_INIT_BUILTIN_ABC(shell_toplevel, this, NULL);
	}

	Toplevel* Shell::initShellBuiltins()
	{
		// Initialize a new Toplevel.  This will also create a new
		// DomainEnv based on the builtinDomain.
		Toplevel* toplevel = initTopLevel();

		// Initialize the shell builtins in the new Toplevel
		handleActionPool(shellPool,
						 toplevel->domainEnv(),
						 toplevel,
						 NULL);

		return toplevel;
	}

	Toplevel* Shell::createToplevel(VTable *vtable)
	{
		return new (vtable->gc(), vtable->getExtraSize()) ShellToplevel(vtable, NULL);
	}

	size_t Shell::getToplevelSize() const
	{
		return sizeof(ShellToplevel);
	}

	Shell::Shell(MMgc::GC *gc) : AvmCore(gc)
	{
		#ifdef DEBUGGER
		debugCLI = NULL;
		#endif

		systemClass = NULL;

		gracePeriod = false;
		inStackOverflow = false;

        #if defined(AVM_SHELL_PLATFORM_HOOKS)
		    consoleOutputStream = AVMShellNewConsoleStream(gc);
		#else
			consoleOutputStream = new (gc) avmshell::ConsoleOutputStream();
		#endif

		setConsoleStream(consoleOutputStream);

		computeStackBase();
	}

	int Shell::start(const char * filename)
	{
		bool show_mem = false;
		AvmCore::CacheSizes cacheSizes;	// defaults to unlimited

		TRY(this, kCatchAction_ReportAsError)
		{
			//timeout
			//config.interrupts = true;

			//mir-nanojit
			//config.sse2 = false;

			//astrace
			//avmplus::Debugger::astrace_console = (avmplus::Debugger::TraceLevel) STRTOL10(argv[++i], 0, 10);

			//nanojit
			//config.runmode = RM_jit_all;

			//interpret all
			config.runmode = RM_interp_all;

			//memstats
			//GetGC()->gcstats = true;

			//memlimit
			//GetGC()->GetGCHeap()->SetHeapLimit(STRTOL10(argv[++i], 0, 10));

			//--------------------------
			setCacheSizes(cacheSizes);
			initBuiltinPool();
			initShellPool();

			// Create the debugger
			debugCLI = new (GetGC()) avmshell::DebugCLI(this);
			debugger = debugCLI;

			// Create the profiler
			profiler = new (GetGC()) avmshell::Profiler(this);

			bool do_debugger = true;
			if (do_debugger)
			{
				// Activate the debug CLI and stop at
				// start of program
				debugCLI->activate();
				debugCLI->stepInto();
			}

			// start the 15 second timeout if applicable
			if (config.interrupts) {
				signal(SIGALRM, alarmProc);
				alarm(15);
				//alarm(kScriptTimeout);
			}

			// init toplevel internally
			Toplevel* toplevel = initShellBuiltins();

			// Create a new Domain for the user code
			Domain* domain = new (GetGC()) Domain(this, builtinDomain);

			// Return a new DomainEnv for the user code
			DomainEnv* domainEnv = new (GetGC()) DomainEnv(this, domain, toplevel->domainEnv());

			ShellCodeContext* lastCodeContext = 0;

			//filename char*
			avmshell::FileInputStream f(filename);
			if (!f.valid()) {

				cerr << "Can't open file: " << filename;
				delete profiler;
				return(1);
			}

			ShellCodeContext* codeContext = new (GetGC()) ShellCodeContext();
			codeContext->m_domainEnv = domainEnv;

			// parse new bytecode
			ScriptBuffer code = newScriptBuffer(f.available());
			f.read(code.getBuffer(), f.available());
			handleActionBlock(code, 0, domainEnv, toplevel, NULL, codeContext);

			lastCodeContext = codeContext;

			delete profiler;

		}
		CATCH(Exception *exception)
		{
			if (!(exception->flags & Exception::SEEN_BY_DEBUGGER))
			{
				cerr << string(exception->atom) << "\n";
			}
			if (exception->getStackTrace()) {
				cerr << exception->getStackTrace()->format(this) << '\n';
			}
			delete profiler;
		}
		END_CATCH
		END_TRY

		if (show_mem)
		{
			MMgc::GCHeap* heap = GetGC()->GetGCHeap();
			size_t codeSize = heap->GetCodeMemorySize();
			console << (int)codeSize << " bytes used by the compiler\n";
		}

		return 0;
	}//end start

}//end avmshell
