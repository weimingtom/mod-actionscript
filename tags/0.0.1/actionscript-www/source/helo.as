include "includes.as"


HTML.simpleHeader();
HTML.print('<hr>Salut from ActionScript<br>\n');
HTML.print('utf-8 test: ö ŞşŢţ<hr>');


/*
//------------------------
// MySQL test
//------------------------
import db.MySQL;

//make sure you use the correct credentials. Failures will not issue any info in the error log
db.MySQL.connect('localhost', 'tamarin', 'root', '<pass>');


var res:Array;
res = MySQL.fetch('SELECT * FROM tbl1');

//table top
HTML.print('<table border><tr>');
for (var n:String in res[0])
	HTML.print('<td><b>' + res[0][n] + '</b></td>');
//table content
for (var i:uint=1; i<res.length; i++)
{
	HTML.print('<tr>');
	for (var n:String in res[i])
		HTML.print('<td>' + res[i][n] + '</td>');
}
HTML.print('</table>');
//------------------------
*/

//------------------------
//use this to output to apache error log
//trace('err out test')
//------------------------

/*
//------------------------
// Speed test
//------------------------
var t:Number = new Date().getTime();
var k:Number = 1;
for (var i:uint=0; i<10000000; i++)
{
	k*= 1.0001;
}
HTML.print(k);
HTML.print('<br>');
HTML.print(new Date().getTime() - t);
//------------------------
*/
