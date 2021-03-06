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

#ifndef __avmplus_CodeContext__
#define __avmplus_CodeContext__

namespace avmplus
{
	enum CodeContextKind 
	{
		CONTEXTKIND_ENV = 0,	// MethodEnv* pointer
		CONTEXTKIND_OBJECT = 1	// CodeContext* object
	};

	const CodeContextAtom CONTEXT_NONE   = 0; // No CodeContext present

	inline CodeContextKind getCodeContextKind(CodeContextAtom c) { return CodeContextKind(uintptr_t(c) & 7); }

	inline MethodEnv* getCodeContextEnv(CodeContextAtom c) { AvmAssert(getCodeContextKind(c) == CONTEXTKIND_ENV); return (MethodEnv*)(uintptr_t(c) & ~7); }
	inline CodeContext* getCodeContextObject(CodeContextAtom c) { AvmAssert(getCodeContextKind(c) == CONTEXTKIND_OBJECT); return (CodeContext*)(uintptr_t(c) & ~7); }

	inline CodeContextAtom makeCodeContextAtom(MethodEnv* e) { return CodeContextAtom(uintptr_t(e) | CONTEXTKIND_ENV); }
	inline CodeContextAtom makeCodeContextAtom(CodeContext* c) { return CodeContextAtom(uintptr_t(c) | CONTEXTKIND_OBJECT); }

	// CodeContext is used to track which security context we are in.
	// When an AS3 method is called, the AS3 method will set core->codeContext to its code context.
	class CodeContext : public MMgc::GCObject
	{
	public:		
		virtual ~CodeContext() {}
#ifdef DEBUGGER
		virtual DomainEnv* domainEnv() const = 0;
#endif
	};

	class EnterCodeContext
	{
	public:
		EnterCodeContext(AvmCore * _core, MethodEnv *env)
		{
			initialize(_core, makeCodeContextAtom(env));
		}
		EnterCodeContext(AvmCore * _core, CodeContext *codeContext)
		{
			initialize(_core, makeCodeContextAtom(codeContext));
		}
		~EnterCodeContext()
		{
			finish();
		}
		void finish()
		{
			core->codeContextAtom = savedCodeContextAtom;
		}
	private:
		void initialize(AvmCore * _core, CodeContextAtom atom)
		{
			this->core = _core;
			savedCodeContextAtom = _core->codeContextAtom;
			_core->codeContextAtom = atom;
		}
		CodeContextAtom savedCodeContextAtom;
		AvmCore *core;
	};
}

#endif /* __avmplus_CodeContext__ */
