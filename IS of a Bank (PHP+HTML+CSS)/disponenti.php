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
$_SESSION['timeout']=time();
?>
<!DOCTYPE html>
<html lang="en" dir="ltr">
<head>
<style>
th, td {
    border: 1px solid #4169E1;
}
</style>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<title>Banka</title>
<link rel="stylesheet" href="styles/layout.css" type="text/css">
</head>
<body style="background-color:#333333;">
<div class="wrapper row1">
   <div id="logout">
   <?php 
  $log = $_SESSION['Login']; 
  $query = "SELECT * FROM klient WHERE PrihlasovacieMeno = '".mysql_real_escape_string($log)."'";
  $result = @mysql_query($query,$db);
  if (!($result))
    echo "fasa";
  $account = mysql_fetch_array($result, MYSQL_ASSOC)
  ?>
  <p style=" font-weight:bold;">Cas do odhlasenia : <span id = "timer" style="color:white;font-weight:normal;">00:04:59</span><br> Prihlaseny pouzivatel : <span id = "timer" style="color:white;font-weight:normal;"> <?php echo $account['MenoKlienta']; echo " ".$account['PriezviskoKlienta']." (".$account['PrihlasovacieMeno'].")";?></span></p> 
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
        <li><a href="succLog.php">Účty</a></li>
        <li><a href="vklady.php">Vklady</a></li>
        <li><a href="vybery.php">Výbery</a></li>
        <li><a href="prevody.php">Prevody</a></li>
        <li class="last"><a href="disponenti.php">Disponenti</a></li>
      </ul>
    </nav>
  </header>
</div>
<!-- content -->
<div class="wrapper row2tmp" style="height:640px;">
  <div id="container" class="clear">
     <?php 
  $log = $_SESSION['Login']; 
  $query = "SELECT * FROM ucet WHERE id_Klient = '".mysql_real_escape_string($log)."' AND id_Disponent IS NOT NULL";
  $result = @mysql_query($query,$db);
  if (!($result))
    echo "fasa";
  $rows = mysql_num_rows($result);
  if ($rows == 0) { ?>
    <table style="width:100%" id="accounts">
  <tr>
    <th> V momentalnej dobe nemate ziadnych disponentov </th>
  </tr>
  </table>
    
  <?php } 
  else { ?>
  <table style="width:100%" id="accounts">
  <tr>
    <th>Login disponenta</th>
    <th>Cislo uctu</th>
  </tr>
  <?php 
  while ($account = mysql_fetch_array($result, MYSQL_ASSOC)) {
    ?>
   <tr>
    <td> <?php echo $account['id_Disponent'];?> </td>
    <td> <?php echo $account['id_cislouctu'];?> </td>
   </tr> 
   
  
  <?php
} }
  ?>
</table>
    <!-- content body -->
    <!-- main content -->
  </div>
</div>
<!-- Footer -->
<div class="wrapper row3">
  <footer id="footer" class="clear">
    <p class="fl_left">Copyright &copy; 2017 - All Rights Reserved</p>
    <p class="fl_right">Created by Jan Gula</a></p>
  </footer>
</div>
</body>
</html>
