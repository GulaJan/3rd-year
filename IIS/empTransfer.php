<?php
ini_set("default_charset", "UTF-8");
include "db_init.php";
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
  $query = "SELECT * FROM zamestnanec WHERE PrihlasovacieMeno = '".mysql_real_escape_string($log)."'";
  $result = @mysql_query($query,$db);
  if (!($result))
    echo "fasa";
  $account = mysql_fetch_array($result, MYSQL_ASSOC)
  ?>
  <p style=" font-weight:bold;">Čas do odhlásenia : <span id = "timer" style="color:white;font-weight:normal;">00:04:59</span><br> Prihlásený použivateľ : <span id = "timer" style="color:white;font-weight:normal;"> <?php echo $account['MenoZamestnanca']; echo " ".$account['PriezviskoZamestnanca']." (".$account['PrihlasovacieMeno'].")";?></span></p> 
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
        <li><a href="empTransfer.php">Prevod</a></li>
        <li><a href="empSelect.php">Výber</a></li>
        <li><a href="empInsert.php">Vklad</a></li>
        <li><a href="addManager.php">Pridať disponenta</a></li>
        <li><a href="addAcc.php">Pridať účet</a></li>
        <li class="last"><a href="addCard.php">Pridať kartu k účtu</a></li>
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
    <tr><td>Číslo účtu odosielateľa:</td><td><input type="text" name="CUO" value = <?php if(isset($_POST['CUO'])) { echo $_POST['CUO']; }?>></td></tr>
    <?php 
      if (empty($_POST['CUO']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td>Prihlasovacie meno žiadateľa:</td><td><input type="text" name="MenoV" value = <?php if(isset($_POST['MenoV'])) { echo $_POST['MenoV']; }?>></td></tr>
    <?php 
      if (empty($_POST['MenoV']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td>Číslo účtu príjemcu:</td><td><input type="text" name="CUP" value = <?php if(isset($_POST['CUP'])) { echo $_POST['CUP']; }?>></td></tr>
    <?php 
      if (empty($_POST['CUP']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td>Prevádzaná čiastka:</td><td><input type="text" name="PC" value = <?php if(isset($_POST['PC'])) { echo $_POST['PC']; }?>></td></tr>
    <?php 
      if (empty($_POST['PC']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td>Prevádzaná mena:</td><td><input type="text" placeholder="EUR CZK USD" name="PM" ></td></tr>
    <tr><td><input type="submit" name="submit" value="Odoslať" style="height: 30px; width: 150px"></td></tr></td></tr>
    </table>
   </form>
   </div>
  </div>
</div>
<?php
 if ((!(empty($_POST["CUO"]))) && (!(empty($_POST["MenoV"]))) && (!(empty($_POST["CUP"]))) && (!(empty($_POST["PC"]))) && (!(empty($_POST["PM"])))) { 
    if($_POST["CUO"] == $_POST["CUP"])
     echo "<script type='text/javascript'>alert('Čísla účtov sa zhodujú! Nemožno previesť prevod v rámci jedného účtu!')</script>";
    else {

      $query = "SELECT * FROM klient WHERE PrihlasovacieMeno = '".mysql_real_escape_string($_POST['MenoV'])."'";
      $result = @mysql_query($query,$db);
      $resultData = mysql_fetch_array($result, MYSQL_ASSOC);
      $query2 = "SELECT * FROM disponent WHERE PrihlasovacieMeno = '".mysql_real_escape_string($_POST['MenoV'])."'";
      $result2 = @mysql_query($query,$db);
      if((!($result)) && !($result2)){
       echo "<script type='text/javascript'>alert('Neexistujúci klient/disponent!')</script>";
      }
      else {
        $query = "SELECT * FROM ucet WHERE id_cislouctu = '".mysql_real_escape_string($_POST['CUP'])."'";
        $result = @mysql_query($query,$db);
        $resultData = mysql_fetch_array($result, MYSQL_ASSOC);
        if((!($result)) || (!($resultData))){
           echo "<script type='text/javascript'>alert('Zlý účet príjemcu!')</script>";
        }
        else {
          $query = "SELECT * FROM ucet WHERE id_cislouctu = '".mysql_real_escape_string($_POST['CUO'])."'";
          $result = @mysql_query($query,$db);
          $resultData = mysql_fetch_array($result, MYSQL_ASSOC);
          if((!($result)) || (($resultData["uctovny_zostatok"] - $_POST["PC"]) < 0)){
           echo "<script type='text/javascript'>alert('Zlý účet odosielateľa alebo nedostatok financií na účte!')</script>";
          }
          else {    
            $date = date('Y-m-d');      
            $resultData = mysql_fetch_array($result, MYSQL_ASSOC);
            $query = "INSERT INTO operace(`MenoVykonavatela`, `CisloUctuPrijemcu`, `PrevadzanaCiastka`,`PrevadzanaMena` ,`id_Klient`, `id_ucet`,`DatumVykonania`) VALUES ('".mysql_real_escape_string($_POST["MenoV"])."','".mysql_real_escape_string($_POST["CUP"])."','".mysql_real_escape_string($_POST["PC"])."','".mysql_real_escape_string($_POST["PM"])."','".mysql_real_escape_string($_POST["MenoV"])."', '".mysql_real_escape_string($_POST["CUO"])."','".mysql_real_escape_string($date)."');";
            $result = @mysql_query($query,$db);
            if(!($result)){
               echo "<script type='text/javascript'>alert('Nesprávne vyplnené údaje vkladanie sa nepodarilo!')</script>";
            }
            else {
              if($_POST["PM"] == "CZK")
                $_POST["PC"] = $_POST["PC"]/25.6208;
              if($_POST["PM"] == "USD")
                $_POST["PC"] = $_POST["PC"]/22.0692;            
              $query = "UPDATE ucet SET uctovny_zostatok = uctovny_zostatok - '".mysql_real_escape_string($_POST["PC"])."' WHERE id_cislouctu = '".mysql_real_escape_string($_POST["CUO"])."';";
              $result = @mysql_query($query,$db);             
              if(!($result))
                 echo "<script type='text/javascript'>alert('Nesprávne údaje vkladanie sa nepodarilo!')</script>";
              $query = "UPDATE ucet SET uctovny_zostatok = uctovny_zostatok + '".mysql_real_escape_string($_POST["PC"])."' WHERE id_cislouctu = '".mysql_real_escape_string($_POST["CUP"])."';";
              $result = @mysql_query($query,$db);             
              if(!($result))
                 echo "<script type='text/javascript'>alert('Nesprávne údaje vkladanie sa nepodarilo!')</script>";
              else {
                 echo "<script type='text/javascript'>alert('Prevod bol úspešný.');window.location.href='empTransfer.php';</script>";

              }
            }
          }
        }
      }
    }
  } else {

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
