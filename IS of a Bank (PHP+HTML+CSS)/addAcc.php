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
<meta http-equiv="Content-Type" content="text/html;charset=UTF-16">
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
    <!-- content body -->
    <form action="" method="POST" class="regForms">
    <table>
    <tr><td>Prihlasovacie meno klienta:</td><td><input type="text" name="MK" value=<?php if(isset($_POST['MK'])) { echo $_POST['MK']; }?>></td></tr>
     <?php 
      if (empty($_POST['MK']) && isset($_POST['submit'])) {
      ?> <tr><td></td><td><font size="3" color="red">Prosim vyplnte pozadovany udaj</font></td></tr>
      <?php }  ?>
    </table>
    <fieldset id="rbutts">
      <legend>
        Typ účtu
      </legend>
        <input type="checkbox" name="stud" value="Studentsky" class="rb">
        Študentský
      <br/>
        <input type="checkbox" name="std" value="Standard" class="rb">
        Štandard
      <br/>
        <input type="checkbox" name="spor" value="Sporiaci" class="rb">
        Sporiaci
      <br/>
        <input type="checkbox" name="popre" value="Povolene precerpanie" class="rb">
        S povolením prečerpaním
      <br/>
    </fieldset>
    <input type="submit" name="submit" value="Odoslať" style="height: 30px; width: 150px">
   </form>
   </div>
  </div> 
<?php
#KONTROLA UDAJOV GENEROVANIE CISLA UCTU IBAN 
  function my_bcmod( $x, $y ) { // arbitrary precision number 
    $take = 5;     
    $mod = ''; 

    do { 
      $a = (int)$mod.substr( $x, 0, $take ); 
      $x = substr( $x, $take ); 
      $mod = $a % $y;    
    } while(strlen($x)); 

    return (int)$mod; 
  } 

  class IBANGenerator {
    protected $bankCode;
    protected $locale = 'CZ';
    protected $bankAccountNum;

    function __construct($bankCode, $bankAccountNum, $locale = 'CZ') {
      $this->locale = $locale;
      $this->bankCode = $bankCode;
      $this->bankAccountNum = $bankAccountNum;
    }

    public function generate($bankCode = '', $bankAccountNum = '', $locale = '') {
      if(empty($locale)) $locale = $this->locale;
      if(empty($bankCode)) $bankCode = $this->bankCode;
      if(empty($bankAccountNum)) $bankAccountNum = $this->bankAccountNum;

      $BBAN = $this->generateBBAN($bankCode, $bankAccountNum);
      $checksum = $this->generateCheckSum($bankCode, $bankAccountNum, $locale);
      // Modulo 97-10
      $checkcipher = $this->generateCheckNum($checksum);
      return $locale.$checkcipher.$BBAN;
    }

    public function generateCheckNum($checksum = '') {
      return str_pad(98 - my_bcmod($checksum, 97), 2, "0", STR_PAD_LEFT);
    }

    public function generateCheckSum($bankCode = '', $bankAccountNum = '', $locale = '') {
      if(empty($locale)) $locale = $this->locale;
      if(empty($bankCode)) $bankCode = $this->bankCode;
      if(empty($bankAccountNum)) $bankAccountNum = $this->bankAccountNum;
      return $this->generateBBAN($bankCode, $bankAccountNum).$this->generateNumericLanguageCode($locale);
    }

    public function generateBBAN($bankCode = '', $bankAccountNum = '') {
      if(empty($bankCode)) $bankCode = $this->bankCode;
      if(empty($bankAccountNum)) $bankAccountNum = $this->bankAccountNum;

      return $bankCode.str_pad($bankAccountNum, 10, "0", STR_PAD_LEFT);
    }

    public function generateNumericLanguageCode($locale = '') {
      if(empty($locale)) $locale = $this->locale;

      $alphabet = array(
      1 => 'A', 2 => 'B', 3 => 'C', 4 => 'D', 5 => 'E', 6 => 'F', 7 => 'G', 8 => 'H',
      9 => 'I', 10 => 'J', 11 => 'K', 12 => 'L', 13 => 'M', 14 => 'N', 15 => 'O',
      16 => 'P', 17 => 'Q', 18 => 'R', 19 => 'S', 20 => 'T', 21 => 'U', 22 => 'V',
      23 => 'W', 24 => 'X', 25 => 'Y', 26 => 'Z'
      );
      $numericLanguageCode = "";
      // step over each char from language code
      foreach(str_split($locale) as $char) {
      // use the number for latein alphabet to decode
      $numericLanguageCode .= array_search($char, $alphabet) + 9;
      }
      return $numericLanguageCode."00";
    }
  }

  $randomizer = rand(100000000, 999999999);
  $bankAccNum = "0000000".$randomizer;

  if ((!(empty($_POST["MK"])))) {
      $query = "SELECT * FROM klient WHERE PrihlasovacieMeno = '".mysql_real_escape_string($_POST['MK'])."'";
      $result = @mysql_query($query,$db);
      if(!($result))
       echo "<script type='text/javascript'>alert('Nesprávny klient!')</script>";
      else{
        $randomizer = rand(100000000, 999999999);
        $bankAccNum = "000000".$randomizer;
        $query = "SELECT * FROM ucet WHERE id_cislouctu = '".mysql_real_escape_string($bankAccNum)."'";
        $result = @mysql_query($query,$db);
        $resultData = mysql_fetch_array($result, MYSQL_ASSOC);
        while(!(empty($resultData))){
          $randomizer = rand(100000000, 999999999);
          $bankAccNum = "000000".$randomizer;
          $query = "SELECT * FROM ucet WHERE id_cislouctu = '".mysql_real_escape_string($bankAccNum)."'";
          $result = @mysql_query($query,$db);
          $resultData = mysql_fetch_array($result, MYSQL_ASSOC);
        } 
        $IBANGenerator = new IBANGenerator("0800", $bankAccNum, "CZ");
        $res = $IBANGenerator->generate();
        $count = 0;
        if(!empty($_POST["stud"])){
          $count = $count + 1;
          $query = "INSERT INTO ucet(`id_cislouctu`, `id_Klient`, `AktivovaneSluzby`, `IBAN`, `Mena`) VALUES ('".mysql_real_escape_string($bankAccNum)."','".mysql_real_escape_string($_POST["MK"])."', '".mysql_real_escape_string($_POST["stud"])."', '".mysql_real_escape_string($res)."','".mysql_real_escape_string($_POST["Mena"])."');"; 
        } if(!empty($_POST["std"])){
          $count = $count + 1;
          $query = "INSERT INTO ucet(`id_cislouctu`, `id_Klient`, `AktivovaneSluzby`, `IBAN`, `Mena`) VALUES ('".mysql_real_escape_string($bankAccNum)."','".mysql_real_escape_string($_POST["MK"])."', '".mysql_real_escape_string($_POST["std"])."', '".mysql_real_escape_string($res)."','".mysql_real_escape_string($_POST["Mena"])."');"; 
        }
        if(!empty($_POST["spor"])){
          $count = $count + 1;
          $query = "INSERT INTO ucet(`id_cislouctu`, `id_Klient`, `AktivovaneSluzby`, `IBAN`, `Mena`) VALUES ('".mysql_real_escape_string($bankAccNum)."','".mysql_real_escape_string($_POST["MK"])."', '".mysql_real_escape_string($_POST["spor"])."', '".mysql_real_escape_string($res)."','".mysql_real_escape_string($_POST["Mena"])."');"; 
        } 
        if(!empty($_POST["popre"])) {
          $count = $count + 1;
          $query = "INSERT INTO ucet(`id_cislouctu`, `id_Klient`, `AktivovaneSluzby`, `IBAN`, `Mena`) VALUES ('".mysql_real_escape_string($bankAccNum)."','".mysql_real_escape_string($_POST["MK"])."', '".mysql_real_escape_string($_POST["popre"])."', '".mysql_real_escape_string($res)."','".mysql_real_escape_string($_POST["Mena"])."');"; 
        } 
        if ($count != 1)
          echo "<script type='text/javascript'>alert('Nesprávny počet zvolených možností')</script>";
        else {
        $result = @mysql_query($query,$db);
        if(!($result))
          echo "<script type='text/javascript'>alert('Nesprávne údaje vkladanie sa nepodarilo!')</script>";
        else {
        $helper = 'Účet úspečne vytvorený ! Číslo účtu: ' . $bankAccNum . ' IBAN: ' . $res;
        echo "<script type='text/javascript'>alert('$helper'); window.location.href='addAcc.php'</script>";
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

