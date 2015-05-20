<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>Справочник дебютов
<?
  include('sqlconnect.php');
  $link=mysqli_connect($sqlserver,$sqluser,$sqlpassword,$dbname);
  mysqli_set_charset($link,'utf8');
  if(isset($_GET['variant'])) {
    $variant=mysqli_real_escape_string($link,trim($_GET['variant'])).' ';
    echo " - $variant";
    if(isset($_GET['tomove']))
      $tomove=$_GET['tomove'];
  }
  else {
    $variant='';
    $tomove=1;
  }
?>
</title>
</head>
<body>
<h1>Справочник дебютов</h1>
<form acttion='' id='mainform' mathod='get'>
<input type='text' name='variant' size=40>
<input type='submit' name='send' value='Поиск'>
<br>
</form>
<p>Вариант: <?
  echo $variant;
?></p>
<table border=1>
<tr><th rowspan=2>Ход</th><th rowspan=2>Количество партий</th><th colspan=3>вероятности результатов</th><th colspan=2>Максимальный рейтинг</th></tr>
<tr><th>1-0</th><th>1/2-1/2</th><th>0-1</th><th>белых</th><th>черных</th></tr>
<?
  mysqli_multi_query($link,"call statvar('$variant')");
  $r=mysqli_store_result($link);
  $sumwim=$sumlose=$total=$maxwelo=$maxbelo=0;
  for($i=0; $i<mysqli_num_rows($r); ++$i) {
    $f=mysqli_fetch_array($r);
    $all=$f['cnt'];
    $probwin=round($f['pwin']*100);
    $probdraw=round((1.-$f['pwin']-$f['plose'])*100);
    $problose=round($f['plose']*100);
    $total+=$all;
    if($f['maxwelo']>$maxwelo) $maxwelo=$f['maxwelo'];
    if($f['maxbelo']>$maxbelo) $maxbelo=$f['maxbelo'];
    echo "<tr align=center><td align=left><a href='openings.php?variant=".urlencode($variant.$f['move'])."&tomove=".(3-$tomove)."'>$f[move] </a></td><td><a href='seekgame.php?variant=".urlencode($variant.$f['move'])."' target=new>$all</a></td><td><a href='seekgame.php?variant=".urlencode($variant.$f['move'])."&result=1-0' target=new>$probwin%</a></td><td><a href='seekgame.php?variant=".urlencode($variant.$f['move'])."&result=1/2-1/2' target=new>$probdraw%</a></td><td><a href='seekgame.php?variant=".urlencode($variant.$f['move'])."&result=0-1' target=new>$problose%</a></td><td><a href='seekgame.php?variant=".urlencode($variant.$f['move'])."&welo=$f[maxwelo]' target=new>$f[maxwelo]</a></td><td><a href='seekgame.php?variant=".urlencode($variant.$f['move'])."&belo=$f[maxbelo]' target=new>$f[maxbelo]</a></td></tr>";
    switch($tomove) {
      case 1:
        if($f['pwin']<1.)
          $sumwin+=$all*$f['pwin']/(1.-$f['pwin']);
        if($f['plose']>0)
          $sumlose+=$all*(1.-$f['plose'])/$f['plose'];
        break;
      case 2:
        if($f['pwin']>0)
          $sumwin+=$all*(1.-$f['pwin'])/$f['pwin'];
        if($f['plose']<1.)
          $sumlose+=$all*$f['plose']/(1.-$f['plose']);
        break;
    }
  }
  mysqli_free_result($r);
  switch($tomove) {
    case 1:
      $sumwin=1.-1./(1+$sumwin/$total);
      $sumlose=1./(1.+$sumlose/$total);
      $sumdraw=1.-$sumwin-$sumlose;
      break;
    case 2:
      $sumwin=1./(1.+$sumwin/$total);
      $sumlose=1.-1./(1+$sumlose/$total);
      $sumdraw=1.-$sumwin-$sumlose;
      break;
  }
  mysqli_close($link);
?>
<tr align=center><td align=left>Всего</td><td><? echo $total; ?></td><td><? echo round($sumwin*100);?>%</td><td><? echo round($sumdraw*100);?>%</td><td><? echo round($sumlose*100);?>%</td><td><? echo $maxwelo?></td><td><? echo $maxbelo?></td></tr>
</table>
<?
  if($variant!=='') {
    $link=mysqli_connect($sqlserver,$sqluser,$sqlpassword,$dbname);
    mysqli_set_charset($link,'utf8');
    mysqli_multi_query($link,"call seekopening('$variant',0.01)");
    $r=mysqli_store_result($link);
    for($i=0; $i<mysqli_num_rows($r); ++$i) {
      $f=mysqli_fetch_array($r);
      echo $f['opening']." (".round($f['frac']*100).")%<br>\n";
    }
    mysqli_free_result($r);
  }
?>
</body>
</html>