" * -*- Mode: vim; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
" * ***** BEGIN LICENSE BLOCK *****
" * Version: MPL 1.1/GPL 2.0/LGPL 2.1
" *
" * The contents of this file are subject to the Mozilla Public License Version
" * 1.1 (the "License"); you may not use this file except in compliance with
" * the License. You may obtain a copy of the License at
" * http://www.mozilla.org/MPL/
" *
" * Software distributed under the License is distributed on an "AS IS" basis,
" * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
" * for the specific language governing rights and limitations under the
" * License.
" *
" * The Original Code is [Open Source Virtual Machine.].
" *
" * The Initial Developer of the Original Code is
" * Adobe System Incorporated.
" * Portions created by the Initial Developer are Copyright (C) 2009
" * the Initial Developer. All Rights Reserved.
" *
" * Contributor(s):
" *   Adobe AS3 Team
" *
" * Alternatively, the contents of this file may be used under the terms of
" * either the GNU General Public License Version 2 or later (the "GPL"), or
" * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
" * in which case the provisions of the GPL or the LGPL are applicable instead
" * of those above. If you wish to allow use of your version of this file only
" * under the terms of either the GPL or the LGPL, and not to allow others to
" * use your version of this file under the terms of the MPL, indicate your
" * decision by deleting the provisions above and replace them with the notice
" * and other provisions required by the GPL or the LGPL. If you do not delete
" * the provisions above, a recipient may use your version of this file under
" * the terms of any one of the MPL, the GPL or the LGPL.
" *
" * ***** END LICENSE BLOCK *****/
" Vim syntax file
" Language:	Adobe ABC Assembler
" Maintainer:	Tom Harwood <tharwood@adobe.com>
" Last Change:	2009 Mar 3

" Usage: add the following (uncommented) line to filetype.vim, and
" put this syntax file in the syntax folder.

"au BufNewFile,BufRead *.abs			setf abcasm

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

syn case ignore

syn keyword asmDir .script_info
syn keyword asmDir .method_info
syn keyword asmDir .method_body_info
syn keyword asmDir .trait
syn keyword asmDir .exception

syn match asmLabel		"[a-z_][a-z0-9_]*:"he=e-1
syn match asmIdentifier		"[a-z_][a-z0-9_]*"

syn match decNumber		"0\+[1-7]\=[\t\n$,; ]"
syn match decNumber		"[1-9]\d*"
syn match hexNumber		"0[xX][0-9a-fA-F]\+"

syn match asmComment		"//.*"hs=s+1
syn region asmComment start=/\/\*/ end=/\*\//

" Define the default highlighting.
" For version 5.7 and earlier: only when not done already
" For version 5.8 and later: only when an item doesn't have highlighting yet
if version >= 508 || !exists("did_abcasm_syntax_inits")
  if version < 508
    let did_abcasm_syntax_inits = 1
    command -nargs=+ HiLink hi link <args>
  else
    command -nargs=+ HiLink hi def link <args>
  endif

  HiLink asmSection	Special
  HiLink asmLabel	Label
  HiLink asmComment	Comment
  HiLink asmDirective	Statement

  HiLink hexNumber	Number
  HiLink decNumber	Number
  HiLink octNumber	Number
  HiLink binNumber	Number

  HiLink asmSpecialComment Comment
  HiLink asmIdentifier Identifier
  HiLink asmType	Type

  delcommand HiLink
endif

let b:current_syntax = "abcasm"

