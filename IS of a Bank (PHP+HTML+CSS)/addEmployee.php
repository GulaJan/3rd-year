<?php
include "db_init.php";
ini_set("default_charset", "UTF-8");
if (!isset($_SESSION))
session_start();
if (!isset($_SESSION['Login'])) {
  header("Location: index.php");
}
$inactive = 300; 
if (isset($_SESSION['timeout'])) {
$session_life = time() - $_SESSION['timeout'];

if($session_life > $inactive)
{  session_unset();
  session_destroy(); 
  header("Location: index.php");     
}
}
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
  <div id="logout">
   <?php 
  $log = $_SESSION['Login']; 
  $query = "SELECT * FROM admin WHERE PrihlasovacieMeno = '".mysql_real_escape_string($log)."'";
  $result = @mysql_query($query,$db);
  if (!($result))
    echo "fasa";
  $account = mysql_fetch_array($result, MYSQL_ASSOC)
  ?>
  <p style=" font-weight:bold;">Čas do odhlásenia : <span id = "timer" style="color:white;font-weight:normal;">00:04:59</span><br> Prihlásený použivateľ : <span id = "timer" style="color:white;font-weight:normal;"> <?php echo $account['PrihlasovacieMeno'];?></span></p> 
  <form method="POST"><input type="submit" name="logout" value="Odhlasit sa" style="height:20px; width: 90px; margin-left:5px;"></form>
  <?php if (isset($_POST['logout'])) {
    session_unset();
    session_destroy();
    header("Location: index.php"); 
  }
    ?>
    <script>
  var seconds = Math.floor(59);
  var minutes = Math.floor(4);
  var i = 0;
  var x = setInterval(function() {
    var sec = Math.floor(seconds);
     if (i != 0) {
      if (sec == 59)
      {
        minutes = Math.floor(minutes - 1);
        min = Math.floor(minutes);
      }
      else min = Math.floor(minutes);
    }
    else
    min = Math.floor(minutes);
    seconds = Math.floor(seconds-1);
    if (sec < 10) {
      document.getElementById("timer").innerHTML="00:0"+min +":0" + sec;
      
    }
    else
    document.getElementById("timer").innerHTML = "00:0"+min +":" + sec;;
    if (sec == 0)
      seconds = Math.floor(59);
    i = Math.floor(i + 1);
  },1000);
  </script>
  </div>
  <script type="text/javascript">
setTimeout(onUserInactivity, 1000 * 300)
function onUserInactivity() {
   window.location.href = "index.php"
}
</script> 
  <header id="header" class="clear">
    <div id="hgroup">
      <h1><a href="#">Banka Ebejam</a></h1>
      <h2>Elterngeld group</h2>
    </div>
    <form action="#" method="post">
    </form>
    <nav>
      <ul>
        <li class="last"><a href="addEmployee.php">Registrácia zamestnanca</a></li>
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
    <tr><td>Meno*:</td><td><input type="text" name="Meno" value=<?php if(isset($_POST['Meno'])) { echo $_POST['Meno']; }?>></td></tr>
     <?php 
      if (empty($_POST['Meno']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td>Priezvisko*:</td><td><input type="text" name="Priezvisko" value=<?php if(isset($_POST['Priezvisko'])) { echo $_POST['Priezvisko']; }?>></td></tr>
     <?php 
      if (empty($_POST['Priezvisko']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td>Adresa*:</td><td><input type="text" name="Adresa" value=<?php if(isset($_POST['Adresa'])) { echo $_POST['Adresa']; }?>></td></tr>
    <?php 
      if (empty($_POST['Adresa']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td><input type="submit" name="submit" value="Odoslať" style="height: 30px; width: 150px"></td></tr></td></tr>
    <legend>Povinné položky sú označené *</legend>
    </table>
   </form>
   </div>
  </div>
</div>
<?php
  session_unset();
  $full_set = 0;
 if (!(empty($_POST["Login"])) && !(empty($_POST["password"])) && !(empty($_POST["Meno"])) && !(empty($_POST["Priezvisko"])) && !(empty($_POST["Adresa"])))
    { $query = "SELECT * FROM zamestnanec WHERE PrihlasovacieMeno = '".mysql_real_escape_string($_POST['Meno'])."'";
        $result = @mysql_query($query,$db);
        $resultData = mysql_fetch_array($result, MYSQL_ASSOC);
        if((!empty($resultData)))

         echo "<script type='text/javascript'>alert('Nesprávny zamestnanec!')</script>";
        else{
      $query = "INSERT INTO zamestnanec (`MenoZamestnanca`, `PriezviskoZamestnanca`, `AdresaZamestnanca`,`id_Pob`,`PrihlasovacieMeno`,`PrihlasovacieHeslo`) VALUES ('".mysql_real_escape_string($_POST["Meno"])."','".mysql_real_escape_string($_POST["Priezvisko"])."','".mysql_real_escape_string($_POST["Adresa"])."','".mysql_real_escape_string(001)."','".mysql_real_escape_string($_POST["Login"])."','".mysql_real_escape_string($_POST["password"])."');";
       $result = @mysql_query($query,$db);
       if(!($result))
          echo "<script type='text/javascript'>alert('Nesprávne vyplnené údaje o zamestnancovi, pridávanie zlyhalo!')</script>";
        else 
              echo "<script type='text/javascript'>alert('Registrácia zamestnanca bola úspešná.');
                    window.location.href='addEmployee.php';</script>";
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
