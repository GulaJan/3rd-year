<?php
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
      <h2>Weltergeld group</h2>
    </div>
    <form action="#" method="post">
    </form>
    <nav>
      <ul>
        <li><a href="succLog.php">Ucty</a></li>
        <li><a href="vklady.php">Vklady</a></li>
        <li><a href="vybery.php">Vybery</a></li>
        <li><a href="prevody.php">Prevody</a></li>
        <li><a href="vypis.php">Vypis</a></li>
        <li class="last"><a href="disponenti.php">Disponenti</a></li>
      </ul>
    </nav>
  </header>
</div>
<!-- content -->
<div class="wrapper row2tmp" style="height:640px;">
  <div id="container" class="clear">
    <div style="margin-left:400px;">
    <p> Mesiac       Rok</p>
 <form method="post">
  <select name="month" method="POST">
    <option value = "1"> 01 </option>
    <option value = "2"> 02 </option>
    <option value = "3"> 03 </option>
    <option value = "4"> 04 </option>
    <option value = "5"> 05 </option>
    <option value = "6"> 06 </option>
    <option value = "7"> 07 </option>
    <option value = "8"> 08 </option>
    <option value = "9"> 09 </option>
    <option value = "10"> 10 </option>
    <option value = "11"> 11 </option>
    <option value = "12"> 12</option>
  </select>
  <select name="year" method="POST">
    <option value = "2017"> 2017 </option>
    <option value = "2016"> 2016 </option>
    <option value = "2015"> 2015 </option>
    <option value = "2014"> 2014 </option>
    <option value = "2013"> 2013 </option>
    <option value = "2012"> 2012</option>
    <option value = "2011"> 2011</option>
    <option value = "2010"> 2010</option>
    <option value = "2009"> 2009</option>
    <option value = "2008"> 2008</option>
    <option value = "2007"> 2007</option>
    <option value = "2006"> 2006</option>
    <option value = "2005"> 2005</option>
    <option value = "2004"> 2004</option>
    <option value = "2003"> 2003</option>
    <option value = "2002"> 2002</option>
    <option value = "2001"> 2001</option>
    <option value = "2000"> 2000</option>
  </select>
  <br>
  <input type="submit" value="Potvrdit"/>
</form>
<?php if (isset($_POST['year']))
  $_SESSION['year'] = $_POST['year'];
  if (isset($_POST['month'])) {
    $_SESSION['month'] = $_POST['month'];
    header("Location: obdobie.php");
  }
 ?>   
</div>
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