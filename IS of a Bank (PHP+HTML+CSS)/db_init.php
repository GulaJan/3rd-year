<?
$DB_HOST = "localhost:/var/run/mysql/mysql.sock";
$DB_USERNAME = "xnizne00";
$DB_PASSWORD = "pomvam7u";
$DB_NAME = "xnizne00";
$db=mysql_connect($DB_HOST,$DB_USERNAME,$DB_PASSWORD);
if (!mysql_select_db($DB_NAME,$db)) {
	echo "chyba pøi pøipojení k databázi.";
	exit();
}
?>