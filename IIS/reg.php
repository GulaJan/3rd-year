<?php
include "db_init.php";
ini_set("default_charset", "UTF-8");
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
    <div id="forms">
    <!-- content body -->
    <form action="" method="POST" class="regForms">
    <table>
    <tr><td>Prihlasovacie meno*:</td><td><input type="text" name="Login" value=<?php if(isset($_POST['Login'])) { echo $_POST['Login']; }?>></td></tr>
    <?php 
      if (empty($_POST['Login']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>

    <tr><td>Heslo*:</td><td><input type="password" name="password" ></td></tr>
    <tr><td>Meno*:</td><td><input type="text" name="Meno" value=<?php if(isset($_POST['Meno'])) { echo $_POST['Meno']; } ?>></td></tr>
     <?php 
      if (isset($_POST['submit']) && empty($_POST['Meno'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td>Priezvisko*:</td><td><input type="text" name="Priezvisko" value=<?php if(isset($_POST['Priezvisko'])) { echo $_POST['Priezvisko']; } ?>></td></tr>
     <?php 
      if (isset($_POST['submit']) && empty($_POST['Priezvisko'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td>Adresa*:</td><td><input type="text" name="Adresa" value=<?php if(isset($_POST['Adresa'])) { echo $_POST['Adresa']; } ?>></td></tr>
     <?php 
      if (isset($_POST['submit']) && empty($_POST['Adresa'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td>Telefón:</td><td><input type="text" name="Telefon" placeholder="+420 XXX XXX XXX" value=<?php if(isset($_POST['Telefon'])) { echo $_POST['Telefon']; } ?>></td></tr>
    <tr><td>E-mail:</td><td><input type="text" name="Email" placeholder="example@example.com" value=<?php if(isset($_POST['Email'])) { echo $_POST['Email']; } ?>></td></tr>
    <tr><td><input type="submit" name="submit" value="Odoslať" style="height: 30px; width: 150px"></td></tr></td></tr>
    <legend>Povinné položky sú označené *</legend>
    </table>
   </form>
   </div>
  </div>
</div>
<?php
//  session_unset();
  $full_set = 0;

 if (!(empty($_POST["Login"])) && !(empty($_POST["password"])) && !(empty($_POST["Meno"])) && !(empty($_POST["Priezvisko"])) && !(empty($_POST["Adresa"])))
    { 
        $pom = 0;
        if (!(empty($_POST["Email"]))){
          if(!(preg_match("/.*[@].*[.].*/", $_POST["Email"]))) {
            echo "<script type='text/javascript'>alert('Nesprávny formát e-mailu!')</script>";
            $pom = 1;
          }
        }

        if (!(empty($_POST["Telefon"]))){
          $string = preg_replace('/\s+/', '', $_POST["Telefon"]);
          if(!(preg_match("/[+][4][2][0][1-9,\s]*/", $_POST["Telefon"]))) {
            echo "<script type='text/javascript'>alert('Nesprávny formát telefónneho čísla!')</script>";
            $pom = 1;
          }
          else if((strlen($string)) != 13) {
            echo "<script type='text/javascript'>alert('Nesprávny formát telefónneho čísla!')</script>";
            $pom = 1;
          }
        }

      if ($pom == 0) {
          $query = "INSERT INTO klient (`MenoKlienta`, `PriezviskoKlienta`, `AdresaKlienta`, `Telefon`, `Email`,`PrihlasovacieMeno`,`PrihlasovacieHeslo`) VALUES ('".mysql_real_escape_string($_POST["Meno"])."','".mysql_real_escape_string($_POST["Priezvisko"])."','".mysql_real_escape_string($_POST["Adresa"])."','".mysql_real_escape_string($_POST["Telefon"])."','".mysql_real_escape_string($_POST["Email"])."','".mysql_real_escape_string($_POST["Login"])."','".mysql_real_escape_string($_POST["password"])."');";
            $result = @mysql_query($query,$db);
            if(!($result))
              echo "<script type='text/javascript'>alert('Nesprávne vyplnené údaje!'/zadany klient uz existuje)</script>";
            else {
              echo "<script type='text/javascript'>alert('Prihlásenie úspešné');
                    window.location.href='index.php';</script>";
              
            }

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
