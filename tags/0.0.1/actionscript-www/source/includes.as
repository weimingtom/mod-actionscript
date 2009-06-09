import avmplus.System;
import flash.utils.ByteArray

function readUTF8File(file:String):String
{
	var data:ByteArray = ByteArray.readFile(file);
	return data.readUTFBytes(data.length-1);
}
class HTML
{
	public static function print(s:String):void
	{
		System.write(s);
	}

	public static function simpleHeader():void
	{
		System.write('Content-type: text/html; charset=utf-8\r\n\r\n');
	}
}
