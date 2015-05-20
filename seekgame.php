<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>Поиск партий
</title>
</head>
<body>
<h1>Поиск партий</h1>
<?
  include('sqlconnect.php');
  $link=mysqli_connect($sqlserver,$sqluser,$sqlpassword,$dbname);
  mysqli_set_charset($link,'utf8');
  if(isset($_REQUEST['variant']))
    $variant=mysqli_real_escape_string($link,$_REQUEST['variant']);
  else exit;
  echo "Вариант: $variant<br>\n";
  $len=strlen($variant);
  $cond="where left(content,$len)='$variant'";
  if(isset($_REQUEST['result']))
    $q.=" and result='".mysqli_real_escape_string($link,$_REQUEST['result'])."'";
  if(isset($_REQUEST['welo']))
    $cond.=" and whiteelo=".mysqli_real_escape_string($link,$_REQUEST['welo']);
  if(isset($_REQUEST['belo']))
    $cond.=" and blackelo=".mysqli_real_escape_string($link,$_REQUEST['belo']);
  $q="select content,white,black,whiteelo,blackelo,gamedate,result from twic $cond union distinct select content,white,black,whiteelo,blackelo,gamedate,result from games $cond order by (whiteelo+blackelo) desc limit 20";
  $r=mysqli_query($link,$q);
  for($i=0; $i<mysqli_num_rows($r); ++$i) {
    $f=mysqli_fetch_array($r);
    echo "<p>$f[white]($f[whiteelo]) - $f[black]($f[blackelo]), $f[gamedate], $f[result]<br>$f[content]</p>\n";
  }
?>
</body>
</html>