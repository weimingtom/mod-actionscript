<?

echo 'Hello from PHP';


$k = 1;
for ($i=0; $i<10000000; $i++)
{
	$k *= 1.0001;
}
print($k);


?>
