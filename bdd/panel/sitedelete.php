<?php
  //include 'local.postgre.conf.php';
  include 'postgresql.conf.inc.php';
  include 'fonction.php';


$id = $_GET['id'];
$result = pg_query( "DELETE FROM Sites WHERE id_site=$id");
header("Location:siteview.php");

?>
