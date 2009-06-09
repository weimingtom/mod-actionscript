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

package avmplus 
{
    import flash.utils.ByteArray

	// this namespace exists solely so that nativegen.py will encounter them during shell building,
	// thus giving us a test case for parsing methods with custom namespaces in nativegen.py
	namespace ns_example = "http://www.example.com"

	// this interface exists solely so that nativegen.py will encounter them during shell building,
	// thus giving us a test case for interface parsing in nativegen.py
	interface ITest
	{
		function test();
	}
	/*class CTest //implements ITest
	{
		public function test() { }
	}*/

	[native(cls="SystemClass", methods="auto")]
	public class System
	{
		// this metadata exists solely so that nativegen.py will encounter it during shell building,
		// thus giving us a test case for metadata parsing in nativegen.py
		[foo]
		public native static function exit(status:int):void

		// this metadata exists solely so that nativegen.py will encounter it during shell building,
		// thus giving us a test case for metadata parsing in nativegen.py
		[bar(attr="whatever")]
		public native static function exec(command:String):int

		public native static function getAvmplusVersion():String
		public native static function trace(a:Array):void
		public native static function write(s:String):void
		public native static function debugger():void
		public native static function isDebugger():Boolean
		public native static function getTimer():uint
		private native static function getArgv():Array
		public static const argv:Array = getArgv();
		public native static function readLine():String;
		public native static function get totalMemory():Number;
		public native static function get freeMemory():Number;
		public native static function get privateMemory():Number;

		// function exists solely to test native-methods with custom namespaces
		ns_example native static function nstest():void;

		// function exists solely to test ScriptObject::isGlobalObject
		public native static function isGlobal(o:Object):Boolean;
	}	
	
	[native(cls="FileClass", methods="auto")]
	public class File
	{
		public native static function exists(filename:String):Boolean;
		public native static function read(filename:String):String;
		public native static function write(filename:String, data:String):void;

		public static function fileToByteArray(filename:String, readOnly:Boolean):ByteArray
		{
			return ByteArray.readFile(filename)
		}

		public static function writeByteArray(filename:String, bytes:ByteArray):Boolean
		{
			bytes.writeFile(filename)
			return true
		}
	}

	public function debugger()
	{
		System.debugger()
	}
}

// The flash.system package is present so identical ATS test media can be used
// in the command-line VM and the Player
package flash.system
{
	import avmplus.*;
	
	public final class Capabilities
	{
		public static function get playerType():String { return "AVMPlus"; }
		public static function get isDebugger():Boolean { return System.isDebugger(); }
	}
}
		
package {

	import avmplus.*

	public function getClassByName(name:String):Class
	{
		return Domain.currentDomain.getClass(name);
	}
		
	/*// nonstandard extensions to ECMAScript
	public function print(...s)
	{
		System.trace(s)
	}*/

	/*
	// nonstandard Flash Player extensions
	public function trace(...s)
	{
		System.trace(s)
	}*/

	public function getTimer():uint
	{
		return System.getTimer()
	}

	/*public function readLine():String
	{
		return System.readLine()
	}*/
}

package db {
	[native(cls="MySQLClass", methods="auto")]
	public class MySQL
	{
		/**
		 * Performs a real connect to the mysql database
		 * No connection pool is implemented at the moment, each script invocation creates a new connection :(
		 * The connection handler is private and global (implicit). Concurent database connections are not possible.
		 * The charset is always UTF-8
		 * @return true only if connection was successfull;
		 */
		public native static function connect(host:String, db:String, user:String, pass:String):Boolean;

		/**
		 * Fetch a query result as a two dimensional Array. The first row of the array contains the column names.
		 * The following rows contains the data returned by the query.
		 * Values are only indexed numerically. To match column names to fields, use the first row.
		 * If an error ocurred, it returns null. Use getError() to see what went wrong.
		 */
		public native static function fetch(sql:String):Array;

		/**
		 * Execute query (insert/update/delete).
		 * @return affected rows or -1 if there was an error.
		 */
		public native static function exec(sql:String):int;

		/**
		 * Escape special chars for safely concatenate query values. Does not add quotes.
		 * Internaly, it uses mysql_real_escape_string
		 */
		public native static function escape(str:String):String;


		/**
		 * Get last mysql error.
		 */
		public native static function getError():String;
	}
}


package http
{
	import avmplus.System;
	public function out(s:String):void
	{
		_HTTP.write(s);
	}
	
	public function logTrace(s:String):void
	{
		_HTTP.logError(s);
	}

	public function simpleHeader():void
	{
		_HTTP.write('Content-type: text/html; charset=utf-8\r\n\r\n');
	}
	
	public function rawPostData():String
	{
		return _HTTP.postData();
	}
	
	public function listEnv():Array
	{
		return _HTTP.listEnv();
	}
	
	public function getEnv(key:String):String
	{
		return _HTTP.getEnv(key);
	}
	
	[native(cls="HTTPClass", methods="auto")]
	class _HTTP
	{
		public native static function write(content:String):void;
		public native static function logError(msg:String):void;
		
		public native static function postData():String;
		public native static function listEnv():Array;
		public native static function getEnv(key:String):String;
	}

	[native(cls="HTTPFormClass", methods="auto")]
	public class _HTTPForm
	{
		public native static function getFields():Array;
		public native static function listFileKeys():Array;
		public native static function isUploaded(key:String):Boolean;
		public native static function saveFile(key:String, destPath:String):Boolean;
	}
}












