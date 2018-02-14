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
<html>
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
    <tr><td>Číslo účtu:</td><td><input type="text" name="CU" value = <?php if(isset($_POST['CU'])) { echo $_POST['CU']; }?>></td></tr>
    <?php 
      if (empty($_POST['CU']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td>Prihlasovacie meno disponenta:</td><td><input type="text" name="MD" value = <?php if(isset($_POST['MD'])) { echo $_POST['MD']; }?>></td></tr>
    <?php 
      if (empty($_POST['MD']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td>Prihlasovacie meno klienta:</td><td><input type="text" name="MK" value = <?php if(isset($_POST['MK'])) { echo $_POST['MK']; }?>></td></tr>
    <?php 
      if (empty($_POST['MK']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    <tr><td><input type="submit" name="submit" value="Odoslať" style="height: 30px; width: 150px"></td></tr></td></tr>
    </table>
   </form>
   </div>
  </div>
</div>   
<?php
# + KONTROLA CI DISPONENT NENI AJ MAJITEL
if((($_POST["MK"]) == ($_POST["MD"])) && ((!empty($_POST["MK"])) && (!empty($_POST["MD"]))))
  echo "<script type='text/javascript'>alert('Klient nemôže byť zároveň disponentom!')</script>";
  if ((!(empty($_POST["CU"]))) && (!(empty($_POST["MK"]))) && (!(empty($_POST["MD"])))) {
    $query = "SELECT * FROM ucet WHERE id_cislouctu = '".mysql_real_escape_string($_POST['CU'])."'";
    $result = @mysql_query($query,$db);
    $resultData = mysql_fetch_array($result, MYSQL_ASSOC);
    if($resultData["id_Klient"] != $_POST["MK"])
     echo "<script type='text/javascript'>alert('Tento klient neni majiteľom zadaného účtu!')</script>";
    if(!($result))
      echo "<script type='text/javascript'>alert('Nesprávny účet!')</script>";
    else{
      $query = "SELECT * FROM klient WHERE PrihlasovacieMeno = '".mysql_real_escape_string($_POST['MK'])."'";
      $result = @mysql_query($query,$db);
      if(!($result))
        echo "<script type='text/javascript'>alert('Nesprávny klient!')</script>";
      else{
        $query = "SELECT * FROM klient WHERE PrihlasovacieMeno = '".mysql_real_escape_string($_POST['MD'])."'";
        $result = @mysql_query($query,$db);
        if(!($result))
          echo "<script type='text/javascript'>alert('Tento disponent neni klientom našej banky!')</script>";
        else{
          $query = "INSERT INTO disponent(`PrihlasovacieMeno`) VALUES ('".mysql_real_escape_string($_POST["MD"])."');";
          $result = @mysql_query($query,$db);
          if(!($result)){
            echo "<script type='text/javascript'>alert('Nesprávne údaje vkladanie sa nepodarilo!')</script>";
          }
          else {
            $query = "UPDATE ucet SET id_Disponent = '".mysql_real_escape_string($_POST["MD"])."' WHERE id_cislouctu = '".mysql_real_escape_string($_POST["CU"])."';";
            $result = @mysql_query($query,$db);             
            if(!($result)){
               echo "<script type='text/javascript'>alert('Nesprávne údaje vkladanie sa nepodarilo!')</script>";
            }
               echo "<script type='text/javascript'>alert('Disponent pridaný!')
               window.location.href='addManager.php';</script>";

          } 

        }  
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
