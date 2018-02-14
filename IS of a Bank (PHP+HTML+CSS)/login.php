<?php
include "db_init.php";
ini_set("default_charset", "UTF-8");
if (!isset($_SESSION))
session_start();
?>
<!DOCTYPE html>
<html lang="en" dir="ltr">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<title>Banka</title>
<link rel="stylesheet" href="styles/layout.css" type="text/css">
</head>
<body>
<div class="wrapper row1">
  <header id="header" class="clear">
    <div id="hgroup">
      <h1><a href="#">Banka Ebejam</a></h1>
      <h2>Elterngeld group</h2>
    </div>
    <form action="#" method="post">
    </form>
    <nav>
      <ul>
        <li><a href="index.php">Domov</a></li>
        <li><a href="accs.html">Účty</a></li>
        <li><a href="contact.html">Kontakt</a></li>
        <li><a href="login.php">Prihlásenie</a></li>
        <li><a href="reg.php">Registrácia</a></li>
        <li class="last"><a href="about.html">O nás</a></li>
      </ul>
    </nav>
  </header>
</div>
<!-- content -->
<div class="wrapper row2">
  <div id="container" class="clear">
    <!-- content body -->
	<form action="" method="POST">
	<table>
		<tr><td>Prihlasovacie meno:</td><td><input type="text" name="Meno" value = <?php if(isset($_POST['Meno'])) { echo $_POST['Meno']; }?>></td></tr>
		<?php 
      if (empty($_POST['Meno']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
		<tr><td>Prihlasovacie heslo:</td><td><input type="password" name="Heslo" ></td></tr>	
	</table>
	<fieldset id="rbutts">
		<legend>
			Typ uživateľa
		</legend>
			<input type="checkbox" name="Klient" value="Klient" class="rb">
			Klient
		<br/>
			<input type="checkbox" name="Zamestnanec" value="Zamestnanec" class="rb">
			Zamestnanec
		<br/>
			<input type="checkbox" name="Admin" value="Admin" class="rb">
			Admin
		<br/>
	</fieldset>
	<input type="submit" name="submit" value="Odoslať" style="height: 30px; width: 150px">
</form>
</div>
</div>

<?php
	if(((isset($_POST["Klient"])) && (isset($_POST["Zamestnanec"]))) || ((isset($_POST["Klient"])) && (isset($_POST["Admin"]))) || ((isset($_POST["Zamestnanec"])) && (isset($_POST["Admin"]))) || ((isset($_POST["Klient"])) && (isset($_POST["Zamestnanec"])) && (isset($_POST["Admin"])))) {
            echo "<script type='text/javascript'>alert('Zvoľte iba jednu možnosť!')</script>";
	} else if(!empty($_POST["Meno"]) && (!(empty($_POST["Heslo"]))) && (!empty($_POST["Klient"]))) {		
		$query = "SELECT PrihlasovacieHeslo FROM klient WHERE PrihlasovacieMeno = '".mysql_real_escape_string($_POST["Meno"])."'";
			$result = @mysql_query($query,$db);
			$resultData = mysql_fetch_array($result, MYSQL_ASSOC);
	    if(!($result)){
           echo "<script type='text/javascript'>alert('Tento klient neexistuje!')</script>";
		}else{
				if($resultData["PrihlasovacieHeslo"] == $_POST["Heslo"]){
					$_SESSION['Login'] = $_POST["Meno"];
				    echo "<script type='text/javascript'>alert('Prihlásenie úspešné')</script>";
					header("Location: succLog.php");
				} else
		            echo "<script type='text/javascript'>alert('Zlá kombinácia mena a hesla klienta!')</script>";
		}
	}
	else if(!empty($_POST["Meno"]) && (!(empty($_POST["Heslo"]))) && (!empty($_POST["Zamestnanec"]))) {		
		$query = "SELECT PrihlasovacieHeslo FROM zamestnanec WHERE PrihlasovacieMeno = '".mysql_real_escape_string($_POST["Meno"])."'";
			$result = @mysql_query($query,$db);
			$resultData = mysql_fetch_array($result, MYSQL_ASSOC);
	    if(!($result)){
           echo "<script type='text/javascript'>alert('Tento zamestnanec neexistuje!')</script>";
		}else{
			if($resultData["PrihlasovacieHeslo"] == $_POST["Heslo"]){
				echo "<script type='text/javascript'>alert('Prihlásenie úspešné')</script>";
				$_SESSION['Login'] = $_POST["Meno"];
				header("Location: employeeLog.php");
			}
			else
	           	echo "<script type='text/javascript'>alert('Zlá kombinácia mena a hesla zamestnanca!')</script>";
		}
	} else if(!empty($_POST["Meno"]) && (!(empty($_POST["Heslo"]))) && (!empty($_POST["Admin"]))) {		
		$query = "SELECT PrihlasovacieHeslo FROM admin WHERE PrihlasovacieMeno = '".mysql_real_escape_string($_POST["Meno"])."'";
			$result = @mysql_query($query,$db);
			$resultData = mysql_fetch_array($result, MYSQL_ASSOC);
	    if(!($result)){
            echo "<script type='text/javascript'>alert('Tento admin neexistuje!')</script>";
		}else{
				if($resultData["PrihlasovacieHeslo"] == $_POST["Heslo"]){
				    echo "<script type='text/javascript'>alert('Prihlásenie úspešné')</script>";
				    $_SESSION['Login'] = $_POST["Meno"];
					header("Location: adminLog.php");
				}
			else
 	           echo "<script type='text/javascript'>alert('Zlá kombinácia mena a hesla admina!')</script>";
		}
	}
?>
<!-- Footer -->
<div class="wrapper row3">
  <footer id="footer" class="clear">
    <p class="fl_left">Copyright &copy; 2017 - All Rights Reserved</p>
    <p class="fl_right">Created by Jan Gula</a></p>
  </footer>
</div>
</body>
</html>

