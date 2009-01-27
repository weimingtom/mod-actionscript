
import db.MySQL;
import http.*;

import avmplus.System;

http.simpleHeader();


http.out('Salut from AS3!<hr>');
//http.out('utf-8 test ö ŞşŢţ<br>');

http.out('raw post data='+http.rawPostData()+'<hr>');
http.out('Env values:<br><br>');
http.out(http.listEnv().join('<br>'));

//http.out(http.getEnv('SCRIPT_FILENAME'));


//------------------
//   Speed test
//------------------
/*class Cls1
{
	public var arr:Array;
	public function fn():Cls1
	{
		arr = [1];
		for (var i=1; i<1000; i++)
			arr[i] = arr[i-1] * 2.5;

		http.out(' x');
		return new Cls1;
	}
}

var o:Cls1 = new Cls1;
for (var i=0; i<1000; i++)
{
	o = o.fn();
}*/





//------------------
// Throw error test
//------------------
/*class Cls1
{
	static public function fn():void
	{
		var v:String = null;
		http.out(String(v.length));
	}
}
Cls1.fn();*/


//------------------
//   MySQL test
//------------------
/*import db.MySQL;

var ret:Boolean = db.MySQL.connect('localhost', 'tamarin', 'root', 'pass');
if (!ret)
	http.out("Can't connect to MySQL: " + MySQL.getError() + '<br>');


var res:Array;
res = MySQL.fetch('SELECT * FROM tbl1');
if (!res) 
	http.out("MySQL Error: " + MySQL.getError() + '<br>');
else
{
	for (var i:uint=0; i<res.length; i++)
	{
		http.out('--- row ' + i + '<br>');
		for (var n:String in res[i])
			http.out(n + ' => ' + res[i][n] + '<br>');
	}
}

var af = MySQL.exec("UPDATE tbl1 SET value=value+1 WHERE id=3");
http.out('affected rows: '+af+'<br>');
http.out('sql err: '+ MySQL.getError() + '<br>');

http.logTrace('trace test');
*/



//trace('err out test')

