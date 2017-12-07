<?php
  //include 'local.postgre.conf.php';
  include 'postgresql.conf.inc.php';
  include 'fonction.php';


$id = $_GET['id'];
$result = pg_query( "DELETE FROM dataaccount WHERE id_account=$id");
$result = pg_query( "DELETE FROM account WHERE id_account=$id");
header("Location:userview.php");

?>
