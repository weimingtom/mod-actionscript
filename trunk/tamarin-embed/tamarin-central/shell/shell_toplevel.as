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
	class CTest //implements ITest
	{
		public function test() { }
	}

	// this class exists solely to test native classes that use MI.
	[native(cls="MIClass", instance="MIObject", methods="MIObjectImpl")]
	internal class MI
	{
		public native function plus(i:Number):Number;
	}

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

	// nonstandard extensions to ECMAScript
	/*public function print(...s)
	{
		System.trace(s)
	}

	// nonstandard Flash Player extensions
	public function trace(...s)
	{
		System.trace(s)
	}*/

	public function getTimer():uint
	{
		return System.getTimer()
	}

	public function readLine():String
	{
		return System.readLine()
	}
}


package modas.db {
	[native(cls="MySQLClass", methods="auto")]
	internal class MySQL
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


    	/**
    	 * Check if MySQL is connected
    	 * if disconnected, ping tries to reconnect
    	 * if connected or reconnected, returns true
    	 */
		public native static function ping():Boolean;
	}
}


package modas
{
	public namespace modas_internal = 'http://modas.ro/2009';

	public function out(str:String):void
	{
		HTTP.write(str);
	}

	public function outXHTML(xml:XML):void
	{
		XHTMLWriter.out(xml);
	}

	public function outXML(xml:XML):void
	{
		HTTP.write(xml.toXMLString());
	}

	public function textXML(s:String):XML
	{
		return XHTMLWriter.newTextNode(s);
	}

	[native(cls="HTTPClass", methods="auto")]
	public class HTTP
	{

		public native static function write(content:String):void;
		public native static function logError(msg:String):void;
		public native static function listEnv():Array;
		public native static function getEnv(key:String):String;
		public native static function getOutputStarted():Boolean;
		
		public static function get postedXML():XML
		{
			var xml:XML;
			try {
				xml = XML(rawPostData());
			} catch (e:Error)
			{
				throw new Error('Invalid posted xml data. ' + e.message);
			}
			return xml;
		}

		/**
		 * POST Form fields as an Object containing arrays of strings
		 */
		public static function get POST():Object
		{
		if (_POST) return _POST;
			var o:Object = Upload.getPOST();
			o.toString = function():String {return '[FormPOSTData]'};
			o.setPropertyIsEnumerable('toString', false);
			return _POST = o;
		}
		
		/**
		 * POST Form fields as an Object containing arrays of strings
		 */
		public static function get GET():Object
		{
			if (_GET) return _GET;
			var o:Object = Upload.getGET();
			o.toString = function():String {return '[FormGETData]'};
			o.setPropertyIsEnumerable('toString', false);
			return _GET = o;
		}
		
		public static function get rawPostData():String
		{
			if (_POSTDATA) return _POSTDATA;
			return _POSTDATA = Upload.rawPostData();
		}
		
		modas_internal static function cleanup():void
		{
			_GET = null;
			_POST = null;
			_POSTDATA = null;
		}
		
		//we cache these objects util the end of request
		static private var _GET:Object;
		static private var _POST:Object;
		static private var _POSTDATA:String;
	}
	

	[native(cls="HTTPFormClass", methods="auto")]
	public class Upload
	{
		internal native static function getGET():Array;
		internal native static function getPOST():Array;
		internal native static function rawPostData():String;
		
		public native static function listFileKeys():Array;
		public native static function getFilename(key:String):String;		
		public native static function isUploaded(key:String):Boolean;
		public native static function getFileSize(key:String):uint;
		public native static function saveFile(key:String, destPath:String):void;
	}

	[native(cls="XHTMLWriter", methods="auto")]
	public class XHTMLWriter
	{
		public native static function out(xml:XML):void;
		public native static function parse(xml:XML):String;
		public native static function newTextNode(s:String):XML;
		public native static function escapeXML(s:String):String;
		public native static function escapeXMLAttr(s:String):String;
	}
}

package {

	import modas.modas_internal;
	import modas.HTTP;
	public class __Cleanup
	{
		public static function clean():void
		{
			HTTP.modas_internal::cleanup();
		}
	}
}

